#include "enemy.h"
#include "map.h"
#include "render.h"
#include "bgm.h"
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

Ghost  ghosts[MAX_GHOSTS];
Butler butler;

/* ════════════════════════════════════════════════
   귀신 5종 랜덤 대화  (스토리 기반)
   ════════════════════════════════════════════════ */
typedef struct {
    const char *ghost_line;
    const char *choice_a;       /* 안전한 선택 */
    const char *choice_b;       /* HP 패널티 or 즉사 선택 */
    int  hp_a;                  /* choice_a HP 변화 (0=무사) */
    int  hp_b;                  /* choice_b HP 패널티 */
    int  ko_a;                  /* choice_a → 즉시 게임오버 */
    int  ko_b;
    int  steal_b;               /* choice_b → 아이템 압수 */
} GhostDialog;

static const GhostDialog GHOST_DIALOGS[5] = {
    {
        "어머.. 여긴 어쩌다 온거니?",
        "날씨가 좋지 않아서요.",
        "어쩌라구요. [-HP 2]",
        0, 2, 0, 0, 0
    },
    {
        "내가... 보여..?",
        "..? (조용히 있는다)",
        "오, 귀신이 보이네요! [-HP 5]",
        0, 5, 0, 0, 0
    },
    {
        "나랑 같이 여기에 있어줄거야..?",
        "아니요. 나가야 해요.",
        "네...! [즉시 게임오버]",
        0, 0, 0, 1, 0
    },
    {
        "여기서 뭐하고 있니?",
        "여기서 나가려구요.",
        "아무것도... [-HP 4]",
        0, 4, 0, 0, 0
    },
    {
        "나한테.. 그거.. 줄래? (아이템을 바라본다)",
        "가져가세요. (아이템 안전)",
        "(도망친다!) [-HP 5, 아이템 압수]",
        0, 5, 0, 0, 1
    },
};


/* ════════════════════════════════════════════════
   초기 배치  (평면도 기반)
   ════════════════════════════════════════════════ */
void enemy_init(void) {
    memset(ghosts, 0, sizeof(ghosts));
    memset(&butler, 0, sizeof(butler));

    /* ── 순찰형 귀신 3마리 (1층) ── */

    /* Ghost 0: 1층 복도 세로 순찰 */
    ghosts[0].alive       = 1;
    ghosts[0].type        = GHOST_TYPE_PATROL;
    ghosts[0].floor       = 1;
    ghosts[0].x           = 20; ghosts[0].y = 5;
    ghosts[0].patrol[0][0]=20; ghosts[0].patrol[0][1]=3;
    ghosts[0].patrol[1][0]=20; ghosts[0].patrol[1][1]=16;
    ghosts[0].patrol_len  = 2;
    ghosts[0].move_delay  = 8;   /* 8틱마다 1칸 이동 (~0.4초) */
    ghosts[0].dialog_id   = 0;

    /* Ghost 1: 1층 복도 가로 순찰 */
    ghosts[1].alive       = 1;
    ghosts[1].type        = GHOST_TYPE_PATROL;
    ghosts[1].floor       = 1;
    ghosts[1].x           = 15; ghosts[1].y = 13;
    ghosts[1].patrol[0][0]=13; ghosts[1].patrol[0][1]=13;
    ghosts[1].patrol[1][0]=38; ghosts[1].patrol[1][1]=13;
    ghosts[1].patrol_len  = 2;
    ghosts[1].move_delay  = 10;
    ghosts[1].dialog_id   = 1;

    /* Ghost 2: 연구실/서재 순찰 */
    ghosts[2].alive       = 1;
    ghosts[2].type        = GHOST_TYPE_PATROL;
    ghosts[2].floor       = 1;
    ghosts[2].x           = 45; ghosts[2].y = 12;
    ghosts[2].patrol[0][0]=42; ghosts[2].patrol[0][1]=9;
    ghosts[2].patrol[1][0]=55; ghosts[2].patrol[1][1]=16;
    ghosts[2].patrol_len  = 2;
    ghosts[2].move_delay  = 12;
    ghosts[2].dialog_id   = 3;

    /* ── 매복형 귀신 2마리 (2층) ── */

    /* Ghost 3: 2층 복도 매복 */
    ghosts[3].alive       = 1;
    ghosts[3].type        = GHOST_TYPE_AMBUSH;
    ghosts[3].floor       = 2;
    ghosts[3].x           = 18; ghosts[3].y = 8;
    ghosts[3].move_delay  = 6;
    ghosts[3].dialog_id   = 2;

    /* Ghost 4: 드레스룸 매복 */
    ghosts[4].alive       = 1;
    ghosts[4].type        = GHOST_TYPE_AMBUSH;
    ghosts[4].floor       = 2;
    ghosts[4].x           = 40; ghosts[4].y = 5;
    ghosts[4].move_delay  = 6;
    ghosts[4].dialog_id   = 4;

    /* Ghost 5: 1층 응접실 매복 */
    ghosts[5].alive       = 1;
    ghosts[5].type        = GHOST_TYPE_AMBUSH;
    ghosts[5].floor       = 1;
    ghosts[5].x           = 6;  ghosts[5].y = 3;
    ghosts[5].move_delay  = 6;
    ghosts[5].dialog_id   = 1;

    /* ── 집사 초기 위치 (2층 계단 근처) ── */
    butler.active     = 0;
    butler.floor      = 1;
    butler.x          = 25; butler.y = 1;
    butler.move_delay = 6;   /* 플레이어보다 약간 빠름 */
}

