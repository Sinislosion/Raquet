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
#include "Raquet_Sound.h"
#include "Raquet_Palette.h"
#include "Raquet_Input.h"
#include "Raquet_Actor.h"
#include "Raquet_Math.h"

/*
 **************************
 *     MISC FUNCTIONS     *
 **************************
 */

/* 1 for display cursor, 0 for disable cursor */
extern void Raquet_ShowCursor(int toggle);

/* Get an absolute string to an asset from the assets folder */
extern const char* Raquet_AbsoluteToAsset(const char* relativepath);

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

/*
 *************************
 *     PPF FUNCTIONS     *
 *************************
 */

// PPF HEADER V1.0
extern const unsigned char PPFHEADER[8];

/*
 * fixed bitmask we use to read CHR data 
 * (this is probably sloppy, but im not smart enough to know another way)
 */
extern const unsigned int ppfbitmask[8];

/* Load a PPF bank into memory. More info is in the wiki */
int Raquet_LoadPPFBank(PPF_Bank* targetarray, const char* dir);

Raquet_CHR Raquet_LoadCHR(PPF_Bank ppfbank, int id, Palette pal[3]);

/* Load a multi-tile sprite. More info is in the wiki */
Raquet_CHR Raquet_LoadCHRMult(PPF_Bank ppfbank, int * id, int xwrap, int ywrap, Palette palette[3]);

/* Returns a struct of the width and height of the CHR, accessable with x and y */
Raquet_Point Raquet_SizeofCHR(SDL_Texture * tex);

/* Returns the width of a CHR */
int Raquet_WidthofCHR(SDL_Texture * tex);

/* returns the height of a CHR */
int Raquet_HeightofCHR(SDL_Texture * tex);

void Raquet_PlaceCHR(Raquet_CHR chr, int x, int y);

/* Place a CHR sprite, with additional control (chr data, x, y, width in pixels, height in pixels, horizontal flip, vertical flip) */
void Raquet_PlaceCHR_ext(Raquet_CHR chr, int x, int y, int xsize, int ysize, double angle, Raquet_Point center, SDL_RendererFlip flip);

/* Swap a CHR's Palette */ 
void Raquet_SwapCHRPalette(Raquet_CHR* chr, Palette pal[3]);

/* Destroy a CHR after we're done using it */
void Raquet_DestroyCHR(SDL_Texture * tex);

void Raquet_DrawPoint(Palette pal, int x, int y, int alpha);

void Raquet_DrawLine(Palette pal, int x1, int y1, int x2, int y2, int alpha);

/*
 ******************
 *     CAMERA     *
 ******************
 */

typedef struct Raquet_Camera {
    float x, y;
} Raquet_Camera;

extern Raquet_Camera Camera;

#endif
