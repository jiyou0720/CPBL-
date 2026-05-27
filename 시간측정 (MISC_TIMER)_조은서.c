//필수 / P1 | 클리어까지 초 단위, ESC 중 정지(time_t/difftime())

#include <stdio.h>
#include <time.h>
#include <conio.h>    // _kbhit(), _getch() 사용
#include <stdbool.h>
#include <windows.h>  // SetConsoleCursorPosition, Sleep() 사용

// 특정 좌표로 커서를 이동시키는 함수 (UI 배치용)
void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 타이머 및 게임 로직을 담당하는 메인 함수
void MISC_TIMER() {
    time_t start_time;
    double accumulated_time = 0; // 정지 전까지 누적된 시간
    bool is_running = true;      // 타이머 가동 상태
    bool game_over = false;      // 게임 종료 상태

    // 초기 화면 설정
    system("cls");
    printf("========================================\n");
    printf("   Simple C Game Timer (ESC to Pause)   \n");
    printf("========================================\n");
    printf("\n  [콘솔 중앙] 게임 플레이 영역입니다.\n");
    printf("  - ESC 키: 일시 정지 및 재개\n");
    printf("  - Q 키: 게임 종료 (결과 확인)\n");

    // 최초 시작 시각 기록
    start_time = time(NULL);

    while (!game_over) {
        // 1. 키보드 입력 처리 (비차단 방식)
        if (_kbhit()) {
            int key = _getch();

            if (key == 27) { // ESC 키
                if (is_running) {
                    // 일시 정지: 지금까지 흐른 시간을 저장
                    accumulated_time += difftime(time(NULL), start_time);
                    is_running = false;

                    gotoxy(0, 20);
                    printf("[ 일시 정지 중... 다시 ESC를 누르면 재개합니다. ]");
                }
                else {
                    // 재개: 현재 시점을 새로운 시작점으로 갱신
                    start_time = time(NULL);
                    is_running = true;

                    gotoxy(0, 20);
                    printf("                                                   "); // 메시지 삭제
                }
            }
            else if (key == 'q' || key == 'Q') {
                game_over = true;
            }
        }

        // 2. 타이머 실시간 업데이트 (화면 하단 구석)
        if (is_running) {
            double total_seconds = accumulated_time + difftime(time(NULL), start_time);

            // 시:분:초 변환
            int h = (int)total_seconds / 3600;
            int m = ((int)total_seconds % 3600) / 60;
            int s = (int)total_seconds % 60;

            // 화면 왼쪽 하단(0, 24)에 작게 표시
            gotoxy(0, 24);
            printf("[Play Time] %02d:%02d:%02d", h, m, s);

            // 커서가 타이머 위치에서 깜빡이지 않도록 중앙 플레이 영역으로 복구
            gotoxy(0, 10);
        }

        // CPU 과부하 방지를 위한 미세한 대기
        Sleep(100);
    }

    // 3. 게임 종료 후 최종 결과 계산
    if (is_running) {
        accumulated_time += difftime(time(NULL), start_time);
    }

    system("cls");
    printf("\n\n========================================\n");
    printf("             GAME FINISHED!             \n");
    printf("========================================\n");
    printf(" 최종 클리어 타임: %.0f초\n", accumulated_time);
    printf(" 기록 측정 결과: %d시간 %d분 %d초\n",
        (int)accumulated_time / 3600,
        ((int)accumulated_time % 3600) / 60,
        (int)accumulated_time % 60);
    printf("========================================\n");
    printf("프로그램을 종료하려면 아무 키나 누르세요.\n");
    _getch();
}

// LNK2019 에러 해결을 위한 프로그램 진입점
int main() {
    // MISC_TIMER 함수를 실행합니다.
    MISC_TIMER();
    return 0;
}
