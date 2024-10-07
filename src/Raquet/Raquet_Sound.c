#include "Raquet_Sound.h"

Raquet_Sound Raquet_LoadSound(const char * file) {
    return Mix_LoadWAV(file);
}

void Raquet_PlaySound(Raquet_Sound wav, int loops) {
    Mix_PlayChannel(-1, wav, loops);
}

void Raquet_DestroySound(Raquet_Sound wav) {
    Mix_FreeChunk(wav);
}
