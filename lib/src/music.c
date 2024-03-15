#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "../include/music.h"
#include "../include/ball.h"

void menuMusic(const char* filename)
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_Mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return;
    }

    Mix_Music* music = Mix_LoadMUS(filename);
    Mix_VolumeMusic(10);
    if (music == NULL)
    {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        return;
    }

    if (Mix_PlayMusic(music, -1) == -1)
    {
        printf("Failed to play music! SDL_mixer Error: %s\n", Mix_GetError());
        return;
    }
}

void PlayMusic(const char* filename)
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_Mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return;
    }

    Mix_Music* music = Mix_LoadMUS(filename);
    Mix_VolumeMusic(10);
    if (music == NULL)
    {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        return;
    }

    if (Mix_PlayMusic(music, -1) == -1)
    {
        printf("Failed to play music! SDL_mixer Error: %s\n", Mix_GetError());
        return;
    }

}


/*void wallCollisionSound(const *char filename)
{
    Mix_Chunk *collision_sound = Mix_LoadWAV("WallCollisionSound.waw");
    if(wallBallCollision(Ball *pBall)!=0)
    {
        Mix_PlayChannel(-1, collision_sound,0);
    }
}*/