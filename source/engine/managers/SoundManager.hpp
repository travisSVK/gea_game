#pragma once
#include "../EngineAPI.hpp"
#include "SDL_mixer.h"

class ENGINE_API SoundManager
{
public:
    static Mix_Music* LoadBackGroundMusic(const char* path);
    static void PlayBackGroundMusic(Mix_Music* bgMusic);
    static Mix_Chunk* LoadSFXMusic(const char* path);
    static void PlaySFXMusic(Mix_Chunk* sfxMusic);
    static void PauseBackGroundMusic();
    static void ResumeBackGroundMusic();
    static void StopSFXMusic();
};