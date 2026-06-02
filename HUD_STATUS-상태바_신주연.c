#include <stdio.h>
#include <string.h>
#include <ncurses.h>

#define MAX_ITEMS 10

typedef struct
{
    int hp;
    int maxHp;

    int floor;

    char roomName[50];
    char holdingItem[50];

    char items[MAX_ITEMS][50];

} Player;

int currentSlot = 1;

/* 상태바 출력 */
void drawStatusBar(Player* p)
{
    int startRow = LINES - 7;

    /* 상태바 영역 지우기 */
    for (int i = 0; i < 7; i++)
    {
        move(startRow + i, 0);
        clrtoeol();
    }

    mvprintw(startRow, 0,
             "==============================================================");

    mvprintw(startRow + 1, 0,
             "Slot : %d",
             currentSlot);

    /* 현재 방 */
    mvprintw(startRow + 2, 0,
             "Room : %s (%dF)",
             p->roomName,
             p->floor);

    /* HP 바 */
    int filled = (p->hp * 20) / p->maxHp;

    char hpBar[21];

    for (int i = 0; i < 20; i++)
    {
        if (i < filled)
            hpBar[i] = '#';
        else
            hpBar[i] = '-';
    }

    hpBar[20] = '\0';

    mvprintw(startRow + 3, 0,
             "HP   : [%s] %d/%d",
             hpBar,
             p->hp,
             p->maxHp);

    /* 손에 든 아이템 */
    mvprintw(startRow + 4, 0,
             "Hand : %s",
             p->holdingItem);

    /* 인벤토리 */
    move(startRow + 5, 0);
    printw("Bag  : ");

    int hasItem = 0;

    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (strcmp(p->items[i], "Empty") != 0)
        {
            printw("[%s] ", p->items[i]);
            hasItem = 1;
        }
    }

    if (!hasItem)
        printw("(Empty)");

    mvprintw(startRow + 6, 0,
             "==============================================================");

    refresh();
}

int main()
{
    Player player;

    player.hp = 75;
    player.maxHp = 100;

    player.floor = 1;

    strcpy(player.roomName, "1F Bedroom 1");
    strcpy(player.holdingItem, "Key");

    for (int i = 0; i < MAX_ITEMS; i++)
        strcpy(player.items[i], "Empty");

    strcpy(player.items[0], "Flashlight");
    strcpy(player.items[1], "Medicine");

    /* ncurses 시작 */
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    while (1)
    {
        clear();

        /* 맵 영역 예시 */
        mvprintw(1, 1, "GAME MAP AREA");
        mvprintw(2, 1, "Player Position");

        drawStatusBar(&player);

        int ch = getch();

        if (ch == 'q')
            break;

        if (ch == 'h' && player.hp > 0)
            player.hp -= 5;

        if (ch == 'j' && player.hp < player.maxHp)
            player.hp += 5;
    }

    endwin();

    return 0;
}
