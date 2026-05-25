// HUD_NOTE - 메모/노트 담당
// N키 목록, 방향키 선택, Enter 읽기 (읽은 메모 표시)

#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>

#define NOTE_COUNT 5

#define KEY_UP    72
#define KEY_DOWN  80
#define KEY_ENTER 13

typedef struct {
    char title[32];
    char content[256];
    int  read;  // 읽었으면 1
} Note;

Note notes[NOTE_COUNT] = {
    { "메모 #1", "여기서 나가려면 열쇠가 필요하다.",       0 },
    { "메모 #2", "지하실에는 절대 혼자 가지 마라.",         0 },
    { "메모 #3", "손전등 배터리가 얼마 없다.",              0 },
    { "메모 #4", "비밀번호는 1-9-8-4.",                    0 },
    { "메모 #5", "누군가 이미 여기 다녀간 것 같다.",        0 },
};

int  selected  = 0;
int  note_open = 0;

void setCursorPos(int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void drawNoteList()
{
    system("cls");

    setCursorPos(0, 0); printf("========== 메모 목록 ==========");
    setCursorPos(0, 1); printf(" 방향키: 선택   Enter: 읽기   N: 닫기");
    setCursorPos(0, 2); printf("--------------------------------");

    for (int i = 0; i < NOTE_COUNT; i++)
    {
        setCursorPos(0, 3 + i);
        if (i == selected)
            printf(" > [%s] %s", notes[i].read ? "읽음" : "    ", notes[i].title);
        else
            printf("   [%s] %s", notes[i].read ? "읽음" : "    ", notes[i].title);
    }

    setCursorPos(0, 3 + NOTE_COUNT + 1);
    printf("================================");
}

void drawNoteContent(int index)
{
    system("cls");

    setCursorPos(0, 0); printf("========== %s ==========", notes[index].title);
    setCursorPos(0, 2); printf("  %s", notes[index].content);
    setCursorPos(0, 5); printf("--------------------------------");
    setCursorPos(0, 6); printf("  아무 키나 누르면 목록으로 돌아갑니다.");
}

int main()
{
    int  px = 10, py = 10;
    char key;

    while (1)
    {
        system("cls");

        if (note_open)
        {
            drawNoteList();

            int ch = _getch();

            if (ch == 0 || ch == 224) 
            {
                int arrow = _getch();
                if      (arrow == KEY_UP   && selected > 0)              selected--;
                else if (arrow == KEY_DOWN && selected < NOTE_COUNT - 1) selected++;
            }
            else if (ch == KEY_ENTER)
            {
                notes[selected].read = 1;
                drawNoteContent(selected);
                _getch();  // 아무 키나 누르면 목록으로
            }
            else if (ch == 'n' || ch == 'N')
            {
                note_open = 0;
            }
        }
        else
        {
            setCursorPos(px, py); printf("P");
            setCursorPos(0, 22);  printf("WASD: 이동  N: 메모 목록");

            key = _getch();

            if      (key == 'w') py--;
            else if (key == 's') py++;
            else if (key == 'a') px--;
            else if (key == 'd') px++;
            else if (key == 'n') { note_open = 1; selected = 0; }

            if (px < 0)  px = 0;
            if (px > 38) px = 38;
            if (py < 0)  py = 0;
            if (py > 19) py = 19;
        }
    }

    return 0;
}
