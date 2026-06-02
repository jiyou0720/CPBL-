#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define NUM_SLOTS 3

// ==========================
// Player 구조체
// ==========================
typedef struct {
    int hp;
    int maxHp;

    int x;
    int y;
    int floor;

    char roomName[50];

    int achievements[11];
} Player;


// ==========================
// SaveData
// ==========================
typedef struct {
    int slotExists[NUM_SLOTS];
    Player playerData[NUM_SLOTS];
} SaveData;


// ==========================
// 전역 변수
// ==========================
SaveData globalSave;
int currentSlot = 1;


// ==========================
// getch (Linux)
// ==========================
int getch_linux() {
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}


// ==========================
// sleep 대체 (ms)
// ==========================
void sleep_ms(int ms) {
    usleep(ms * 1000);
}


// ==========================
// 파일 로드
// ==========================
void loadSaveFile() {
    FILE* fp = fopen("save.dat", "rb");

    if (!fp) {
        memset(&globalSave, 0, sizeof(globalSave));
        return;
    }

    fread(&globalSave, sizeof(globalSave), 1, fp);
    fclose(fp);
}


// ==========================
// 파일 저장
// ==========================
void writeSaveFile() {
    FILE* fp = fopen("save.dat", "wb");

    if (!fp) {
        printf("저장 실패!\n");
        return;
    }

    fwrite(&globalSave, sizeof(globalSave), 1, fp);
    fclose(fp);
}


// ==========================
// 슬롯 저장
// ==========================
void saveToSlot(Player* p, int slot) {
    int idx = slot - 1;

    globalSave.slotExists[idx] = 1;
    globalSave.playerData[idx] = *p;

    writeSaveFile();

    printf("\n[저장 완료] Slot %d\n", slot);
    sleep_ms(800);
}


// ==========================
// 슬롯 불러오기
// ==========================
void loadFromSlot(Player* p, int slot) {
    int idx = slot - 1;

    if (!globalSave.slotExists[idx]) {
        printf("\n빈 슬롯입니다!\n");
        sleep_ms(800);
        return;
    }

    *p = globalSave.playerData[idx];
    currentSlot = slot;

    printf("\n[불러오기 완료] Slot %d\n", slot);
    sleep_ms(800);
}


// ==========================
// 슬롯 삭제
// ==========================
void deleteSlot(int slot) {
    int idx = slot - 1;

    if (!globalSave.slotExists[idx]) {
        printf("\n이미 빈 슬롯입니다.\n");
        sleep_ms(800);
        return;
    }

    globalSave.slotExists[idx] = 0;
    memset(&globalSave.playerData[idx], 0, sizeof(Player));

    writeSaveFile();

    printf("\n[삭제 완료] Slot %d\n", slot);
    sleep_ms(800);
}


// ==========================
// 슬롯 출력
// ==========================
void printSlotInfo() {
    printf("=====================================\n");
    printf("           저장 슬롯 목록\n");
    printf("=====================================\n");

    for (int i = 0; i < NUM_SLOTS; i++) {
        if (globalSave.slotExists[i]) {
            Player* p = &globalSave.playerData[i];

            printf("[%d] 저장 데이터 존재\n", i + 1);
            printf(" 위치 : %s\n", p->roomName);
            printf(" HP : %d / %d\n", p->hp, p->maxHp);
            printf(" 층수 : %dF\n\n", p->floor);
        } else {
            printf("[%d] 빈 슬롯\n\n", i + 1);
        }
    }
}


// ==========================
// 메뉴
// ==========================
void showSaveMenu(Player* p) {
    while (1) {

        system("clear");

        printSlotInfo();

        printf("=====================================\n");
        printf("1. 저장\n");
        printf("2. 불러오기\n");
        printf("3. 슬롯 삭제\n");
        printf("4. 종료\n");
        printf("=====================================\n");
        printf("선택 : ");

        char choice = getch_linux();

        if (choice == '1') {
            printf("\n슬롯 선택 (1~3): ");
            int slot = getch_linux() - '0';

            if (slot >= 1 && slot <= 3)
                saveToSlot(p, slot);
        }

        else if (choice == '2') {
            printf("\n슬롯 선택 (1~3): ");
            int slot = getch_linux() - '0';

            if (slot >= 1 && slot <= 3)
                loadFromSlot(p, slot);
        }

        else if (choice == '3') {
            printf("\n슬롯 선택 (1~3): ");
            int slot = getch_linux() - '0';

            if (slot >= 1 && slot <= 3)
                deleteSlot(slot);
        }

        else if (choice == '4') {
            break;
        }
    }
}


// ==========================
// main
// ==========================
int main() {
    Player player;

    loadSaveFile();

    player.hp = 100;
    player.maxHp = 100;
    player.x = 5;
    player.y = 5;
    player.floor = 1;
    strcpy(player.roomName, "1F Hallway");

    showSaveMenu(&player);

    return 0;
}
