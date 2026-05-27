// 필수 | F키: 아이템 줍기, NPC 대화, 퍼즐 활성화(오브젝트 검사→단서 출력)

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>
#include <math.h>

// --- 시스템 설정 ---
#define MAP_WIDTH 50
#define MAP_HEIGHT 18

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
int pX = 5, pY = 5, currentFloor = 1;
int diaryCount = 0;
bool gameRunning = true;

// --- [데이터] 아이템 및 일기 배치 (이미지 기획 기반) ---
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
const char* map1F[MAP_HEIGHT] = {
    "##################################################",
    "# [Main Room]     | [Stairs]  | [Utility]        #",
    "#                 |    (U)    |                  #",
    "#---------+-------|           |-------+----------#",
    "#         |                           | Secret   #",
    "# [Hall]  |         [Main Hall]       | Path     #",
    "#         |                           |          #",
    "#---------+-------|           |-------+----------#",
    "#                 |           |                  #",
    "# [Reception]     | [Entrance]|      [Library]   #",
    "#                 |           |                  #",
    "#                 |           |                  #",
    "#---------+-------|           |-------+----------#",
    "# [Entrance]      |           | [Study / Office] #",
    "#                 | [Front]   |                  #",
    "##################################################"
};

const char* map2F[MAP_HEIGHT] = {
    "##################################################",
    "#  [Bedroom 1]    | [Stairs]  |  [Bedroom 2]     #",
    "#                 |    (D)    |                  #",
    "#---------+-------|           |-------+----------#",
    "# [Storage]| [Bath]           | [Bath] | [Small] #",
    "#---------+-------| [Hallway] |-------+----------#",
    "# [DressRoom]     |           |                  #",
    "#---------+-------|           |   [Servant Room] #",
    "#                 |           |                  #",
    "# [Master Room]   | [Terrace] |                  #",
    "#                 |           |                  #",
    "##################################################"
};

void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Render() {
    gotoxy(0, 0);
    printf("==== Mansion Map [%dF] ====  Diary: %d/15\n", currentFloor, diaryCount);
    const char** curMap = (currentFloor == 1) ? map1F : map2F;

    // 맵 렌더링 루프
    int height = (currentFloor == 1) ? 16 : 12; // 층별 높이 다름 대응
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (x == pX && y == pY) printf("P");
            else {
                bool printed = false;
                for (int i = 0; i < 15; i++) {
                    if (!diaries[i].is_collected && diaries[i].floor == currentFloor && diaries[i].x == x && diaries[i].y == y) {
                        printf("?"); // 조사 포인트
                        printed = true; break;
                    }
                }
                if (!printed) {
                    for (int i = 0; i < 2; i++) {
                        if (!keys[i].is_collected && keys[i].floor == currentFloor && keys[i].x == x && keys[i].y == y) {
                            printf("k"); printed = true; break;
                        }
                    }
                }
                if (!printed) printf("%c", curMap[y][x]);
            }
        }
        printf("\n");
    }
    printf("\n[F] Interact  [WASD] Move  [Q] Quit\n");
}

void CTRL_INTERACT() {
    bool found = false;
    for (int i = 0; i < 15; i++) {
        if (!diaries[i].is_collected && diaries[i].floor == currentFloor && abs(pX - diaries[i].x) <= 1 && abs(pY - diaries[i].y) <= 1) {
            diaries[i].is_collected = true;
            diaryCount++;
            system("cls");
            printf("\n[ Item Found: %s ]\nLocation: %s\nContent: (Diary details...)\n", diaries[i].name, diaries[i].hint);
            printf("\nPress any key to return...");
            _getch(); system("cls");
            found = true; break;
        }
    }
    for (int i = 0; i < 2; i++) {
        if (!keys[i].is_collected && keys[i].floor == currentFloor && abs(pX - keys[i].x) <= 1 && abs(pY - keys[i].y) <= 1) {
            keys[i].is_collected = true;
            system("cls");
            printf("\n[ Key Found: %s ]\nYou can now open locked doors.\n", keys[i].name);
            printf("\nPress any key to return...");
            _getch(); system("cls");
            found = true; break;
        }
    }
    // 계단 상호작용
    if (pX >= 20 && pX <= 30 && pY <= 2) {
        currentFloor = (currentFloor == 1) ? 2 : 1;
        system("cls");
        printf("\n[ Moving to Floor %d... ]\n", currentFloor);
        Sleep(500); system("cls");
        found = true;
    }
}

int main() {
    CONSOLE_CURSOR_INFO ci = { 100, FALSE };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);

    while (gameRunning) {
        Render();
        if (_kbhit()) {
            int key = _getch();
            if (key == 'q' || key == 'Q') break;
            if (key == 'w' || key == 'W') pY--;
            if (key == 's' || key == 'S') pY++;
            if (key == 'a' || key == 'A') pX--;
            if (key == 'd' || key == 'D') pX++;
            if (key == 'f' || key == 'F') CTRL_INTERACT();
        }
        Sleep(50);
    }
    return 0;
}
