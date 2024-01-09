#define SDL_MAIN_HANDLED
#ifndef SDL2
#define SDL2
#include "SDL.h"
#endif
#include <stdio.h>
#include <gme/gme.h>

// WINDOW CONSTANTS
const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 240;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Rect gRectScrn = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_Event e;

/*
************************
*     AUDIO STUFFS     *
************************
* big thanks to gme btw, legally I must iterate this part of the
* source code is licensed under LGPL2 yada yada libgme devs blah blah go to
* https://github.com/libgme/game-music-emu for more info

Copyright (C) 2005-2010 by Shay Green. Permission is hereby granted, free of
charge, to any person obtaining a copy of this software module and associated
documentation files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and
to permit persons to whom the Software is furnished to do so, subject to the
following conditions: The above copyright notice and this permission notice
shall be included in all copies or substantial portions of the Software. THE
SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ 

#include "Audio_Scope.cpp"
#include "Music_Player.cpp"

/*
 ************************
 *     PALETTE INFO     *
 ************************
*/

typedef struct Palette
{
	Uint32 color;
} Palette;

Palette PAL00 = {0x6A6A6AFF};
Palette PAL01 = {0x001380FF};
Palette PAL02 = {0x1E008AFF};
Palette PAL03 = {0x39007AFF};
Palette PAL04 = {0x550056FF};
Palette PAL05 = {0x5A0018FF};
Palette PAL06 = {0x4F1000FF};
Palette PAL07 = {0x3D1C00FF};
Palette PAL08 = {0x253200FF};
Palette PAL09 = {0x003D00FF};
Palette PAL0A = {0x004000FF};
Palette PAL0B = {0x003924FF};
Palette PAL0C = {0x002E55FF};
Palette PAL0D = {0x000000FF};
Palette PAL0E = {0x000000FF};
Palette PAL0F = {0x00000000};	// TRANSPARENCY

Palette PAL10 = {0xB9BCB9FF};
Palette PAL11 = {0x1850C7FF};
Palette PAL12 = {0x4B30E3FF};
Palette PAL13 = {0x9B22D6FF};
Palette PAL14 = {0x951FA9FF};
Palette PAL15 = {0x9D285CFF};
Palette PAL16 = {0x983700FF};
Palette PAL17 = {0x7F4C00FF};
Palette PAL18 = {0x5E6400FF};
Palette PAL19 = {0x227700FF};
Palette PAL1A = {0x027E02FF};
Palette PAL1B = {0x007645FF};
Palette PAL1C = {0x006E8AFF};
Palette PAL1D = {0x000000FF};
Palette PAL1E = {0x000000FF};
Palette PAL1F = {0x00000000};	// TRANSPARENCY

Palette PAL20 = {0xFFFFFFFF};
Palette PAL21 = {0x68A6FFFF};
Palette PAL22 = {0x8C9CFFFF};
Palette PAL23 = {0xB586F4FF};
Palette PAL24 = {0xD975FDFF};
Palette PAL25 = {0xE377B9FF};
Palette PAL26 = {0xE58D68FF};
Palette PAL27 = {0xD49D29FF};
Palette PAL28 = {0xB3AF0CFF};
Palette PAL29 = {0x7BC211FF};
Palette PAL2A = {0x55CA47FF};
Palette PAL2B = {0x46CB81FF};
Palette PAL2C = {0x47C1C5FF};
Palette PAL2D = {0x4A4D4AFF};
Palette PAL2E = {0x000000FF};
Palette PAL2F = {0x00000000};	// TRANSPARENCY

Palette PAL30 = {0xFFFFFFFF};
Palette PAL31 = {0xCCEAFFFF};
Palette PAL32 = {0xDDDEFFFF};
Palette PAL33 = {0xECDAFFFF};
Palette PAL34 = {0xF8D7FEFF};
Palette PAL35 = {0xFCD6D5FF};
Palette PAL36 = {0xFDDBCFFF};
Palette PAL37 = {0xF9E7B5FF};
Palette PAL38 = {0xF1F0AAFF};
Palette PAL39 = {0xDAFAA9FF};
Palette PAL3A = {0xC9FFBCFF};
Palette PAL3B = {0xC3FBD7FF};
Palette PAL3C = {0xC4F6F6FF};
Palette PAL3D = {0xBEC1BEFF};
Palette PAL3E = {0x000000FF};
Palette PAL3F = {0x00000000};	// TRANSPARENCY

Palette PALINVALID = {0xFF00FFFF};

/*
 *********************************
 *     CUSTOM MATH FUNCTIONS     *
 *********************************
*/

int fsign(float comp) 
{
	return ((0) < comp) - (comp < (0));
}

/*
 ***************************
 *     INPUT FUNCTIONS     *
 ***************************
*/


/*
 * NES KEYMAP
 * UP, DOWN, LEFT, RIGHT
 * B, A, SELECT, START
 * 
 * If a key is being pressed, it is 1
 * If it is not being pressed, it is 0
 * TODO: make this rely on a configuration file for key remapping
 */

int key_up = 0, key_down = 0, key_left = 0, key_right = 0, 
key_a = 0, key_b = 0, key_select = 0, key_start = 0,
key_zoomin = 0, key_zoomout = 0;

