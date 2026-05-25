// CTRL_BAG - 가방 열기
// 필수 | E키: 인벤 윈도우 토글, 열린 상태에서 방향키로 선택

#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#define MAX_ITEMS 4
#define KEY_UP    72
#define KEY_DOWN  80

// 인벤토리 화면 렌더링
void drawInventory(int is_open, int selected_slot, char items[MAX_ITEMS][32])
{
    if (!is_open)
    {
        for (int i = 0; i < 8; i++)
        {
            COORD pos = { 45, 3 + i };
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
            printf("                                     ");
        }
        return;
    }

    // 토글 열림 상태일 때 인벤토리 UI 출력
    short start_y = 3;
    COORD pos;
    
    pos.X = 45; pos.Y = start_y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("┌─── INVENTORY ───┐");

    for (int i = 0; i < MAX_ITEMS; i++)
    {
        pos.Y = start_y + 1 + i;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        
        // 현재 선택된 슬롯에만 포인터(>) 표시
        if (i == selected_slot)
            printf("│  > [%d] %-10s │", i + 1, items[i]);
        else
            printf("│    [%d] %-10s │", i + 1, items[i]);
    }

    pos.Y = start_y + 1 + MAX_ITEMS;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("├─────────────────┤");
    
    pos.Y = start_y + 1 + MAX_ITEMS + 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("│ 방향키: 선택     │");
    
    pos.Y = start_y + 1 + MAX_ITEMS + 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("└─── E: 닫기  ───┘");
}

int main()
{
    int px = 10, py = 10;
    
    // 인벤토리 상태 관리 변수 
    int inv_open = 0;       // 0: 닫힘, 1: 열림
    int selected_slot = 0;  
    
    char inventory[MAX_ITEMS][32] = { // 예시
        "낡은 열쇠",
        "회복 약초",
        "소형 손전등",
        "비어있음"
    };

    char key;

    while (1)
    {
        system("cls");

        COORD p_pos = { px, py };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p_pos);
        printf("P");

        // 렌더링 루프에 추가할 인벤토리 출력부
        drawInventory(inv_open, selected_slot, inventory);

        key = _getch();

        // 인벤토리 열림/닫힘 상태 처리 루프
        if (inv_open)
        {
            if (key == 0 || key == (char)224) 
            {
                char arrow = _getch();
                if (arrow == KEY_UP && selected_slot > 0)             selected_slot--;
                if (arrow == KEY_DOWN && selected_slot < MAX_ITEMS - 1) selected_slot++;
            }
            else if (key == 'e' || key == 'E')
            {
                inv_open = 0; // 토글 오프
            }
        }
        else
        {
            if      (key == 'w' || key == 'W') py--;
            else if (key == 's' || key == 'S') py++;
            else if (key == 'a' || key == 'A') px--;
            else if (key == 'd' || key == 'D') px++;
            else if (key == 'e' || key == 'E')
            {
                inv_open = 1; // 토글 온
            }
        }

        if (px < 0)  px = 0;
        if (px > 38) px = 38;
        if (py < 0)  py = 0;   
        if (py > 20) py = 20;  
    }

    return 0;
}
