// --- 실시간으로 게임 클리어 기록을 추가하는 함수 ---
void AddNewRecord(double finalTime, const char* diff) {
    printf("\e[1;1H\e[2J");
    printf("\n");
    printf("  ======================================================\n");
    printf("  🎉 저택을 성공적으로 탈출하셨습니다! 축하합니다!\n");
    printf("  ======================================================\n");
    
    PlayerRank newPlayer;
    newPlayer.time = finalTime;
    strcpy(newPlayer.difficulty, diff);
    
    // 1. 실시간 이름 입력받기
    printf("  ▶ 명예의 전당에 등록할 이름을 입력하세요 (최대 19자): ");
    // 공백 없는 문자열을 안전하게 입력받음
    if (scanf("%19s", newPlayer.name) != 1) {
        strcpy(newPlayer.name, "Unknown");
    }
    // scanf 버퍼 비우기
    while (getchar() != '\n'); 

    // 2. 기존 기록 다 읽어오기
    PlayerRank list[100];
    int count = 0;
    
    FILE* fp = fopen("rank.dat", "rb");
    if (fp != NULL) {
        while (count < 99 && fread(&list[count], sizeof(PlayerRank), 1, fp) == 1) {
            count++;
        }
        fclose(fp);
    }

    // 3. 새 플레이어 기록을 리스트 마지막에 병합
    list[count] = newPlayer;
    count++;

    // 4. 버블 정렬 (시간 기준 오름차순)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (list[j].time > list[j + 1].time) {
                PlayerRank temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }

    // 5. 정렬된 전체 기록 중에서 최대 상위 10개만 파일에 새로 저장
    fp = fopen("rank.dat", "wb");
    if (fp == NULL) {
        printf("\n[오류] 랭킹을 실시간으로 업데이트하지 못했습니다.\n");
        return;
    }
    
    int saveCount = (count < 10) ? count : 10;
    fwrite(list, sizeof(PlayerRank), saveCount, fp);
    fclose(fp);

    printf("\n  [SYSTEM] 실시간 데이터 저장이 완료되었습니다!\n");
    printf("  아무 키나 누르면 명예의 전당을 확인합니다.");
    _getch();

    // 6. 저장 완료 후 바로 화면에 랭킹판 띄워주기
    START_RANK();
}
