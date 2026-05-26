#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <ctype.h>

#define MAX_ITEMS 5
#define MAP_WIDTH 50
#define MAP_HEIGHT 15
#define MAX_FLOORS 2 
#define NUM_SLOTS 3 
#define NUM_ROOMS_PER_FLOOR 12

// ==========================
// Structures
// ==========================
typedef struct
{
    int hp;
    int maxHp;
    int x;
    int y;
    int floor;
    char roomName[50];
    char items[MAX_ITEMS][30];
    int visitedRooms[MAX_FLOORS][NUM_ROOMS_PER_FLOOR];
} Player;

typedef struct
{
    int slotExists[NUM_SLOTS];
    Player playerData[NUM_SLOTS];
} SaveData;

// Global Variables
int currentSlot = 1;
SaveData globalSave;

// Room names for Minimap rendering
const char* roomNames1F[NUM_ROOMS_PER_FLOOR] = {
    "1F Bedroom 1", "1F Main Stairs", "1F Bedroom 2",
    "1F Storage", "1F Bathroom (L)", "1F Bathroom (R)",
    "1F Small Room", "1F Dressroom", "1F Servant Room",
    "1F Master Room", "1F Terrace", "1F Hallway"
};

const char* roomNames2F[NUM_ROOMS_PER_FLOOR] = {
    "2F Bedroom 1", "2F Main Stairs", "2F Bedroom 2",
    "2F Office", "2F Library", "2F Laboratory",
    "2F Art Room", "2F Gallery", "2F Corridor",
    "2F Attic Room", "2F Balcony", "2F Hallway"
};

// ==========================
// Map Data
// ==========================
char map[MAX_FLOORS][MAP_HEIGHT][MAP_WIDTH + 1] =
{
    {
        "##################################################",
        "# BED1        #      STAIRS      #      BED2      #",
        "#             #                  #                #",
        "#             .                  .                #",
        "#######.#######                  ##########.#######",
        "#STOR # BATH #                  # BATH # SMALL #",
        "#     #      .                  .      #       #",
        "########.######                  #######.##########",
        "# DRESSROOM   #                  # SERVANT ROOM #",
        "#             .                  .              #",
        "#######.#######                  #######.##########",
        "# MASTER ROOM #                  #   TERRACE    #",
        "#             .                  .              #",
        "#             #                  #              #",
        "##################################################"
    },
    {
        "##################################################",
        "# 2F_BED1     #     2F_STAIRS    #     2F_BED2    #",
        "#             #                  #                #",
        "#             .                  .                #",
        "#######.#######                  ##########.#######",
        "#OFFIC# LIBRARY#                  # LAB  # ART   #",
        "#     #      .                  .      #       #",
        "########.######                  #######.##########",
        "# GALLERY     #                  # CORRIDOR     #",
        "#             .                  .              #",
        "#######.#######                  #######.##########",
        "# ATTIC ROOM  #                  #   BALCONY    #",
        "#             .                  .              #",
        "#             #                  #              #",
        "##################################################"
    }
};

// Forward Declarations
void drawMap(Player* p);
void drawMinimap(Player* p);
void drawUI(Player* p);
void saveToSlot(Player* p, int slotIdx);