/* ════════════════════════════════════════════════
   맨해튼 거리
   ════════════════════════════════════════════════ */
static int mdist(int x1, int y1, int x2, int y2) {
    int dx = x1 - x2; if (dx < 0) dx = -dx;
    int dy = y1 - y2; if (dy < 0) dy = -dy;
    return dx + dy;
}

/* ════════════════════════════════════════════════
   1칸 이동 (BFS-lite: 가장 가까운 방향으로 이동)
   ════════════════════════════════════════════════ */
static void move_toward(GameState *gs, int *ex, int *ey, int tx, int ty) {
    int best_d = mdist(*ex, *ey, tx, ty);
    int bx = *ex, by = *ey;
    int dirs[4][2] = {{0,-1},{0,1},{-1,0},{1,0}};
    for (int d = 0; d < 4; d++) {
        int nx = *ex + dirs[d][0];
        int ny = *ey + dirs[d][1];
        if (!map_walkable(gs, nx, ny)) continue;
        int dist = mdist(nx, ny, tx, ty);
        if (dist < best_d) {
            best_d = dist;
            bx = nx; by = ny;
        }
    }
    *ex = bx; *ey = by;
}

/* ════════════════════════════════════════════════
   순찰형 AI 업데이트
   ════════════════════════════════════════════════ */
static void update_patrol(GameState *gs, Ghost *g) {
    if (!g->alive || g->floor != gs->floor) return;

    g->move_tick++;
    if (g->move_tick < g->move_delay) return;
    g->move_tick = 0;

    int pdist = mdist(g->x, g->y, gs->px, gs->py);

    /* 감지 범위 내 플레이어 → 추격 모드 */
    if (pdist <= GHOST_DETECT_RANGE) {
        g->chasing = 1;
    } else if (pdist > GHOST_DETECT_RANGE + 3) {
        g->chasing = 0;  /* 너무 멀어지면 순찰 복귀 */
    }

    if (g->chasing) {
        move_toward(gs, &g->x, &g->y, gs->px, gs->py);
    } else {
        /* 순찰: 현재 목표 경유점으로 이동 */
        int *tgt = g->patrol[g->patrol_idx];
        move_toward(gs, &g->x, &g->y, tgt[0], tgt[1]);
        if (g->x == tgt[0] && g->y == tgt[1])
            g->patrol_idx = (g->patrol_idx + 1) % g->patrol_len;
    }

    /* 플레이어 접촉 → 조우 or 피해 */
    if (g->x == gs->px && g->y == gs->py) {
        if (!g->encountered) {
            ghost_encounter_start(gs, (int)(g - ghosts));
            g->encountered = 1;
        } else {
            /* 이미 대화한 귀신은 HP 직접 감소 */
            gs->hp -= 2;
            if (gs->hp <= 0) { gs->hp = 0; gs->mode = MODE_GAMEOVER; }
            state_set_msg(gs, "귀신이 공격했습니다! (-2 HP)");
            /* 약간 뒤로 밀어내기 */
            g->x += (g->x > gs->px) ? 1 : -1;
        }
    }
}

/* ════════════════════════════════════════════════
   매복형 AI 업데이트
   ════════════════════════════════════════════════ */
static void update_ambush(GameState *gs, Ghost *g) {
    if (!g->alive || g->floor != gs->floor) return;

    int pdist = mdist(g->x, g->y, gs->px, gs->py);

    /* 2타일 이내 접근 시 즉시 달려들기 */
    if (pdist <= 2) {
        g->move_tick++;
        if (g->move_tick < g->move_delay) return;
        g->move_tick = 0;

        move_toward(gs, &g->x, &g->y, gs->px, gs->py);

        if (g->x == gs->px && g->y == gs->py) {
            if (!g->encountered) {
                ghost_encounter_start(gs, (int)(g - ghosts));
                g->encountered = 1;
            } else {
                gs->hp -= 3;
                if (gs->hp <= 0) { gs->hp = 0; gs->mode = MODE_GAMEOVER; }
                state_set_msg(gs, "귀신에게 기습당했습니다! (-3 HP)");
                /* 원위치로 복귀 */
                g->x = g->patrol[0][0];
                g->y = g->patrol[0][1];
                if (g->x == 0 && g->y == 0) {
                    /* patrol 미설정 매복형 → 제자리 */
                }
            }
        }
    }
}

/* ════════════════════════════════════════════════
   집사 추격 업데이트
   ════════════════════════════════════════════════ */
