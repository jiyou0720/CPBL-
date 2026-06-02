#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>    
#include <termios.h>   
#include <fcntl.h>     

clock_t gameStartTime;
double totalPausedTime = 0.0; 

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

void DrawGameScreen(double currentPlayTime) {
    printf("\e[1;1H\e[2J");
    printf("==================================================\n");
    printf("                 　전장 탐색 중 　　                \n");
    printf("==================================================\n");
    printf("  [플레이 시간] : %.1f초\n", currentPlayTime);
    printf("  [현재 상태]   : 사방이 고요합니다...\n");
    printf("==================================================\n");
    printf("  ▶ [ESC] 키를 누르면 시스템 메뉴가 열립니다.\n");
    fflush(stdout);
}

int main() {
    char CTRL_PAUSE; 
    bool isMenuOpen = false;

    printf("\e[?25l"); 
    gameStartTime = clock();

    while (1) {
        if (!isMenuOpen) {
            double playTime = ((double)(clock() - gameStartTime) / CLOCKS_PER_SEC) - totalPausedTime;
            DrawGameScreen(playTime);

            if (_kbhit()) {
                int key = _getch();
                if (key == 27) { 
                    isMenuOpen = true;
                }
            }
            usleep(100000); 
            continue;
        }

        clock_t pauseStartTime = clock(); 
        printf("\e[1;1H\e[2J");

        printf("\n");
        printf("    ==================================================\n");
        printf("    |                　　GAME PAUSED 　               |\n");
        printf("    ==================================================\n");
        printf("    |                                                |\n");
        printf("    |   [1] REPLAY  - 게임 재개 (시간 다시 흐름)     |\n");
        printf("    |   [2] OPTION  - 게임 옵션 및 설정              |\n");
        printf("    |   [3] BOOK    - 도감 열람                      |\n");
        printf("    |   [4] RANK    - 명예의 전당 (랭킹)             |\n");
        printf("    |   [5] TITLE   - 타이틀 화면으로 복귀           |\n");
        printf("    |                                                |\n");
        printf("    ==================================================\n");
        printf("    ＞ 명령을 선택하세요 (1~5): ");
        fflush(stdout);

        CTRL_PAUSE = _getch();
        printf("\n");

        if (CTRL_PAUSE == '1') {
            printf("    [SYSTEM] 게임을 재개합니다.\n");
            fflush(stdout);
            usleep(500000);

            clock_t pauseEndTime = clock();
            totalPausedTime += (double)(pauseEndTime - pauseStartTime) / CLOCKS_PER_SEC;

            isMenuOpen = false; 
        }
        else if (CTRL_PAUSE == '2') {
            printf("    [OPTION] 사운드 및 그래픽 설정 화면 (미구현)\n");
            printf("    ＞ 아무 키나 누르면 일시정지 메뉴로 돌아갑니다.");
            fflush(stdout);
            _getch();
        }
        else if (CTRL_PAUSE == '3') {
            printf("    [BOOK] 몬스터 및 아이템 도감 화면 (미구현)\n");
            printf("    ＞ 아무 키나 누르면 일시정지 메뉴로 돌아갑니다.");
            fflush(stdout);
            _getch();
        }
        else if (CTRL_PAUSE == '4') {
            printf("    [RANK] 현재 시즌 유저 랭킹 보드 (미구현)\n");
            printf("    ＞ 아무 키나 누르면 일시정지 메뉴로 돌아갑니다.");
            fflush(stdout);
            _getch();
        }
        else if (CTRL_PAUSE == '5') {
            printf("    [SYSTEM] 정말 타이틀 화면으로 돌아가시겠습니까? (Y/N): ");
            fflush(stdout);
            char confirm = _getch();
            if (confirm == 'y' || confirm == 'Y') {
                printf("\n    [SYSTEM] 타이틀 화면으로 이동합니다. 프로그램을 종료합니다.\n");
                printf("\e[?25h"); 
                exit(0);
            }
        }
    }

    printf("\e[?25h"); 
    return 0;
}
