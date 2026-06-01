#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define MAX_TEXT_LEN 256
#define MAX_OBJECTS  100

typedef struct {
    int hp;
    int has_holy_water;
    int has_weapon;
    int diaries_found;
    int puzzle_fail_count;
    int trap_count;
    int is_spotted_by_steward;
    int ghost_evade_streak;
} GameState;

typedef struct {
    int  id;
    char text[MAX_TEXT_LEN];
} InspectData;

InspectData inspect_db[MAX_OBJECTS];
int         inspect_count = 0;

void play_bgm(const char* filename)
{
    char cmd[512];
    mciSendString("stop bgm", NULL, 0, NULL);
    mciSendString("close bgm", NULL, 0, NULL);
    sprintf(cmd, "open \"%s\" type mpegvideo alias bgm", filename);
    mciSendString(cmd, NULL, 0, NULL);
    mciSendString("play bgm repeat", NULL, 0, NULL);
}

void stop_bgm()
{
    mciSendString("stop bgm", NULL, 0, NULL);
    mciSendString("close bgm", NULL, 0, NULL);
}

void play_sfx(const char* filename)
{
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
}

void load_inspect_data()
{
    FILE* file = fopen("inspect.dat", "r");
    if (!file) return;

    inspect_count = 0;
    while (fscanf(file, "%d %255s",
        &inspect_db[inspect_count].id,
        inspect_db[inspect_count].text) == 2)
    {
        for (int i = 0; inspect_db[inspect_count].text[i]; i++)
            if (inspect_db[inspect_count].text[i] == '_')
                inspect_db[inspect_count].text[i] = ' ';
        inspect_count++;
        if (inspect_count >= MAX_OBJECTS) break;
    }
    fclose(file);
}

void inspect_object(int object_id)
{
    int found = 0;
    for (int i = 0; i < inspect_count; i++)
    {
        if (inspect_db[i].id == object_id)
        {
            printf("\n[F] %s\n", inspect_db[i].text);
            found = 1;

            if (object_id >= 201 && object_id <= 202)
                play_sfx("paper_turn.wav");
            else if (object_id >= 301 && object_id <= 302)
                play_sfx("item_bell.wav");
            else
                play_sfx("interact.wav");
            break;
        }
    }
    if (!found) printf("\n[F] \ud3c9\ubc94\ud55c \ubb3c\uac74\uc774\ub2e4.\n");
}
//튜토
void start_tutorial()
{
    int choice1, choice2;
    while (1)
    {
        printf("\n1. \uc870\uc791\ud0a4 \uc124\uba85\n2. \uac8c\uc784 \uc2dc\uc791\n\uc120\ud0dd: ");
        if (scanf("%d", &choice1) != 1) { while (getchar() != '\n'); continue; }

        if (choice1 == 1)
        {
            printf("\n1. \uc774\ub3d9\ud0a4\n2. \uc0c1\ud638\uc791\uc6a9\n\uc120\ud0dd: ");
            if (scanf("%d", &choice2) != 1) { while (getchar() != '\n'); continue; }
            if (choice2 == 1) printf("-> \uc774\ub3d9: WASD\n");
            else if (choice2 == 2) printf("-> \uc870\uc0ac: F\ud0a4  /  \uc778\ubca4\ud1a0\ub9ac: E\ud0a4\n");
        }
        else break;
    }
}

