#define NUM_ACHIEVEMENTS 11

const char* achNames[NUM_ACHIEVEMENTS] = {

    "귀신 설득하는 영매사",
    "진실은 하나!",
    "죽어버리다니 한심하네요!",
    "살아남은 자",
    "당신.. 혹시 전생자?",
    "쫄?",
    "하하!! 함정에 걸려버렸구나!",
    "괜찮아, 못할 수도 있지",
    "Are you a puzzle master?",
    "당신은 투명 인간 희망자",
    "거부할 수 없는 유혹"
};

// 업적 설명
const char* achDescs[NUM_ACHIEVEMENTS] = {

    "10회 연속 귀신에게 HP를 잃지 않음",
    "모든 쪽지 획득",
    "배드엔딩 도달",
    "해피엔딩 도달",
    "HP 1 상태에서 회복",
    "체크포인트를 여러 번 사용",
    "함정에 5번 이상 걸림",
    "퍼즐을 5번 이상 실패",
    "모든 퍼즐을 1트만에 성공",
    "집사에게 한 번도 들키지 않음",
    "귀신의 유혹을 받아들임"
};

typedef struct {

    int hp;
    int maxHp;

    // 업적 저장 여부
    int achievements[NUM_ACHIEVEMENTS];

    // 업적 조건 체크용 변수
    int ghostAvoidStreak;
    int checkpointCount;
    int trapCount;
    int puzzleFailCount;
    int allPuzzleFirstTry;
    int caughtByButler;
    int collectedJournalCount;

} Player;


// ==========================
// 업적 해금 함수
// ==========================

void unlockAchievement(Player* p, int idx) {

    if (p->achievements[idx] == 0) {

        p->achievements[idx] = 1;

        gotoxy(0, MAP_HEIGHT + 13);

        printf("\n");
        printf("=====================================\n");
        printf("[도전과제 해금!]\n");
        printf("%s\n", achNames[idx]);
        printf("=====================================\n");

        Sleep(1500);
    }
}


// =====================================================
// 1. 귀신 설득하는 영매사
// 10회 연속 귀신에게 HP를 잃지 않음
// =====================================================

void surviveGhost(Player* p) {

    p->ghostAvoidStreak++;

    if (p->ghostAvoidStreak >= 10) {
        unlockAchievement(p, 0);
    }
}

// 귀신에게 피해 입으면 초기화
void damagedByGhost(Player* p) {

    p->ghostAvoidStreak = 0;
}


// =====================================================
// 2. 진실은 하나!
// 모든 쪽지 획득
// =====================================================

void collectJournal(Player* p) {

    p->collectedJournalCount++;

    if (p->collectedJournalCount >= 15) {
        unlockAchievement(p, 1);
    }
}


// =====================================================
// 3. 죽어버리다니 한심하네요!
// 배드엔딩 도달
// =====================================================

void triggerBadEnding(Player* p) {

    unlockAchievement(p, 2);

    system("cls");

    printf("BAD ENDING...\n");

    Sleep(2000);
}


// =====================================================
// 4. 살아남은 자
// 해피엔딩 도달
// =====================================================

void triggerHappyEnding(Player* p) {

    unlockAchievement(p, 3);

    system("cls");

    printf("HAPPY ENDING!\n");

    Sleep(2000);
}


// =====================================================
// 5. 당신.. 혹시 전생자?
// HP 1 상태에서 회복
// =====================================================

void useHealingItem(Player* p) {

    // HP 1 상태 확인
    if (p->hp == 1) {

        unlockAchievement(p, 4);
    }

    p->hp += 30;

    if (p->hp > p->maxHp)
        p->hp = p->maxHp;
}


// =====================================================
// 6. 쫄?
// 체크포인트 여러 번 사용
// =====================================================

void openCheckpointMenu(Player* p) {

    p->checkpointCount++;

    if (p->checkpointCount >= 5) {

        unlockAchievement(p, 5);
    }
}


// =====================================================
// 7. 하하!! 함정에 걸려버렸구나!
// 함정 5회 이상
// =====================================================

void triggerTrap(Player* p) {

    p->trapCount++;

    p->hp -= 15;

    printf("함정 발동! HP -15\n");

    if (p->trapCount >= 5) {

        unlockAchievement(p, 6);
    }
}


// =====================================================
// 8. 괜찮아, 못할 수도 있지
// 퍼즐 5회 이상 실패
// =====================================================

void failPuzzle(Player* p) {

    p->puzzleFailCount++;

    if (p->puzzleFailCount >= 5) {

        unlockAchievement(p, 7);
    }
}


// =====================================================
// 9. Are you a puzzle master?
// 모든 퍼즐을 1트만에 성공
// =====================================================

void clearPuzzleFirstTry(Player* p) {

    if (p->allPuzzleFirstTry == 1) {

        unlockAchievement(p, 8);
    }
}


// =====================================================
// 10. 당신은 투명 인간 희망자
// 집사에게 한 번도 안 들킴
// =====================================================

void caughtByButler(Player* p) {

    p->caughtByButler = 1;
}

// 게임 클리어 시 확인
void checkInvisibleAchievement(Player* p) {

    if (p->caughtByButler == 0) {

        unlockAchievement(p, 9);
    }
}


// =====================================================
// 11. 거부할 수 없는 유혹
// 귀신의 유혹을 받아들임
// =====================================================

void acceptGhostTemptation(Player* p) {

    unlockAchievement(p, 10);

    system("cls");

    printf("당신은 귀신이 되어버렸다...\n");

    Sleep(2000);
}


// ==========================
// 업적 메뉴 출력
// ==========================

void showAchievementMenu(Player* p) {

    system("cls");

    printf("=====================================\n");
    printf("            도전과제 목록\n");
    printf("=====================================\n\n");

    for (int i = 0; i < NUM_ACHIEVEMENTS; i++) {

        if (p->achievements[i]) {

            printf("[해금] %s\n", achNames[i]);
            printf(" - %s\n\n", achDescs[i]);
        }
        else {

            printf("[잠김] ???\n\n");
        }
    }

    printf("아무 키나 누르면 종료...");
    _getch();
}
