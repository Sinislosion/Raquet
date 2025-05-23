#ifndef RAQUET_SOUND_H
#define RAQUET_SOUND_H

extern ALCdevice* Raquet_AudioDevice;
extern ALCcontext* Raquet_AudioContext;

void Raquet_InitializeAudio();

ALenum Raquet_To_AL_Format(short channels, short samples);

#endif

