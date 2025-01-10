#include "Raquet.h"

/* 1 for display cursor, 0 for disable cursor */
void Raquet_ShowCursor(int toggle) {
    SDL_ShowCursor(toggle);
}

/* Get an absolute string to an asset from the assets folder */
const char* Raquet_AbsoluteToAsset(const char* relativepath) {
    char ret[1024] = "";
    strcpy(ret, SDL_GetBasePath());
    strcat(ret, "assets/");
    strcat(ret, relativepath);
    return (const char*)strcpy((char*)malloc(sizeof(char*) * strlen(ret)), ret);
}
