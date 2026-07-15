#ifndef RAQUET_SOUND_H
#define RAQUET_SOUND_H

/** @file */

#include "Raquet.h"
#include <SDL3_mixer/SDL_mixer.h>

#define RAQUET_AVAIABLE_AUDIO_TRACKS    256

/** @brief SDL_Mixer structure needed to output audio */
extern MIX_Mixer* Raquet_Mixer;

typedef struct Raquet_Track {
    MIX_Track* track;
    bool taken;
} Raquet_Track;

extern Raquet_Track Raquet_Tracks[RAQUET_AVAIABLE_AUDIO_TRACKS];

/** @brief The alias for SDL_Mixer's audio data variables */
typedef struct Raquet_Sound {
    MIX_Audio* sound;
    int trackID;
} Raquet_Sound;

/**
 * @brief Initializes the SDL_Mixer Context that Raquet uses
 */
extern int Raquet_InitAudio(void);

/**
 * @brief Loads a sound file into a Raquet_Sound variable
 * @param file The path to the audio file to load
 */
Raquet_Sound Raquet_LoadSound(const char * file);

/**
 * @brief Find an available mixer track from Raquet_Tracks
 */
int Raquet_FindAvailableTrack(void);

/**
 * @brief Plays the Raquet_Sound provided
 * @param sound The Raquet_Sound to play
 * @param loops Number of times to loop the sound (-1 will be infinite)
 */
extern void Raquet_PlaySound(Raquet_Sound sound, int loops);

/**
 * @brief Stops the Raquet_Sound
 * @param sound The Raquet_Sound to stop playing
 * @param fadeOut How long it takes for the sound to fade out
 */
extern void Raquet_StopSound(Raquet_Sound sound, Sint16 fadeOut);

/**
 * @brief Frees the memory used by a Raquet_Sound variable
 * @param sound The Raquet_Sound to destroy
 */
extern void Raquet_DestroySound(Raquet_Sound* sound);

#endif