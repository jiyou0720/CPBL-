#include "notes.h"
#include "state.h"
#include "render.h"
#include <ncurses.h>
#include <string.h>

NoteViewer note_viewer = {0, 0, 0};

/* ════════════════════════════════════════════════
   Diary note content  (15 entries)
   All ASCII — Korean can be added via dialog lines
   ════════════════════════════════════════════════ */

typedef struct {
    const char  *title;
    const char  *body[20];
    int          line_count;
} NoteEntry;

static const NoteEntry NOTES[NOTE_COUNT] = {

/* 0 */
{
"사용인의 일기 [1/15]",
{
    "오늘도 저택은 평화롭다.",
    "주인님께서는 평소처럼 아내분과 함께",
    "정원을 거닐고 계신다.",
    "집사님 역시 변함없이",
    "저택을 관리하고 계신다.",
    "오늘은 평소보다 일이 많았지만,",
    "어쩐지 저택의 분위기가",
    "유난히 밝게 느껴져",
    "기분이 나쁘지 않았다.",
    NULL
},
9
},

/* 1 */
{
"사용인의 일기 [2/15]",
{
    "나는 다른 사용인들에게",
    "집사님에 대한 이야기를 들었다.",
    "집사님은 가족이 없으시며,",
    "아주 어릴 적부터",
    "이 저택에서 일해오셨다고 한다.",
    "주인님과는 어린 시절부터",
    "함께 자라온 사이라",
    "가족에 가까운 사이였다고 했다.",
    "나는 그저 두 분이",
    "서로를 가족처럼 여기신다고 생각했다.",
    NULL
},
10
},

/* 2 */
{
"사용인의 일기 [3/15]",
{
    "평소처럼 일에 몰두하던 어느 날—",
    "산책하고 계셨던 부인이 쓰러지셨다.",
    "누구도 예상하지 못한 일이었다.",
    "저택 전체가 순식간에",
    "혼란에 휩싸였다.",
    "나 또한 일이 손에 잡히지 않을 만큼",
    "마음이 불안하다...",
    NULL
},
7
},

/* 3 */
{
"사용인의 일기 [4/15]",
{
    "그분이 쓰러진 이후,",
    "저택의 분위기는 변하기 시작했다.",
    "안주인님께서는 점점 쇠약해지셨고,",
    "의사조차 상태가 악화될 것이라 말했다.",
    "주인님께서는 도서관에만",
    "틀어박혀 계신다.",
    "집사님은 그런 주인님의 곁을",
    "한시도 떠나지 않는다.",
    "이 저택에서 무언가가",
    "달라지고 있는 듯하다.",
    NULL
},
10
},

/* 4 */
{
"사용인의 일기 [5/15]",
{
    "요 며칠 사이,",
    "저택에서 이상한 일이 계속되고 있다.",
    "알고 지내던 사용인들이",
    "어느샌가 보이지 않게 되었다.",
    "익숙했던 복도마저",
    "낯설게 느껴진다.",
    "단순한 착각이라 생각하고 싶지만...",
    "불안감이 점점 커져간다.",
    NULL
},
8
},

/* 5 */
{
"사용인의 일기 [6/15]",
{
    "오늘, 도서관 근처에서",
    "이상한 광경을 목격했다.",
    "문틈 사이로 보인 것은—",
    "주인님이 누군가와",
    "이야기를 나누는 모습이었다.",
    "다가가려는 순간,",
    "집사님이 나를 응시하고 있었다.",
    "그 눈빛이 아직도",
    "이질적으로 느껴진다.",
    NULL
},
9
},

/* 6 */
{
"사용인의 일기 [7/15]",
{
    "사람들이 하나둘씩 사라지고 있다.",
    "처음에는 외출이라 생각했다.",
    "다음에는 이직이라 여겼다.",
    "하지만 아무도 돌아오지 않았다.",
    "아무도 입 밖에 내지 않지만—",
    "모두 알고 있다.",
    "이 저택에서 무언가가",
    "벌어지고 있다는 것을.",
    NULL
},
8
},

/* 7 */
{
"사용인의 일기 [8/15]",
{
    "오늘... 확실히 보았다.",
    "실종되었던 사용인이",
    "복도 끝에 서 있었다.",
    "하지만 그것은 더 이상",
    "사람이 아니었다.",
    "형체는 흐릿했지만,",
    "그는 나를 보며 웃고 있었다.",
    "도망쳤지만 그 모습이",
    "계속 머릿속에 맴돈다.",
    NULL
},
9
},

/* 8 */
{
"사용인의 일기 [9/15]",
{
    "혼란스럽다.",
    "주인님과 집사님은",
    "무슨 생각이신 걸까.",
    "사용인들은 점차 줄어들고 있다.",
    "하지만 두 분은",
    "아무런 변화도 없는 듯 행동한다.",
    "마치 내가 이상한 사람인 것처럼...",
    NULL
},
7
},

/* 9 */
{
"사용인의 일기 [10/15]",
{
    "요즘 들어 이상한 소리가 들린다.",
    "처음에는 바람 소리인 줄 알았다.",
    "하지만 점점 선명해졌다.",
    "\"이곳은 위험해.\"",
    "\"여기를 떠나야 해.\"",
    "누가 말하는 것일까.",
    NULL
},
6
},

/* 10 */
{
"사용인의 일기 [11/15]",
{
    "한 가지 깨달은 것이 있다.",
    "내게 들리는 목소리는",
    "사라진 사람들의 것이다.",
    "그들은 이 저택을",
    "빠져나가지 못하는 것 같다.",
    "무슨 이유가 있는 걸까.",
    "나는 더 이상",
    "이곳에 남아있으면 안 될 것 같다.",
    NULL
},
8
},

/* 11 */
{
"사용인의 일기 [12/15]",
{
    "도망치려고 시도했다.",
    "몰래 문으로 나가려 했지만",
    "문이 잠겨 있었다.",
    "뒷문도 마찬가지였다.",
    "낮인데도 모든 문이",
    "굳게 닫혀 있었다.",
    "다른 출구를 찾았지만",
    "아무것도 발견하지 못했다.",
    NULL
},
8
},

/* 12 */
{
"사용인의 일기 [13/15]",
{
    "사라진 사람들의 목소리가",
    "끊임없이 말한다.",
    "\"그걸... 파괴해야 해...\"",
    "\"그곳에... 있어...\"",
    "무엇을 파괴해야 하는지는 모르겠다.",
    "하지만 그것을 파괴하면",
    "나갈 수 있을 것 같다.",
    NULL
},
7
},

/* 13 */
{
"사용인의 일기 [14/15]",
{
    "오늘, 집사님과 마주쳤다.",
    "아무 말도 하지 않으셨지만,",
    "그 차가운 눈빛으로 확신했다.",
    "그는 알고 있다.",
    "내가 저택의 진실을",
    "알아가고 있다는 것을.",
    "이 저택에서 가장 위험한 존재는",
    "귀신이 아닐지도 모른다.",
    NULL
},
8
},

/* 14 */
{
"사용인의 일기 [15/15]",
{
    "더 이상 시간을 지체할 수 없다.",
    "나는 이 저택을 벗어날 것이다.",
    "그 목소리들이 말한",
    "\"심장\"을 찾아",
    "이 모든 것을 끝내야 한다.",
    "그것만이 이 저택에서",
    "벗어날 수 있는 유일한 방법이다.",
    "...발소리가 들린다.",
    "누군가 이쪽으로 오고 있다.",
    NULL
},
9
}

};