// ==========================
// Utilities
// ==========================
void gotoxy(int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void hideCursor()
{
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// ==========================
// Save System File IO
// ==========================
void loadSaveDatFile()
{
    FILE* fp = fopen("save.dat", "rb");
    if (fp == NULL)
    {
        memset(&globalSave, 0, sizeof(SaveData));
        return;
    }
    fread(&globalSave, sizeof(SaveData), 1, fp);
    fclose(fp);
}

void writeSaveDatFile()
{
    FILE* fp = fopen("save.dat", "wb");
    if (fp == NULL) return;
    fwrite(&globalSave, sizeof(SaveData), 1, fp);
    fclose(fp);
}

void saveToSlot(Player* p, int slotIdx)
{
    int idx = slotIdx - 1;
    globalSave.slotExists[idx] = 1;
    globalSave.playerData[idx] = *p;
    writeSaveDatFile();
}

void deleteSlot(int slotIdx)
{
    int idx = slotIdx - 1;
    globalSave.slotExists[idx] = 0;
    memset(&globalSave.playerData[idx], 0, sizeof(Player));
    writeSaveDatFile();
}

// ==========================
// Game Logic
// ==========================
void updateRoomName(Player* p)
{
    char oldRoom[50];
    strcpy(oldRoom, p->roomName);
    int roomIdx = 11;

    if (p->floor == 1)
    {
        if (p->x >= 1 && p->x <= 13 && p->y >= 1 && p->y <= 3) { strcpy(p->roomName, "1F Bedroom 1"); roomIdx = 0; }
        else if (p->x >= 14 && p->x <= 34 && p->y >= 1 && p->y <= 13) { strcpy(p->roomName, "1F Main Stairs"); roomIdx = 1; }
        else if (p->x >= 35 && p->x <= 48 && p->y >= 1 && p->y <= 3) { strcpy(p->roomName, "1F Bedroom 2"); roomIdx = 2; }
        else if (p->x >= 1 && p->x <= 6 && p->y >= 5 && p->y <= 6) { strcpy(p->roomName, "1F Storage"); roomIdx = 3; }
        else if (p->x >= 8 && p->x <= 13 && p->y >= 5 && p->y <= 6) { strcpy(p->roomName, "1F Bathroom (L)"); roomIdx = 4; }
        else if (p->x >= 35 && p->x <= 40 && p->y >= 5 && p->y <= 6) { strcpy(p->roomName, "1F Bathroom (R)"); roomIdx = 5; }
        else if (p->x >= 42 && p->x <= 48 && p->y >= 5 && p->y <= 6) { strcpy(p->roomName, "1F Small Room"); roomIdx = 6; }
        else if (p->x >= 1 && p->x <= 13 && p->y >= 8 && p->y <= 9) { strcpy(p->roomName, "1F Dressroom"); roomIdx = 7; }
        else if (p->x >= 35 && p->x <= 48 && p->y >= 8 && p->y <= 9) { strcpy(p->roomName, "1F Servant Room"); roomIdx = 8; }
        else if (p->x >= 1 && p->x <= 13 && p->y >= 11 && p->y <= 13) { strcpy(p->roomName, "1F Master Room"); roomIdx = 9; }
        else if (p->x >= 35 && p->x <= 48 && p->y >= 11 && p->y <= 13) { strcpy(p->roomName, "1F Terrace"); roomIdx = 10; }
        else { strcpy(p->roomName, "1F Hallway"); roomIdx = 11; }
    }
    else if (p->floor == 2)
    {
        if (p->x >= 1 && p->x <= 13 && p->y >= 1 && p->y <= 3) { strcpy(p->roomName, "2F Bedroom 1"); roomIdx = 0; }
        else if (p->x >= 14 && p->x <= 34 && p->y >= 1 && p->y <= 13) { strcpy(p->roomName, "2F Main Stairs"); roomIdx = 1; }
        else if (p->x >= 35 && p->x <= 48 && p->y >= 1 && p->y <= 3) { strcpy(p->roomName, "2F Bedroom 2"); roomIdx = 2; }
        else if (p->x >= 1 && p->x <= 6 && p->y >= 5 && p->y <= 6) { strcpy(p->roomName, "2F Office"); roomIdx = 3; }
        else if (p->x >= 8 && p->x <= 13 && p->y >= 5 && p->y <= 6) { strcpy(p->roomName, "2F Library"); roomIdx = 4; }
        else if (p->x >= 35 && p->x <= 40 && p->y >= 5 && p->y <= 6) { strcpy(p->roomName, "2F Laboratory"); roomIdx = 5; }
        else if (p->x >= 42 && p->x <= 48 && p->y >= 5 && p->y <= 6) { strcpy(p->roomName, "2F Art Room"); roomIdx = 6; }
        else if (p->x >= 1 && p->x <= 13 && p->y >= 8 && p->y <= 9) { strcpy(p->roomName, "2F Gallery"); roomIdx = 7; }
        else if (p->x >= 35 && p->x <= 48 && p->y >= 8 && p->y <= 9) { strcpy(p->roomName, "2F Corridor"); roomIdx = 8; }
        else if (p->x >= 1 && p->x <= 13 && p->y >= 11 && p->y <= 13) { strcpy(p->roomName, "2F Attic Room"); roomIdx = 9; }
        else if (p->x >= 35 && p->x <= 48 && p->y >= 11 && p->y <= 13) { strcpy(p->roomName, "2F Balcony"); roomIdx = 10; }
        else { strcpy(p->roomName, "2F Hallway"); roomIdx = 11; }
    }

    p->visitedRooms[p->floor - 1][roomIdx] = 1;

    if (strcmp(oldRoom, p->roomName) != 0)
    {
        saveToSlot(p, currentSlot);
        gotoxy(0, MAP_HEIGHT + 18);
        printf("[Auto Save] Progress saved to Slot %d.          \n", currentSlot);
    }
}

int addItem(Player* p, const char* itemName)
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (strcmp(p->items[i], itemName) == 0) return 0;
    }
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (strlen(p->items[i]) == 0)
        {
            strcpy(p->items[i], itemName);
            return 1;
        }
    }
    return -1;
}

