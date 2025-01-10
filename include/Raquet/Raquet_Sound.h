#ifndef RAQUET_SOUND_H
#define RAQUET_SOUND_H

/** @file */

#include "Raquet.h"

/** @brief The alias for SDL_Mixer's audio data variables */
typedef Mix_Chunk * Raquet_Sound;

// This typedef is useless until I make more wrapper functions
//typedef Mix_Music * Raquet_BGM;

/**
 * @brief Loads a sound file into a Raquet_Sound variable
 * @param file The path to the audio file to load
 */
Raquet_Sound Raquet_LoadSound(const char * file);

/**
 * @brief Plays the Raquet_Sound provided
 * @param wav The Raquet_Sound to play
 * @param loops Whether or not to loop the sound (1 = true, 0 = false)
 */
extern void Raquet_PlaySound(Raquet_Sound wav, int loops);

/**
 * @brief Frees the memory used by a Raquet_Sound variable
 * @param wav The Raquet_Sound to destroy
 */
extern void Raquet_DestroySound(Raquet_Sound wav);

#endif
