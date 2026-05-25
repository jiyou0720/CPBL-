// MISC_TERMSIZE - 터미널 크기 체크
// 필수 / P1 | 시작 시 최소 80×24 미달이면 경고(getmaxyx)

#include <stdio.h>
#include <Windows.h>
#include <conio.h>

// 리눅스의 getmaxyx() 개념을 윈도우 콘솔 크기 체크로 구현
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
        printf(" [경고] 콘솔 창 크기가 지나치게 작습니다!\n");
        printf(" 현재 크기: %d x %d | 최소 필요 크기: 80 x 24\n");
        printf("========================================================\n");
        printf(" 창 크기를 키우거나 전체화면으로 변경한 뒤 다시 실행해 주세요.\n");
        _getch();
        return 0; 
    }
    return 1; 
}

int main()
{
    // 시작 시 해상도 체크 (미달이면 즉시 프로그램 종료)
    if (!checkConsoleSize())
    {
        return 0; 
    }

    // 실제 게임 실행 루프
    while (1)
    {
        system("cls");

        printf("=========================================\n");
        printf(" 해상도 검사 완료. 정상 작동 중인 게임 화면\n");
        printf("=========================================\n");
        printf(" ESC를 누르면 프로그램이 종료됩니다.\n");

        // 게임 입력 처리 루프 예시
        char key = _getch();
        if (key == 27) // ESC 키
        {
            break;
        }
    }

    return 0;
}
