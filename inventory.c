// HUD_INVENTORY - 인벤토리 담당
// E키 토글, 난이도별 슬롯 제한, 선택/사용/버리기 UI
// 아이템과 상호작용하면 자동 획득하도록 함

#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>

#define MAP_W            40
#define MAP_H            20
#define MAX_SLOTS_EASY   8
#define MAX_SLOTS_NORMAL 5
#define MAX_SLOTS_HARD   3
#define ITEM_COUNT       3  // 맵에 배치된 아이템 수

typedef struct {
    char name[32];
    int  empty;
} Item;

typedef struct {
    int  x, y;
    char name[32];
    int  picked;  // 이미 획득했으면 1
} MapItem;

// 난이도 (0=쉬움, 1=보통, 2=어려움)
int difficulty = 1;

Item    inventory[MAX_SLOTS_EASY];
int     selected  = 0;
int     inv_open  = 0;

// 맵에 배치된 아이템 (여기 있는 아이템들은 확인용, 나중에 (빈슬롯)으로 수정 예정. *확인용*)
MapItem map_items[ITEM_COUNT] = {
    {  5,  5, "손전등", 0 },
    { 15, 10, "붕대",   0 },
    { 30,  3, "열쇠",   0 },
};

void setCursorPos(int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int getMaxSlots()
{
    if (difficulty == 0) return MAX_SLOTS_EASY;
    if (difficulty == 2) return MAX_SLOTS_HARD;
    return MAX_SLOTS_NORMAL;
}

void initInventory()
{
    for (int i = 0; i < MAX_SLOTS_EASY; i++)
    {
        inventory[i].empty = 1;
        strcpy(inventory[i].name, "(빈 슬롯)");
    }
}

// 빈 슬롯에 아이템 추가
void addItem(const char* itemName)
{
    int max = getMaxSlots();
    for (int i = 0; i < max; i++)
    {
        if (inventory[i].empty)
        {
            strcpy(inventory[i].name, itemName);
            inventory[i].empty = 0;
            return;
        }
    }
}

// 플레이어 위치와 아이템 상호작용 시 자동 획득
void checkItemPickup(int px, int py)
{
    for (int i = 0; i < ITEM_COUNT; i++)
    {
        if (!map_items[i].picked &&
            map_items[i].x == px &&
            map_items[i].y == py)
        {
            addItem(map_items[i].name);
            map_items[i].picked = 1;
        }
    }
}

void useItem()
{
    if (inventory[selected].empty) return;
    inventory[selected].empty = 1;
    strcpy(inventory[selected].name, "(빈 슬롯)");
}

void dropItem()
{
    if (inventory[selected].empty) return;
    inventory[selected].empty = 1;
    strcpy(inventory[selected].name, "(빈 슬롯)");
}

void drawInventory()
{
    int max = getMaxSlots();
    const char* diff_str[] = { "쉬움", "보통", "어려움" };

    setCursorPos(0, 2);
    printf("========== 인벤토리 ==========\n");
    printf(" 난이도: %s  |  슬롯: %d칸\n", diff_str[difficulty], max);
    printf("------------------------------\n");

    for (int i = 0; i < max; i++)
    {
        if (i == selected)
            printf(" > [%d] %s\n", i + 1, inventory[i].name);
        else
            printf("   [%d] %s\n", i + 1, inventory[i].name);
    }

    printf("------------------------------\n");
    printf(" W/S: 선택   U: 사용   X: 버리기\n");
    printf(" E: 닫기\n");
    printf("==============================\n");
}

void drawMap(int px, int py)
{
    for (int ty = 0; ty < MAP_H; ty++)
    {
        for (int tx = 0; tx < MAP_W; tx++)
        {
            setCursorPos(tx, ty);

            if (tx == px && ty == py) { printf("P"); continue; }

            // 아이템 표시
            int drawn = 0;
            for (int i = 0; i < ITEM_COUNT; i++)
            {
                if (!map_items[i].picked &&
                    map_items[i].x == tx &&
                    map_items[i].y == ty)
                {
                    printf("I");
                    drawn = 1;
                    break;
                }
            }
            if (!drawn) printf(".");
        }
    }

    setCursorPos(0, MAP_H + 1);
    printf("WASD: 이동  E: 인벤토리  (I = 아이템)");
}

int main()
{
    initInventory();

    int  px = 10, py = 10;
    char key;

    while (1)
    {
        system("cls");

        if (inv_open)
        {
            drawInventory();

            key = _getch();
            int max = getMaxSlots();

            if      (key == 'w') { if (selected > 0)       selected--; }
            else if (key == 's') { if (selected < max - 1) selected++; }
            else if (key == 'u') useItem();
            else if (key == 'x') dropItem();
            else if (key == 'e') inv_open = 0;
        }
        else
        {
            drawMap(px, py);

            key = _getch();

            if      (key == 'w') py--;
            else if (key == 's') py++;
            else if (key == 'a') px--;
            else if (key == 'd') px++;
            else if (key == 'e') { inv_open = 1; selected = 0; }

            if (px < 0)      px = 0;
            if (px >= MAP_W) px = MAP_W - 1;
            if (py < 0)      py = 0;
            if (py >= MAP_H) py = MAP_H - 1;

            checkItemPickup(px, py);
        }
    }

    return 0;
}
