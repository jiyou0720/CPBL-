// 게임 데이터 저장 공간 3개 지원

#define NUM_SLOTS 3

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
// SaveData 구조체
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
// save.dat 파일 불러오기
// ==========================

void loadSaveFile() {

    FILE* fp = fopen("save.dat", "rb");

    // 파일 없으면 초기화
    if (fp == NULL) {

        memset(&globalSave, 0, sizeof(SaveData));
        return;
    }

    fread(&globalSave, sizeof(SaveData), 1, fp);

    fclose(fp);
}


// ==========================
// save.dat 파일 저장
// ==========================

void writeSaveFile() {

    FILE* fp = fopen("save.dat", "wb");

    if (fp == NULL) {

        printf("저장 실패!\n");
        return;
    }

    fwrite(&globalSave, sizeof(SaveData), 1, fp);

    fclose(fp);
}


// ==========================
// 특정 슬롯에 저장
// ==========================

void saveToSlot(Player* p, int slot) {

    int idx = slot - 1;

    // 슬롯 존재 표시
    globalSave.slotExists[idx] = 1;

    // 플레이어 데이터 저장
    globalSave.playerData[idx] = *p;

    // 파일 기록
    writeSaveFile();

    printf("\n[저장 완료] Slot %d\n", slot);

    Sleep(1000);
}


// ==========================
// 특정 슬롯 불러오기
// ==========================

void loadFromSlot(Player* p, int slot) {

    int idx = slot - 1;

    if (globalSave.slotExists[idx] == 0) {

        printf("\n빈 슬롯입니다!\n");

        Sleep(1000);
        return;
    }

    // 데이터 복사
    *p = globalSave.playerData[idx];

    currentSlot = slot;

    printf("\n[불러오기 완료] Slot %d\n", slot);

    Sleep(1000);
}


// ==========================
// 슬롯 삭제
// ==========================

void deleteSlot(int slot) {

    int idx = slot - 1;

    if (globalSave.slotExists[idx] == 0) {

        printf("\n이미 빈 슬롯입니다.\n");

        Sleep(1000);
        return;
    }

    // 슬롯 초기화
    globalSave.slotExists[idx] = 0;

    memset(&globalSave.playerData[idx],
           0,
           sizeof(Player));

    writeSaveFile();

    printf("\n[삭제 완료] Slot %d\n", slot);

    Sleep(1000);
}


// ==========================
// 저장 슬롯 정보 출력
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
        }
        else {

            printf("[%d] 빈 슬롯\n\n", i + 1);
        }
    }
}


// ==========================
// 저장 관리 메뉴
// ==========================

void showSaveMenu(Player* p) {

    while (1) {

        system("cls");

        printSlotInfo();

        printf("=====================================\n");
        printf("1. 저장\n");
        printf("2. 불러오기\n");
        printf("3. 슬롯 삭제\n");
        printf("4. 종료\n");
        printf("=====================================\n");

        printf("선택 : ");

        char choice = _getch();

        // ======================
        // 저장
        // ======================

        if (choice == '1') {

            printf("\n저장할 슬롯 선택 (1~3) : ");

            int slot = _getch() - '0';

            if (slot >= 1 && slot <= 3) {

                saveToSlot(p, slot);
            }
        }

        // ======================
        // 불러오기
        // ======================

        else if (choice == '2') {

            printf("\n불러올 슬롯 선택 (1~3) : ");

            int slot = _getch() - '0';

            if (slot >= 1 && slot <= 3) {

                loadFromSlot(p, slot);
            }
        }

        // ======================
        // 삭제
        // ======================

        else if (choice == '3') {

            printf("\n삭제할 슬롯 선택 (1~3) : ");

            int slot = _getch() - '0';

            if (slot >= 1 && slot <= 3) {

                deleteSlot(slot);
            }
        }

        // ======================
        // 종료
        // ======================

        else if (choice == '4') {

            break;
        }
    }
}


// ==========================
// 게임 시작 시 사용 예시
// ==========================

int main() {

    Player player;

    // 저장 파일 로드
    loadSaveFile();

    // 기본 플레이어 값
    player.hp = 100;
    player.maxHp = 100;

    player.x = 5;
    player.y = 5;

    player.floor = 1;

    strcpy(player.roomName, "1F Hallway");

    // 저장 메뉴 실행
    showSaveMenu(&player);

    return 0;
}
