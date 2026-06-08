#include "player.h"
#include "map.h"
#include "save.h"
#include "notes.h"
#include "puzzle.h"
#include <string.h>

/* ────────────────────────────────────────────────
   MAP: note_id 배치  (맵 좌표 → note ID 매핑)
   평면도 기반으로 일기가 놓인 타일 위치 정의
   ──────────────────────────────────────────────── */
typedef struct { int floor; int x; int y; int note_id; int item_id; } ObjSpot;

/* 일기 위치 (타일 'n'이 놓인 곳) */
static const ObjSpot NOTE_SPOTS[] = {
    /* 1층 */
    {1,  9,  4, NOTE_GUEST1, ITEM_NONE},
    {1, 18,  2, NOTE_GUEST2, ITEM_NONE},
    {1, 65,  2, NOTE_LOUNGE1, ITEM_NONE},
    {1, 67,  7, NOTE_LOUNGE2, ITEM_NONE},
    {1, 48, 10, NOTE_STUDY1, ITEM_NONE},
    {1, 64, 12, NOTE_STUDY2, ITEM_NONE},
    {1,  4, 12, NOTE_HALL, ITEM_NONE},

    /* 2층 */
    {2,  9, 16, NOTE_DRESSING, ITEM_NONE},
    {2,  3, 14, NOTE_SERVANT1, ITEM_NONE},
    {2, 29, 14, NOTE_SERVANT2, ITEM_NONE},
    {2, 33, 11, NOTE_TERRACE, ITEM_NONE},
    {2,  7,  3, NOTE_STAIRS, ITEM_NONE},
    {2, 34,  4, NOTE_CORRIDOR1, ITEM_NONE},
    {2, 11,  4, NOTE_CHANDELIER, ITEM_NONE},
    {2, 26,  4, NOTE_CORRIDOR2, ITEM_NONE},

    {-1,0,0,0,0}
};
/* 아이템 위치 */
static const ObjSpot ITEM_SPOTS[] = {

    {1, 15, 15, -1, ITEM_DIARY},

    {2,  6, 11, -1, ITEM_HAMMER},
    {2, 64, 4, -1, ITEM_HOLY},
    {1, 48,  6, -1, ITEM_KEY_LIB},   /* 도서관 열쇠 */
    {1, 37, 15, -1, ITEM_KEY_SRV},   /* 사용인방 열쇠 */

    {-1,0,0,0,0}
};
static const ObjSpot PUZZLE_SPOTS[] = {

    {1, 15,  4, PUZZLE_SWITCH,     ITEM_NONE},
    {1, 65,  4, PUZZLE_EQUATION,   ITEM_NONE},
    {1, 45, 15, PUZZLE_FIRSTNUM,   ITEM_NONE},
    {1, 12, 15, PUZZLE_PATTERN,    ITEM_NONE},
    {1,  7, 11, PUZZLE_BUTLER_AGE, ITEM_NONE},

    {2, 11,  2, PUZZLE_SERVANT,    ITEM_NONE},
    {2, 62, 16, PUZZLE_REASON,     ITEM_NONE},

    {-1,0,0,0,0}
};
/* ────────────────────────────────────────────────
   이동
   ──────────────────────────────────────────────── */
void player_move(GameState *gs, int dx, int dy)
{
    int nx = gs->px + dx;
    int ny = gs->py + dy;

    char tile = map_get(gs, nx, ny);

    /* 도서관 잠금문 (1층) */
    if (gs->floor == 1 && nx == 56 && ny == 3) {
        if (!gs->keys[0]) {
            state_set_msg(gs, "도서관 열쇠가 필요합니다.");
            return;
        }
    }

    /* 사용인방 잠금문 (2층) */
    if (gs->floor == 2 && nx == 48 && ny == 5) {
        if (!gs->keys[1]) {
            state_set_msg(gs, "사용인방 열쇠가 필요합니다.");
            return;
        }
    }

    /* 핵 방 잠금문 (2층) */
    if (gs->floor == 2 && nx == 65 && ny == 14) {

        for (int i = 0; i < MAX_PUZZLES; i++) {
            if (!gs->puzzles[i]) {
                state_set_msg(gs, "모든 퍼즐을 해결해야 합니다.");
                return;
            }
        }
    }

    /* 벽 */
    if (!map_walkable(gs, nx, ny))
        return;

    /* 계단 */
    if (tile == T_STAIRS_U) {
        gs->floor = 2;
        gs->px = 11;
        gs->py = 16;
        state_set_msg(gs, "2층으로 올라갔습니다.");
        autosave(gs);
        return;
    }

    if (tile == T_STAIRS_D) {
        gs->floor = 1;
        gs->px = 25;
        gs->py = 17;
        state_set_msg(gs, "1층으로 내려갔습니다.");
        autosave(gs);
        return;
    }

    /* 함정 */
    if (tile == T_TRAP) {
        player_damage(gs, 1);
        gs->trap_count++;

        if (gs->trap_count >= 5)
            gs->achieve[6] = 1;

        state_set_msg(gs, "함정에 걸렸습니다! (-1 HP)");
    }

    /* 이동 */
    gs->px = nx;
    gs->py = ny;

    /* 방 이동 → 자동 저장 */
    int cur_room = map_get_room(gs, nx, ny);

    if (cur_room != gs->prev_room &&
        cur_room != ROOM_NONE)
    {
        gs->prev_room = cur_room;

        gs->checkpoint_count++;

        if (gs->checkpoint_count >= 5)
            gs->achieve[5] = 1;

        autosave(gs);
    }
}
/* ────────────────────────────────────────────────
   F키 상호작용  — 4방향 타일 스캔
   ──────────────────────────────────────────────── */
