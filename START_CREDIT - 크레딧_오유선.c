// START_CREDIT - 크레딧 화면 담당
// 개발 일자, 만든 사람 등 표시

#include <stdio.h>
#include <Windows.h>
#include <conio.h>

void setCursorPos(int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void drawCredit()
{
    system("cls");

    setCursorPos(10, 2);  printf("================================");
    setCursorPos(10, 3);  printf("           CREDIT               ");
    setCursorPos(10, 4);  printf("================================");

    setCursorPos(10, 6);  printf("  게임 제목  :  CODE_BLUE");
    setCursorPos(10, 7);  printf("  버전       :  v1.0");
    setCursorPos(10, 8);  printf("  개발 일자  :  2026.05.25");

    setCursorPos(10, 10); printf("  기획 / 개발      박지유, 조은서, 신주연");
    setCursorPos(10, 11); printf("  그래픽           오유선");
    setCursorPos(10, 12); printf("  사운드           오유선");

    setCursorPos(10, 14); printf("  Special Thanks");
    setCursorPos(10, 15); printf("    -");

    setCursorPos(10, 17); printf("================================");
    setCursorPos(10, 18); printf("  아무 키나 누르면 돌아갑니다.  ");
    setCursorPos(10, 19); printf("================================");
}

int main()
{
    drawCredit();
    _getch();
    return 0;
}
