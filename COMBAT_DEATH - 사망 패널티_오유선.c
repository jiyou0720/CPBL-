// COMBAT_DEATH - 사망/부활 담당
// 즉사 또는 체크포인트 부활 (난이도 연동)
//참고사항: 체크 포인트, 함정 타일 임시 설정됨.

#include <stdio.h>
#include <Windows.h>
#include <conio.h>

// 난이도 (0=쉬움, 1=보통, 2=어려움)
// 쉬움/보통 : 체크포인트 부활
// 어려움    : 즉사 (처음부터)
int difficulty = 1;

typedef struct {
    int x, y;
} Pos;

Pos checkpoint  = { 5,  5 };   // 마지막으로 밟은 체크포인트
Pos start_pos   = { 2,  2 };   // 게임 시작 위치
int px, py;

void setCursorPos(int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void drawScreen()
{
    system("cls");
    setCursorPos(px, py);   printf("P");
    setCursorPos(checkpoint.x, checkpoint.y); printf("C");  // 체크포인트 타일
    setCursorPos(20, 8);    printf("X");                    // 함정 타일 (즉사 트리거: 유령)
    setCursorPos(0, 22);
    printf("WASD: 이동  |  난이도: %s  |  체크포인트: (%d, %d)",
           difficulty == 0 ? "쉬움" :
           difficulty == 1 ? "보통" : "어려움",
           checkpoint.x, checkpoint.y);
}

void drawDeathScreen(const char* msg)
{
    system("cls");
    setCursorPos(10, 8);  printf("==============================");
    setCursorPos(10, 9);  printf("          사망했습니다        ");
    setCursorPos(10, 10); printf("  %s", msg);
    setCursorPos(10, 11); printf("==============================");
    setCursorPos(10, 12); printf("  아무 키나 누르면 계속합니다.");
    _getch();
}

// 난이도에 따라 부활 위치 결정
void respawn()
{
    if (difficulty == 2)
    {
        // 어려움: 즉사 — 시작 위치로
        drawDeathScreen("[ 어려움 ] 처음부터 시작합니다.");
        px = start_pos.x;
        py = start_pos.y;
        checkpoint = start_pos;  // 체크포인트 초기화
    }
    else
    {
        // 쉬움/보통: 체크포인트 부활
        drawDeathScreen("[ 체크포인트 ] 마지막 저장 위치로 돌아갑니다.");
        px = checkpoint.x;
        py = checkpoint.y;
    }
}

// 체크포인트 타일 밟으면 저장
void checkCheckpoint()
{
    if (px == checkpoint.x && py == checkpoint.y) return;  // 이미 같은 위치

    // 체크포인트 타일 좌표 목록
    Pos cp_tiles[] = {
        {  5,  5 },
        { 15, 10 },
        { 30, 15 },
    };
    int count = sizeof(cp_tiles) / sizeof(cp_tiles[0]);

    for (int i = 0; i < count; i++)
    {
        if (px == cp_tiles[i].x && py == cp_tiles[i].y)
        {
            checkpoint = cp_tiles[i];
            setCursorPos(0, 23);
            printf("체크포인트 저장! (%d, %d)        ", checkpoint.x, checkpoint.y);
            Sleep(600);
            return;
        }
    }
}

// 함정 타일 밟으면 사망 트리거
void checkTrap()
{
    // 함정 타일 좌표 목록
    Pos trap_tiles[] = {
        { 20,  8 },
        { 10, 15 },
    };
    int count = sizeof(trap_tiles) / sizeof(trap_tiles[0]);

    for (int i = 0; i < count; i++)
    {
        if (px == trap_tiles[i].x && py == trap_tiles[i].y)
        {
            respawn();
            return;
        }
    }
}

int main()
{
    px = start_pos.x;
    py = start_pos.y;

    char key;

    while (1)
    {
        drawScreen();

        key = _getch();

        if      (key == 'w') py--;
        else if (key == 's') py++;
        else if (key == 'a') px--;
        else if (key == 'd') px++;

        if (px < 0)  px = 0;
        if (px > 38) px = 38;
        if (py < 0)  py = 0;
        if (py > 19) py = 19;

        checkCheckpoint();
        checkTrap();
    }

    return 0;
}
