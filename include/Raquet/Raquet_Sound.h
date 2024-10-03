#ifndef RAQUET_SOUND_H
#define RAQUET_SOUND_H

#include "Raquet.h"

typedef Mix_Chunk * Raquet_Sound;
typedef Mix_Music * Raquet_BGM;

// Loads a sound into a Raquet_Sound variable from a file
Raquet_Sound Raquet_LoadSound(const char * file);

// Plays a Raquet Sound
extern void Raquet_PlaySound(Raquet_Sound wav, int loops);

// Frees the allocated memory from a Raquet_Sound
extern void Raquet_DestroySound(Raquet_Sound wav);

#endif