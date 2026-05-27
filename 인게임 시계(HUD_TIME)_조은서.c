// 필수 / P1 | 경과 시간(랭킹)+게임 내 시각(time()/difftime())
// 보류 - 인게임 시계의 작동에 관해 세부적인 논의 필요 (05.04)

#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>

// 화면 좌표 이동 함수
void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 상단 또는 하단에 시간을 표시하는 HUD 함수
void HUD_TIME(time_t start_ref, double acc_ref, bool is_running) {
    // 1. 현재 시스템 시각 가져오기
    time_t raw_time = time(NULL);
    struct tm* local_time = localtime(&raw_time);

    // 2. 경과 시간(랭킹용) 계산
    double elapsed = acc_ref;
    if (is_running) {
        elapsed += difftime(raw_time, start_ref);
    }

    int h = (int)elapsed / 3600;
    int m = ((int)elapsed % 3600) / 60;
    int s = (int)elapsed % 60;

    // HUD 출력 (화면 맨 위쪽 라인에 배치)
    gotoxy(0, 0);
    printf("[ 현재 시각: %02d:%02d:%02d ]",
        local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

    gotoxy(40, 0); // 오른쪽으로 탭 이동 후 경과 시간 표시
    printf("[ 경과 시간(Ranking): %02d:%02d:%02d ]", h, m, s);
}

int main() {
    time_t start_time;
    double accumulated_time = 0;
    bool is_running = true;
    bool game_over = false;

    system("cls");
    printf("\n\n\n   --- 게임이 실행 중입니다 ---   \n");
    printf("   (ESC: 일시정지, Q: 게임 종료)   \n");

    start_time = time(NULL);

    while (!game_over) {
        // 키 입력 처리
        if (_kbhit()) {
            int key = _getch();
            if (key == 27) { // ESC
                if (is_running) {
                    accumulated_time += difftime(time(NULL), start_time);
                    is_running = false;
                    gotoxy(0, 5);
                    printf(">> 게임 일시 정지됨 <<          ");
                }
                else {
                    start_time = time(NULL);
                    is_running = true;
                    gotoxy(0, 5);
                    printf("                               ");
                }
            }
            else if (key == 'q' || key == 'Q') {
                game_over = true;
            }
        }

        // HUD 업데이트 호출
        HUD_TIME(start_time, accumulated_time, is_running);

        Sleep(200); // 0.2초마다 업데이트
    }

    system("cls");
    printf("게임 종료!\n");
    return 0;
}
