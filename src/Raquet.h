#define SDL_MAIN_HANDLED
#include "SDL.h"
#include <stdio.h>
#include "nes.h"

// WINDOW CONSTANTS
const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 240;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Rect gRectScrn = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_Event e;

int initsdl()
{
	
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
		gWindow = SDL_CreateWindow("Raquet! Game Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (gWindow == NULL)
		{
			printf("rip :p\n");
			succ = 0;
		}
		else
		{
			printf("SDL Initialized\n");
			fflush(stdout);
			// Init Window Renderer
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			SDL_RenderSetViewport(gRenderer, NULL);
			SDL_RenderSetLogicalSize(gRenderer, 256, 240);
			SDL_GL_SetSwapInterval(-1);	// Uncomment this for VSYNC
			SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
			
		}

	}

	return succ;
	
}

void quitit()
{
	printf("bye bye :)\n");
	// Deallocate surface
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

void RaquetClear(NES pal)
{
	Uint32 palr  = (pal.color >> 24) & 0x000000FF;
	Uint32 palg  = (pal.color >> 16) & 0x000000FF;
	Uint32 palb  = (pal.color >> 8) & 0x000000FF;
	SDL_SetRenderDrawColor(gRenderer, palr, palg, palb, 255);
	SDL_RenderFillRect(gRenderer, NULL); 
}

void RaquetUpdate()
{
	SDL_UpdateWindowSurface(gWindow);
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(gRenderer);
	SDL_RenderClear(gRenderer);
}