static void update_butler(GameState *gs) {
    if (!butler.active) return;

    /* 핵의 방 진입 시 추격 중단 */
    if (map_get_room(gs, gs->px, gs->py) == ROOM_MASTER) {
        butler.active = 0;
        state_set_msg(gs, "집사가 문 앞에서 멈춥니다...");
        return;
    }

    /* 층이 다르면 계단 방향으로 이동 */
    if (butler.floor != gs->floor) {
        /* 1층 계단 위치(26,18)로 이동 후 층 변경 */
        move_toward(gs, &butler.x, &butler.y, 26, 18);
        if (butler.x == 26 && butler.y == 18) {
            butler.floor = gs->floor;
            butler.x = 11; butler.y = 16;
        }
        return;
    }

    butler.move_tick++;
    if (butler.move_tick < butler.move_delay) return;
    butler.move_tick = 0;

    move_toward(gs, &butler.x, &butler.y, gs->px, gs->py);

    /* 플레이어 접촉 → 배드엔딩 */
    if (butler.x == gs->px && butler.y == gs->py) {
        sfx_play("assets/death.wav");
        gs->mode = MODE_ENDING_BAD;
        gs->achieve[2] = 1; /* 죽어버리다니 */

        /* 도전과제: 집사에게 끝까지 안 걸린 경우 체크는 엔딩에서 */
    }
}

/* ════════════════════════════════════════════════
   전체 업데이트 (매 프레임)
   ════════════════════════════════════════════════ */
void enemy_update(GameState *gs) {
    if (gs->mode != MODE_MAP) return;

    for (int i = 0; i < MAX_GHOSTS; i++) {
        if (!ghosts[i].alive) continue;

        if (ghosts[i].type == GHOST_TYPE_PATROL)
            update_patrol(gs, &ghosts[i]);
        else
            update_ambush(gs, &ghosts[i]);
    }

    /* 집사 추격 30초 제한 */
    if (gs->butler_chase) {

        if (time(NULL) - gs->butler_start_time >= 30) {

            butler.active = 0;
            gs->butler_chase = 0;

            state_set_msg(gs,
                "집사의 기척이 사라졌다...");
        }
    }

    update_butler(gs);
}

/* ════════════════════════════════════════════════
   귀신 조우 대화 시작
   ════════════════════════════════════════════════ */
void ghost_encounter_start(GameState *gs, int ghost_idx) {
    int did = ghosts[ghost_idx].dialog_id % 5;
    const GhostDialog *dlg = &GHOST_DIALOGS[did];

    gs->mode = MODE_EVENT;
    gs->active_npc_id = ghost_idx + 1;  /* 1~6 = ghost */

    /* 대화 세팅 */
    memset(&gs->dialog, 0, sizeof(gs->dialog));
    gs->dialog.line_count = 1;
    strncpy(gs->dialog.lines[0], dlg->ghost_line, MAX_DIALOG_LEN-1);

    gs->dialog.choice_count = 2;
    strncpy(gs->dialog.choices[0], dlg->choice_a, 63);
    strncpy(gs->dialog.choices[1], dlg->choice_b, 63);

    gs->dialog.hp_penalty[0] = dlg->hp_a;
    gs->dialog.hp_penalty[1] = dlg->hp_b;
    gs->dialog.instant_ko[0] = dlg->ko_a;
    gs->dialog.instant_ko[1] = dlg->ko_b;
    gs->dialog.item_steal     = 0;  /* choice_b steal은 handle에서 처리 */

    gs->dialog.choice_sel = 0;

    /* 도전과제: 귀신 설득하는 영매사 (10회 연속 안전 선택) 추적용 */
    /* → main.c handle_input에서 선택 결과를 보고 streak 갱신 */
}

/* ════════════════════════════════════════════════
   집사 추격 시작
   ════════════════════════════════════════════════ */
void butler_chase_start(GameState *gs)
{
    butler.active = 1;

    /* 플레이어와 같은 층에 생성 */
    butler.floor = gs->floor;

    /* 플레이어 근처 생성 */
    butler.x = gs->px + 5;
    butler.y = gs->py;

    if(!map_walkable(gs, butler.x, butler.y))
    {
        butler.x = gs->px - 5;
    }

    /* 맵 밖 방지 */
    if(butler.x > 75)
        butler.x = gs->px - 8;

    if(butler.x < 1)
        butler.x = 1;

    if(butler.y < 1)
        butler.y = 1;

    butler.detected = 1;

    gs->butler_chase = 1;
    sfx_play("assets/butler.wav");
    gs->butler_start_time = time(NULL);

    state_set_msg(gs,
        "집사가 당신을 발견했습니다! 도망치세요!");
}

/* ════════════════════════════════════════════════
   도전과제 체크
   ════════════════════════════════════════════════ */
void enemy_check_achievements(GameState *gs) {
    /* 도전과제 10: 당신은 투명 인간 희망자
       집사 추격이 시작됐는데 한 번도 접촉 없이 핵 방 도달 */
    if (gs->butler_chase && !butler.active && !gs->achieve[10])
        gs->achieve[10] = 1;
}
