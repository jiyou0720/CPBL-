#ifndef NOTES_H
#define NOTES_H

#include "state.h"

typedef enum {
    NOTE_GUEST1,
    NOTE_GUEST2,
    NOTE_LOUNGE1,
    NOTE_LOUNGE2,
    NOTE_STUDY1,
    NOTE_STUDY2,
    NOTE_HALL,

    NOTE_DRESSING,
    NOTE_SERVANT1,
    NOTE_SERVANT2,
    NOTE_TERRACE,
    NOTE_STAIRS,
    NOTE_CORRIDOR1,
    NOTE_CHANDELIER,
    NOTE_CORRIDOR2,

    NOTE_COUNT
} NoteID;

/* ── 일기 뷰어 상태 ── */
typedef struct {
    int  active;        /* 뷰어 열림 여부 */
    int  selected;      /* 현재 선택된 일기 인덱스 */
    int  scroll;        /* 본문 스크롤 줄 */
} NoteViewer;

extern NoteViewer note_viewer;

/* ── 함수 ── */
void notes_init(void);
int  note_collect(GameState *gs, int note_id);   /* 일기 획득, 1=성공 */
void note_viewer_open(GameState *gs);
void note_viewer_close(void);
void note_viewer_input(GameState *gs, int ch);
void note_viewer_render(GameState *gs);

/* 일기 제목/본문 조회 */
const char *note_title(int note_id);
const char * const *note_body(int note_id, int *line_count);

#endif
