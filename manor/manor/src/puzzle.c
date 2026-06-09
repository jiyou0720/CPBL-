#include "puzzle.h"
#include "render.h"
#include "state.h"
#include "map.h"
#include <ncurses.h>
#include <string.h>
#include <stdio.h>

PuzzleState active_puzzle = {0};

/* ════════════════════════════════════════════════
   Puzzle definitions
   ════════════════════════════════════════════════

   PUZZLE 0 — Clock  (Lounge)
     4 dials, each 0-9.  Answer: 1  1  2  3
     Hint in Note [10]: "Puzzle 1: The clock in the lounge."
     Clue: a broken clock on the wall shows   1 1 : 2 3

   PUZZLE 1 — Painting order  (2F corridor)
     4 panels, each labeled 1-4.  Answer: 3  1  4  2
     Hint: numbers scratched lightly on the back of each frame.

   PUZZLE 2 — Lock mechanism  (Study)
     3 dials, each A-E (0-4).  Answer: B  D  A  (1  3  0)
     Hint: symbols on the study carpet match B, D, A.
   ════════════════════════════════════════════════ */

typedef struct {
    const char *title;
    const char *description[8];
    int  desc_lines;
    int  dial_count;
    int  dial_max;       /* max value per dial (inclusive) */
    int  answer[PUZZLE_MAX_DIALS];
    const char *hint;
} PuzzleDef;

static const PuzzleDef DEFS[7] = {

/* PUZZLE 0 — ON/OFF 스위치 */
{
    "퍼즐: 스위치 제어실",

    {
        "5개의 스위치가 놓여 있다.",
        "어떤 스위치를 켜야 하는지 알아내야 한다.",
        "1번째 스위치는 마지막 스위치와 같은 상태였다.",
        "2번째 스위치는 항상 꺼져 있었다.",
        "3번째 스위치는 작동 중이었다.",
        "정답을 입력하라. (ON=1 OFF=0)",
        NULL
    },

    6,
    5, 2,

    {1, 0, 1, 0, 1},

    "10101"
},

/* PUZZLE 1 — a,b,c,d 구하기 */
{
    "퍼즐: 집사의 기록",

    {
        "a, b, c, d를 구하시오.",
        "a + b = c",
        "c + d = 10",
        "d = a + 2",
        "b × d = 24",
        NULL
    },

    5,
    4, 10,

    {2, 6, 8, 2},

    "a=2 b=6 c=8 d=2"
},

/* PUZZLE 2 — 처음 숫자 맞추기 */
{
    "퍼즐: 시작의 숫자",

    {
        "어떤 수에 2를 곱한 후",
        "6을 더했더니 14가 되었다.",
        "처음 숫자는 무엇인가?",
        NULL,
        NULL,
        NULL
    },

    3,
    1, 10,

    {4, 0, 0, 0},

    "처음 숫자는 4"
},

/* PUZZLE 3 — 숫자 규칙 */
{
    "퍼즐: 숫자의 규칙",

    {
        "2 → 6 → 7 → 21 → 22 → ?",
        "규칙을 찾아 다음 숫자를 입력하라.",
        NULL,
        NULL,
        NULL,
        NULL
    },

    2,
    2, 10,

    {6, 6, 0, 0},

    "3을 곱하고 1을 더하는 규칙"
},

/* PUZZLE 4 — 집사의 나이 */
{
    "퍼즐: 집사의 고백",

    {
        "나는 인생의 1/6을 하인으로 보냈다.",
        "그 후 1/3은 주인님과 함께하였다.",
        "그 다음 1/7이 흐른 뒤 맹세했다.",
        "그리고 18년 후 심장이 만들어졌다.",
        "나는 몇 살인가?",
        NULL
    },

    5,
    2, 10,

    {4, 8, 0, 0},

    "나이는 48세"
},

/* PUZZLE 5 — 사용인 계산 */
{
    "퍼즐: 사라진 사용인",

    {
        "50명의 사용인이 있었다.",
        "2/5가 사라지고, 절반이 사라졌다.",
        "추가 실종과 탈출이 있었다.",
        "최종적으로 남은 사용인의 수는?",
        NULL,
        NULL
    },

    4,
    1, 100,

    {0, 0, 0, 0},

    "모든 사용인이 사라졌다."
},

/* PUZZLE 6 — 사건의 원인 */
{
    "퍼즐: 모든 사건의 시작",

    {
        "이 모든 사건의 근본적인 원인은 무엇인가?",
        "1. 귀신의 저주",
        "2. 집사의 행동",
        "3. 저택 주인의 집착",
        "4. 저택 자체의 영향",
        NULL
    },

    5,
    1, 10,

    {3, 0, 0, 0},

    "주인의 집착이 모든 비극의 시작이었다."
}

};


/* ── char display helpers ── */
static char dial_char(int puzzle_id, int val) {
    if (puzzle_id == 9) return (char)('A' + val);  /* A-E for lock */
    return (char)('0' + val);
}

