// ============================================
// achievement_collection.c
// Part 1 : 업적 시스템
// Linux Compatible
// ============================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// ============================================
// 설정
// ============================================

#define NUM_ACHIEVEMENTS 11
#define NUM_ITEMS 20
#define NUM_JOURNALS 15

typedef struct
{
    int hp;
    int maxHp;

    int achievements[NUM_ACHIEVEMENTS];
    int itemCollection[NUM_ITEMS];

    int ghostAvoidStreak;
    int checkpointCount;
    int trapCount;
    int puzzleFailCount;

    int allPuzzleFirstTry;

    int caughtByButler;

    int collectedJournalCount;

} Player;

void showJournalPage(int page);
// ============================================
// 업적 이름
// ============================================

const char* achNames[NUM_ACHIEVEMENTS] =
{
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

// ============================================
// 업적 설명
// ============================================

const char* achDescs[NUM_ACHIEVEMENTS] =
{
    "10회 연속 귀신에게 HP를 잃지 않음",
    "모든 사용인 일기 획득",
    "배드 엔딩 도달",
    "해피 엔딩 도달",
    "HP 1 상태에서 회복",
    "체크포인트를 5회 이상 사용",
    "함정에 5번 이상 걸림",
    "퍼즐을 5번 이상 실패",
    "모든 퍼즐을 1트만에 성공",
    "집사에게 한 번도 들키지 않음",
    "귀신의 유혹을 받아들임"
};

// ============================================
// 플레이어 구조체 확장
// 기존 Player에 추가할 항목
// ============================================

/*

int achievements[NUM_ACHIEVEMENTS];

int itemCollection[NUM_ITEMS];

int ghostAvoidStreak;
int checkpointCount;
int trapCount;
int puzzleFailCount;

int allPuzzleFirstTry;

int caughtByButler;

int collectedJournalCount;

*/

// ============================================
// 업적 해금
// ============================================

void unlockAchievement(Player* p, int idx)
{
    if(idx < 0 || idx >= NUM_ACHIEVEMENTS)
        return;

    if(p->achievements[idx])
        return;

    p->achievements[idx] = 1;

    printf("\n");
    printf("=====================================\n");
    printf("       도전과제 해금!\n");
    printf("   %s\n", achNames[idx]);
    printf("=====================================\n");

    usleep(1200000);
}

// ============================================
// 귀신 설득하는 영매사
// 10회 연속 귀신에게 HP를 잃지 않음
// ============================================

void surviveGhost(Player* p)
{
    p->ghostAvoidStreak++;

    if(p->ghostAvoidStreak >= 10)
    {
        unlockAchievement(p, 0);
    }
}

void damagedByGhost(Player* p)
{
    p->ghostAvoidStreak = 0;
}

// ============================================
// 죽어버리다니 한심하네요!
// 배드엔딩
// ============================================

void triggerBadEnding(Player* p)
{
    unlockAchievement(p, 2);
}

// ============================================
// 살아남은 자
// 해피엔딩
// ============================================

void triggerHappyEnding(Player* p)
{
    unlockAchievement(p, 3);
}

// ============================================
// 당신.. 혹시 전생자?
// HP 1 상태에서 회복
// ============================================

void useHealingItemAchievement(Player* p)
{
    if(p->hp == 1)
    {
        unlockAchievement(p, 4);
    }
}

// ============================================
// 쫄?
// 체크포인트 5회 이상
// ============================================

void openCheckpointMenu(Player* p)
{
    p->checkpointCount++;

    if(p->checkpointCount >= 5)
    {
        unlockAchievement(p, 5);
    }
}

// ============================================
// 하하!! 함정에 걸려버렸구나!
// ============================================

void triggerTrap(Player* p)
{
    p->trapCount++;

    if(p->trapCount >= 5)
    {
        unlockAchievement(p, 6);
    }
}

// ============================================
// 괜찮아, 못할 수도 있지
// ============================================

void failPuzzle(Player* p)
{
    p->puzzleFailCount++;

    if(p->puzzleFailCount >= 5)
    {
        unlockAchievement(p, 7);
    }
}

// ============================================
// Are you a puzzle master?
// ============================================

void clearPuzzleFirstTry(Player* p)
{
    if(p->allPuzzleFirstTry)
    {
        unlockAchievement(p, 8);
    }
}

// ============================================
// 집사에게 들킴
// ============================================

void caughtByButler(Player* p)
{
    p->caughtByButler = 1;
}

// ============================================
// 당신은 투명 인간 희망자
// 게임 클리어 시 호출
// ============================================

void checkInvisibleAchievement(Player* p)
{
    if(p->caughtByButler == 0)
    {
        unlockAchievement(p, 9);
    }
}

// ============================================
// 거부할 수 없는 유혹
// ============================================

void acceptGhostTemptation(Player* p)
{
    unlockAchievement(p, 10);
}

// ============================================
// 업적 메뉴
// ============================================

void showAchievementMenu(Player* p)
{
    system("clear");

    printf("=====================================\n");
    printf("           도전과제\n");
    printf("=====================================\n\n");

    for(int i = 0; i < NUM_ACHIEVEMENTS; i++)
    {
        printf("[%02d] ", i + 1);

        if(p->achievements[i])
        {
            printf("%s\n", achNames[i]);
            printf("     %s\n\n", achDescs[i]);
        }
        else
        {
            printf("???\n\n");
        }
    }

    printf("Enter 입력...");
    getchar();
}
// ============================================
// Part 2 : 아이템 도감
// achievement_collection.c 에 이어붙이기
// ============================================

// ============================================
// 아이템 ID
// ============================================

enum
{
    ITEM_HEAL_POTION = 0,
    ITEM_HOLY_WATER,
    ITEM_HAMMER,
    ITEM_NORMAL_KEY,
    ITEM_SECRET_KEY
};

// ============================================
// 아이템 이름
// ============================================

const char* itemNames[NUM_ITEMS] =
{
    // 일반 아이템

    "HP 회복제",
    "성수",
    "망치",
    "일반 열쇠",
    "비밀의 방 열쇠",

    // 사용인 일기

    "사용인의 일기 1",
    "사용인의 일기 2",
    "사용인의 일기 3",
    "사용인의 일기 4",
    "사용인의 일기 5",
    "사용인의 일기 6",
    "사용인의 일기 7",
    "사용인의 일기 8",
    "사용인의 일기 9",
    "사용인의 일기 10",
    "사용인의 일기 11",
    "사용인의 일기 12",
    "사용인의 일기 13",
    "사용인의 일기 14",
    "사용인의 일기 15"
};

// ============================================
// 아이템 획득
// ============================================

void obtainItem(Player* p, int itemID)
{
    if(itemID < 0 || itemID >= NUM_ITEMS)
        return;

    if(p->itemCollection[itemID])
        return;

    p->itemCollection[itemID] = 1;

    printf("\n");
    printf("=====================================\n");
    printf("      새로운 아이템 발견!\n");
    printf("       %s\n", itemNames[itemID]);
    printf("=====================================\n");

    usleep(1000000);
}

// ============================================
// 아이템 보유 여부 확인
// ============================================

int hasItem(Player* p, int itemID)
{
    if(itemID < 0 || itemID >= NUM_ITEMS)
        return 0;

    return p->itemCollection[itemID];
}

// ============================================
// 아이템 제거
// (열쇠 사용 등에 활용)
// ============================================

void removeItem(Player* p, int itemID)
{
    if(itemID < 0 || itemID >= NUM_ITEMS)
        return;

    p->itemCollection[itemID] = 0;
}

// ============================================
// 아이템 도감 메뉴
// ============================================

void showCollectionMenu(Player* p)
{
    system("clear");

    printf("=====================================\n");
    printf("           아이템 도감\n");
    printf("=====================================\n\n");

    for(int i = 0; i < 5; i++)
    {
        printf("%d. ", i + 1);

        if(p->itemCollection[i])
        {
            printf("%s\n", itemNames[i]);
        }
        else
        {
            printf("???\n");
        }
    }

    printf("\n");
    printf("획득률 : ");

    int count = 0;

    for(int i = 0; i < 5; i++)
    {
        if(p->itemCollection[i])
            count++;
    }

    printf("%d / 5\n", count);

    printf("\nEnter 입력...");
    getchar();
}

// ============================================
// 게임 메뉴용 도감 선택창
// ============================================

int showCodexMenu()
{
    int choice;

    system("clear");

    printf("=====================================\n");
    printf("              도감\n");
    printf("=====================================\n\n");

    printf("1. 아이템 도감\n");
    printf("2. 사용인 일기\n");
    printf("0. 뒤로가기\n");

    printf("\n선택 : ");

    scanf("%d", &choice);
    getchar();

    return choice;
}

// ============================================
// 사용 예시
// ============================================

/*

// 성수 획득

obtainItem(player, ITEM_HOLY_WATER);


// 망치 획득

obtainItem(player, ITEM_HAMMER);


// 일반 열쇠 획득

obtainItem(player, ITEM_NORMAL_KEY);


// 열쇠 사용

removeItem(player, ITEM_NORMAL_KEY);


// 아이템 보유 확인

if(hasItem(player, ITEM_SECRET_KEY))
{
    ...
}

*/
// ============================================
// 사용인 일기 
// ============================================

const char* journalTexts[NUM_JOURNALS] =
{
    // 1
    "오늘도 저택은 평화롭다.\n\n"
    "주인님께서는 평소처럼 아내분과 함께 정원을 거닐고 계신다.\n\n"
    "집사님 역시 변함없이 저택을 관리하고 계신다.\n\n"
    "오늘은 평소보다 일이 많았지만,\n\n"
    "어쩐지 저택의 분위기가 유난히 밝게 느껴져 기분이 나쁘지 않았다.",

    // 2
    "나는 다른 사용인들에게 집사님에 대한 이야기를 들었다.\n\n"
    "집사님은 가족이 없으시며,\n"
    "아주 어릴 적부터 이 저택에서 일해오셨다고 한다.\n\n"
    "특히 주인님과는 어린 시절부터 함께 자라온 사이라,\n"
    "단순한 주종 관계라기보다는 가족에 가까운 사이였다고 했다.\n\n"
    "그래서인지 집사님은 언제나 주인님의 곁을 지키고 계셨고,\n"
    "그 모습은 자연스럽게 느껴질 정도였다.",

    // 3
    "평소처럼 일에 몰두하고 있던 어느 날.\n\n"
    "산책하고 계셨던 부인이 쓰러지셨다.\n\n"
    "누구도 예상하지 못한 일이었다.\n\n"
    "저택 전체가 순식간에 혼란에 휩싸였고,\n\n"
    "나 또한 일이 손에 잡히지 않을 만큼 마음이 불안하다.",

    // 4
    "그분이 쓰러진 이후,\n"
    "저택의 분위기는 서서히 변하기 시작했다.\n\n"
    "방문한 의사조차 심장이 좋지 않으며\n"
    "점점 나빠질 것이라는 말을 남겼다.\n\n"
    "주인님께서는 도서관에만 틀어박혀 계시고,\n"
    "집사님은 그런 주인님의 곁을 한시도 떠나지 않는다.",

    // 5
    "요 며칠 사이,\n"
    "저택에서 이상한 일이 계속되고 있다.\n\n"
    "알고 지내던 사용인들이\n"
    "어느샌가 보이지 않게 되었으며,\n\n"
    "내 안의 불안감이 점차 커지고 있었다.",

    // 6
    "오늘 도서관 근처에 갔다가\n"
    "이상한 광경을 목격했다.\n\n"
    "문틈 사이로 보인 것은\n"
    "주인님이 누군가와 이야기를 하는 모습이었다.\n\n"
    "그리고 집사님의 눈빛은\n"
    "내가 알던 그 사람과 달랐다.",

    // 7
    "사람들이 하나둘씩 사라지고 있다.\n\n"
    "처음에는 외출,\n"
    "그 다음에는 이직이라 생각했다.\n\n"
    "하지만 그 누구도\n"
    "그 이후로는 본 적이 없었다.",

    // 8
    "오늘 확실히 보았다.\n\n"
    "실종되었던 사용인이\n"
    "복도 끝에 서 있었다.\n\n"
    "하지만 그것은 더 이상 사람이 아니었다.\n\n"
    "도망쳤지만\n"
    "그 모습이 계속 머릿속에 맴돈다.",

    // 9
    "혼란스럽다.\n\n"
    "주인님 그리고 집사님은 무슨 생각이신 걸까.\n\n"
    "사용인들은 점차 줄어갔으며,\n"
    "이 저택을 유지하는 것이 힘들 정도가 되었다.\n\n"
    "하지만 집사님과 주인님은 아무런 변화도 없으신 듯 행동하고 있다.",

    // 10
    "요즘 들어 이상한 소리가 들리기 시작했다.\n\n"
    "처음에는 바람 소리라고 생각했다.\n\n"
    "하지만 점차 그 목소리는 선명해져 갔다.\n\n"
    "'이곳은 위험해.'\n\n"
    "'여기를 떠나야 해.'",

    // 11
    "이런 일들이 일어나는 것을 보며\n"
    "한 가지 깨달은 것이 있다.\n\n"
    "나에게 들리는 목소리는\n"
    "사라진 사람들의 것이라는 것을.\n\n"
    "그들은 이 저택을 빠져나가지 못하는 것 같다.",

    // 12
    "도망치려고 시도했다.\n\n"
    "몰래 문으로 나가려고 했지만\n"
    "문이 잠겨있어서 나갈 수가 없었다.\n\n"
    "뒷문도 마찬가지였다.",

    // 13
    "사라진 사람들의 목소리가\n"
    "끊임없이 나에게 말한다.\n\n"
    "'그걸 파괴해야 해.'\n\n"
    "'그곳에 있어.'\n\n"
    "무엇을 파괴해야 하는지는 모르겠다.",

    // 14
    "오늘 집사님과 마주쳤다.\n\n"
    "아무 말도 하지 않으셨지만\n"
    "그 차가운 눈빛으로 확실했다.\n\n"
    "그는 알고 있다.\n\n"
    "내가 진실에 가까워지고 있다는 것을.",

    // 15
    "더 이상 시간을 지체할 수 없다.\n\n"
    "나는 이 저택을 벗어날 것이다.\n\n"
    "그 목소리들이 말한 심장을 찾아.\n\n"
    "이 모든 것을 끝내야 한다.\n\n"
    "발소리가 들린다.\n\n"
    "누군가 이쪽으로 오고 있다."
};

// ============================================
// 일기 획득
// page = 1 ~ 15
// ============================================

void obtainJournal(Player* p, int page)
{
    if(page < 1 || page > 15)
        return;

    int itemID = 5 + (page - 1);

    if(p->itemCollection[itemID])
        return;

    obtainItem(p, itemID);

    p->collectedJournalCount++;

    printf("\n");
    printf("=====================================\n");
    printf("      문서를 발견했다.\n");
    printf("    사용인의 일기 (%d)\n", page);
    printf("=====================================\n");

    usleep(1200000);

    // 업적 : 진실은 하나!

    if(p->collectedJournalCount >= 15)
    {
        unlockAchievement(p, 1);
    }
}

// ============================================
// 사용인 일기 목록
// ============================================

void showJournalMenu(Player* p)
{
    int page;

    system("clear");

    printf("=====================================\n");
    printf("          사용인 일기\n");
    printf("=====================================\n\n");

    for(int i = 0; i < 15; i++)
    {
        int itemID = 5 + i;

        printf("%2d. ", i + 1);

        if(p->itemCollection[itemID])
        {
            printf("사용인의 일기 %d\n", i + 1);
        }
        else
        {
            printf("???\n");
        }
    }

    printf("\n");
    printf("읽을 번호 입력 (0 = 뒤로가기)\n");

    printf("선택 : ");

    scanf("%d", &page);
    getchar();

    if(page == 0)
        return;

    if(page < 1 || page > 15)
        return;

    if(!p->itemCollection[5 + (page - 1)])
    {
        printf("\n아직 획득하지 못한 일기입니다.\n");

        printf("\nEnter...");
        getchar();
        return;
    }

    showJournalPage(page);
}

// ============================================
// 일기 읽기
// ============================================

void showJournalPage(int page)
{
    if(page < 1 || page > NUM_JOURNALS)
        return;
    
    system("clear");

    printf("=====================================\n");
    printf("       사용인의 일기 %d\n", page);
    printf("=====================================\n\n");

    printf("%s\n", journalTexts[page - 1]);

    printf("\n");
    printf("Enter 입력...");
    getchar();
}
