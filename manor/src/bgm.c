#include <stdio.h>
#include "bgm.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

static Mix_Music *current = NULL;

void bgm_init(void)
{
    SDL_Init(SDL_INIT_AUDIO);

    Mix_OpenAudio(
        44100,
        MIX_DEFAULT_FORMAT,
        2,
        2048
    );
}

void bgm_play(const char *filename)
{
    if(current)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(current);
    }

    current = Mix_LoadMUS(filename);

    if(current)
    {
        Mix_PlayMusic(current, 0);   // 무한 반복
    }
}

void sfx_play(const char *filename)
{
    Mix_Chunk *sound = Mix_LoadWAV(filename);

    if(!sound)
    {
        fprintf(stderr,
            "SFX ERROR (%s): %s\n",
            filename,
            Mix_GetError());

        return;
    }

    if(Mix_PlayChannel(-1, sound, 0) == -1)
    {
        fprintf(stderr,
            "PLAY ERROR (%s): %s\n",
            filename,
            Mix_GetError());
    }
}

void bgm_stop(void)
{
    Mix_HaltMusic();
}

void bgm_quit(void)
{
    if(current)
        Mix_FreeMusic(current);

    Mix_CloseAudio();
    SDL_Quit();
}