/* ── 초기화 ── */
void puzzle_init(void) {
    memset(&active_puzzle, 0, sizeof(active_puzzle));
}

int puzzle_is_active(void) { return active_puzzle.active; }

/* ── 퍼즐 시작 ── */
void puzzle_start(GameState *gs, int puzzle_id) {
    if (puzzle_id < 0 || puzzle_id >= MAX_PUZZLES) return;
    if (gs->puzzles[puzzle_id]) {
        state_set_msg(gs, "이 퍼즐은 이미 해결되었습니다!");
        return;
    }
    memset(&active_puzzle, 0, sizeof(active_puzzle));
    active_puzzle.id     = puzzle_id;
    active_puzzle.active = 1;
    active_puzzle.cursor = 0;
    active_puzzle.hint_shown = 0;
}

/* ── 입력 처리 ── */
void puzzle_input(GameState *gs, int ch) {
    if (!active_puzzle.active) return;
    const PuzzleDef *def = &DEFS[active_puzzle.id];

    switch (ch) {
    /* 다이얼 커서 이동 */
    case KEY_LEFT:  case 'a': case 'A':
        active_puzzle.cursor =
            (active_puzzle.cursor - 1 + def->dial_count) % def->dial_count;
        break;
    case KEY_RIGHT: case 'd': case 'D':
        active_puzzle.cursor =
            (active_puzzle.cursor + 1) % def->dial_count;
        break;

    /* 다이얼 값 조절 */
    case KEY_UP:   case 'w': case 'W':
        active_puzzle.dials[active_puzzle.cursor] =
            (active_puzzle.dials[active_puzzle.cursor] + 1) % (def->dial_max + 1);
        break;
    case KEY_DOWN: case 's': case 'S':
        active_puzzle.dials[active_puzzle.cursor] =
            (active_puzzle.dials[active_puzzle.cursor] - 1 + def->dial_max + 1)
            % (def->dial_max + 1);
        break;

    /* 힌트 */
    case 'h': case 'H':
        active_puzzle.hint_shown = !active_puzzle.hint_shown;
        break;

    /* 확인 */
    case '\n': case '\r': case KEY_ENTER: {
        int correct = 1;
        for (int i = 0; i < def->dial_count; i++)
            if (active_puzzle.dials[i] != def->answer[i])
                { correct = 0; break; }

        if (correct) {
            gs->puzzles[active_puzzle.id] = 1;
            active_puzzle.active = 0;
/* 퍼즐 타일 제거 */
            switch(active_puzzle.id){

            case 0:
                map_set(gs, 14, 4, T_FLOOR);
                break;

            case 1:
                map_set(gs, 5, 12, T_FLOOR);
                break;

            case 2:
                map_set(gs, 11, 15, T_FLOOR);
                break;

            case 3:
                map_set(gs, 65, 6, T_FLOOR);
                break;

            case 4:
                map_set(gs, 7, 2, T_FLOOR);
                break;

            case 5:
                map_set(gs, 7, 15, T_FLOOR);
                break;

            case 6:
                map_set(gs, 53, 16, T_FLOOR);
                break;
            }
            /* 도전과제: 1트 성공 */
            if (active_puzzle.failed == 0)
                gs->puzzle_first_try[active_puzzle.id] = 1;

            /* 도전과제: 모든 퍼즐 1트 */
            int all1 = 1;
            int solved = 1;

            for(int i=0;i<MAX_PUZZLES;i++){
                if(!gs->puzzles[i]){
                    solved = 0;
                    break;
                }
            }

            if(all1 && solved)
                gs->achieve[8] = 1;

            /* 모든 퍼즐 완료 → 핵의 방 접근 가능 */
            if (solved) {
                gs->core_accessible = 1;
                state_set_msg(gs, "퍼즐 7개 완료! 마스터 침실 스위트에 접근할 수 있습니다.");
            } else {
                state_set_msg(gs, "퍼즐 해결!");
            }
            gs->mode = MODE_MAP;

        } else {
            /* 실패 */
            active_puzzle.failed++;
            gs->puzzle_fail_count++;

            /* 도전과제: 퍼즐 5회 이상 실패 */
            if (gs->puzzle_fail_count >= 5) gs->achieve[7] = 1;

            /* 1트 플래그 해제 */
            gs->puzzle_first_try[active_puzzle.id] = 0;

            /* HP 패널티 (3회 초과 시) */
            if (active_puzzle.failed > 3) {
                gs->hp--;
                if (gs->hp <= 0) { gs->hp = 0; gs->mode = MODE_GAMEOVER; }
                state_set_msg(gs, "틀렸습니다! 경보가 울립니다. (-1 HP)");
            } else {
                state_set_msg(gs, "오답입니다. 다시 시도하세요. [H] 힌트");
            }
        }
        break;
    }

    /* 취소 */
    case 27: case 'q': case 'Q':
        active_puzzle.active = 0;
        gs->mode = MODE_MAP;
        break;
    }
}

