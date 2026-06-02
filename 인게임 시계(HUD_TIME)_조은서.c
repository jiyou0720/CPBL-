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

void HUD_TIME(time_t start_ref, double acc_ref, bool is_running) {
    time_t raw_time = time(NULL);
    struct tm* local_time = localtime(&raw_time);

    double elapsed = acc_ref;
    if (is_running) {
        elapsed += difftime(raw_time, start_ref);
    }

    int h = (int)elapsed / 3600;
    int m = ((int)elapsed % 3600) / 60;
    int s = (int)elapsed % 60;

    gotoxy(0, 0);
    printf("[ 현재 시각: %02d:%02d:%02d ]",
        local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

    gotoxy(40, 0); 
    printf("[ 경과 시간(Ranking): %02d:%02d:%02d ]", h, m, s);
    fflush(stdout);
}

int main() {
    time_t start_time;
    double accumulated_time = 0;
    bool is_running = true;
    bool game_over = false;

    printf("\e[?25l"); 
    printf("\e[1;1H\e[2J");
    printf("\n\n\n    --- 게임이 실행 중입니다 ---   \n");
    printf("    (ESC: 일시정지, Q: 게임 종료)   \n");

    start_time = time(NULL);

    while (!game_over) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 27) { 
                if (is_running) {
                    accumulated_time += difftime(time(NULL), start_time);
                    is_running = false;
                    gotoxy(0, 5);
                    printf(">> 게임 일시 정지됨 <<          ");
                    fflush(stdout);
                }
                else {
                    start_time = time(NULL);
                    is_running = true;
                    gotoxy(0, 5);
                    printf("                               ");
                    fflush(stdout);
                }
            }
            else if (key == 'q' || key == 'Q') {
                game_over = true;
            }
