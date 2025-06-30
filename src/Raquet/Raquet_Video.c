#include "Raquet.h"

/* Define the camera*/
Raquet_Camera Camera = {0, 0};

/* Set a palette variable within runtime */
void Raquet_SetPalette(Palette dest[4], Uint32 pal1, Uint32 pal2, Uint32 pal3, Uint32 pal4) {
    dest[0] = pal1;
    dest[1] = pal2;
    dest[2] = pal3;
    dest[3] = pal4;
}

void Raquet_CopyPalette(Palette dest[4], Palette origin[4]) {
    dest[0] = origin[0];
    dest[1] = origin[1];
    dest[2] = origin[2];
    dest[3] = origin[3];
}

/* I'm actually not sure what you can use this for yet, but you can use it */
void Raquet_SetDrawColor(Palette pal, int alpha) {
    Uint32 palr = (pal >> 24) & 0x000000FF;
    Uint32 palg = (pal >> 16) & 0x000000FF;
    Uint32 palb = (pal >> 8) & 0x000000FF;
    SDL_SetRenderDrawColor(Raquet_Renderer, palr, palg, palb, alpha);
}

/* Clear the screen with a solid color */
#ifdef BACKGROUND_CLEARING_COLOR
    Palette gClearColor;
#endif

void Raquet_Clear(Palette pal) {
    Raquet_SetDrawColor(pal, 255);
    SDL_RenderFillRect(Raquet_Renderer, NULL);

    #ifdef BACKGROUND_CLEARING_COLOR
        Raquet_ClearColor = pal;
    #endif

}

/* Draw a rectangle (x position, y position, width, height, color, alpha, fill) */
void Raquet_DrawRectangle(int x1, int y1, int width, int height, Palette pal, int alpha, int fill) {
    const SDL_FRect rect = {
        x1,
        y1,
        width,
        height
    };
    Raquet_SetDrawColor(pal, alpha);

    switch (fill) {
    default:
        SDL_RenderRect(Raquet_Renderer, & rect);
        break;

    case 1: 
        SDL_RenderFillRect(Raquet_Renderer, & rect);
        break;
    }

}

void Raquet_DrawPoint(int x, int y, Palette pal, int alpha) {
    Raquet_SetDrawColor(pal, alpha);
    SDL_RenderPoint(Raquet_Renderer, x, y);
}

void Raquet_DrawLine(int x1, int y1, int x2, int y2, Palette pal, int alpha) {
    Raquet_SetDrawColor(pal, alpha);
    SDL_RenderLine(Raquet_Renderer, x1, y1, x2, y2);
}

