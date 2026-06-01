//각각의 퍼즐들은 주석으로 나누어서 구분하였음.

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

typedef struct {
    int  hp;
    int  has_heart_hint;
    int  is_alive;
} Player;

void apply_penalty(Player* player)
{
    player->hp--;
    printf("\n[!] HP: %d\n", player->hp);
    if (player->hp <= 0)
    {
        printf("\n==================================================\n");
        printf(" \uc9d1\uc0ac\uac00 \ub2e4\uac00\uc635\ub2c8\ub2e4.\n");
        printf(" GAME OVER\n");
        printf("==================================================\n");
        player->is_alive = 0;
    }
}
// PUZZLE 1: 버튼 순서 암기
void puzzle_simon_says(Player* player)
{
    if (!player->is_alive) return;

    printf("\n--- PUZZLE 1: \uc5b4\ub460 \uc18d\uc758 \uc554\uae30 ---\n");
    printf("\ubcbd\uba74\uc758 \uc7a5\uce58\uac00 \uc21c\uc11c\ub300\ub85c \ube5b\ub0a9\ub2c8\ub2e4.\n");
    printf("[ \uc81c\uc2dc\ub41c \uc21c\uc11c: 4 -> 2 -> 1 -> 5 -> 3 ]\n");

    int answer[5] = { 4, 2, 1, 5, 3 };
    int input;

    for (int i = 0; i < 5; i++)
    {
        printf("%d\ubc88\uc9f8 \ubc84\ud2bc \uc785\ub825: ", i + 1);
        if (scanf("%d", &input) != 1)
        {
            while (getchar() != '\n');
            continue;
        }
        if (input != answer[i])
        {
            apply_penalty(player);
            if (!player->is_alive) return;
            printf("\uc7a5\uce58\uac00 \ucd08\uae30\ud654\ub418\uc5c8\uc2b5\ub2c8\ub2e4. \ub2e4\uc2dc \uc2dc\ub3c4\ud558\uc138\uc694.\n");
            i = -1;
        }
    }
    printf("\n[OK] \ube44\ubc00 \uc11c\ub099\uc774 \uc5f4\ub838\uc2b5\ub2c8\ub2e4.\n");
}

// PUZZLE 2: 스위치 논리
void puzzle_switch_logic(Player* player)
{
    if (!player->is_alive) return;

    printf("\n--- PUZZLE 2: \uc81c\uc5b4\uc2e4\uc758 \uc2a4\uc704\uce58 ---\n");
    printf("1. \uccab \ubc88\uc9f8 \uc2a4\uc704\uce58\ub294 \ub9c8\uc9c0\ub9c9 \uc2a4\uc704\uce58\uc640 \uc0c1\ud0dc\uac00 \uac19\ub2e4.\n");
    printf("2. \ub450 \ubc88\uc9f8 \uc2a4\uc704\uce58\ub294 \ud56d\uc0c1 \uaebc\uc838(0) \uc788\uc5c8\ub2e4.\n");
    printf("3. \uc138 \ubc88\uc9f8 \uc2a4\uc704\uce58\ub294 \uc791\ub3d9 \uc911(1)\uc774\uc5c8\ub2e4.\n");
    printf("4. \ub124 \ubc88\uc9f8 \uc2a4\uc704\uce58\ub294 \uccab \ubc88\uc9f8\uc640 \ubc18\ub300 \uc0c1\ud0dc\uc774\ub2e4.\n");
    printf("5. \ucf1c\uc838(1) \uc788\ub294 \uc2a4\uc704\uce58 \ucd1d\uac1c\uc218\ub294 2\uac1c\uc774\ub2e4.\n");

    int s1, s2, s3, s4, s5;
    while (player->is_alive)
    {
        printf("\uc2a4\uc704\uce58 5\uac1c \uc785\ub825 (\uc608: 0 1 0 1 0): ");
        if (scanf("%d %d %d %d %d", &s1, &s2, &s3, &s4, &s5) != 5)
        {
            while (getchar() != '\n');
            apply_penalty(player);
            continue;
        }
        if (s1 == 0 && s2 == 0 && s3 == 1 && s4 == 1 && s5 == 0)
        {
            printf("\n[OK] \uc911\uc559 \ud640\uc758 \ubb38\uc774 \uc5f4\ub838\uc2b5\ub2c8\ub2e4.\n");
            break;
        }
        else apply_penalty(player);
    }
}

// PUZZLE 3: 연립방정식
void puzzle_math_equations(Player* player)
{
    if (!player->is_alive) return;

    printf("\n--- PUZZLE 3: \uc11c\uc7ac\uc758 \ube44\ubc00\ubc88\ud638 ---\n");
    printf("[a, b, c, d, e\ub294 \uc11c\ub85c \ub2e4\ub978 \ud55c \uc790\ub9ac \uc790\uc5f0\uc218]\n");
    printf("  a * b = 12\n");
    printf("  b + c = d\n");
    printf("  d - a = e\n");
    printf("  e + c = 7\n");

    int a, b, c, d, e;
    while (player->is_alive)
    {
        printf("a b c d e \uc785\ub825: ");
        if (scanf("%d %d %d %d %d", &a, &b, &c, &d, &e) != 5)
        {
            while (getchar() != '\n');
            apply_penalty(player);
            continue;
        }
        if (a == 3 && b == 4 && c == 1 && d == 5 && e == 2)
        {
            printf("\n[OK] \uae08\uace0\uac00 \uc5f4\ub838\uc2b5\ub2c8\ub2e4.\n");
            break;
        }
        else apply_penalty(player);
    }
}