/* ── 초기화 ── */
void notes_init(void) {
    note_viewer.active   = 0;
    note_viewer.selected = 0;
    note_viewer.scroll   = 0;
}

/* ── 일기 획득 ── */
int note_collect(GameState *gs, int note_id) {
    if (note_id < 0 || note_id >= NOTE_COUNT) return 0;
    if (gs->notes[note_id]) return 0;   /* 이미 획득 */
    gs->notes[note_id] = 1;

    /* 도전과제: 진실은 하나! — 모든 15장 획득 */
    int all = 1;
    for (int i = 0; i < NOTE_COUNT; i++)
        if (!gs->notes[i]) { all = 0; break; }
    if (all) gs->achieve[1] = 1;

    return 1;
}

/* ── 일기 제목 ── */
const char *note_title(int note_id) {
    if (note_id < 0 || note_id >= NOTE_COUNT) return "???";
    return NOTES[note_id].title;
}

/* ── 일기 본문 ── */
const char * const *note_body(int note_id, int *line_count) {
    if (note_id < 0 || note_id >= NOTE_COUNT) {
        *line_count = 0; return NULL;
    }
    *line_count = NOTES[note_id].line_count;
    return NOTES[note_id].body;
}

/* ── 뷰어 열기 ── */
void note_viewer_open(GameState *gs) {
    (void)gs;
    note_viewer.active   = 1;
    note_viewer.selected = 0;
    note_viewer.scroll   = 0;
}

/* ── 뷰어 닫기 ── */
void note_viewer_close(void) {
    note_viewer.active = 0;
}