void handleInput(SDL_Event e)
{ 
	switch (e.type)
	{
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym) 
			{
				case SDLK_UP:
					key_up = 1;
				break;

				case SDLK_DOWN:
					key_down = 1;
				break;
					
				case SDLK_LEFT:
					key_left = 1;
				break;
				
				case SDLK_RIGHT:
					key_right = 1;
				break;

				case SDLK_x:
					key_a = 1;
				break;

				case SDLK_w:
					key_zoomin = 1;
				break;

				case SDLK_s:
					key_zoomout = 1;
				break;
			}
		break;

		case SDL_KEYUP:
			switch(e.key.keysym.sym) 
			{
				case SDLK_UP:
					key_up = 0;
				break;

				case SDLK_DOWN:
					key_down = 0;
				break;
					
				case SDLK_LEFT:
					key_left = 0;
				break;
				
				case SDLK_RIGHT:
					key_right = 0;
				break;

				case SDLK_x:
					key_a = 0;
				break;

				case SDLK_w:
					key_zoomin = 0;
				break;

				case SDLK_s:
					key_zoomout = 0;
				break;
			}
		break;
		
	}
}


/*
 ****************************
 *     RAQUET FUNCTIONS     *
 ****************************
*/

// DOG FUNCTIONS
void runthedog(); // put this somewhere in your program, the default code to run
void createthedog(); // put all your creation code for the program here

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
		gWindow = SDL_CreateWindow("Raquet Game Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

int initRaquet()
{	
	int ready = 1;
	if (!initsdl())
	{
		printf("Failed to Initialize SDL\n");
		ready = 0;
	}
	// PUT YOUR SETUP CODE HERE
	
	return ready;
}

void quitit()
{
	printf("Done.\n");
	// Deallocate surface
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

void RaquetClear(Palette pal)
{
	Uint32 palr  = (pal.color >> 24) & 0x000000FF;
	Uint32 palg  = (pal.color >> 16) & 0x000000FF;
	Uint32 palb  = (pal.color >> 8) & 0x000000FF;
	SDL_SetRenderDrawColor(gRenderer, palr, palg, palb, 255);
	SDL_RenderFillRect(gRenderer, NULL); 
}

void RaquetDrawRectangle(int x1, int y1, int width, int height, Palette pal, int alpha, int fill)
{
	SDL_Rect rect = {x1, y1, width,height};
	Uint32 palr  = (pal.color >> 24) & 0x000000FF;
	Uint32 palg  = (pal.color >> 16) & 0x000000FF;
	Uint32 palb  = (pal.color >> 8) & 0x000000FF;

	SDL_SetRenderDrawColor(gRenderer, palr, palg, palb, alpha);
	
	switch (fill)
	{
		default:
			SDL_RenderDrawRect(gRenderer, &rect);
		break;

		case 1:
			SDL_RenderFillRect(gRenderer, &rect);
		break;
	}

}

void RaquetUpdate()
{
	SDL_UpdateWindowSurface(gWindow);
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(gRenderer);
	SDL_RenderClear(gRenderer);
}

void RaquetMain() {
	if (!initRaquet())
	{
		printf("Failed to Initialize\n");
	}
	else
	{
		printf("Raquet Initialized\n");
		fflush(stdout);
		createthedog();
		
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
			runthedog();
			
		}
			
	}
		
	quitit();

}

/*
 *************************
 *     PPF FUNCTIONS     *
 *************************
*/

// the array we store our data in, with a max file size of 8KB
char CHARDATASET[8192];

// PPF HEADER V1.0
const unsigned char PPFHEADER[8] =
{
    0x50, 0x50, 0x46, 0x76, 0x01, 0x00, 0x00, 0x00
};

const unsigned int ppfbitmask[8] =
{
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 
};

int LoadPPFData(const char* dir)
{
	
	SDL_RWops* ppfdata = SDL_RWFromFile(dir, "rb");

	// check if ppf data is a valid directory
	if (ppfdata != NULL) 
	{
		
		SDL_RWread(ppfdata, CHARDATASET, 8, 1024);

		for (int i = 0; i < 8; i++)
		{
			if (CHARDATASET[i] != PPFHEADER[i])
			{
				printf("WARNING HEADER DATA DOES NOT MATCH\n");
			}
			
		} 

		SDL_RWclose(ppfdata);
		return 1;
		
	} 
	else 
	{
		printf("Failed to load PPF");
		return 0;
	}

}

SDL_Texture* LoadCHR(int id, Palette palette[3])
{
	
	SDL_Texture* tex = SDL_CreateTexture(
		gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 8, 8);

	Uint32 pixels[64];
	
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			int dest = x + (y * 8);
			int index = y + 8 + (8 * (id * 2));
			int index2 = y + 16 + (16 * (id * 2));

			int check1 = fsign(CHARDATASET[index] & ppfbitmask[x]);
			int check2 = fsign(CHARDATASET[index2] & ppfbitmask[x]);
			int place =  check1 +  check2;

			switch (place)
			{
				case 0:
					pixels[dest] = PAL0F.color;
				break;

				case 1:
					if (check1)
					{
						pixels[dest] = palette[0].color;
					} 
					else {
						pixels[dest] = palette[1].color;
					}
				break;

				case 2:
					pixels[dest] = palette[2].color;
				break;
			}

		}
		
	}

	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	SDL_UpdateTexture(tex, NULL, pixels, 8 * sizeof(Uint32));
	
	return tex;
}

void PlaceCHR(SDL_Texture* tex, int x, int y) {
	SDL_Rect dstrect;
		dstrect.x = x;
		dstrect.y = y;
		dstrect.w = 8;
		dstrect.h = 8;

	SDL_RenderCopy(gRenderer, tex, NULL, &dstrect);
}



void DestroyCHR(SDL_Texture* tex)
{
	SDL_DestroyTexture(tex);
}
