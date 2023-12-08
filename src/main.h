#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>

// WINDOW CONSTANTS
#define SCREEN_WIDTH	256
#define SCREEN_HEIGHT	240

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Rect gRectScrn = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_Event e;

// Math and Graphical LIBS
#include "math_functions.c"
#include "nes.h"
#include "ppf_functions.c"

// GAME LIBS
#include "input.h"

int initsdl(); 		// Initalize SDL

void quitit(); 		// Quit SDL and cleanup

int runthedog(); 	// Our game code goes here

int main();			// you're a C programmer, you know what this does
