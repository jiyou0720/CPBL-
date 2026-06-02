#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>    
#include <termios.h>   
#include <fcntl.h>     

void gotoxy(int x, int y) {
    printf("\e[%d;%dH", y + 1, x + 1);
}

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

void MISC_TIMER() {
    time_t start_time;
    double accumulated_time = 0; 
    bool is_running = true;      
    bool game_over = false;      

    printf("\e[?25l"); 
    printf("\e[1;1H\e[2J");

    printf("========================================\n");
    printf("   Simple C Game Timer (ESC to Pause)   \n");
    printf("========================================\n");
    printf("\n  [콘솔 중앙] 게임 플레이 영역입니다.\n");
    printf("  - ESC 키: 일시 정지 및 재개\n");
    printf("  - Q 키: 게임 종료 (결과 확인)\n");

    start_time = time(NULL);

    while (!game_over) {
        if (_kbhit()) {
            int key = _getch();

            if (key == 27) { 
                if (is_running) {
                    accumulated_time += difftime(time(NULL), start_time);
                    is_running = false;

                    gotoxy(0, 20);
                    printf("[ 일시 정지 중... 다시 ESC를 누르면 재개합니다. ]");
                    fflush(stdout);
                }
                else {
                    start_time = time(NULL);
                    is_running = true;

                    gotoxy(0, 20);
                    printf("                                                   "); 
                    fflush(stdout);
                }
            }
            else if (key == 'q' || key == 'Q') {
                game_over = true;
            }
        }

        if (is_running) {
            double total_seconds = accumulated_time + difftime(time(NULL), start_time);

            int h = (int)total_seconds / 3600;
            int m = ((int)total_seconds % 3600) / 60;
            int s = (int)total_seconds % 60;

            gotoxy(0, 24);
            printf("[Play Time] %02d:%02d:%02d", h, m, s);

            gotoxy(0, 10);
            fflush(stdout); 
        }

        usleep(100000);
    }

    if (is_running) {
        accumulated_time += difftime(time(NULL), start_time);
    }

    printf("\e[1;1H\e[2J"); 
    printf("\n\n========================================\n");
    printf("               GAME FINISHED!             \n");
    printf("========================================\n");
    printf(" 최종 클리어 타임: %.0f초\n", accumulated_time);
    printf(" 기록 측정 결과: %d시간 %d분 %d초\n",
        (int)accumulated_time / 3600,
        ((int)accumulated_time % 3600) / 60,
        (int)accumulated_time % 60);
    printf("========================================\n");
    printf("프로그램을 종료하려면 아무 키나 누르세요.\n");
    
    _getch();
    printf("\e[?25h"); 
}

int main() {
    MISC_TIMER();
    return 0;
}
