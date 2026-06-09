#include "save.h"
#include "map.h"
#include <stdio.h>
#include <string.h>

/* 저장 파일 레이아웃:
   [slot0: sizeof(GameState)] [slot1] [slot2]
   슬롯 0 = 자동저장 전용 */

int save_game(GameState *gs, int slot) {
    if (slot < 0 || slot >= SAVE_SLOTS) return 0;
    FILE *f = fopen(SAVE_FILE, "r+b");
    if (!f) f = fopen(SAVE_FILE, "w+b"); /* 없으면 새로 생성 */
    if (!f) return 0;

    /* 슬롯 위치로 이동 */
    fseek(f, (long)(slot * sizeof(GameState)), SEEK_SET);
    size_t written = fwrite(gs, sizeof(GameState), 1, f);
    fclose(f);
    return (written == 1);
}

int load_game(GameState *gs, int slot) {
    if (slot < 0 || slot >= SAVE_SLOTS) return 0;
    FILE *f = fopen(SAVE_FILE, "rb");
    if (!f) return 0;

    fseek(f, (long)(slot * sizeof(GameState)), SEEK_SET);
    size_t read = fread(gs, sizeof(GameState), 1, f);
    fclose(f);
    if (read != 1) return 0;

    /* 안전 복구 */
    gs->running = 1;
    gs->mode    = MODE_MAP;

    /* 저장 상태를 맵에 반영 */
    map_sync_with_save(gs);

    return 1;
}

int save_exists(int slot) {
    if (slot < 0 || slot >= SAVE_SLOTS) return 0;
    FILE *f = fopen(SAVE_FILE, "rb");
    if (!f) return 0;

    /* 슬롯이 실제로 데이터를 가지고 있는지 확인 */
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fclose(f);
    return (size >= (long)((slot+1) * sizeof(GameState)));
}

void autosave(GameState *gs) {
    save_game(gs, 0); /* 슬롯 0 = 자동저장 */
}

void save_show_slots(void) {
    printf("\n  저장 슬롯 현황:\n");
    printf("  [0] 자동저장  %s\n", save_exists(0) ? "(데이터 있음)" : "(비어있음)");
    for (int i = 1; i < SAVE_SLOTS; i++) {
        printf("  [%d] 슬롯 %d    %s\n", i, i,
               save_exists(i) ? "(데이터 있음)" : "(비어있음)");
    }
    printf("\n");
}
