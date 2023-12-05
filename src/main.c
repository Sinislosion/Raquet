#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>

// WINDOW CONSTANTS
const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 240;

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

// CONSTANTS


// GAME VARS
SDL_Color backgroundColorControl = {255, 0, 255, 255};

uint8_t demox = 0;
uint8_t demoy = 0;

int initsdl()
{
	LoadPPFData("./assets/test.ppf");
	
	// Init flag
	int succ = 1;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("FAILED TO INITIALIZE. YOU SUCK.\n");
		succ = 0;
	}
	else
	{
		// Create window
		gWindow = SDL_CreateWindow("Pixel Palette Format", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (gWindow == NULL)
		{
			printf("rip :p\n");
			succ = 0;
		}
		else
		{
			// Init Window Renderer
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			SDL_RenderSetViewport(gRenderer, NULL);
			SDL_RenderSetLogicalSize(gRenderer, 256, 240);
			SDL_GL_SetSwapInterval(1);
			SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
			
		}

	}

	return succ;
	
}

void quitit()
{
	// Deallocate surface
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

int runthedog()
{

	int move = key_right - key_left;
	int vmove = key_down - key_up;
	demox += move;
	demoy += vmove;

	// Draw our stuffs
	SDL_SetRenderDrawColor(gRenderer, backgroundColorControl.r, backgroundColorControl.g, backgroundColorControl.b, 255);
	SDL_RenderFillRect(gRenderer, NULL); 
	
	NES paltest[3] = {NES_PAL0D, NES_PAL00, NES_PAL20};
	for (int y = 0; y < 30; y++) {
		for (int x = 0; x < 32; x++)
		{
			PlaceCHR(0, demox + (x * 8), demoy + (y * 8), paltest);
		}
	}

	// Reset the Window
	SDL_UpdateWindowSurface(gWindow);
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(gRenderer);
	SDL_RenderClear(gRenderer);

	return 1;
}

int main() {

	if (!initsdl())
	{
		printf("ugh i hate writing error messages\n");
	}
	else
	{
		// hack 
		int quit = 0; 
		while(quit == 0)
		{ 
			while(SDL_PollEvent(&e))
			{
				handleInput(e);
				if(e.type == SDL_QUIT)
				{
					quit = 1;
				}
				
			}

			// do our game stuff
			printf("%d\n", key_a);
			fflush(stdout);
			runthedog();

			SDL_Delay(1);
			
		}
			
	}
	
	quitit();

	return 0;
	
}
