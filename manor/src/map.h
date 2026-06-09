#ifndef MAP_FILE_H
#define MAP_FILE_H

#include "state.h"

#define T_VOID    ' '
#define T_WALL    '#'
#define T_FLOOR   '.'
#define T_DOOR    'D'
#define T_DOOR_L  'd'
#define T_ITEM    'I'
#define T_PUZZLE  'P'
#define T_NOTE    'N'
#define T_KEY     'K'
#define T_STAIRS_D 'v'
#define T_STAIRS_U '^'
#define T_CORE    '*'
#define T_TRAP    '~'
#define T_NPC     'C'

#define ROOM_NONE       -1
#define ROOM_ENTRANCE    0
#define ROOM_CORRIDOR1   1
#define ROOM_GUESTROOM   2
#define ROOM_LOUNGE      3
#define ROOM_LIBRARY     4
#define ROOM_STUDY       5
#define ROOM_UTILITY     6
#define ROOM_STAIRS1     7
#define ROOM_CORRIDOR2   8
#define ROOM_BED1        9
#define ROOM_BED2       10
#define ROOM_STORAGE    11
#define ROOM_DRESSING   12
#define ROOM_SERVANT    13
#define ROOM_MASTER     14
#define ROOM_BATHROOM   15
#define ROOM_TERRACE    16
#define ROOM_MAX        17

#define MAP_COLS 78
#define MAP_ROWS 22

extern char map1f[MAP_ROWS][MAP_COLS+1];
extern char map2f[MAP_ROWS][MAP_COLS+1];

void        map_init(void);
void        map_sync_with_save(GameState *gs);
char        map_get(GameState *gs, int x, int y);
void        map_set(GameState *gs, int x, int y, char tile);
int         map_walkable(GameState *gs, int x, int y);
int         map_get_room(GameState *gs, int x, int y);
const char *room_name(int room_id);

#endif