void checkItemEvent(Player* p)
{
    if (p->floor == 1)
    {
        if (p->x == 2 && p->y == 5)
        {
            int result = addItem(p, "Hammer");
            if (result == 1)
            {
                gotoxy(0, MAP_HEIGHT + 18);
                printf("[EVENT] Found a 'Hammer' in the Storage!       \n");
                saveToSlot(p, currentSlot);
                Sleep(1200);
            }
        }
        if (p->x == 45 && p->y == 5)
        {
            int result = addItem(p, "Holy Water");
            if (result == 1)
            {
                gotoxy(0, MAP_HEIGHT + 18);
                printf("[EVENT] Found 'Holy Water' on the desk!       \n");
                saveToSlot(p, currentSlot);
                Sleep(1200);
            }
        }
    }
}

void checkStairsEvent(Player* p)
{
    if (p->y == 1 && p->x >= 21 && p->x <= 26)
    {
        system("cls");
        gotoxy(0, MAP_HEIGHT + 18);

        if (p->floor == 1)
        {
            p->floor = 2;
            printf("[STAIRS] Moving up to the 2nd Floor...            \n");
        }
        else
        {
            p->floor = 1;
            printf("[STAIRS] Moving down to the 1st Floor...          \n");
        }

        p->y = 2;

        Sleep(1000);
        system("cls");
        updateRoomName(p);
        saveToSlot(p, currentSlot);
    }
}

// ==========================
// Menus
// ==========================
void showSaveManagerMenu(Player* p)
{
    system("cls");
    while (1)
    {
        printf("==================================================\n");
        printf("               SAVE MANAGEMENT MENU               \n");
        printf("==================================================\n");
        printf("  [Current Active Slot: Slot %d]\n\n", currentSlot);
        printf("  1. Save Game (Overwrite Current Slot)\n");
        printf("  2. Load Game (Select & Switch Slot)\n");
        printf("  3. Delete Save Slot Data\n");
        printf("  4. Resume Game\n");
        printf("==================================================\n");
        printf(" Choose an option: ");

        char choice = _getch();
        printf("\n");

        if (choice == '1')
        {
            saveToSlot(p, currentSlot);
            printf("\n   [SUCCESS] Game saved to Slot %d!\n", currentSlot);
            Sleep(1200);
            break;
        }
        else if (choice == '2')
        {
            printf("\n Select Slot to Load (1, 2, 3): ");
            char slotCh = _getch();
            int targetSlot = slotCh - '0';

            if (targetSlot >= 1 && targetSlot <= 3)
            {
                if (globalSave.slotExists[targetSlot - 1] == 1)
                {
                    currentSlot = targetSlot;
                    *p = globalSave.playerData[targetSlot - 1];
                    printf("\n   [SUCCESS] Loaded Slot %d data!\n", currentSlot);
                }
                else
                {
                    printf("\n   [FAILED] Slot %d is empty!\n", targetSlot);
                }
            }
            Sleep(1200);
            break;
        }
        else if (choice == '3')
        {
            printf("\n Select Slot to Delete (1, 2, 3): ");
            char slotCh = _getch();
            int targetSlot = slotCh - '0';

            if (targetSlot >= 1 && targetSlot <= 3)
            {
                if (globalSave.slotExists[targetSlot - 1] == 1)
                {
                    deleteSlot(targetSlot);
                    printf("\n   [DELETED] Slot %d data erased.\n", targetSlot);
                }
                else
                {
                    printf("\n   [INFO] Slot %d is already empty.\n", targetSlot);
                }
            }
            Sleep(1200);
        }
        else if (choice == '4')
        {
            break;
        }
        system("cls");
    }

    system("cls");
    drawMap(p);
    drawMinimap(p);
    drawUI(p);
}

// ==========================
// Rendering Functions
// ==========================
void drawHPBar(Player* p)
{
    int barLength = 20;
    int filled = (p->hp * barLength) / p->maxHp;

    printf("HP : [");
    for (int i = 0; i < filled; i++) printf("#");
    for (int i = filled; i < barLength; i++) printf("-");
    printf("] %d/%d\n", p->hp, p->maxHp);
}

void drawInventory(Player* p)
{
    printf("Items : ");
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (strlen(p->items[i]) > 0) printf("[%s] ", p->items[i]);
        else printf("[Empty] ");
    }
    printf("\n");
}

void drawMinimap(Player* p)
{
    int f = p->floor - 1;
    printf("--- [ %dF MANSION MINIMAP ] ---\n", p->floor);

    for (int row = 0; row < 4; row++)
    {
        printf(" ");
        for (int col = 0; col < 3; col++)
        {
            int idx = row * 3 + col;
            if (idx >= NUM_ROOMS_PER_FLOOR) break;

            const char* name = (f == 0) ? roomNames1F[idx] : roomNames2F[idx];
            int isCurrent = (strcmp(p->roomName, name) == 0);

            if (p->visitedRooms[f][idx] == 1)
            {
                if (isCurrent) printf("[ P: %-13s ] ", name);
                else           printf("[ %-16s ] ", name);
            }
            else
            {
                printf("[ ? - Unknown Room ] ");
            }
        }
        printf("\n");
    }
}