// PUZZLE 4: 숫자 문제 2연속
void puzzle_movement_count(Player* player)
{
    if (!player->is_alive) return;

    printf("\n--- PUZZLE 4: \uae30\ud558\ud559\uc758 \ud68c\ub791 ---\n");
    printf("[Q1] \uc5b4\ub5a4 \uc218\uc5d0 2\ub97c \uacf1\ud55c \ud6c4 6\uc744 \ub354\ud558\uba74 14\uac00 \ub41c\ub2e4. \ucc98\uc74c \uc22b\uc790\ub294? : ");

    int num1;
    while (player->is_alive)
    {
        if (scanf("%d", &num1) != 1) while (getchar() != '\n');
        if (num1 == 4) { printf("\uc815\ub2f5\uc785\ub2c8\ub2e4.\n\n"); break; }
        else { apply_penalty(player); if (!player->is_alive) return; printf("\ub2e4\uc2dc \uc785\ub825: "); }
    }

    printf("[Q2] \uc774 \ubcf5\ub3c4\ub294 \ucd1d 7\uce78\uc785\ub2c8\ub2e4. 1\uce78 \ub610\ub294 2\uce78\uc529 \uc774\ub3d9 \uac00\ub2a5.\n");
    printf("\ub054\uae4c\uc9c0 \uc774\ub3d9\ud558\ub294 \ubc29\ubc95\uc740 \ucd1d \uba87 \uac00\uc9c0? : ");

    int num2;
    while (player->is_alive)
    {
        if (scanf("%d", &num2) != 1) while (getchar() != '\n');
        if (num2 == 21) { printf("\n[OK] \ubc1c\ud310\ub4e4\uc774 \uace0\uc815\ub418\uc5c8\uc2b5\ub2c8\ub2e4.\n"); break; }
        else { apply_penalty(player); if (!player->is_alive) return; printf("\ub2e4\uc2dc \uc785\ub825: "); }
    }
}

// PUZZLE 5: 집사 나이
void puzzle_butler_age(Player* player)
{
    if (!player->is_alive) return;

    printf("\n--- PUZZLE 5: \ub099\uc740 \uc77c\uae30\uc7a5 ---\n");
    printf("\"\ub098\ub294 \uc774 \uc800\ud0dd\uc5d0\uc11c \ub0b4 \uc778\uc0dd\uc758 1/6\uc744 \ud558\uc778\uc73c\ub85c \ubcf4\ub0c8\ub2e4.\n");
    printf(" \uadf8 \ud6c4\uc758 1/3\uc740 \uadf8\ubd84\uacfc \ud568\uaed8 \uc790\ub77c\uc628 \uc2dc\uac04\uc774\uc5c8\ub2e4.\n");
    printf(" \uadf8 \ub2e4\uc74c 1/7\uc774 \ud750\ub978 \ub4a4, \uc800\ud0dd\uc5d0 \ud3c9\uc0dd\uc744 \ubc14\uce58\uae30\ub85c \ud588\ub2e4.\n");
    printf(" \uadf8\ub9ac\uace0 12\ub144 \ud6c4, '\uc2ec\uc7a5'\uc744 \ucc98\uc74c\uc73c\ub85c \ub9cc\ub4e4\uc5c8\ub2e4.\"\n");
    printf("\uc774 \uc0ac\ub78c\uc758 \ub098\uc774\ub294? : ");

    int age;
    while (player->is_alive)
    {
        if (scanf("%d", &age) != 1) while (getchar() != '\n');
        if (age == 84)
        {
            printf("\n[OK] \uc77c\uae30\uc7a5 \ub4a4\uba74\uc5d0 \uc228\uaca8\uc9c4 \uacf5\uac04\uc774 \uc5f4\ub838\uc2b5\ub2c8\ub2e4.\n");
            player->has_heart_hint = 1;
            break;
        }
        else { apply_penalty(player); if (!player->is_alive) return; printf("\ub2e4\uc2dc \uc785\ub825: "); }
    }
}

