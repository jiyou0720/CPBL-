#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define MAX_OBJECTS   20
#define MAX_NAME      50
#define MAX_CLUE      200
#define INSPECT_RANGE 1

typedef struct {
    int  id;
    int  floor;
    int  x, y;
    char name[MAX_NAME];
    char clue[MAX_CLUE];
} InspectObject;

InspectObject inspectList[MAX_OBJECTS];
int           inspectCount = 0;

void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Load_InspectData(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("[warning] cannot open %s\n", filename);
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp) && inspectCount < MAX_OBJECTS) {
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0') continue;

        char* token = strtok(line, ",");
        if (!token) continue;
        int id = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        char name[MAX_NAME];
        strncpy(name, token, MAX_NAME - 1);
        name[MAX_NAME - 1] = '\0';

        // 단서 텍스트는 쉼표 포함 가능
        token = strtok(NULL, "\n");
        if (!token) continue;
        char clue[MAX_CLUE];
        strncpy(clue, token, MAX_CLUE - 1);
        clue[MAX_CLUE - 1] = '\0';

        inspectList[inspectCount].id = id;
        inspectList[inspectCount].floor = 0;
        inspectList[inspectCount].x = 0;
        inspectList[inspectCount].y = 0;
        strncpy(inspectList[inspectCount].name, name, MAX_NAME - 1);
        strncpy(inspectList[inspectCount].clue, clue, MAX_CLUE - 1);
        inspectCount++;
    }
    fclose(fp);
}

void Show_CluePopup(const char* objectName, const char* clueText) {
    int popY = 20;
    int popX = 2;

    gotoxy(popX, popY);
    printf("+-------------------------------------------------+");
    gotoxy(popX, popY + 1);
    printf("|  [ %s ]", objectName);
    int nameLen = (int)strlen(objectName) + 6;
    for (int i = nameLen; i < 50; i++) printf(" ");
    printf("|");
    gotoxy(popX, popY + 2);
    printf("|                                                 |");
    gotoxy(popX, popY + 3);
    printf("|  ");

    int len = (int)strlen(clueText);
    int lineWidth = 45;
    if (len <= lineWidth) {
        printf("%-45s", clueText);
    }
    else {
        char line1[50];
        strncpy(line1, clueText, lineWidth);
        line1[lineWidth] = '\0';
        printf("%-45s", line1);
        gotoxy(popX, popY + 4);
        printf("|  %-45s", clueText + lineWidth);
    }
    printf("  |");

    gotoxy(popX, popY + 5);
    printf("|                                                 |");
    gotoxy(popX, popY + 6);
    printf("|         [ press any key to continue... ]        |");
    gotoxy(popX, popY + 7);
    printf("+-------------------------------------------------+");

    _getch();

    for (int i = popY; i <= popY + 7; i++) {
        gotoxy(popX, i);
        printf("                                                   ");
    }
}

void INSPECT_SetPos(int id, int floor, int x, int y) {
    for (int i = 0; i < inspectCount; i++) {
        if (inspectList[i].id == id) {
            inspectList[i].floor = floor;
            inspectList[i].x = x;
            inspectList[i].y = y;
            return;
        }
    }
}

int INSPECT_Check(int pX, int pY, int floor) {
    for (int i = 0; i < inspectCount; i++) {
        if (inspectList[i].floor != 0 && inspectList[i].floor != floor)
            continue;

        int dx = pX - inspectList[i].x;
        int dy = pY - inspectList[i].y;
        if (dx < -INSPECT_RANGE || dx > INSPECT_RANGE) continue;
        if (dy < -INSPECT_RANGE || dy > INSPECT_RANGE) continue;

        Show_CluePopup(inspectList[i].name, inspectList[i].clue);
        return 1;
    }
    return 0;
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    Load_InspectData("inspect.dat");

    INSPECT_SetPos(1, 1, 30, 13);
    INSPECT_SetPos(2, 1, 45, 14);
    INSPECT_SetPos(3, 1, 30, 13);
    INSPECT_SetPos(4, 1, 48, 8);
    INSPECT_SetPos(5, 2, 10, 9);

    printf("=== inspect system test ===\n");
    printf("%d objects loaded\n\n", inspectCount);
    Sleep(1000);

    int result = INSPECT_Check(30, 13, 1);
    if (!result) printf("nothing to inspect nearby.\n");

    return 0;
}
