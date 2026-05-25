// START_HOW - 게임 방법
// 필수 / P2 | 조작키·목표·UI 설명(ASCII or 텍스트)

#include <stdio.h>
#include <Windows.h>
#include <conio.h>

void setCursorPos(int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void drawGameUI(int px, int py, int has_item, int ix, int iy)
{
    setCursorPos(0, 0);
    printf("==================================================\n");
    printf(" [MISSION] 탈출구를 찾아 아이템(I)을 소지한 채 탈출하라!\n");
    printf("==================================================\n");

    // 2. 우측 사이드바: 플레이어의 현재 실시간 상태 (UI)
    // 맵 렌더링 영역(0~40)과 겹치지 않도록 X 좌표를 45로 지정
    setCursorPos(45, 3);  printf("┌─── STATUS ───┐");
    setCursorPos(45, 4);  printf("│ 플레이어: P  │");
    setCursorPos(45, 5);  printf("│ 현재 좌표: (%2d, %2d) │", px, py);
    setCursorPos(45, 6);  printf("├──────────────┤");
    setCursorPos(45, 7);  
    if (has_item)
        printf("│ 인벤토리: [I]│ (손에 쥐고 있음)");
    else
        printf("│ 인벤토리: [ ]│ (바닥에 드롭함)");
    setCursorPos(45, 8);  printf("└───  ───  ───┘");

    if (!has_item)
    {
        setCursorPos(45, 10);
        printf("[드롭된 아이템 위치]: (%d, %d)", ix, iy);
    }

    // 게임 조작키 가이드 
    setCursorPos(0, 22);
    printf("--------------------------------------------------\n");
    printf(" [ CONTROL KEY ]\n");
    printf("  ▲ 이동: W(위) / A(왼쪽) / S(아래) / D(오른쪽)\n");
    printf("  ▲ 드롭: Q (현재 플레이어 위치에 손에 든 아이템 내려놓기)\n");
    printf("--------------------------------------------------\n");
}

int main()
{
    int px = 10, py = 10;
    int has_item = 1;
    int ix = -1, iy = -1;
    char key;

    while (1)
    {
        system("cls");

        // 아이템이 드롭되었을 때만 바닥에 'I' 타일 마킹
        if (!has_item)
        {
            setCursorPos(ix, iy);
            printf("I");
        }

        setCursorPos(px, py);
        printf("P");

        // UI랑 조작 설명 화면을 출력
        drawGameUI(px, py, has_item, ix, iy);

        key = _getch();

        if      (key == 'w' || key == 'W') py--;
        else if (key == 's' || key == 'S') py++;
        else if (key == 'a' || key == 'A') px--;
        else if (key == 'd' || key == 'D') px++;
        else if (key == 'q' || key == 'Q')
        {
            if (has_item)
            {
                has_item = 0;
                ix = px;
                iy = py;
            }
        }

        if (px < 0)  px = 0;
        if (px > 38) px = 38;
        if (py < 2)  py = 2;   // 상단 UI 영역(0~1) 침범 방지 경계면
        if (py > 20) py = 20;  // 하단 UI 영역(22~) 침범 방지 경계면
    }

    return 0;
}
