// COMBAT_GAMEOVER - 게임 오버 화면 담당
// HP 0 또는 시간 초과 시 전용 화면 표시 (재시작 / 타이틀 선택)

#include <stdio.h>
#include <Windows.h>
#include <conio.h>

// 게임 오버 원인
#define GAMEOVER_HP   0  // HP 0
#define GAMEOVER_TIME 1  // 시간 초과

// 메뉴 선택지
#define MENU_RESTART 0
#define MENU_TITLE   1

void setCursorPos(int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int showGameOver(int cause)
{
    int selected = 0;  // 현재 선택된 메뉴 (0=재시작, 1=타이틀)
    int key;

    while (1)
    {
        system("cls");

        // 게임 오버 원인 메시지
        setCursorPos(12, 3);
        if (cause == GAMEOVER_HP)
            printf("HP 가 0 이 되었습니다.");
        else
            printf("시간이 초과되었습니다.");

        // 게임 오버 타이틀
        setCursorPos(10, 5);  printf("   ██████╗  █████╗ ███╗   ███╗███████╗");
        setCursorPos(10, 6);  printf("  ██╔════╝ ██╔══██╗████╗ ████║██╔════╝");
        setCursorPos(10, 7);  printf("  ██║  ███╗███████║██╔████╔██║█████╗  ");
        setCursorPos(10, 8);  printf("  ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝  ");
        setCursorPos(10, 9);  printf("  ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗");
        setCursorPos(10, 10); printf("   ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝");
        setCursorPos(10, 11); printf("              O V E R                  ");
        

        // 메뉴 선택지 (현재 선택된 항목에 > 표시)
        setCursorPos(14, 14);
        printf("%s 재시작", selected == MENU_RESTART ? ">" : " ");
        setCursorPos(14, 15);
        printf("%s 타이틀로 돌아가기", selected == MENU_TITLE ? ">" : " ");

        setCursorPos(10, 17); printf("방향키: 선택   Enter: 확인");

        // 입력 처리 (방향키는 두 바이트)
        key = _getch();
        if (key == 0 || key == 224)
        {
            int arrow = _getch();
            if      (arrow == 72 && selected > 0) selected--;  // 위
            else if (arrow == 80 && selected < 1) selected++;  // 아래
        }
        else if (key == 13)  // Enter
        {
            return selected;
        }
    }
}

// 예시
int main()
{
    // HP 0으로 게임 오버
    int result = showGameOver(GAMEOVER_HP);

    system("cls");
    setCursorPos(0, 0);
    if (result == MENU_RESTART)
        printf("재시작 선택됨 -> 게임 재시작 처리");
    else
        printf("타이틀 선택됨 -> 타이틀 화면으로 이동");

    _getch();
    return 0;
}
