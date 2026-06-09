#ifndef RANKING_H
#define RANKING_H

#include "state.h"

#define MAX_RANKING 10

void ranking_load(void);
void ranking_save(void);
void ranking_add(GameState *gs);

const RankingEntry *ranking_get(void);

#endif