/* ── 뷰어 입력 처리 ── */
void note_viewer_input(GameState *gs, int ch) {
    /* 획득한 일기 목록 */
    int collected[NOTE_COUNT], cnt = 0;
    for (int i = 0; i < NOTE_COUNT; i++)
        if (gs->notes[i]) collected[cnt++] = i;

    switch (ch) {
    case KEY_LEFT: case 'a': case 'A':
        if (note_viewer.selected > 0) {
            note_viewer.selected--;
            note_viewer.scroll = 0;
        }
        break;
    case KEY_RIGHT: case 'd': case 'D':
        if (note_viewer.selected < cnt - 1) {
            note_viewer.selected++;
            note_viewer.scroll = 0;
        }
        break;
    case KEY_UP: case 'w': case 'W':
        if (note_viewer.scroll > 0) note_viewer.scroll--;
        break;
    case KEY_DOWN: case 's': case 'S':
        if (cnt > 0) {
            int nid = collected[note_viewer.selected];
            if (note_viewer.scroll < NOTES[nid].line_count - 1)
                note_viewer.scroll++;
        }
        break;
    case 'n': case 'N': case 27: case 'q': case 'Q':
        note_viewer_close();
        break;
    }
}

/* ── 뷰어 렌더링 ── */
void note_viewer_render(GameState *gs) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    /* 배경 어둡게 */
    attron(COLOR_PAIR(CP_WALL) | A_DIM);
    for (int y = 0; y < rows; y++)
        mvhline(y, 0, ' ', cols);
    attroff(COLOR_PAIR(CP_WALL) | A_DIM);

    /* 획득 목록 */
    int collected[NOTE_COUNT], cnt = 0;
    for (int i = 0; i < NOTE_COUNT; i++)
        if (gs->notes[i]) collected[cnt++] = i;

    /* 왼쪽 패널: 일기 목록 */
    int lw = 22, lx = 2, ly = 2;
    attron(COLOR_PAIR(CP_WALL) | A_BOLD);
    mvprintw(ly-1, lx, "+-- DIARY NOTES (%2d/15)--+", cnt);
    for (int i = 0; i < cnt && i < rows-6; i++) {
        int nid = collected[i];
        if (i == note_viewer.selected) {
            attron(COLOR_PAIR(CP_PLAYER) | A_BOLD | A_REVERSE);
            mvprintw(ly+i, lx, "| %-*.*s |", lw-4, lw-4, note_title(nid));
            attroff(COLOR_PAIR(CP_PLAYER) | A_BOLD | A_REVERSE);
        } else {
            attron(COLOR_PAIR(CP_NOTE));
            mvprintw(ly+i, lx, "| %-*.*s |", lw-4, lw-4, note_title(nid));
            attroff(COLOR_PAIR(CP_NOTE));
        }
    }
    if (cnt == 0) {
        attron(COLOR_PAIR(CP_WALL) | A_DIM);
        mvprintw(ly, lx, "| (아직 수집한 일지가 없습니다)        |");
        attroff(COLOR_PAIR(CP_WALL) | A_DIM);
    }
    mvprintw(ly+cnt, lx, "+------------------------+");
    attroff(COLOR_PAIR(CP_WALL) | A_BOLD);

    /* 오른쪽 패널: 본문 */
    int rx = lx + lw + 4, rw = cols - rx - 2;
    if (cnt > 0 && rw > 10) {
        int nid = collected[note_viewer.selected];
        int lc  = 0;
        const char * const *body = note_body(nid, &lc);

        attron(COLOR_PAIR(CP_ITEM) | A_BOLD);
        mvprintw(ly-1, rx, "[ %s ]", note_title(nid));
        attroff(COLOR_PAIR(CP_ITEM) | A_BOLD);

        /* 본문 줄 출력 */
        int max_lines = rows - 8;
        for (int i = 0; i < lc && i < max_lines; i++) {
            int idx = i + note_viewer.scroll;
            if (idx >= lc) break;
            attron(COLOR_PAIR(CP_WALL));
            mvprintw(ly+i, rx, "%-*.*s", rw, rw, body[idx]);
            attroff(COLOR_PAIR(CP_WALL));
        }

        /* 스크롤 표시 */
        if (lc > max_lines) {
            attron(COLOR_PAIR(CP_WALL) | A_DIM);
            mvprintw(ly+max_lines, rx,
                "-- line %d/%d --", note_viewer.scroll+1, lc);
            attroff(COLOR_PAIR(CP_WALL) | A_DIM);
        }
    }

    /* 하단 조작 힌트 */
    attron(COLOR_PAIR(CP_WALL) | A_DIM);
    mvhline(rows-2, 0, ACS_HLINE, cols);
    mvprintw(rows-1, 2,
        "[A/D] 이전/다음 일지   [W/S] 스크롤   [N/ESC] 닫기");
    attroff(COLOR_PAIR(CP_WALL) | A_DIM);
}
