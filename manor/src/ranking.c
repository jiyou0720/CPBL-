#include "ranking.h"
#include "state.h"

#include <stdio.h>
#include <string.h>

#define RANK_FILE "ranking.dat"

static RankingEntry rankings[MAX_RANKING];

static int calc_score(GameState *gs)
{
    int score = 0;

    for(int i = 0; i < MAX_PUZZLES; i++)
        if(gs->puzzles[i])
            score += 100;

    for(int i = 0; i < MAX_NOTES; i++)
        if(gs->notes[i])
            score += 20;

    for(int i = 0; i < 11; i++)
        if(gs->achieve[i])
            score += 50;

    score += gs->hp * 10;

    return score;
}

void ranking_load(void)
{
    FILE *f = fopen(RANK_FILE, "rb");

    if(!f) {
        memset(rankings, 0, sizeof(rankings));
        return;
    }

    fread(rankings, sizeof(rankings), 1, f);
    fclose(f);
}

void ranking_save(void)
{
    FILE *f = fopen(RANK_FILE, "wb");

    if(!f)
        return;

    fwrite(rankings, sizeof(rankings), 1, f);
    fclose(f);
}

void ranking_add(GameState *gs)
{
    RankingEntry entry;

    strcpy(entry.name, "PLAYER");
    entry.score = calc_score(gs);
    entry.play_time = state_elapsed(gs);

    rankings[MAX_RANKING - 1] = entry;

    for(int i = 0; i < MAX_RANKING - 1; i++) {

        for(int j = i + 1; j < MAX_RANKING; j++) {

            if(rankings[j].score > rankings[i].score) {

                RankingEntry tmp = rankings[i];
                rankings[i] = rankings[j];
                rankings[j] = tmp;
            }
        }
    }

    ranking_save();
}

const RankingEntry *ranking_get(void)
{
    return rankings;
}