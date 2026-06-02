#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>    
#include <termios.h>   
#include <fcntl.h>     

typedef struct {
    int id;
    char title[50];
    char content[256];
} Note;

Note inventory[10];
int collected_count = 0;

int _getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int _kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

void NOTE_COLLECT(int target_id) {
    FILE* file = fopen("notes.dat", "r");
    if (file == NULL) return;

    Note temp;
    while (fscanf(file, "%d \"%[^\"]\" \"%[^\"]\"", &temp.id, temp.title, temp.content) != EOF) {
        if (temp.id == target_id) {
            inventory[collected_count++] = temp;

            printf("\n\n>>> [획득] %s <<<\n", temp.title);
            printf(">>> 내용: %s\n", temp.content);
            fflush(stdout);
            break;
        }
    }
    fclose(file);
}

int main() {
    bool is_near_object = true; 
    int object_id = 101;        

    printf("\e[?25l"); 
    printf("\e[1;1H\e[2J");

    printf("게임 진행 중... (오브젝트 앞에 도착했습니다)\n");
    printf("[ F ] 키를 눌러 조사를 시작하세요.\n");
    printf("[ Q ] 키를 누르면 종료합니다.\n");
    fflush(stdout);

    while (1) {
        if (_kbhit()) {
            int key = _getch();

            if (key == 'f' || key == 'F') {
                if (is_near_object) {
                    NOTE_COLLECT(object_id);

                    is_near_object = false;
                    printf("\n(조사가 완료되었습니다. 다음 장소로 이동하세요.)\n");
                    fflush(stdout);
                }
            }

            if (key == 'q' || key == 'Q') break;
        }
        usleep(10000);
    }

    printf("\e[?25h"); 
    return 0;
}
