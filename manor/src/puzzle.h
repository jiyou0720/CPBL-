#ifndef PUZZLE_H
#define PUZZLE_H

#include "state.h"

/* ── 퍼즐 ID ── */
#define PUZZLE_SWITCH      0   /* ON/OFF 스위치 */
#define PUZZLE_EQUATION    1   /* a,b,c,d 구하기 */
#define PUZZLE_FIRSTNUM    2   /* 처음 숫자 맞추기 */
#define PUZZLE_PATTERN     3   /* 숫자 규칙 찾기 */
#define PUZZLE_BUTLER_AGE  4   /* 집사의 나이 */
#define PUZZLE_SERVANT     5   /* 사용인 생존자 계산 */
#define PUZZLE_REASON      6   /* 사건의 근본 원인 */


/* ── 퍼즐 상태 ── */
#define PUZZLE_MAX_DIALS 8

typedef struct {
    int  id;                        /* 현재 퍼즐 ID */
    int  active;                    /* 현재 퍼즐 진행 중 */
    int  dials[PUZZLE_MAX_DIALS];   /* 현재 입력값 */
    int  cursor;                    /* 커서 위치 */
    int  failed;                    /* 실패 횟수 */
    int  hint_shown;                /* 힌트 메시지 표시 여부 */
} PuzzleState;

extern PuzzleState active_puzzle;

/* ── 함수 ── */
void puzzle_init(void);
void puzzle_start(GameState *gs, int puzzle_id);
void puzzle_input(GameState *gs, int ch);
void puzzle_render(GameState *gs);
int  puzzle_is_active(void);

#endif