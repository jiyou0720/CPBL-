#ifndef PLAYER_H
#define PLAYER_H

#include "state.h"
#include <stdio.h>

void player_move(GameState *gs, int dx, int dy);
void player_interact(GameState *gs);
void player_damage(GameState *gs, int amount);
void player_heal(GameState *gs, int amount);

#endif
