// CTRL_BAG - 가방 열기
// 필수 | E키: 인벤 윈도우 토글, 열린 상태에서 방향키로 선택

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <conio.h>

// 고정 슬롯: 20개
#define SLOT_HOLY_WATER  0
#define SLOT_HAMMER      1
#define SLOT_POTION      2
#define SLOT_KEY_SECRET  3
#define SLOT_KEY_STORAGE 4
#define SLOT_DIARY_START 5   // 5 ~ 19
#define MAX_ITEMS        20

#define KEY_UP   72
#define KEY_DOWN 80

typedef struct {
    char name[32];
    int  count;
} Item;

void setCursorPos(int x, int y)
{
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void drawInventory(int is_open, int selected_slot, Item items[MAX_ITEMS])
{
    if (!is_open)
    {
        for (int i = 0; i < MAX_ITEMS + 6; i++)
        {
            setCursorPos(45, 3 + i);
            printf("                              ");
        }
        return;
    }

    int start_y = 3;

    setCursorPos(45, start_y);
    printf("┌──── INVENTORY ────┐");

    for (int i = 0; i < MAX_ITEMS; i++)
    {
        setCursorPos(45, start_y + 1 + i);
        if (items[i].count == 0)
        {
            if (i == selected_slot)
                printf("│  > [%2d] %-12s│", i + 1, "비어있음");
            else
                printf("│    [%2d] %-12s│", i + 1, "비어있음");
        }
        else
        {
            char label[32];
            if (items[i].count > 1)
                sprintf(label, "%s x%d", items[i].name, items[i].count);
            else
                strcpy(label, items[i].name);

            if (i == selected_slot)
                printf("│  > [%2d] %-12s│", i + 1, label);
            else
                printf("│    [%2d] %-12s│", i + 1, label);
        }
    }

    setCursorPos(45, start_y + 1 + MAX_ITEMS);
    printf("├───────────────────┤");
    setCursorPos(45, start_y + 2 + MAX_ITEMS);
    printf("│ 방향키: 선택       │");
    setCursorPos(45, start_y + 3 + MAX_ITEMS);
    printf("│ Enter: 사용/읽기   │");
    setCursorPos(45, start_y + 4 + MAX_ITEMS);
    printf("└──── E: 닫기   ────┘");
}

void useItem(Item items[MAX_ITEMS], int slot, int* hp, int max_hp)
{
    if (items[slot].count == 0) return;

    setCursorPos(0, 23);

    if (slot == SLOT_POTION)
    {
        *hp += 3;
        if (*hp > max_hp) *hp = max_hp;
        items[slot].count--;
        printf("HP 회복제를 사용했습니다. (HP +3)          ");
    }
    else if (slot == SLOT_HOLY_WATER)
    {
        printf("성수를 사용했습니다.                        ");
    }
    else if (slot == SLOT_HAMMER)
    {
        printf("망치를 사용했습니다.                        ");
    }
    else if (slot == SLOT_KEY_SECRET || slot == SLOT_KEY_STORAGE)
    {
        printf("[%s] 을(를) 선택했습니다.          ", items[slot].name);
    }
    else if (slot >= SLOT_DIARY_START && slot < MAX_ITEMS)
    {
        printf("일기를 읽습니다: [%s]               ", items[slot].name);
    }
}

// 아이템 획득 함수 (맵에서 밟으면 호출)
void pickupItem(Item items[MAX_ITEMS], int slot)
{
    if (slot == SLOT_DIARY_START)
    {
        // 일기는 빈 슬롯 찾아서 순서대로 추가
        for (int i = SLOT_DIARY_START; i < MAX_ITEMS; i++)
        {
            if (items[i].count == 0)
            {
                sprintf(items[i].name, "일기 #%d", i - SLOT_DIARY_START + 1);
                items[i].count = 1;
                return;
            }
        }
    }
    else
    {
        if (items[slot].count < 15)
            items[slot].count++;
    }
}

int main()
{
    int px = 10, py = 10;
    int hp = 50, max_hp = 100;
    int inv_open = 0;
    int selected_slot = 0;

    Item inventory[MAX_ITEMS] = {
        {"성수",         0},
        {"망치",         0},
        {"HP 회복제",    0},
        {"비밀방 열쇠",  0},
        {"창고 열쇠",    0},
        {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
        {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
        {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}
    };

    char key;
    while (1)
    {
        system("cls");

        setCursorPos(px, py);
        printf("P");

        setCursorPos(0, 0);
        printf("HP: %d / %d", hp, max_hp);

        drawInventory(inv_open, selected_slot, inventory);

        key = _getch();

        if (inv_open)
        {
            if (key == 0 || key == (char)224)
            {
                char arrow = _getch();
                if (arrow == KEY_UP && selected_slot > 0)              selected_slot--;
                if (arrow == KEY_DOWN && selected_slot < MAX_ITEMS - 1)  selected_slot++;
            }
            else if (key == 13)
                useItem(inventory, selected_slot, &hp, max_hp);
            else if (key == 'e' || key == 'E')
                inv_open = 0;
        }
        else
        {
            if (key == 'w' || key == 'W') py--;
            else if (key == 's' || key == 'S') py++;
            else if (key == 'a' || key == 'A') px--;
            else if (key == 'd' || key == 'D') px++;
            else if (key == 'e' || key == 'E') inv_open = 1;
            // 테스트용: E키로 아이템 획득 시뮬레이션
            else if (key == 't' || key == 'T') pickupItem(inventory, SLOT_POTION);
        }

        if (px < 0)  px = 0;
        if (px > 38) px = 38;
        if (py < 0)  py = 0;
        if (py > 20) py = 20;
    }
    return 0;
}
