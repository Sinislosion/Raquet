#ifndef RAQUET_GAME_ENGINE
#define SDL_MAIN_HANDLED
#define RAQUET_GAME_ENGINE

/* headers we need */
#include "Raquet_Settings.h"
#ifdef PRINT_DEBUG
    #include <stdio.h>
#endif
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

/* More Constants */
extern SDL_Window * gWindow;
extern uint8_t gFullscreen;
extern SDL_Renderer * gRenderer;
extern SDL_Texture * gFinalTexture;

extern const SDL_Rect gRectScreen;
extern SDL_Rect gRectScreenScale;
extern SDL_Event e;

/* Raquet Headers */
#include "Raquet_Types.h"
#include "Raquet_Actor.h"
#include "Raquet_Input.h"
#include "Raquet_Math.h"
#include "Raquet_Misc.h"
#include "Raquet_Palette.h"
#include "Raquet_Sound.h"
#include "Raquet_Video.h"

/*
 ****************************
 *     RAQUET FUNCTIONS     *
 ****************************
 */

// FRAMERATE
extern Uint64 start_tick;
extern Uint64 last_tick;
extern double Raquet_DeltaTime;

/* Initialize SDL2 */
extern int Raquet_InitSDL();

/* Initialize the Raquet Engine */
extern int Raquet_Init();

/* Set a palette variable within runtime */
extern void Raquet_SetPalette(Palette dest[3], Uint32 pal1, Uint32 pal2, Uint32 pal3);

/* Copy a Palette to another array */
extern void Raquet_CopyPalette(Palette dest[3], Palette origin[3]);

/* I'm actually not sure what you can use this for yet, but you can use it */
extern void Raquet_SetDrawColor(Palette pal, int alpha);

/* Clear the screen with a solid color */
#ifdef BACKGROUND_CLEARING_COLOR
    Palette gClearColor;
#endif

/* Clear the screen */
extern void Raquet_Clear(Palette pal);

/* Draw a rectangle (x position, y position, width, height, color, alpha, fill) */
extern void Raquet_DrawRectangle(int x1, int y1, int width, int height, Palette pal, int alpha, int fill);

/* This is used to update the Window within the Raquet_Main function */
extern void Raquet_Update();

/* The main Raquet function. Everything runs from here. */
extern void Raquet_Main();

#endif
