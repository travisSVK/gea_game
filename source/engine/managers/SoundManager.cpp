#include "SoundManager.hpp"
#include "SDL.h"

Mix_Music* SoundManager::LoadBackGroundMusic(const char* path)
{
    Mix_Music* bgMusic = Mix_LoadMUS(path);
    if (!bgMusic)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create bg music from %s!\n", path);
        return nullptr;
    }
    return bgMusic;
}

void SoundManager::PlayBackGroundMusic(Mix_Music* bgMusic)
{
    Mix_PlayMusic(bgMusic, -1);
}

Mix_Chunk* SoundManager::LoadSFXMusic(const char* path)
{
    return Mix_LoadWAV(path);
}

void SoundManager::PlaySFXMusic(Mix_Chunk* sfxMusic)
{
    Mix_PlayChannel(-1, sfxMusic, 0);
}

void SoundManager::PauseBackGroundMusic()
{
    Mix_PauseMusic();
}

void SoundManager::ResumeBackGroundMusic()
{
    Mix_ResumeMusic();
}

void SoundManager::StopSFXMusic()
{
    Mix_HaltChannel(-1);
}