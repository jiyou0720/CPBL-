// 선택 / P2 | 책장 밀기·전원 차단기 등(동적 맵 업데이트)

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>
#include <math.h>

// --- 시스템 설정 ---
#define MAP_WIDTH 50
#define MAP_HEIGHT 18 // 맵 배열의 세로 크기

// --- 구조체 정의 ---
typedef struct {
    int floor;
    int x, y;
    char name[50];
    char hint[100];
    bool is_collected;
} Diary;

typedef struct {
    int id; // 1: 도서관, 2: 사용인방
    int floor;
    int x, y;
    char name[30];
    bool is_collected;
} Key;

// --- 글로벌 변수 ---
// 플레이어 시작 위치를 빈 공간인 (5, 5)로 설정합니다.
int pX = 5, pY = 5, currentFloor = 1;
int diaryCount = 0;
bool gameRunning = true;

// --- 퍼즐 관련 글로벌 변수 ---
bool is_breaker_off = false;
int bookcaseX = 42, bookcaseY = 4;

// --- [데이터] 아이템 및 일기 배치 ---
Diary diaries[15] = {
    {1, 5, 2, "Diary #1", "주인공 방 비밀 구석", false}, {1, 6, 2, "Diary #2", "주인공 방 비밀 구석", false}, {1, 7, 2, "Diary #3", "주인공 방 비밀 구석", false},
    {1, 10, 14, "Diary #4", "응접실 소파 사이", false}, {1, 3, 14, "Diary #5", "응접실 화분 아래", false},
    {1, 15, 16, "Diary #6", "현관 신발장", false}, {1, 45, 14, "Diary #7", "서재 책장", false}, {1, 42, 16, "Diary #8", "서재 카펫 아래", false},
    {2, 10, 11, "Diary #9", "드레스룸 옷 사이", false}, {2, 45, 14, "Diary #10", "사용인 방 탁자1", false}, {2, 47, 14, "Diary #11", "사용인 방 탁자2", false},
    {2, 45, 16, "Diary #12", "사용인 방 테라스", false}, {1, 25, 2, "Diary #13", "계단 큰 그림 뒤", false},
    {1, 30, 13, "Diary #14", "1층 복도 조각상", false}, {2, 25, 5, "Diary #15", "2층 복도 샹들리에", false}
};

Key keys[2] = {
    {1, 1, 40, 15, "Library Key", false},
    {2, 2, 5, 11, "Servant Room Key", false}
};

// --- [지도 데이터] ---
// 가변 배열로 정의합니다.
char map1F[MAP_HEIGHT][MAP_WIDTH + 1] = {
    "##################################################", // 0
    "# [Main Room]     | [Stairs]  | [Utility]  S     #", // 1 (S: 35, 1)
    "#                 |    (U)    |                  #", // 2
    "#---------+-------|           |-------+----------#", // 3
    "#         |                           | Secret   #", // 4 (M: 42, 4 동적)
    "# [Hall]  |         [Main Hall]       | Path     #", // 5
    "#         |                           |          #", // 6
    "#---------+-------|           |-------+----------#", // 7
    "#                 |           |                  #", // 8
    "# [Reception]     | [Entrance]| D    [Library]   #", // 9 (D: 32, 9)
    "#                 |           |                  #", // 10
    "#                 |           |                  #", // 11
    "#---------+-------|           |-------+----------#", // 12
    "# [Entrance]      |           | [Study / Office] #", // 13
    "#                 | [Front]   |                  #", // 14
    "##################################################", // 15
    "                                                  ", // 16 (빈 행)
    "                                                  "  // 17 (빈 행)
};

const char* map2F[MAP_HEIGHT] = {
    "##################################################", // 0
    "#  [Bedroom 1]    | [Stairs]  |  [Bedroom 2]     #", // 1
    "#                 |    (D)    |                  #", // 2
    "#---------+-------|           |-------+----------#", // 3
    "# [Storage]| [Bath]           | [Bath] | [Small] #", // 4
    "#---------+-------| [Hallway] |-------+----------#", // 5
    "# [DressRoom]     |           |                  #", // 6
    "#---------+-------|           |   [Servant Room] #", // 7
    "#                 |           |                  #", // 8
    "# [Master Room]   | [Terrace] |                  #", // 9
    "#                 |           |                  #", // 10
    "##################################################", // 11
    "                                                  ", // 12
    "                                                  ", // 13
    "                                                  ", // 14
    "                                                  ", // 15
    "                                                  ", // 16
    "                                                  "  // 17
};

void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Render() {
    gotoxy(0, 0); // 화면 맨 위에서부터 다시 그리기 시작 (잔상 방지)
    printf("==== Mansion Map [%dF] ====  Diary: %d/15\n", currentFloor, diaryCount);

    // 중요한 수정: 출력 루프가 전체 MAP_HEIGHT(18)를 커버해야 플레이어가 보입니다.
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {

            // 1. 플레이어 위치 최우선 출력
            if (x == pX && y == pY) {
                printf("P");
                continue;
            }

            // 2. 1층 퍼즐 오브젝트 동적 실시간 위치 처리
            if (currentFloor == 1) {
                // 책장 위치 처리 (IsWalkable에서 이동 불가로 처리됨)
                if (x == bookcaseX && y == bookcaseY) {
                    printf("M"); // 밀 수 있는 책장
                    continue;
                }
            }

            // 3. 일기 및 열쇠 조사 포인트 출력
            bool printed = false;
            for (int i = 0; i < 15; i++) {
                if (!diaries[i].is_collected && diaries[i].floor == currentFloor && diaries[i].x == x && diaries[i].y == y) {
                    printf("?");
                    printed = true;
                    break;
                }
            }
            if (!printed) {
                for (int i = 0; i < 2; i++) {
                    if (!keys[i].is_collected && keys[i].floor == currentFloor && keys[i].x == x && keys[i].y == y) {
                        printf("k");
                        printed = true;
                        break;
                    }
                }
            }

            // 4. 기본 배경 및 맵 구조물 출력
            if (!printed) {
                if (currentFloor == 1) printf("%c", map1F[y][x]);
                else printf("%c", map2F[y][x]);
            }
        }
        printf("\n");
    }
    printf("\n[F] Interact  [WASD] Move  [Q] Quit\n");
}