// PUZZLE 6: 진실의 거울 (선택지)
void puzzle_mansion_mystery(Player* player)
{
    if (!player->is_alive) return;

    printf("\n--- PUZZLE 6: \uc9c4\uc2e4\uc758 \uac70\uc6b8 ---\n");
    printf("[Q1] \uc774 \ubaa8\ub4e0 \uc0ac\uac74\uc758 \uadfc\ubcf8\uc801\uc778 \uc6d0\uc778\uc740?\n");
    printf("1.\uadc0\uc2e0\uc758 \uc800\uc8fc  2.\uc9d1\uc0ac\uc758 \ud589\ub3d9  3.\uc800\ud0dd \uc8fc\uc778\uc758 \uc9d1\ub9e4  4.\uc800\ud0dd \uc790\uccb4\uc758 \uc601\ud5a5\n\uc120\ud0dd: ");

    int ans1;
    while (player->is_alive)
    {
        if (scanf("%d", &ans1) != 1) while (getchar() != '\n');
        if (ans1 == 3) { printf("\n[OK] \uac70\uc6b8\uc774 \uae68\uc9c0\uba70 \uc228\uaca8\uc9c4 \ud1b5\ub85c\uac00 \ub098\ud0d0\ub2c8\ub2e4.\n"); break; }
        else { apply_penalty(player); if (!player->is_alive) return; printf("\ub2e4\uc2dc \uc120\ud0dd: "); }
    }
}

// FINAL PUZZLE: 사용인 수 + 병명 맞추기
void puzzle_final_room(Player* player)
{
    if (!player->is_alive) return;

    printf("\n--- FINAL PUZZLE: \uc800\ud0dd\uc758 \uc911\uc2ec\ubd80 ---\n");
    printf("\ucc98\uc74c\uc5d0\ub294 50\uba85\uc758 \uc0ac\uc6a9\uc778\uc774 \uc788\uc5c8\ub2e4.\n");
    printf("\uccab\uc9f8 \ub0a0: \uc804\uccb4\uc758 2/5\uac00 \uc0ac\ub77c\uc84c\ub2e4.\n");
    printf("\ub458\uc9f8 \ub0a0: \ub0a8\uc740 \uc778\uc6d0\uc758 \uc808\ubc18\uc774 \uc0ac\ub77c\uc84c\ub2e4. (\uc911\ubcf5 3\uba85, \ubc14\uadc0 1\uba85)\n");
    printf("\uc14b\uc9f8 \ub0a0: 7\uba85\uc774 \ub354 \uc0ac\ub77c\uc9c0\uace0, \uc0ac\ub77c\uc9c4 \uc774\ub4e4 \uc911 3\uba85\uc774 4\uba85\uc744 \ub370\ub824\uac14\ub2e4.\n");
    printf("\ub113\uc9f8 \ub0a0: 6\uba85 \ud0c8\ucd9c, \ub0a8\uc740 \uc778\uc6d0\uc758 1/3\uc774 \uc0ac\ub77c\uc84c\ub2e4.\n");
    printf("\uc0b4\uc544\uc788\ub294 \uc0ac\uc6a9\uc778\uc740 \uba87 \uba85? : ");

    int survivors;
    while (player->is_alive)
    {
        if (scanf("%d", &survivors) != 1) while (getchar() != '\n');
        if (survivors == 0) { printf("\uc815\ub2f5\uc785\ub2c8\ub2e4.\n\n"); break; }
        else { apply_penalty(player); if (!player->is_alive) return; printf("\ub2e4\uc2dc \uc785\ub825: "); }
    }

    if (player->has_heart_hint)
        printf("(\ud78c\ud2b8: \uc77c\uae30\uc7a5\uc5d0\uc11c \ubcf8 '\uc2ec\uc7a5 \ub300\uccb4\uc81c'\uc758 \uae30\ub85d\uc774 \ub5a0\uc624\ub985\ub2c8\ub2e4.)\n");

    printf("\uc774 \ubc29\uc758 \uc8fc\uc778\uc774 \uc549\uc558\ub358 \ubcd1\uc740?\n");
    printf("1.\ub2f9\ub2e8\ubcd1  2.\ube48\ud601  3.\uc2ec\uc7a5\ubcd1  4.\ud3d0\ub834\n\uc120\ud0dd: ");

    int disease;
    while (player->is_alive)
    {
        if (scanf("%d", &disease) != 1) while (getchar() != '\n');
        if (disease == 3)
        {
            printf("\n==================================================\n");
            printf(" [OK] \ubaa8\ub4e0 \ubbf8\uc2a4\ud130\ub9ac\ub97c \ud480\uc5c8\uc2b5\ub2c8\ub2e4!\n");
            printf(" \uc800\ud0dd\uc758 \ubb38\uc774 \uc5f4\ub838\uc2b5\ub2c8\ub2e4.\n");
            printf("==================================================\n");
            break;
        }
        else { apply_penalty(player); if (!player->is_alive) return; printf("\ub2e4\uc2dc \uc120\ud0dd: "); }
    }
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX) };
    GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    wcscpy(cfi.FaceName, L"gulim");
    cfi.dwFontSize.X = 8;
    cfi.dwFontSize.Y = 16;
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    Player player = { 5, 0, 1 };

    printf("==================================================\n");
    printf("           \uc800\ud0dd\uc758 \ubbf8\uc2a4\ud130\ub9ac \ud0c8\ucd9c \uac8c\uc784\n");
    printf("==================================================\n");

    puzzle_simon_says(&player);
    puzzle_switch_logic(&player);
    puzzle_math_equations(&player);
    puzzle_movement_count(&player);
    puzzle_butler_age(&player);
    puzzle_mansion_mystery(&player);
    puzzle_final_room(&player);

    return 0;
}
