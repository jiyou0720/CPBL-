#ifndef STATE_H
#define STATE_H

#include <time.h>

/* ── 게임 모드 ── */
typedef enum {
    MODE_MAP,           /* 탑다운 이동 */
    MODE_EVENT,         /* 1인칭 대면 (NPC·아이템 조사) */
    MODE_INVENTORY,     /* 인벤토리 UI */
    MODE_PAUSE,         /* ESC 일시정지 */
    MODE_GAMEOVER,      /* 게임오버 화면 */
    MODE_ENDING_HAPPY,  /* 해피엔딩 */
    MODE_ENDING_BAD     /* 배드엔딩 */
} GameMode;

/* ── 아이템 ID ── */
#define ITEM_NONE    0
#define ITEM_HOLY    1   /* 성수 */
#define ITEM_HAMMER  2   /* 망치 */
#define ITEM_KEY_LIB 3   /* 도서관 열쇠 */
#define ITEM_KEY_SRV 4   /* 사용인방 열쇠 */
#define ITEM_DIARY   5   /* 일기 조각 */

/* ── 맵 상수 ── */
#define MAX_NOTES    15
#define MAX_PUZZLES   7
#define MAX_INV       8
#define HP_MAX       10

/* ── 이벤트 대화 ── */
#define MAX_DIALOG_LEN 256
#define MAX_CHOICES      3

typedef struct {
    char  lines[4][MAX_DIALOG_LEN]; /* 대화 줄 (최대 4줄) */
    int   line_count;
    char  choices[MAX_CHOICES][64];
    int   choice_count;
    int   choice_sel;               /* 현재 선택 커서 */
    int   hp_penalty[MAX_CHOICES];  /* 선택지별 HP 패널티 */
    int   instant_ko[MAX_CHOICES];  /* 선택지별 즉사 여부 */
    int   item_steal;               /* 아이템 압수 여부 */
} DialogState;

/* ── 게임 전체 상태 ── */
typedef struct {
    GameMode mode;

    /* 위치 */
    int floor;          /* 1 or 2 */
    int px, py;         /* 플레이어 좌표 */
    int prev_room;      /* 자동저장용 이전 방 ID */

    /* 스탯 */
    int hp;

    /* 인벤토리 */
    int items[MAX_INV];
    int item_count;

    /* 수집 */
    int notes[MAX_NOTES];   /* 획득한 일기 플래그 */
    int puzzles[MAX_PUZZLES]; /* 퍼즐 완료 여부 */
    int keys[2];            /* 0:도서관 1:사용인방 */

    /* 진행 */
    int core_accessible;    /* 핵의 방 접근 가능 여부 */
    int butler_chase;       /* 집사 추격 활성 */

    int butler_phase;       /* 집사 등장 단계 */

    time_t butler_start_time; /* 집사 추격 시작 시간 */

    /* 도전과제 (11종) */
    int achieve[11];
    int ghost_safe_streak;  /* 귀신 안전 선택 연속 횟수 */
    int trap_count;
    int puzzle_fail_count;
    int puzzle_first_try[MAX_PUZZLES];
    int checkpoint_count;

    /* 시간 */
    time_t start_time;
    time_t pause_accum;     /* 일시정지 누적 시간 */
    time_t pause_start;

    /* 대화 */
    DialogState dialog;
    int active_npc_id;      /* 현재 이벤트 NPC/오브젝트 ID */

    /* 기타 */
    int running;            /* 메인 루프 플래그 */
    char msg[80];           /* 하단 메시지 표시 */
    int  msg_timer;         /* 메시지 표시 남은 틱 */

    int ending_page;
} GameState;

typedef struct {
    char name[32];
    int score;
    int play_time;
} RankingEntry;

/* ── 함수 선언 ── */
void state_init(GameState *gs);
int  state_has_item(GameState *gs, int item_id);
int  state_add_item(GameState *gs, int item_id);
void state_remove_item(GameState *gs, int item_id);
void state_set_msg(GameState *gs, const char *msg);
int  state_elapsed(GameState *gs); /* 경과 초 */

#endif /* STATE_H */
