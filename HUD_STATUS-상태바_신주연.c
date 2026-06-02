#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

/* 화면 지우기 */
void clearScreen()
{
    // 리눅스 터미널 clear
    printf("\033[H\033[J");
}

/* 상태바 출력 */
void drawStatusBar(Player* p)
{
    printf("\n==============================================================\n");

    printf("Slot : %d\n", currentSlot);

    printf("Room : %s (%dF)\n", p->roomName, p->floor);

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

    printf("HP   : [%s] %d/%d\n", hpBar, p->hp, p->maxHp);

    printf("Hand : %s\n", p->holdingItem);

    printf("Bag  : ");

    int hasItem = 0;

    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (strcmp(p->items[i], "Empty") != 0)
        {
            printf("[%s] ", p->items[i]);
            hasItem = 1;
        }
    }

    if (!hasItem)
        printf("(Empty)");

    printf("\n==============================================================\n");
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

    while (1)
    {
        clearScreen();

        printf("GAME MAP AREA\n");
        printf("Player Position\n\n");

        drawStatusBar(&player);

        printf("\n[h] HP -5 | [j] HP +5 | [q] quit\n");
        printf("Input : ");

        char ch;
        scanf(" %c", &ch);

        if (ch == 'q')
            break;

        if (ch == 'h' && player.hp > 0)
            player.hp -= 5;

        if (ch == 'j' && player.hp < player.maxHp)
            player.hp += 5;
    }

    return 0;
}
