#include <stdio.h>
#include <string.h>

#define MAX_SLOT 3

typedef struct
{
    int x;
    int y;
    int floor;
    char roomName[50];
} Player;

typedef struct
{
    int slotExists[MAX_SLOT];
    Player playerData[MAX_SLOT];
} SaveData;

SaveData globalSave;

int currentSlot = 1;
int autoSaveEnabled = 1;

/* save.dat 불러오기 */
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

/* save.dat 저장 */
void writeSaveDatFile()
{
    FILE* fp = fopen("save.dat", "wb");

    if (fp == NULL)
    {
        printf("Save file open failed!\n");
        return;
    }

    fwrite(&globalSave, sizeof(SaveData), 1, fp);
    fclose(fp);
}

/* 슬롯 저장 */
void saveToSlot(Player* p, int slotIdx)
{
    int idx = slotIdx - 1;

    if (idx < 0 || idx >= MAX_SLOT)
        return;

    globalSave.slotExists[idx] = 1;
    globalSave.playerData[idx] = *p;

    writeSaveDatFile();
}

/* 슬롯 불러오기 */
int loadFromSlot(Player* p, int slotIdx)
{
    int idx = slotIdx - 1;

    if (idx < 0 || idx >= MAX_SLOT)
        return 0;

    if (!globalSave.slotExists[idx])
        return 0;

    *p = globalSave.playerData[idx];
    return 1;
}

/* 자동 저장 */
void autoSave(Player* p)
{
    if (!autoSaveEnabled)
        return;

    saveToSlot(p, currentSlot);

    printf("\n[Auto Save] Progress saved automatically!\n");
    fflush(stdout);
}

/* 방 이름 갱신 */
void updateRoomName(Player* p)
{
    char oldRoom[50];
    strcpy(oldRoom, p->roomName);

    if (p->floor == 1)
    {
        if (p->x >= 1 && p->x <= 13 &&
            p->y >= 1 && p->y <= 3)
        {
            strcpy(p->roomName, "1F Bedroom 1");
        }
        else if (p->x >= 14 && p->x <= 25 &&
                 p->y >= 1 && p->y <= 3)
        {
            strcpy(p->roomName, "1F Bedroom 2");
        }
        else
        {
            strcpy(p->roomName, "1F Hallway");
        }
    }

    if (strcmp(oldRoom, p->roomName) != 0)
    {
        autoSave(p);
    }
}

/* 플레이어 이동 */
void movePlayer(Player* p, int dx, int dy)
{
    p->x += dx;
    p->y += dy;

    updateRoomName(p);
}

/* 자동 저장 ON/OFF */
void toggleAutoSave()
{
    autoSaveEnabled = !autoSaveEnabled;

    printf("\nAuto Save : %s\n",
           autoSaveEnabled ? "ON" : "OFF");
}

int main()
{
    Player player = {5, 2, 1, "1F Bedroom 1"};

    loadSaveDatFile();

    currentSlot = 1;

    printf("Current Room : %s\n", player.roomName);

    movePlayer(&player, 15, 0);

    printf("Current Room : %s\n", player.roomName);

    toggleAutoSave();

    movePlayer(&player, -20, 0);

    printf("Current Room : %s\n", player.roomName);

    return 0;
}