// 귀신 이벤트
void trigger_ghost_event(GameState* game)
{
    play_sfx("ghost_whisper.wav");

    int ghost_type = (rand() % 5) + 1;
    int sel = 0;

    printf("\n[\uadc0\uc2e0 \uc870\uc6b0]\n");

    switch (ghost_type)
    {
    case 1:
        printf("\uadc0\uc2e0: \"\uc5b4\uba38.. \uc5ec\uae34 \uc5b4\uc9a9\ub2e4\uc628\uac70\ub2c8?\"\n");
        printf("1. \ub0a0\uc528\uac00 \uc88b\uc9c0 \uc54a\uc544\uc11c\uc694.  2. \uc5b4\uc9a9\ub77c\uad6c\uc694.\n\uc120\ud0dd: ");
        if (scanf("%d", &sel) != 1) sel = 0;
        if (sel == 1) printf("\uadc0\uc2e0: \"\uadf8\uac70 \ucc38 \uc548\ud0c0\uae4c\uc6b4 \uc560\uae30\ub124~\"\n");
        else { play_sfx("hit.wav"); game->hp -= 2; printf("\uadc0\uc2e0: \"\ub098\uc05c \uc544\uc774\uad6c\ub098..?^^\"\n"); }
        break;
    case 2:
        printf("\uadc0\uc2e0: \"\ub0b4\uac00 \ubcf4\uc5ec..?\"\n");
        printf("1. ..?  2. \uc624, \uadc0\uc2e0\uc774 \ubcf4\uc774\ub124\uc694.\n\uc120\ud0dd: ");
        if (scanf("%d", &sel) != 1) sel = 0;
        if (sel == 1) printf("\uadc0\uc2e0: \"\ub0b4\uac00 \uc548\ubcf4\uc5ec\uc11c \ub2e4\ud589\uc774\ub124..\"\n");
        else { play_sfx("hit.wav"); game->hp -= 5; printf("\uadc0\uc2e0: \"\ub108\ub3c4 \uacf3 \ub098\ucccc\ub9bc \ub418\ub824\ub098..?\"\n"); }
        break;
    case 3:
        printf("\uadc0\uc2e0: \"\ub098\ub791 \uac19\uc774 \uc5ec\uae30\uc5d0 \uc788\uc5b4\uc904\uac70\uc57c..?\"\n");
        printf("1. \ub124..!  2. \uc544\ub2c8\uc694.\n\uc120\ud0dd: ");
        if (scanf("%d", &sel) != 1) sel = 0;
        if (sel == 1) { game->hp = 0; printf("\uadc0\uc2e0: \"\ub098\ucccc\ub9bc \uadc0\uc2e0\uc774 \ub418\ub294\uac70\uc57c..!\"\n"); }
        else printf("\uadc0\uc2e0: \"\uc544\uc4f0\ub124.. \ub098\ucccc\ub9bc \ub9cc\ub4e4\uc5b4\uc8fc\uace0 \uc2f6\uc5c8\ub294\ub370..\"\n");
        break;
    case 4:
        printf("\uadc0\uc2e0: \"\uc5ec\uae30\uc11c \ub2ed\ud558\uace0 \uc788\ub2c8?\"\n");
        printf("1. \uc5ec\uae30\uc11c \ub098\uac00\ub824\uad6c\uc694.  2. \uc544\ubb34\uac83\ub3c4..\n\uc120\ud0dd: ");
        if (scanf("%d", &sel) != 1) sel = 0;
        if (sel == 1) printf("\uadc0\uc2e0: \"\uac70\uc9d3\ub9d0 \ud558\uc9c0\uc54a\ub294 \uc544\uc774\ub294 \uc2eb\uc5b4\ud558\uc9c0 \uc54a\uc544.\"\n");
        else { play_sfx("hit.wav"); game->hp -= 3; printf("\uadc0\uc2e0: \"\uc5b4\ub514\uc11c \uac70\uc9d3\ub9d0\uc774\uc57c..!!\"\n"); }
        break;
    case 5:
        printf("\uadc0\uc2e0: \"\ub098\ud55c\ud14c.. \uadf8\uac70.. \uc904\ub798..?\"\n");
        printf("1. \uac00\uc838\uac00\uc138\uc694  2. (\ub3c4\ub9dd\uce5c\ub2e4.)\n\uc120\ud0dd: ");
        if (scanf("%d", &sel) != 1) sel = 0;
        if (sel == 1) printf("\uadc0\uc2e0: \"\uc544\ub2c8\uc57c.. \ucc29\ud55c \uc544\uc774\uc758 \ubb3c\uac74\uc740 \uc548 \uac00\uc838\uac08\uac70\uc57c..\"\n");
        else
        {
            play_sfx("hit.wav");
            game->hp -= 2;
            game->has_holy_water = 0;
            game->has_weapon = 0;
            printf("\uadc0\uc2e0: \"\uc5b4\ub525\ub97c \ub3c4\ub9dd\uac00..!!\"\n");
        }
        break;
    }
    printf("[\ud604\uc7ac HP: %d]\n", game->hp);
}

