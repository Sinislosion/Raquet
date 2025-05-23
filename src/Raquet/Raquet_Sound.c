#include "Raquet.h"

ALCdevice* Raquet_AudioDevice;
ALCcontext* Raquet_AudioContext;
ALuint Raquet_AudioSource;
ALuint Raquet_AudioBuffer;

void Raquet_InitializeAudio() {
    Raquet_AudioDevice = alcOpenDevice(NULL);
    
    #ifdef PRINT_DEBUG
        if (!Raquet_AudioDevice) {
            printf("Raquet failed to initialize OpenAL, this may lead to errors or segfaults.\n");
        }
    #endif

    Raquet_AudioContext = alcCreateContext(Raquet_AudioDevice, NULL);
    if (!alcMakeContextCurrent(Raquet_AudioContext)) {
        #ifdef PRINT_DEBUG
            printf("Raquet failed to create the audio context\n");
        #endif
    }

    alGenSources((ALuint)1, &Raquet_AudioSource);

    alSourcef(Raquet_AudioSource, AL_PITCH, 1);
    alSourcef(Raquet_AudioSource, AL_GAIN, 1);
    alSource3f(Raquet_AudioSource, AL_POSITION, 0, 0, 0);
    alSource3f(Raquet_AudioSource, AL_VELOCITY, 0, 0, 0);
    alSourcei(Raquet_AudioSource, AL_LOOPING, AL_FALSE);

    alGenBuffers((ALuint)1, &Raquet_AudioBuffer);

}

ALenum Raquet_To_AL_Format(short channels, short samples) {
    int stereo = (channels > 1);

    switch (samples) {
        case 16:
            if (stereo) {
                return AL_FORMAT_STEREO16;
            } else {
                return AL_FORMAT_MONO16;
            }
            break;

        case 8:
            if (stereo) {
                return AL_FORMAT_STEREO8;
            } else {
                return AL_FORMAT_MONO8;
            }
            break;

        default:
            return -1;
            break;

    }

}