/* ── 렌더링 ── */
void puzzle_render(GameState *gs) {
    if (!active_puzzle.active) return;
    const PuzzleDef *def = &DEFS[active_puzzle.id];
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    erase();

    /* 타이틀 */
    attron(COLOR_PAIR(CP_BUTLER) | A_BOLD);
    mvprintw(1, cols/2 - (int)strlen(def->title)/2, "%s", def->title);
    attroff(COLOR_PAIR(CP_BUTLER) | A_BOLD);

    /* 설명 */
    attron(COLOR_PAIR(CP_WALL));
    for (int i = 0; i < def->desc_lines && def->description[i]; i++)
        mvprintw(3+i, 4, "%s", def->description[i]);
    attroff(COLOR_PAIR(CP_WALL));

    /* 힌트 */
    if (active_puzzle.hint_shown) {
        attron(COLOR_PAIR(CP_NOTE) | A_BOLD);
        mvprintw(3 + def->desc_lines + 1, 4, "힌트: %s", def->hint);
        attroff(COLOR_PAIR(CP_NOTE) | A_BOLD);
    }

    /* 다이얼 UI */
    int dy = rows / 2;
    int total_w = def->dial_count * 6;
    int dx = cols / 2 - total_w / 2;

    attron(COLOR_PAIR(CP_WALL) | A_BOLD);
    /* 위 화살표 */
    for (int i = 0; i < def->dial_count; i++) {
        if (i == active_puzzle.cursor)
            attron(COLOR_PAIR(CP_PLAYER) | A_BOLD);
        mvprintw(dy-2, dx + i*6, "  /\\  ");
        if (i == active_puzzle.cursor)
            attroff(COLOR_PAIR(CP_PLAYER) | A_BOLD);
    }
    /* 다이얼 박스 위 */
    for (int i = 0; i < def->dial_count; i++)
        mvprintw(dy-1, dx + i*6, "+----+");
    /* 다이얼 값 */
    for (int i = 0; i < def->dial_count; i++) {
        if (i == active_puzzle.cursor) {
            attron(COLOR_PAIR(CP_PLAYER) | A_BOLD | A_REVERSE);
            mvprintw(dy, dx + i*6, "| %c  |",
                     dial_char(active_puzzle.id, active_puzzle.dials[i]));
            attroff(COLOR_PAIR(CP_PLAYER) | A_BOLD | A_REVERSE);
        } else {
            attron(COLOR_PAIR(CP_ITEM) | A_BOLD);
            mvprintw(dy, dx + i*6, "| %c  |",
                     dial_char(active_puzzle.id, active_puzzle.dials[i]));
            attroff(COLOR_PAIR(CP_ITEM) | A_BOLD);
        }
    }
    /* 다이얼 박스 아래 */
    for (int i = 0; i < def->dial_count; i++)
        mvprintw(dy+1, dx + i*6, "+----+");
    /* 아래 화살표 */
    for (int i = 0; i < def->dial_count; i++) {
        if (i == active_puzzle.cursor)
            attron(COLOR_PAIR(CP_PLAYER) | A_BOLD);
        mvprintw(dy+2, dx + i*6, "  \\/  ");
        if (i == active_puzzle.cursor)
            attroff(COLOR_PAIR(CP_PLAYER) | A_BOLD);
    }
    attroff(COLOR_PAIR(CP_WALL) | A_BOLD);

    /* 실패 횟수 */
    if (active_puzzle.failed > 0) {
        attron(COLOR_PAIR(CP_DANGER));
        mvprintw(dy+4, dx, "실패 횟수: %d  (3회 초과 시 HP -1)",
                 active_puzzle.failed);
        attroff(COLOR_PAIR(CP_DANGER));
    }

    /* 완료한 퍼즐 개수 계산 */
    int count = 0;

    for (int i = 0; i < MAX_PUZZLES; i++) {
        if (gs->puzzles[i])
            count++;
    }

    /* 완료한 퍼즐 진행 상황 */
    attron(COLOR_PAIR(CP_HUD));
    mvprintw(dy+6, dx,
            "퍼즐: %d/%d 완료",
            count, MAX_PUZZLES);
    attroff(COLOR_PAIR(CP_HUD));

    /* 하단 조작키 */
    attron(COLOR_PAIR(CP_WALL) | A_DIM);
    mvhline(rows-2, 0, ACS_HLINE, cols);
    mvprintw(rows-1, 2,
        "[A/D] 커서이동  [W/S] 값조절  [Enter] 확인  [H] 힌트  [ESC] 취소");
    attroff(COLOR_PAIR(CP_WALL) | A_DIM);

    wnoutrefresh(stdscr);
    doupdate();
    (void)gs;
}
