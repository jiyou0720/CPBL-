#ifndef RENDER_H
#define RENDER_H

#include "state.h"
#include "enemy.h"
#include <ncurses.h>

#define CP_GHOST   1
#define CP_ITEM    2
#define CP_PLAYER  3
#define CP_HUD     4
#define CP_WALL    5
#define CP_BUTLER  6
#define CP_DANGER  7
#define CP_NOTE    8

void render_init(void);
void render(GameState *gs);
void render_map(GameState *gs);
void render_hud(GameState *gs);
void render_pause(GameState *gs);
void render_inventory(GameState *gs);
void render_gameover(GameState *gs);
void render_ending_happy(GameState *gs);
void render_ending_bad(GameState *gs);
void render_event(GameState *gs);
void render_achievements(GameState *gs);
void render_title(void);

#endif