void player_interact(GameState *gs) {
    int dirs[4][2] = {{0,-1},{0,1},{-1,0},{1,0}};

    for (int d = 0; d < 4; d++) {
        int tx = gs->px + dirs[d][0];
        int ty = gs->py + dirs[d][1];
        char tile = map_get(gs, tx, ty);

        /* ── 일기 ── */
        if (tile == T_NOTE) {
            /* 어떤 일기인지 위치로 조회 */
            int note_id = -1;
            for (int i = 0; NOTE_SPOTS[i].floor != -1; i++) {
                if (NOTE_SPOTS[i].floor == gs->floor &&
                    NOTE_SPOTS[i].x == tx && NOTE_SPOTS[i].y == ty) {
                    note_id = NOTE_SPOTS[i].note_id;
                    break;
                }
            }
            if (note_id >= 0) {
                int fresh = note_collect(gs, note_id);
                map_set(gs, tx, ty, T_FLOOR);
                if (fresh)
                    state_set_msg(gs, "일기 조각을 획득했습니다! [N]으로 읽기");
                else
                    state_set_msg(gs, "이미 획득한 일기입니다.");
            } else {
                /* 위치 미등록 일기 — 가장 가까운 미획득 일기로 배정 */
                for (int i = 0; i < MAX_NOTES; i++) {
                    if (!gs->notes[i]) {
                        note_collect(gs, i);
                        map_set(gs, tx, ty, T_FLOOR);
                        state_set_msg(gs, "일기 조각을 획득했습니다! [N]으로 읽기");
                        break;
                    }
                }
            }
            return;
        }

        /* ── 아이템 ── */
        if (tile == T_ITEM) {
            
            int item_id = ITEM_NONE;
            for (int i = 0; ITEM_SPOTS[i].floor != -1; i++) {
                if (ITEM_SPOTS[i].floor == gs->floor &&
                    ITEM_SPOTS[i].x == tx && ITEM_SPOTS[i].y == ty) {
                    item_id = ITEM_SPOTS[i].item_id;
                    break;
                }
            }
            
            if (item_id == ITEM_NONE) item_id = ITEM_DIARY; /* 기본값 */

            if (gs->item_count >= MAX_INV) {
                state_set_msg(gs, "인벤토리가 가득 찼습니다! 먼저 아이템을 버려주세요.");
                return;
            }
            state_add_item(gs, item_id);
            map_set(gs, tx, ty, T_FLOOR);

            static const char *inames[] =
                {"???","Holy Water","Hammer","Library Key","Servant Key","Diary"};
            char buf[60];
            snprintf(buf, sizeof(buf), "획득: %s", inames[item_id < 6 ? item_id : 0]);
            state_set_msg(gs, buf);

            /* 열쇠 바로 인벤 대신 플래그로 처리 */
            if (item_id == ITEM_KEY_LIB) {
                gs->keys[0] = 1;
                state_remove_item(gs, ITEM_KEY_LIB);
                state_set_msg(gs, "도서관 열쇠를 획득했습니다!");
            }
            if (item_id == ITEM_KEY_SRV) {
                gs->keys[1] = 1;
                state_remove_item(gs, ITEM_KEY_SRV);
                state_set_msg(gs, "사용인방 열쇠를 획득했습니다!");
            }
            return;
        }

        /* ── 열쇠 타일 ── */
        // if (tile == T_KEY) {
        //     /* 어느 열쇠인지 위치로 판별 */
        //     int is_lib = (gs->floor == 1 && tx == 42 && ty == 10);
        //     map_set(gs, tx, ty, T_FLOOR);
        //     if (is_lib) {
        //         gs->keys[0] = 1;
        //         state_set_msg(gs, "도서관 열쇠를 획득했습니다!");
        //     } else {
        //         gs->keys[1] = 1;
        //         state_set_msg(gs, "사용인방 열쇠를 획득했습니다!");
        //     }
        //     return;
        // }

        /* ── 퍼즐 ── */
        if (tile == T_PUZZLE) {
            int puz_id = -1;

            for (int i = 0; PUZZLE_SPOTS[i].floor != -1; i++) {
                if (PUZZLE_SPOTS[i].floor == gs->floor &&
                    PUZZLE_SPOTS[i].x == tx &&
                    PUZZLE_SPOTS[i].y == ty) {

                    puz_id = PUZZLE_SPOTS[i].note_id;   /* note_id 필드에 puzzle id 저장 */
                    break;
                }
            }

            /* 좌표가 등록되지 않은 경우 */
            if (puz_id < 0) {
                return;
            }

            /* 아직 해결하지 않은 퍼즐 */
            if (!gs->puzzles[puz_id]) {
                puzzle_start(gs, puz_id);
                gs->mode = MODE_EVENT;
                gs->active_npc_id = -10 - puz_id;   /* -10/-11/-12 */
            }
            /* 이미 해결한 퍼즐 */
            else {
                map_set(gs, tx, ty, T_FLOOR);
            }

            return;
        }
        /* ── NPC (집사) ── */
        if (tile == T_NPC) {
            gs->mode = MODE_EVENT;
            gs->active_npc_id = 0;
            /* 집사 기본 대사 */
            gs->dialog.line_count = 2;
            strncpy(gs->dialog.lines[0],
                "\"...Please, make yourself at home, guest.\"",
                MAX_DIALOG_LEN-1);
            strncpy(gs->dialog.lines[1],
                "\"There is no need to wander the manor at this hour.\"",
                MAX_DIALOG_LEN-1);
            gs->dialog.choice_count = 0;
            return;
        }

        /* ── 핵 ── */
        if (tile == T_CORE) {
            if (!gs->core_accessible) {
                state_set_msg(gs, "방이 봉인되어 있습니다. 먼저 퍼즐 3개를 모두 풀어야 합니다.");
                return;
            }
            /* 핵 파괴 분기 */
            int has_holy   = state_has_item(gs, ITEM_HOLY);
            int has_hammer = state_has_item(gs, ITEM_HAMMER);

            gs->mode = MODE_EVENT;
            gs->active_npc_id = 99;

            if (has_holy && has_hammer) {
                /* 해피엔딩 */
                gs->dialog.line_count = 3;
                strncpy(gs->dialog.lines[0],
                    "성수를 보석에 붓습니다.",
                    MAX_DIALOG_LEN-1);
                strncpy(gs->dialog.lines[1],
                    "보석이 흔들리며 금이 갑니다. 망치를 들어올립니다 --",
                    MAX_DIALOG_LEN-1);
                strncpy(gs->dialog.lines[2],
                    "[Enter를 눌러 핵을 파괴하세요]",
                    MAX_DIALOG_LEN-1);
                gs->dialog.choice_count = 1;
                strncpy(gs->dialog.choices[0], "핵을 파괴한다!", 63);
                gs->dialog.hp_penalty[0] = 0;
                gs->dialog.instant_ko[0] = 0;
                /* handle_input에서 선택 확인 후 MODE_ENDING_HAPPY로 전환 */
                /* active_npc_id=99 + choice 선택 시 main에서 처리 */
            } else if (!has_holy) {
                gs->dialog.line_count = 2;
                strncpy(gs->dialog.lines[0],
                    "보석이 붉게 맥동합니다. 망치는 효과가 없습니다.",
                    MAX_DIALOG_LEN-1);
                strncpy(gs->dialog.lines[1],
                    "먼저 성수로 약하게 만들어야 합니다.",
                    MAX_DIALOG_LEN-1);
                gs->dialog.choice_count = 0;
            } else { /* has_holy but no hammer */
                gs->dialog.line_count = 2;
                strncpy(gs->dialog.lines[0],
                    "성수를 붓습니다. 보석이 깜빡입니다 --",
                    MAX_DIALOG_LEN-1);
                strncpy(gs->dialog.lines[1],
                    "하지만 부술 도구가 없습니다. 망치를 찾아야 합니다!",
                    MAX_DIALOG_LEN-1);
                gs->dialog.choice_count = 0;
            }
            return;
        }
    }

    state_set_msg(gs, "여기에는 상호작용할 대상이 없습니다.");
}

/* ────────────────────────────────────────────────
   HP 관리
   ──────────────────────────────────────────────── */
void player_damage(GameState *gs, int amount) {
    int prev_hp = gs->hp;
    gs->hp -= amount;
    if (gs->hp <= 0) {
        gs->hp = 0;
        gs->mode = MODE_GAMEOVER;
        gs->achieve[2] = 1;
    }
    /* 도전과제: HP 1 남을 시 회복 */
    if (prev_hp > 1 && gs->hp == 1) {
        /* 회복 기회 메시지 — achieve[4]는 player_heal에서 세팅 */
    }
}

void player_heal(GameState *gs, int amount) {
    int prev = gs->hp;
    gs->hp += amount;
    if (gs->hp > HP_MAX) gs->hp = HP_MAX;
    /* 도전과제: HP 1에서 회복 */
    if (prev == 1) gs->achieve[4] = 1;
}
