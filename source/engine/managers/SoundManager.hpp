/**
 * @file     SoundManager.hpp
 * @author   Marek Cernak
 * @date     3/25/2019
 */

#pragma once
#include "../EngineAPI.hpp"
#include "SDL_mixer.h"

namespace engine
{
    namespace managers
    {
        /**
         * Sound manager class used to load and play background and effects sounds.
         */
        class ENGINE_API SoundManager
        {
        public:

            /**
             * Loads background music from file.
             * path [in] Path to sound file.
             * @return Mix_Music - SDL_Mixer music file format.
             */
            static Mix_Music* LoadBackGroundMusic(const char* path);
            
            /**
             * Plays background music.
             * bgMusic [in] Background music to play.
             */
            static void PlayBackGroundMusic(Mix_Music* bgMusic);

            /**
             * Loads sound effect from file.
             * path [in] Path to sound effect.
             * @return Mix_Chunk - SDL_Mixer sound effect file format.
             */
            static Mix_Chunk* LoadSFXMusic(const char* path);
            
            /**
             * Plays sound effect.
             * sfxMusic [in] Sound effect to play.
             */
            static void PlaySFXMusic(Mix_Chunk* sfxMusic);
            
            /**
             * Pauses background music.
             */
            static void PauseBackGroundMusic();
            
            /**
             * Resumes background music.
             */
            static void ResumeBackGroundMusic();
            
            /**
             * Stops sound effect.
             */
            static void StopSFXMusic();
        };
    }
}