void drawUI(Player* p)
{
    printf("==================================================\n");
    printf("Active Slot      : [ Slot %d ]\n", currentSlot);
    printf("Current Floor    : %dF\n", p->floor);
    printf("Current Location : %s\n", p->roomName);
    printf("Player Position  : (%d, %d)\n", p->x, p->y);
    drawHPBar(p);
    drawInventory(p);
    printf("==================================================\n");
    printf("WASD : Move | M : Save/Load Menu | Q : Quit Game\n");
}

void drawMap(Player* p)
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (x == p->x && y == p->y) printf("P");
            else printf("%c", map[p->floor - 1][y][x]);
        }
        printf("\n");
    }
}

void movePlayer(Player* p, char key)
{
    int nextX = p->x;
    int nextY = p->y;
    char lowerKey = tolower(key);

    if (lowerKey == 'w') nextY--;
    else if (lowerKey == 's') nextY++;
    else if (lowerKey == 'a') nextX--;
    else if (lowerKey == 'd') nextX++;
    else return;

    if (nextX < 0 || nextX >= MAP_WIDTH || nextY < 0 || nextY >= MAP_HEIGHT) return;
    if (map[p->floor - 1][nextY][nextX] == '#') return;

    p->x = nextX;
    p->y = nextY;

    updateRoomName(p);
    checkItemEvent(p);
    checkStairsEvent(p);
}

void showTitleMenu(Player* p)
{
    system("cls");
    loadSaveDatFile();

    printf("==================================================\n");
    printf("           MYSTERY MANSION HORROR RPG             \n");
    printf("==================================================\n\n");
    printf(" Select a Save Slot to Start:\n\n");

    for (int i = 1; i <= NUM_SLOTS; i++)
    {
        if (globalSave.slotExists[i - 1] == 1)
        {
            printf("  [%d] Slot %d - (Floor: %dF / Loc: %s)\n",
                i, i, globalSave.playerData[i - 1].floor, globalSave.playerData[i - 1].roomName);
        }
        else
        {
            printf("  [%d] Slot %d - (Empty - New Game)\n", i, i);
        }
    }
    printf("\n==================================================\n");
    printf(" Press [1, 2, 3] to select slot, or [Q] to quit: ");

    while (1)
    {
        char ch = _getch();
        if (ch == '1' || ch == '2' || ch == '3')
        {
            currentSlot = ch - '0';
            break;
        }
        else if (ch == 'q' || ch == 'Q') exit(0);
    }

    if (globalSave.slotExists[currentSlot - 1] == 1)
    {
        *p = globalSave.playerData[currentSlot - 1];
        system("cls");
        printf("\n\n   [LOAD SUCCESS] Welcome back! Entering Slot %d...\n", currentSlot);
        Sleep(1500);
    }
    else
    {
        p->hp = 100;
        p->maxHp = 100;
        p->x = 4;
        p->y = 2;
        p->floor = 1;
        strcpy(p->roomName, "1F Bedroom 1");

        memset(p->visitedRooms, 0, sizeof(p->visitedRooms));
        p->visitedRooms[0][0] = 1;

        strcpy(p->items[0], "Key");
        strcpy(p->items[1], "Potion");
        for (int i = 2; i < MAX_ITEMS; i++) p->items[i][0] = '\0';

        system("cls");
        printf("\n\n   [NEW GAME] Initializing fresh story in Slot %d...\n", currentSlot);
        saveToSlot(p, currentSlot);
        Sleep(1500);
    }
}

// ==========================
// Main Function
// ==========================
int main()
{
    // 💡 1. UTF-8 인코딩 설정 및 콘솔 창 크기를 세로로 넉넉하게 고정 (스크롤 밀림 차단)
    system("chcp 65001");
    system("mode con cols=100 lines=45");
    system("cls");

    Player player;
    hideCursor();

    showTitleMenu(&player);

    system("cls");
    drawMap(&player);
    drawMinimap(&player);
    drawUI(&player);

    while (1)
    {
        if (player.hp <= 0)
        {
            printf("GAME OVER\n");
            break;
        }

        int inputKey = _getch();

        if (inputKey == 'q' || inputKey == 'Q') break;

        if (inputKey == 'm' || inputKey == 'M')
        {
            showSaveManagerMenu(&player);
            continue;
        }

        movePlayer(&player, (char)inputKey);

        // 💡 2. 매 턴마다 화면 전체를 깨끗하게 밀고 새로 그리는 구조로 변경하여 잔상/복사 완벽 방지
        system("cls");
        drawMap(&player);
        drawMinimap(&player);
        drawUI(&player);
    }

    return 0;
}