void CTRL_INTERACT() {
    // --- 기믹 1: 1층 [Utility] 전원 차단기(S) 상호작용 ---
    if (currentFloor == 1 && abs(pX - 35) <= 1 && abs(pY - 1) <= 1 && !is_breaker_off) {
        is_breaker_off = true;
        map1F[1][35] = ' '; // 차단기(S) 제거
        map1F[9][32] = ' '; // 도서관 문(D) 개방

        system("cls");
        printf("\n[ ⚡ SYSTEM NOTICE ]\n");
        printf("전원 차단기를 내렸습니다.\n");
        printf("도서관 문이 열리는 소리가 들립니다.\n");
        printf("\nPress any key...");
        _getch(); system("cls");
        return;
    }

    // --- 기믹 2: 1층 [Study] 책장(M) 밀기 상호작용 ---
    if (currentFloor == 1 && abs(pX - bookcaseX) <= 1 && abs(pY - bookcaseY) <= 1 && bookcaseY == 4) {
        bookcaseY = 2; // 책장을 위쪽 비밀 통로 안으로 밀어냄
        system("cls");
        printf("\n[ 📦 PUZZLE SOLVED ]\n");
        printf("무거운 책장을 밀어내고 비밀 통로를 열었습니다.\n");
        printf("\nPress any key...");
        _getch(); system("cls");
        return;
    }

    // --- 기존 기믹: 일기/열쇠/계단 처리 ---
    bool found = false;
    for (int i = 0; i < 15; i++) {
        if (!diaries[i].is_collected && diaries[i].floor == currentFloor && abs(pX - diaries[i].x) <= 1 && abs(pY - diaries[i].y) <= 1) {
            diaries[i].is_collected = true; diaryCount++;
            system("cls"); printf("\n[ Found: %s ]\n", diaries[i].name);
            _getch(); system("cls"); found = true; break;
        }
    }
    if (found) return;

    for (int i = 0; i < 2; i++) {
        if (!keys[i].is_collected && keys[i].floor == currentFloor && abs(pX - keys[i].x) <= 1 && abs(pY - keys[i].y) <= 1) {
            keys[i].is_collected = true;
            system("cls"); printf("\n[ Found: %s Key ]\n", keys[i].name);
            _getch(); system("cls"); found = true; break;
        }
    }
    if (found) return;

    if (currentFloor == 1 && pX >= 22 && pX <= 26 && pY <= 2) { // 계단 올라가기
        currentFloor = 2; pX = 24; pY = 2; // 계단 입구 앞으로 소환
        system("cls"); printf("\n[ Moving to 2F... ]\n"); Sleep(400); system("cls");
    }
    else if (currentFloor == 2 && pX >= 22 && pX <= 26 && pY <= 2) { // 계단 내려가기
        currentFloor = 1; pX = 24; pY = 2;
        system("cls"); printf("\n[ Moving to 1F... ]\n"); Sleep(400); system("cls");
    }
}

// 이동 전 벽이나 장애물 충돌 검사
bool IsWalkable(int nextX, int nextY) {
    if (nextX < 0 || nextX >= MAP_WIDTH || nextY < 0 || nextY >= MAP_HEIGHT) return false;

    char tile;
    if (currentFloor == 1) tile = map1F[nextY][nextX];
    else tile = map2F[nextY][nextX];

    // 벽(#), 문 구분선(-, +), 닫힌 문(D)은 통과 불가
    if (tile == '#' || tile == '-' || tile == '+' || tile == 'D') return false;

    // 동적 배치된 책장(M) 위치도 이동 불가
    if (currentFloor == 1 && nextX == bookcaseX && nextY == bookcaseY) return false;

    return true;
}

int main() {
    CONSOLE_CURSOR_INFO ci = { 100, FALSE }; // 커서 숨기기
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
    system("cls"); // 화면 최초 청소

    while (gameRunning) {
        Render();
        if (_kbhit()) {
            int key = _getch();
            int nextX = pX;
            int nextY = pY;

            if (key == 'q' || key == 'Q') break;
            else if (key == 'w' || key == 'W') nextY--;
            else if (key == 's' || key == 'S') nextY++;
            else if (key == 'a' || key == 'A') nextX--;
            else if (key == 'd' || key == 'D') nextX++;
            else if (key == 'f' || key == 'F') CTRL_INTERACT();

            // 충돌 검사 후 플레이어 좌표 갱신
            if (IsWalkable(nextX, nextY)) {
                pX = nextX;
                pY = nextY;
            }
        }
        Sleep(30);
    }
    return 0;
}
