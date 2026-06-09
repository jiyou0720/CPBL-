#ifndef ENEMY_H
#define ENEMY_H

#include "state.h"

/* ── 귀신 종류 ── */
#define GHOST_TYPE_PATROL  0   /* 순찰형: 경로 순찰 → 감지 → 추격 */
#define GHOST_TYPE_AMBUSH  1   /* 매복형: 특정 지점 대기 → 근접 즉시 공격 */

#define MAX_GHOSTS        6    /* 맵에 존재하는 최대 귀신 수 */
#define GHOST_DETECT_RANGE 5   /* 순찰형 감지 반경 (타일) */
#define GHOST_ATTACK_RANGE 1   /* 공격 범위 */

/* ── 귀신 1개 ── */
typedef struct {
    int  alive;         /* 현재 활성 여부 */
    int  type;          /* GHOST_TYPE_PATROL / AMBUSH */
    int  floor;         /* 존재하는 층 */
    int  x, y;          /* 현재 좌표 */
    /* 순찰형 전용 */
    int  patrol[8][2];  /* 순찰 경유점 목록 (x,y) */
    int  patrol_len;    /* 경유점 개수 */
    int  patrol_idx;    /* 현재 목표 경유점 */
    int  chasing;       /* 추격 중 여부 */
    /* 공통 */
    int  move_tick;     /* 이동 틱 카운터 */
    int  move_delay;    /* 이동 속도 (틱/칸) */
    int  dialog_id;     /* 조우 시 대화 ID (0~4) */
    int  encountered;   /* 이미 대화 조우했는지 */
} Ghost;

/* ── 집사 ── */
typedef struct {
    int  active;        /* 추격 활성 */
    int  floor;
    int  x, y;
    int  move_tick;
    int  move_delay;
    int  detected;      /* 플레이어를 발견했는지 */
} Butler;

/* ── 전역 ── */
extern Ghost  ghosts[MAX_GHOSTS];
extern Butler butler;

/* ── 함수 ── */
void enemy_init(void);
void enemy_update(GameState *gs);   /* 매 프레임 호출 */

/* 귀신 조우 대화 시작 */
void ghost_encounter_start(GameState *gs, int ghost_idx);

/* 집사 추격 시작 (도서관 조사 후) */
void butler_chase_start(GameState *gs);

/* 도전과제 체크 */
void enemy_check_achievements(GameState *gs);

#endif
