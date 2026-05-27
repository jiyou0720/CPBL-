// 필수 | ESC: 재개·옵션·도감·랭킹·타이틀(시간 측정 일시정지 포함)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

// 게임 내 시간 측정을 위한 변수
clock_t gameStartTime;
double totalPausedTime = 0.0; // 총 일시정지된 시간 (초 단위)

// 임의의 게임 상태 출력 함수
void DrawGameScreen(double currentPlayTime) {
    system("cls");
    printf("==================================================\n");
    printf("                  　전장 탐색 중 　　               \n");
    printf("==================================================\n");
    printf("  [플레이 시간] : %.1f초\n", currentPlayTime);
    printf("  [현재 상태]   : 사방이 고요합니다...\n");
    printf("==================================================\n");
    printf("  ▶ [ESC] 키를 누르면 시스템 메뉴가 열립니다.\n");
}

int main() {
    char CTRL_PAUSE; // 메뉴 선택을 저장할 변수
    bool isMenuOpen = false;

    // 게임 시작 시간 기록
    gameStartTime = clock();

    while (1) {
        // 1. 평상시 게임 루프 (메뉴가 닫혀있을 때)
        if (!isMenuOpen) {
            // 일시정지된 시간을 제외한 실제 플레이 시간 계산
            double playTime = ((double)(clock() - gameStartTime) / CLOCKS_PER_SEC) - totalPausedTime;
            DrawGameScreen(playTime);

            // 키 입력 검사
            if (_kbhit()) {
                int key = _getch();
                if (key == 27) { // 27은 ESC 키의 아스키코드 값입니다.
                    isMenuOpen = true;
                }
            }
            Sleep(100); // CPU 과부하 방지
            continue;
        }

        // 2. [ESC] 일시정지 메뉴 진입 (시간 측정 일시정지 시작)
        clock_t pauseStartTime = clock(); // 일시정지가 시작된 시점 기록
        system("cls");

        printf("\n");
        printf("    ==================================================\n");
        printf("    |               　　GAME PAUSED 　               |\n");
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

        // 메뉴 입력 받기
        CTRL_PAUSE = _getch();
        printf("\n");

        // 3. CTRL_PAUSE 분기 처리
        if (CTRL_PAUSE == '1') {
            printf("    [SYSTEM] 게임을 재개합니다.\n");
            Sleep(500);

            // 일시정지 유지 시간을 계산하여 총 일시정지 시간에 누적
            clock_t pauseEndTime = clock();
            totalPausedTime += (double)(pauseEndTime - pauseStartTime) / CLOCKS_PER_SEC;

            isMenuOpen = false; // 메뉴 닫기
        }
        else if (CTRL_PAUSE == '2') {
            printf("    [OPTION] 사운드 및 그래픽 설정 화면 (미구현)\n");
            printf("    ＞ 아무 키나 누르면 일시정지 메뉴로 돌아갑니다.");
            _getch();
        }
        else if (CTRL_PAUSE == '3') {
            printf("    [BOOK] 몬스터 및 아이템 도감 화면 (미구현)\n");
            printf("    ＞ 아무 키나 누르면 일시정지 메뉴로 돌아갑니다.");
            _getch();
        }
        else if (CTRL_PAUSE == '4') {
            printf("    [RANK] 현재 시즌 유저 랭킹 보드 (미구현)\n");
            printf("    ＞ 아무 키나 누르면 일시정지 메뉴로 돌아갑니다.");
            _getch();
        }
        else if (CTRL_PAUSE == '5') {
            printf("    [SYSTEM] 정말 타이틀 화면으로 돌아가시겠습니까? (Y/N): ");
            char confirm = _getch();
            if (confirm == 'y' || confirm == 'Y') {
                printf("\n    [SYSTEM] 타이틀 화면으로 이동합니다. 프로그램을 종료합니다.\n");
                exit(0);
            }
        }
    }

    return 0;
}