// 해피엔딩 / 배드엔딩
void ending_happy()
{
    stop_bgm();
    play_bgm("core_room_mystery.mp3");

    printf("\n\uc8fc\uc778\uacf5\uc740 \ubd89\uac8c \ube5b\ub098\ub294 \ubcf4\uc11d \uc55e\uc5d0 \uc120\ub2e4.\n");
    printf("\"\uc774\uc81c... \uc774\uac83\ub9cc \ubd80\uc218\uba74 \ub098\ub294 \ub3cc\uc544\uac08 \uc218 \uc788\uc5b4.\"\n");
    printf("\uc9d1\uc0ac: \"\uadf8\uac83\ub9cc\uc740 \uc548 \ub41c\ub2e4!! \uc8fc\uc778\ub2d8\ub9c8\uc800 \uc644\uc804\ud788 \uc0ac\ub77c\uc838\ubc84\ub9b0\ub2e4...\"\n");

    play_bgm("core_destruction_climax.mp3");
    printf("\n\uc131\uc218\ub85c \uc57d\uccb4\ud654 \ud6c4 \ub9dd\uce58\ub85c \ud575\uc744 \ud30c\uad34\ud569\ub2c8\ub2e4.\n");

    stop_bgm();
    play_sfx("glass_shatter.wav");
    printf("\n\uc800\ud0dd \uc804\uccb4\uac00 \ub4a4\ud2c0\ub9ac\uba70 \ubb34\ub108\uc838 \ub0b4\ub9ac\uace0 \uc9d1\uc0ac\ub294 \uc0ac\ub77c\uc9d1\ub2c8\ub2e4.\n");

    play_bgm("ending_forest.mp3");
    printf("\n\uc815\ubb38\uc744 \ub098\uc624\uc790 \uc800\ud0dd\uc740 \ud759\uc801\ub3c4 \uc5c6\uc774 \uc0ac\ub77c\uc838 \uc788\uc5c8\ub2e4.\n");

    play_sfx("phone_ring.wav");
    printf("\n\uc804\ud654\uac00 \uc6b8\ub9bd\ub2c8\ub2e4.\n");
    printf("\"\uc57c! \ub108 \uc5b4\ub514\uc57c? \ube68\ub9ac \ub0b4\ub824\uc640!\"\n");
    printf("\"\uc5b4... \ubbf8\uc548. \uae38\uc744 \uc88b \ud5e4\ub300.\"\n");
    printf("\n- The End (Happy Ending) -\n");
}

void ending_bad()
{
    stop_bgm();
    play_sfx("capture.wav");
    printf("\n\uc9d1\uc0ac\uc5d0\uac8c \ubc1c\ubaa9\uc744 \uc7a1\ud600\uc2b5\ub2c8\ub2e4.\n");
    printf("\"\ub2f9\uc2e0\uc758 '\uc601\ud63c'\uc740 \ud5db\ub418\uc774 \uc4f0\uc774\uc9c0 \uc54a\uc744 \ud14c\ub2c8\uae4c\uc694.\"\n");
    printf("\n- The End (Bad Ending) -\n");
}

