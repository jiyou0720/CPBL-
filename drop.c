#include <stdio.h>
#include <Windows.h>
#include <conio.h>

void setCursorPos(int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int main()
{
    // 플레이어 초기 위치
    int px = 10;
    int py = 10;

    // 아이템 변수
    int has_item = 1;  
    int ix = -1;  
    int iy = -1;      

    char key;

    while (1)
    {
        // 화면 초기화
        system("cls");

        // 1. 드롭된 아이템이 있다면 'I' 타일 표시
        if (!has_item)
        {
            setCursorPos(ix, iy);
            printf("I");
        }

        // 플레이어 위치에 'P' 표시
        setCursorPos(px, py);
        printf("P");

        // 하단 UI 및 안내 메시지 표시
        setCursorPos(0, 22);
        printf("WASD: 이동   Q: 손 아이템 드롭 (현재 위치)");
        setCursorPos(0, 23);
        printf("인벤토리 상태: %s", has_item ? "[아이템 보유 중]" : "[비어 있음 (바닥에 드롭함)]");

        // 키 입력 처리
        key = _getch();

        // 이동 로직 (WASD)
        if      (key == 'w' || key == 'W') py--;
        else if (key == 's' || key == 'S') py++;
        else if (key == 'a' || key == 'A') px--;
        else if (key == 'd' || key == 'D') px++;
        
        // 아이템 드롭 
        else if (key == 'q' || key == 'Q')
        {
            // 현재 손에 아이템을 들고 있는 상태일 때만 드롭 가능
            if (has_item)
            {
                has_item = 0;  // 손에서 아이템이 없어짐
                ix = px;      
                iy = py;      
            }
        }

        // 맵 경계 제한 
        if (px < 0)  px = 0;
        if (px > 38) px = 38;
        if (py < 0)  py = 0;
        if (py > 19) py = 19;
    }

    return 0;
}
