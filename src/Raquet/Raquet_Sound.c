#include "Raquet_Sound.h"
#include <SDL3_mixer/SDL_mixer.h>

MIX_Mixer* Raquet_Mixer;
Raquet_Track Raquet_Tracks[RAQUET_AVAIABLE_AUDIO_TRACKS];

int Raquet_InitAudio(void) {

    if (MIX_Init() == false) {

        #ifdef PRINT_DEBUG
            puts("Failed to initialized SDL Mixer");
            fflush(stdout);
        #endif

        return 0;

    }

    Raquet_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);

    for (int i = 0; i < RAQUET_AVAIABLE_AUDIO_TRACKS; i++) {
        Raquet_Tracks[i].track = MIX_CreateTrack(Raquet_Mixer);
        Raquet_Tracks[i].taken = false;
    }

    return 1;
}

Raquet_Sound Raquet_LoadSound(const char * file) {

    Raquet_Sound newSound;

    newSound.sound = MIX_LoadAudio(Raquet_Mixer, file, true);
    int track = (Raquet_FindAvailableTrack());

    if (track > -1) {
        newSound.trackID = track;
        Raquet_Tracks[track].taken = true;
        MIX_SetTrackAudio(Raquet_Tracks[newSound.trackID].track, newSound.sound);
    }

    return newSound;
}

int Raquet_FindAvailableTrack() {
    for (int i = 0; i < RAQUET_AVAIABLE_AUDIO_TRACKS; i++) {
        if (Raquet_Tracks[i].taken == false) {
            return i;
        }
    }
    return -1;
}

void Raquet_PlaySound(Raquet_Sound sound, int loops) {
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
    MIX_PlayTrack(Raquet_Tracks[sound.trackID].track, props);
}

void Raquet_StopSound(Raquet_Sound sound, Sint16 fadeOut) {
    MIX_StopTrack(Raquet_Tracks[sound.trackID].track, fadeOut);
}

void Raquet_DestroySound(Raquet_Sound* sound) {
    MIX_DestroyAudio(sound->sound);
    Raquet_Tracks[sound->trackID].taken = false;
    sound->trackID = -1;
    free(sound);
}

