#ifndef SAVE_H
#define SAVE_H

#include "state.h"

#define SAVE_FILE  "data/save.dat"
#define SAVE_SLOTS 3

int  save_game(GameState *gs, int slot);  /* 슬롯 0~2 */
int  load_game(GameState *gs, int slot);
int  save_exists(int slot);
void autosave(GameState *gs);             /* 슬롯 0 = 자동저장 */
void save_show_slots(void);               /* 슬롯 목록 출력 */

#endif
