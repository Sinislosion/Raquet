#include "Raquet.h"

/* 1 for display cursor, 0 for disable cursor */
void Raquet_ShowCursor(int toggle) {
    if (toggle) {
        SDL_ShowCursor();
    } else {
        SDL_HideCursor();
    }
}

/* Get an absolute string to an asset from the assets folder */
const char* Raquet_AbsoluteToAsset(const char* relativepath) {
    const char* newpath = calloc(512, sizeof(char));
    #ifndef _MSC_VER
        sprintf(newpath, "%sassets/%s", SDL_GetBasePath(), relativepath);
    #else
        sprintf_s(newpath, 512, "%sassets\\%s", SDL_GetBasePath(), relativepath);
    #endif
    return newpath;
}
