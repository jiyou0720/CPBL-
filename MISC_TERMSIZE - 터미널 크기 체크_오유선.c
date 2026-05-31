// MISC_TERMSIZE - 터미널 크기 체크
// 필수 / P1 | 시작 시 최소 80×24 미달이면 경고(getmaxyx)

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

int checkConsoleSize()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    int cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    if (cols < 80 || rows < 24)
    {
        system("cls");
        printf("========================================================\n");
        printf(" [경고] 창 크기가 너무 작아 저택을 표시할 수 없습니다.\n");
        printf(" 현재 크기: %d x %d | 최소 필요 크기: 80 x 24\n", cols, rows);
        printf("========================================================\n");
        printf(" 창 크기를 키우거나 전체화면으로 변경한 뒤 다시 실행해 주세요.\n");
        _getch();
        return 0;
    }
    return 1;
}

int main()
{
    if (!checkConsoleSize())
    {
        return 0;
    }

    while (1)
    {
        system("cls");
        printf("=========================================\n");
        printf(" 해상도 검사 완료. 정상 작동 중인 게임 화면\n");
        printf("=========================================\n");
        printf(" ESC를 누르면 프로그램이 종료됩니다.\n");

        char key = _getch();
        if (key == 27)
        {
            break;
        }
    }
    return 0;
}