// 메인 게임 루프
void main_game(GameState* game)
{
    int menu;

    play_bgm("guest_room.mp3");
    printf("\n[\ud604\uc7ac\uc704\uce58: \uc190\ub2d8\ubc29]\n");

    while (game->hp > 0)
    {
        printf("\n--------------------------------------------------\n");
        printf("[HP: %d] [\uc77c\uae30: %d/15] [\uc131\uc218: %s] [\ub9dd\uce58: %s]\n",
            game->hp, game->diaries_found,
            game->has_holy_water ? "O" : "X",
            game->has_weapon ? "O" : "X");
        printf("1. \ubcf5\ub3c4 \uc870\uc0ac\n2. \uc624\ube0c\uc81d\ud2b8 \uc870\uc0ac (F\ud0a4)\n");
        printf("3. \ud37c\uc990 \ud480\uae30\n4. \uc544\uc774\ud15c \uc218\uc0c9\n5. \ud575 \ubc29\uc73c\ub85c \uc774\ub3d9\n\uc120\ud0dd: ");
        if (scanf("%d", &menu) != 1) { while (getchar() != '\n'); continue; }

        if (menu == 1)
        {
            trigger_ghost_event(game);
        }
        else if (menu == 2)
        {
            int obj_id;
            char f_key;
            printf("\uc870\uc0ac\ud560 \uc624\ube0c\uc81d\ud2b8 ID: ");
            if (scanf("%d", &obj_id) != 1) { while (getchar() != '\n'); continue; }
            while (getchar() != '\n');
            printf("F\ud0a4\ub97c \ub88c\ub974\uc138\uc694: ");
            if (scanf("%c", &f_key) != 1) continue;
            if (f_key == 'f' || f_key == 'F') inspect_object(obj_id);
        }
        else if (menu == 3)
        {
            play_bgm("puzzle_bgm.mp3");
            int p_ans;
            printf("\ud37c\uc990 \uc815\ub2f5 \uc785\ub825: ");
            if (scanf("%d", &p_ans) != 1) { while (getchar() != '\n'); continue; }
            if (p_ans == 2)
            {
                play_sfx("box_open.wav");
                if (game->diaries_found < 15)
                {
                    game->diaries_found++;
                    play_sfx("item_pickup.wav");
                }
            }
            else game->puzzle_fail_count++;
        }
        else if (menu == 4)
        {
            if (!game->has_holy_water) { game->has_holy_water = 1; play_sfx("item_pickup.wav"); printf("\uc131\uc218 \ud68d\ub4dd!\n"); }
            else if (!game->has_weapon) { game->has_weapon = 1; play_sfx("item_pickup.wav"); printf("\ub9dd\uce58 \ud68d\ub4dd!\n"); }
        }
        else if (menu == 5)
        {
            stop_bgm();
            play_bgm("chase.mp3");

            if (game->diaries_found < 3) { ending_bad(); return; }

            if (game->has_holy_water && game->has_weapon)
            {
                ending_happy();
                return;
            }
            else
            {
                int sub;
                printf("\uc544\uc774\ud15c \ubd80\uc871. \ub4e4\uc5b4\uac00\uc2dc\uaca0\uc2b5\ub2c8\uae4c? (1.\uc608 / 2.\uc544\ub2c8\uc624): ");
                if (scanf("%d", &sub) != 1) continue;
                if (sub == 1 && game->has_holy_water && !game->has_weapon)
                {
                    play_bgm("steward_nearby.mp3");
                    play_sfx("hit.wav");
                    game->hp -= 8;
                    if (game->hp <= 0) { ending_bad(); return; }
                }
            }
        }
    }
    ending_bad();
}
//main
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

    srand((unsigned int)time(NULL));

    GameState game = { 10, 0, 0, 0, 0, 0, 0, 0 };
    int choice;

    load_inspect_data();

    play_bgm("outside_mansion.mp3");
    play_bgm("rain_fog.mp3");
    play_sfx("door_open.wav");
    stop_bgm();
    play_sfx("door_slam.wav");
    play_bgm("steward_appearance.mp3");

    printf("[%s] 1. %s / 2. %s: ",
        "\uba54\ub274",
        "\ud29c\ud1a0\ub9ac\uc5bc",
        "\uc2dc\uc791");
    if (scanf("%d", &choice) != 1) choice = 2;
    if (choice == 1) start_tutorial();

    main_game(&game);
    return 0;
}
