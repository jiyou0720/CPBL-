#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_ACHIEVEMENTS 11
#define NUM_ITEMS 20
#define NUM_JOURNALS 15

// =====================================
// 업적 이름
// =====================================

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

// =====================================
// 업적 설명
// =====================================

const char* achDescs[NUM_ACHIEVEMENTS] = {

    "10회 연속 귀신에게 HP를 잃지 않음",
    "모든 사용인 일기 획득",
    "배드 엔딩 도달",
    "해피 엔딩 도달",
    "HP 1 상태에서 회복",
    "체크포인트 5회 이상 사용",
    "함정에 5번 이상 걸림",
    "퍼즐 5회 이상 실패",
    "모든 퍼즐을 1트만에 성공",
    "집사에게 한 번도 들키지 않음",
    "귀신의 유혹을 받아들임"
};

// =====================================
// 아이템 이름
// =====================================

const char* itemNames[NUM_ITEMS] = {

    "HP 회복제",
    "성수",
    "망치",
    "일반 열쇠",
    "비밀의 방 열쇠",

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

// =====================================
// 플레이어
// =====================================

typedef struct {

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

// =====================================
// 업적 해금
// =====================================

void unlockAchievement(Player* p, int idx)
{
    if(p->achievements[idx])
        return;

    p->achievements[idx] = 1;

    printf("\n=====================================\n");
    printf("도전과제 해금!\n");
    printf("%s\n", achNames[idx]);
    printf("=====================================\n");

    usleep(1500000);
}

// =====================================
// 아이템 획득
// =====================================

void obtainItem(Player* p, int itemID)
{
    if(itemID < 0 || itemID >= NUM_ITEMS)
        return;

    if(p->itemCollection[itemID] == 0)
    {
        p->itemCollection[itemID] = 1;

        printf("\n[도감 등록] %s\n",
               itemNames[itemID]);

        usleep(800000);
    }
}

// =====================================
// 업적 함수
// =====================================

void surviveGhost(Player* p)
{
    p->ghostAvoidStreak++;

    if(p->ghostAvoidStreak >= 10)
        unlockAchievement(p, 0);
}

void damagedByGhost(Player* p)
{
    p->ghostAvoidStreak = 0;
}

void triggerBadEnding(Player* p)
{
    unlockAchievement(p, 2);
}

void triggerHappyEnding(Player* p)
{
    unlockAchievement(p, 3);
}

void useHealingItem(Player* p)
{
    if(p->hp == 1)
        unlockAchievement(p, 4);

    p->hp += 30;

    if(p->hp > p->maxHp)
        p->hp = p->maxHp;
}

void openCheckpointMenu(Player* p)
{
    p->checkpointCount++;

    if(p->checkpointCount >= 5)
        unlockAchievement(p, 5);
}

void triggerTrap(Player* p)
{
    p->trapCount++;

    if(p->trapCount >= 5)
        unlockAchievement(p, 6);
}

void failPuzzle(Player* p)
{
    p->puzzleFailCount++;

    if(p->puzzleFailCount >= 5)
        unlockAchievement(p, 7);
}

void clearPuzzleFirstTry(Player* p)
{
    if(p->allPuzzleFirstTry)
        unlockAchievement(p, 8);
}

void caughtByButler(Player* p)
{
    p->caughtByButler = 1;
}

void checkInvisibleAchievement(Player* p)
{
    if(p->caughtByButler == 0)
        unlockAchievement(p, 9);
}

void acceptGhostTemptation(Player* p)
{
    unlockAchievement(p, 10);
}
// =====================================
// 사용인 일기 원문
// =====================================

const char* journalTexts[NUM_JOURNALS] = {

"오늘도 저택은 평화롭다.\n\n주인님께서는 평소처럼 아내분과 함께 정원을 거닐고 계신다.\n\n집사님 역시 변함없이 저택을 관리하고 계신다.\n\n오늘은 평소보다 일이 많았지만,\n\n어쩐지 저택의 분위기가 유난히 밝게 느껴져 기분이 나쁘지 않았다.",

"나는 다른 사용인들에게 집사님에 대한 이야기를 들었다.\n\n집사님은 가족이 없으시며,\n아주 어릴 적부터 이 저택에서 일해오셨다고 한다.\n\n특히 주인님과는 어린 시절부터 함께 자라온 사이라,\n단순한 주종 관계라기보다는 가족에 가까운 사이였다고 했다.\n\n그래서인지 집사님은 언제나 주인님의 곁을 지키고 계셨고,\n그 모습은 자연스럽게 느껴질 정도였다.\n\n나는 그저 두 분이 서로를 가족처럼 여기고 계신다고만 생각했다.",

"평소처럼 일에 몰두하고 있던 어느 날.\n\n산책하고 계셨던 부인이 쓰러지셨다.\n\n누구도 예상하지 못한 일이었다.\n\n저택 전체가 순식간에 혼란에 휩싸였고,\n\n나 또한 일이 손에 잡히지 않을 만큼 마음이 불안하다.",

"그분이 쓰러진 이후, 저택의 분위기는 서서히 변하기 시작했다.\n\n쓰러진 안주인님께서는 안색이 좋지 않으셨다.\n\n방문한 의사조차 심장이 좋지 않으며 점점 나빠질 것이라는 말을 남겼다.\n\n마치 생기를 잃어버린 것처럼 공기가 무겁게 가라앉아 있다.\n\n주인님께서는 어찌된 일인지 도서관에만 틀어박혀 계시고,\n집사님은 그런 주인님의 곁을 한시도 떠나지 않는다.\n\n이 저택에 무언가가 달라지고 있는 듯했다.",

"요 며칠 사이 저택에서 이상한 일이 계속되고 있다.\n\n알고 지내던 다른 사용인들은 어느샌가 보이지 않게 되었으며,\n\n분명 익숙했던 복도가 낯설게 느껴진다.\n\n단순한 착각이라 생각하고 싶어도,\n\n내 안의 불안감이 점차 커지고 있었다.",

"오늘 도서관 근처에 갔다가 이상한 광경을 목격했다.\n\n문틈 사이로 보인 것은 주인님이 누군가와 이야기를 하는 모습이었다.\n\n무슨 내용인지 궁금해서 더 가까이 가고 싶었지만,\n\n어느샌가 오신 집사님이 나를 응시하고 있었다.\n\n그 눈빛이 내가 알던 집사님과 다른 사람 같았다.",

"사람들이 하나둘씩 사라지고 있다.\n\n처음에는 외출, 그 다음에는 이직이라 생각했다.\n\n하지만 그 누구도 그 이후로는 본 적이 없었다.\n\n아무도 이 이야기를 입 밖에 내지 않지만,\n\n모두가 알고 있다.\n\n이 저택에서 무언가가 벌어지고 있다는 것을.",

"오늘 확실히 보았다.\n\n외출 후 실종되었던 사용인이 복도 끝에 서 있었다.\n\n하지만 그것은 더 이상 사람이 아니었다.\n\n형체는 흐릿했지만,\n\n그는 나를 보며 웃고 있었다.\n\n도망치듯 자리를 벗어났지만 그의 모습이 계속 머릿속에 맴돈다.",

"혼란스럽다.\n\n주인님 그리고 집사님은 무슨 생각이신 걸까.\n\n사용인들은 점차 줄어갔으며,\n이 저택을 유지하는 것이 힘들 정도가 되었다.\n\n하지만 집사님과 주인님은 아무런 변화도 없으신 듯 행동하고 있다.",

"요즘 들어 이상한 소리가 들리기 시작했다.\n\n처음에는 바람 소리라고 생각했다.\n\n하지만 점차 그 목소리는 선명해져 갔다.\n\n'이곳은 위험해.'\n\n'여기를 떠나야 해.'\n\n무슨 이유인지는 모르겠지만 나에게 이런 말들을 해온다.",

"이런 일들이 일어나는 것을 보며 한 가지 깨달은 것이 있다.\n\n나에게 들리는 목소리는 사라진 사람들의 것이라는 것을.\n\n그들은 왠지 이 저택을 빠져나가지 못하는 것 같다.\n\n무슨 이유가 있는 걸까.",

"도망치려고 시도했다.\n\n몰래 문으로 나가려고 했지만 문이 잠겨있어서 나갈 수가 없었다.\n\n뒷문도 마찬가지였다.\n\n왜 정문과 뒷문은 낮에도 굳게 닫혀있었을까.",

"사라진 사람들의 목소리가 끊임없이 나에게 말한다.\n\n'그걸 파괴해야 해.'\n\n'그곳에 있어.'\n\n무엇을 파괴해야 하는지 모르겠지만,\n그것을 파괴하면 내가 나갈 수 있다는 것만은 알 것 같다.",

"오늘 집사님과 마주쳤다.\n\n아무 말도 하지 않으셨지만,\n그 차가운 눈빛으로 확실했다.\n\n그는 알고 있다.\n\n내가 이 저택의 진실을 점점 알아가고 있다는 것을.\n\n그 순간 확신했다.\n\n이 저택에서 가장 위험한 존재는 귀신이 아니라는 것을.",

"더 이상 시간을 지체할 수 없다.\n\n나는 이 저택을 벗어날 것이다.\n\n그 목소리들이 말한 심장을 찾아.\n\n이 모든 것을 끝내야 한다.\n\n그것이 무엇이든 그것만이 이 저택에서 벗어날 수 있는 유일한 방법이다.\n\n발소리가 들린다.\n\n누군가 이쪽으로 오고 있다."
};

// =====================================
// 일기 획득
// =====================================

void obtainJournal(Player* p, int page)
{
    if(page < 1 || page > 15)
        return;

    int itemID = 5 + (page - 1);

    if(p->itemCollection[itemID] == 0)
    {
        obtainItem(p, itemID);

        p->collectedJournalCount++;

        printf("\n[사용인의 일기 %d 획득]\n", page);

        usleep(1000000);

        if(p->collectedJournalCount >= 15)
        {
            unlockAchievement(p, 1);
        }
    }
}

// =====================================
// 업적 목록
// =====================================

void showAchievementMenu(Player* p)
{
    system("clear");

    printf("=====================================\n");
    printf("          도전과제 목록\n");
    printf("=====================================\n\n");

    for(int i=0;i<NUM_ACHIEVEMENTS;i++)
    {
        if(p->achievements[i])
        {
            printf("[해금] %s\n", achNames[i]);
            printf(" - %s\n\n", achDescs[i]);
        }
        else
        {
            printf("[잠김] ???\n\n");
        }
    }

    printf("Enter 입력...");
    getchar();
}
// =====================================
// 아이템 도감
// =====================================

void showCollectionMenu(Player* p)
{
    system("clear");

    printf("=====================================\n");
    printf("            아이템 도감\n");
    printf("=====================================\n\n");

    for(int i=0;i<NUM_ITEMS;i++)
    {
        if(p->itemCollection[i])
            printf("[획득] %s\n", itemNames[i]);
        else
            printf("[미획득] ???\n");
    }

    printf("\nEnter 입력...");
    getchar();
}

// =====================================
// 일기 열람 메뉴
// =====================================

void showJournalMenu(Player* p)
{
    int page;

    system("clear");

    printf("=====================================\n");
    printf("          사용인의 일기\n");
    printf("=====================================\n\n");

    int found = 0;

    for(int i=0;i<15;i++)
    {
        int itemID = 5 + i;

        if(p->itemCollection[itemID])
        {
            printf("%2d. 사용인의 일기 %d\n",
                   i + 1,
                   i + 1);

            found = 1;
        }
    }

    if(!found)
    {
        printf("획득한 일기가 없습니다.\n");

        printf("\nEnter 입력...");
        getchar();
        return;
    }

    printf("\n읽을 번호 입력 : ");
    scanf("%d",&page);
    getchar();

    if(page < 1 || page > 15)
        return;

    if(!p->itemCollection[5 + (page - 1)])
    {
        printf("아직 획득하지 못한 일기입니다.\n");

        printf("\nEnter 입력...");
        getchar();
        return;
    }

    system("clear");

    printf("=====================================\n");
    printf("사용인의 일기 %d\n", page);
    printf("=====================================\n\n");

    printf("%s\n", journalTexts[page - 1]);

    printf("\nEnter 입력...");
    getchar();
}

// =====================================
// 테스트용 메뉴
// =====================================

void printMenu()
{
    system("clear");

    printf("=====================================\n");
    printf(" 도전과제 / 도감 시스템 테스트\n");
    printf("=====================================\n\n");

    printf("1  : HP 회복제 획득\n");
    printf("2  : 성수 획득\n");
    printf("3  : 망치 획득\n");
    printf("4  : 일반 열쇠 획득\n");
    printf("5  : 비밀의 방 열쇠 획득\n");

    printf("\n");

    printf("6  ~ 20 : 사용인의 일기 획득\n");

    printf("\n");

    printf("21 : 아이템 도감 보기\n");
    printf("22 : 사용인의 일기 보기\n");
    printf("23 : 도전과제 보기\n");

    printf("\n");

    printf("24 : 귀신 10회 회피\n");
    printf("25 : 배드 엔딩\n");
    printf("26 : 해피 엔딩\n");
    printf("27 : HP 1 상태 회복\n");
    printf("28 : 체크포인트 5회 사용\n");
    printf("29 : 함정 5회\n");
    printf("30 : 퍼즐 5회 실패\n");
    printf("31 : 퍼즐 마스터\n");
    printf("32 : 투명 인간 업적\n");
    printf("33 : 귀신 유혹 수락\n");

    printf("\n");

    printf("0 : 종료\n");

    printf("\n선택 : ");
}

// =====================================
// main
// =====================================

int main()
{
    Player player;

    memset(&player, 0, sizeof(Player));

    player.hp = 100;
    player.maxHp = 100;

    player.allPuzzleFirstTry = 1;

    int choice;

    while(1)
    {
        printMenu();

        scanf("%d",&choice);
        getchar();

        switch(choice)
        {
            case 1:
                obtainItem(&player,0);
                break;

            case 2:
                obtainItem(&player,1);
                break;

            case 3:
                obtainItem(&player,2);
                break;

            case 4:
                obtainItem(&player,3);
                break;

            case 5:
                obtainItem(&player,4);
                break;

            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20:

                obtainJournal(&player,
                              choice - 5);
                break;

            case 21:
                showCollectionMenu(&player);
                break;

            case 22:
                showJournalMenu(&player);
                break;

            case 23:
                showAchievementMenu(&player);
                break;

            case 24:

                for(int i=0;i<10;i++)
                    surviveGhost(&player);

                break;

            case 25:
                triggerBadEnding(&player);
                break;

            case 26:
                triggerHappyEnding(&player);
                break;

            case 27:

                player.hp = 1;

                useHealingItem(&player);

                break;

            case 28:

                for(int i=0;i<5;i++)
                    openCheckpointMenu(&player);

                break;

            case 29:

                for(int i=0;i<5;i++)
                    triggerTrap(&player);

                break;

            case 30:

                for(int i=0;i<5;i++)
                    failPuzzle(&player);

                break;

            case 31:

                clearPuzzleFirstTry(&player);

                break;

            case 32:

                checkInvisibleAchievement(&player);

                break;

            case 33:

                acceptGhostTemptation(&player);

                break;

            case 0:

                printf("프로그램 종료\n");

                return 0;
        }
    }

    return 0;
}
