#include "state.h"
#include <string.h>
#include <stdio.h>

// void state_init(GameState *gs) {
//     memset(gs, 0, sizeof(GameState));

//     gs->mode      = MODE_MAP;
//     gs->floor     = 1;
//     gs->px        = 3;
//     gs->py        = 3;
//     gs->hp        = HP_MAX;
//     gs->running   = 1;
//     gs->start_time = time(NULL);

//     gs->ending_page = 0;   // 추가

//     for (int i = 0; i < MAX_PUZZLES; i++)
//         gs->puzzle_first_try[i] = 1;
// }
void state_init(GameState *gs)
{
    memset(gs, 0, sizeof(GameState));

    gs->mode = MODE_MAP;
    gs->running = 1;

    gs->floor = 1;
    gs->px = 2;
    gs->py = 2;

    gs->hp = HP_MAX;

    gs->start_time = time(NULL);
    gs->ending_page = 0;
}
int state_has_item(GameState *gs, int item_id) {
    for (int i = 0; i < gs->item_count; i++)
        if (gs->items[i] == item_id) return 1;
    return 0;
}

int state_add_item(GameState *gs, int item_id) {
    if (gs->item_count >= MAX_INV) return 0;
    gs->items[gs->item_count++] = item_id;
    return 1;
}

void state_remove_item(GameState *gs, int item_id) {
    for (int i = 0; i < gs->item_count; i++) {
        if (gs->items[i] == item_id) {
            for (int j = i; j < gs->item_count - 1; j++)
                gs->items[j] = gs->items[j+1];
            gs->items[--gs->item_count] = ITEM_NONE;
            return;
        }
    }
}

void state_set_msg(GameState *gs, const char *msg) {
    strncpy(gs->msg, msg, sizeof(gs->msg) - 1);
    gs->msg[sizeof(gs->msg)-1] = '\0';
    gs->msg_timer = 40; /* 약 2초 (20fps 기준) */
}

int state_elapsed(GameState *gs) {
    time_t now  = time(NULL);
    int elapsed = (int)(now - gs->start_time) - (int)gs->pause_accum;
    return elapsed < 0 ? 0 : elapsed;
}
