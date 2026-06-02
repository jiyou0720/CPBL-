#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>    // 리눅스 표준 시스템 콜 (usleep)
#include <termios.h>   // 터미널 속성 제어 (getch, kbhit 구현용)
#include <fcntl.h>     // 파일 제어 플래그 (kbhit 논블로킹 설정용)

// --- 시스템 설정 ---
#define MAP_WIDTH 50
#define MAP_HEIGHT 18

// --- 구조체 정의 ---
typedef struct {
    char name[20];       // 유저 이름
    double time;         // 클리어 시간 (초)
    char difficulty[15]; // 난이도
} PlayerRank;

// --- 글로벌 변수 ---
int pX = 5, pY = 5;
int diaryCount = 0;
bool gameRunning = true;
clock_t gameStartTime;
double totalPausedTime = 0.0;

// --- 리눅스 전용 입력 함수 (getch & kbhit) ---
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

// --- 랭킹 시스템 (START_RANK) ---
void SaveDummyData() {
    FILE* fp = fopen("rank.dat", "wb");
    if (fp == NULL) return;

    PlayerRank dummy[5] = {
        {"Alice", 124.5, "Normal"},
        {"Bob", 89.2, "Easy"},
        {"Charlie", 210.0, "Hard"},
        {"Player1", 155.3, "Normal"},
        {"SpeedRun", 45.7, "Easy"}
    };
    fwrite(dummy, sizeof(PlayerRank), 5, fp);
    fclose(fp);
}

void START_RANK() {
    FILE* fp = fopen("rank.dat", "rb");

    if (fp == NULL) {
        SaveDummyData();
        fp = fopen("rank.dat", "rb");
        if (fp == NULL) {
            printf("\e[1;1H\e[2J"); // 리눅스 네이티브 화면 클리어
            printf("\n[오류] 랭킹 데이터를 로드할 수 없습니다.\n");
            printf("아무 키나 누르면 메뉴로 복귀합니다.");
            _getch();
            return;
        }
    }

    PlayerRank ranks[100];
    int count = 0;

    while (fread(&ranks[count], sizeof(PlayerRank), 1, fp) == 1) {
        count++;
        if (count >= 100) break;
    }
    fclose(fp);

    // 정렬 (오름차순)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (ranks[j].time > ranks[j + 1].time) {
                PlayerRank temp = ranks[j];
                ranks[j] = ranks[j + 1];
                ranks[j + 1] = temp;
            }
        }
    }

    // 출력 화면 렌더링
    printf("\e[1;1H\e[2J");
    printf("\n");
    printf("  ======================================================\n");
    printf("  ║                명예의 전당 (TOP 10)                ║\n");
    printf("  ======================================================\n");
    printf("     순위     이름           클리어 시간     난이도\n");
    printf("  ------------------------------------------------------\n");

    int displayCount = (count < 10) ? count : 10;
    if (displayCount == 0) {
        printf("     등록된 랭킹 기록이 없습니다.\n");
    }
    else {
        for (int i = 0; i < displayCount; i++) {
            if (i == 0)      printf("      1등    ");
            else if (i == 1) printf("      2등    ");
            else if (i == 2) printf("      3등    ");
            else             printf("      %d등    ", i + 1);

            printf("%-12s   %7.1f초       %-10s\n",
                ranks[i].name, ranks[i].time, ranks[i].difficulty);
        }
    }
    printf("  ------------------------------------------------------\n");
    printf("\n  ▶ 아무 키나 누르면 일시정지 메뉴로 돌아갑니다.");
    _getch();
    printf("\e[1;1H\e[2J");
}

// --- 게임 화면 출력 ---
void DrawGameScreen(double currentPlayTime) {
    printf("\e[1;1H\e[2J");
    printf("==== Mansion Map [1F] ====  Diary: %d/15\n", diaryCount);
    printf(" 플레이어 궤적 추적 중... (현재 위치: %d, %d)\n", pX, pY);
    printf(" [실제 플레이 시간: %.1f초]\n\n", currentPlayTime);
    printf(" [WASD] 이동  |  [ESC] 일시정지 메뉴 열기\n");
}

// --- 메인 함수 ---
int main() {
    // 리눅스 터미널 커서 숨기기 및 초기화
    printf("\e[?25l"); 
    printf("\e[1;1H\e[2J");

    char CTRL_PAUSE;
    bool isMenuOpen = false;
    gameStartTime = clock();

    while (gameRunning) {
        if (!isMenuOpen) {
            double playTime = ((double)(clock() - gameStartTime) / CLOCKS_PER_SEC) - totalPausedTime;
            DrawGameScreen(playTime);

            if (_kbhit()) {
                int key = _getch();
                if (key == 27) { 
                    isMenuOpen = true;
                }
                else if (key == 'w' || key == 'W') pY--;
                else if (key == 's' || key == 'S') pY++;
                else if (key == 'a' || key == 'A') pX--;
                else if (key == 'd' || key == 'D') pX++;
            }
            usleep(80000); // 80ms 대기 (리눅스 커널 타이머 지연)
            continue;
        }

        // 일시정지 메뉴 루프
        clock_t pauseStartTime = clock();
        printf("\e[1;1H\e[2J");

        printf("\n");
        printf("    ==================================================\n");
        printf("    ║                   GAME PAUSED                  ║\n");
        printf("    ==================================================\n");
        printf("    ║                                                ║\n");
        printf("    ║   [1] REPLAY  - 게임 재개                      ║\n");
        printf("    ║   [2] OPTION  - 게임 옵션 및 설정              ║\n");
        printf("    ║   [3] BOOK    - 도감 열람                      ║\n");
        printf("    ║   [4] RANK    - 명예의 전당 (랭킹)             ║\n");
        printf("    ║   [5] TITLE   - 타이틀 화면으로 복귀           ║\n");
        printf("    ║                                                ║\n");
        printf("    ==================================================\n");
        printf("    ▶ 명령을 선택하세요 (1~5): ");

        CTRL_PAUSE = _getch();

        if (CTRL_PAUSE == '1') {
            clock_t pauseEndTime = clock();
            totalPausedTime += (double)(pauseEndTime - pauseStartTime) / CLOCKS_PER_SEC;
            isMenuOpen = false;
            printf("\e[1;1H\e[2J");
        }
        else if (CTRL_PAUSE == '2' || CTRL_PAUSE == '3') {
            printf("\n    준비 중인 기능입니다. 아무 키나 누르세요.");
            _getch();
        }
        else if (CTRL_PAUSE == '4') {
            START_RANK();
        }
        else if (CTRL_PAUSE == '5') {
            printf("\n    [SYSTEM] 정말 종료하시겠습니까? (Y/N): ");
            char confirm = _getch();
            if (confirm == 'y' || confirm == 'Y') {
                printf("\e[?25h"); // 종료 전 터미널 커서 원상복구
                exit(0);
            }
        }
    }
    
    printf("\e[?25h"); 
    return 0;
}
