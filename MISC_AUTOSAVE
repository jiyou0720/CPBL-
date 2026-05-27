// save.dat 파일 불러오기
void loadSaveDatFile() {
    FILE* fp = fopen("save.dat", "rb");

    if (fp == NULL) {
        memset(&globalSave, 0, sizeof(SaveData));
        return;
    }

    fread(&globalSave, sizeof(SaveData), 1, fp);
    fclose(fp);
}

// save.dat 파일 작성
void writeSaveDatFile() {
    FILE* fp = fopen("save.dat", "wb");

    if (fp == NULL) return;

    fwrite(&globalSave, sizeof(SaveData), 1, fp);
    fclose(fp);
}

// 특정 슬롯에 저장
void saveToSlot(Player* p, int slotIdx) {
    int idx = slotIdx - 1;

    globalSave.slotExists[idx] = 1;
    globalSave.playerData[idx] = *p;

    writeSaveDatFile();
}

// 방 이동 시 자동 저장
void updateRoomName(Player* p) {
    char oldRoom[50];
    strcpy(oldRoom, p->roomName);

    // 방 이름 판정 로직
    if (p->floor == 1) {
        if (p->x >= 1 && p->x <= 13 && p->y >= 1 && p->y <= 3) {
            strcpy(p->roomName, "1F Bedroom 1");
        }
        else {
            strcpy(p->roomName, "1F Hallway");
        }
    }

    // 새 방 진입 시 자동 저장
    if (strcmp(oldRoom, p->roomName) != 0) {

        saveToSlot(p, currentSlot);

        gotoxy(0, MAP_HEIGHT + 9);
        printf("[Auto Save] Progress saved automatically!\n");
    }
}
