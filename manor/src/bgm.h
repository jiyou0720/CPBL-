#ifndef BGM_H
#define BGM_H

void bgm_init(void);
void bgm_play(const char *filename);
void bgm_stop(void);
void bgm_quit(void);
void sfx_play(const char *filename);

#endif