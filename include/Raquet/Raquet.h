#ifndef RAQUET_GAME_ENGINE
#define SDL_MAIN_HANDLED
#define RAQUET_GAME_ENGINE

/** @file */

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

/**
 * @brief The Window context used by Raquet
 */
extern SDL_Window * Raquet_Window;

/**
 * @brief Fullscreen variable! -1 means no fullscreen, 1 means fullscreen! :3
 */
extern uint8_t Raquet_Fullscreen;

/**
 * @brief The Rendering context used by Raquet
 */
extern SDL_Renderer * Raquet_Renderer;

/**
 * @brief The final texture rendered onto if Integer Scaling is enabled ( See Raquet_Settings.h )
 */
extern SDL_Texture * Raquet_FinalTexture;

/**
 * @brief The size of the Rendering context as a Rectangle
 */
extern const SDL_Rect Raquet_RectScreen;

/**
 * @brief The multiple the Rendering context is multiplied by
 */
extern SDL_Rect Raquet_RectScreenScale;

/**
 * @brief Window event handeler used by Raquet
 */
extern SDL_Event Raquet_Event;

/* Raquet Headers */
#include "Raquet_Types.h"
#include "Raquet_Actor.h"
#include "Raquet_Input.h"
#include "Raquet_Math.h"
#include "Raquet_Misc.h"
#include "Raquet_Palette.h"
#include "Raquet_Sound.h"
#include "Raquet_Video.h"
#include "Raquet_CHR.h"
#include "Raquet_PPF.h"

/*
 ****************************
 *     RAQUET FUNCTIONS     *
 ****************************
 */

/**
 * @brief Starting Tick used to calculate Delta Time
 */
extern Uint64 Raquet_StartTick;

/**
 * @brief Ending Tick used to calculate Delta Time
 */
extern Uint64 Raquet_LastTick;

/**
 * @brief Delta Time, only works if Delta time is enabled ( See Raquet_Settings.h )
 */
extern double Raquet_DeltaTime;

/**
 * @brief Initializes the SDL2 Context that Raquet uses
 */
extern int Raquet_InitSDL(void);

/**
 * @brief Initializes the Raquet Engine
 */
/* Initialize the Raquet Engine */
extern int Raquet_Init(void);

/**
 * @brief Used to update the Window within the Raquet_Main function
 */
extern void Raquet_Update(void);

/**
 * @brief The main Raquet function. This needs to be in the main() function of your program.
 */
extern void Raquet_Main(void);

#endif
