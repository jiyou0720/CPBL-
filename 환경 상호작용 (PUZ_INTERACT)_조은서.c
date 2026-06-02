#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>    
#include <termios.h>   
#include <fcntl.h>     
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
    int id; 
    int floor;
    int x, y;
    char name[30];
    bool is_collected;
} Key;

// --- 글로벌 변수 ---
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
char map1F[MAP_HEIGHT][MAP_WIDTH + 1] = {
    "##################################################", 
    "# [Main Room]     | [Stairs]  | [Utility]  S     #", 
    "#                 |    (U)    |                  #", 
    "#---------+-------|           |-------+----------#", 
    "#         |                   | Secret   #", 
    "# [Hall]  |         [Main Hall]       | Path     #", 
    "#         |                   |          #", 
    "#---------+-------|           |-------+----------#", 
    "#                 |           |                  #", 
    "# [Reception]     | [Entrance]| D    [Library]   #", 
    "#                 |           |                  #", 
    "#                 |           |                  #", 
    "#---------+-------|           |-------+----------#", 
    "# [Entrance]      |           | [Study / Office] #", 
    "#                 | [Front]   |                  #", 
    "##################################################", 
    "                                                  ", 
    "                                                  "  
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
    "##################################################", 
    "                                                  ", 
    "                                                  ", 
    "                                                  ", 
    "                                                  ", 
    "                                                  ", 
    "                                                  "  
};

// --- 터미널 제어 및 입력 함수 ---
void gotoxy(int x, int y) {
    printf("\e[%d;%dH", y + 1, x + 1);
}

int _getch(void) {
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

int _kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

void Render() {
    gotoxy(0, 0); 
    printf("==== Mansion Map [%dF] ====  Diary: %d/15\n", currentFloor, diaryCount);

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {

            if (x == pX && y == pY) {
                printf("P");
                continue;
            }

            if (currentFloor == 1) {
                if (x == bookcaseX && y == bookcaseY) {
                    printf("M"); 
                    continue;
                }
            }

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

            if (!printed) {
                if (currentFloor == 1) printf("%c", map1F[y][x]);
                else printf("%c", map2F[y][x]);
            }
        }
        printf("\n");
    }
    printf("\n[F] Interact  [WASD] Move  [Q] Quit\n");
    fflush(stdout);
}

void CTRL_INTERACT() {
    if (currentFloor == 1 && abs(pX - 35) <= 1 && abs(pY - 1) <= 1 && !is_breaker_off) {
        is_breaker_off = true;
        map1F[1][35] = ' '; 
        map1F[9][32] = ' '; 

        printf("\e[1;1H\e[2J");
        printf("\n[ ⚡ SYSTEM NOTICE ]\n");
        printf("전원 차단기를 내렸습니다.\n");
        printf("도서관 문이 열리는 소리가 들립니다.\n");
        printf("\nPress any key...");
        fflush(stdout);
        _getch(); printf("\e[1;1H\e[2J");
        return;
    }

    if (currentFloor == 1 && abs(pX - bookcaseX) <= 1 && abs(pY - bookcaseY) <= 1 && bookcaseY == 4) {
        bookcaseY = 2; 
        printf("\e[1;1H\e[2J");
        printf("\n[ 📦 PUZZLE SOLVED ]\n");
        printf("무거운 책장을 밀어내고 비밀 통로를 열었습니다.\n");
        printf("\nPress any key...");
        fflush(stdout);
        _getch(); printf("\e[1;1H\e[2J");
        return;
    }

    bool found = false;
    for (int i = 0; i < 15; i++) {
        if (!diaries[i].is_collected && diaries[i].floor == currentFloor && abs(pX - diaries[i].x) <= 1 && abs(pY - diaries[i].y) <= 1) {
            diaries[i].is_collected = true; diaryCount++;
            printf("\e[1;1H\e[2J"); printf("\n[ Found: %s ]\n", diaries[i].name);
            fflush(stdout); _getch(); printf("\e[1;1H\e[2J"); found = true; break;
        }
    }
    if (found) return;

    for (int i = 0; i < 2; i++) {
        if (!keys[i].is_collected && keys[i].floor == currentFloor && abs(pX - keys[i].x) <= 1 && abs(pY - keys[i].y) <= 1) {
            keys[i].is_collected = true;
            printf("\e[1;1H\e[2J"); printf("\n[ Found: %s Key ]\n", keys[i].name);
            fflush(stdout); _getch(); printf("\e[1;1H\e[2J"); found = true; break;
        }
    }
    if (found) return;

    if (currentFloor == 1 && pX >= 22 && pX <= 26 && pY <= 2) { 
        currentFloor = 2; pX = 24; pY = 2; 
        printf("\e[1;1H\e[2J"); printf("\n[ Moving to 2F... ]\n"); fflush(stdout); usleep(400000); printf("\e[1;1H\e[2J");
    }
    else if (currentFloor == 2 && pX >= 22 && pX <= 26 && pY <= 2) { 
        currentFloor = 1; pX = 24; pY = 2;
        printf("\e[1;1H\e[2J"); printf("\n[ Moving to 1F... ]\n"); fflush(stdout); usleep(400000); printf("\e[1;1H\e[2J");
    }
}

bool IsWalkable(int nextX, int nextY) {
    if (nextX < 0 || nextX >= MAP_WIDTH || nextY < 0 || nextY >= MAP_HEIGHT) return false;

    char tile;
    if (currentFloor == 1) tile = map1F[nextY][nextX];
    else tile = map2F[nextY][nextX];

    if (tile == '#' || tile == '-' || tile == '+' || tile == 'D') return false;

    if (currentFloor == 1 && nextX == bookcaseX && nextY == bookcaseY) return false;

    return true;
}

int main() {
    printf("\e[?25l"); 
    printf("\e[1;1H\e[2J"); 

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

            if (IsWalkable(nextX, nextY)) {
                pX = nextX;
                pY = nextY;
            }
        }
        usleep(30000);
    }

    printf("\e[?25h"); 
    return 0;
}
