#define SDL_MAIN_HANDLED
#include "SDL.h"
#include <stdio.h>

// WINDOW CONSTANTS
const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 240;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Rect gRectScrn = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_Event e;

/*
 *****************************
 *     NES PALETTE STUFF     *
 *****************************
*/

typedef struct NES
{
	Uint32 color;
} NES;

NES NES_PAL00 = {0x6A6A6AFF};
NES NES_PAL01 = {0x001380FF};
NES NES_PAL02 = {0x1E008AFF};
NES NES_PAL03 = {0x39007AFF};
NES NES_PAL04 = {0x550056FF};
NES NES_PAL05 = {0x5A0018FF};
NES NES_PAL06 = {0x4F1000FF};
NES NES_PAL07 = {0x3D1C00FF};
NES NES_PAL08 = {0x253200FF};
NES NES_PAL09 = {0x003D00FF};
NES NES_PAL0A = {0x004000FF};
NES NES_PAL0B = {0x003924FF};
NES NES_PAL0C = {0x002E55FF};
NES NES_PAL0D = {0x000000FF};
NES NES_PAL0E = {0x000000FF};
NES NES_PAL0F = {0x00000000};	// TRANSPARENCY

NES NES_PAL10 = {0xB9BCB9FF};
NES NES_PAL11 = {0x1850C7FF};
NES NES_PAL12 = {0x4B30E3FF};
NES NES_PAL13 = {0x9B22D6FF};
NES NES_PAL14 = {0x951FA9FF};
NES NES_PAL15 = {0x9D285CFF};
NES NES_PAL16 = {0x983700FF};
NES NES_PAL17 = {0x7F4C00FF};
NES NES_PAL18 = {0x5E6400FF};
NES NES_PAL19 = {0x227700FF};
NES NES_PAL1A = {0x027E02FF};
NES NES_PAL1B = {0x007645FF};
NES NES_PAL1C = {0x006E8AFF};
NES NES_PAL1D = {0x000000FF};
NES NES_PAL1E = {0x000000FF};
NES NES_PAL1F = {0x00000000};	// TRANSPARENCY

NES NES_PALINVALID = {0xFF00FFFF};

NES NES_PAL20 = {0xFFFFFFFF};
/*
namespace NES
{
	static const Uint32
		// RED, GREEN, BLUE, ALPHA

		PAL10{0xB9BCB9FF}, PAL11{0x1850C7FF}, PAL12{0x4B30E3FF}, PAL13{0x9B22D6FF},
		PAL14{0x951FA9FF}, PAL15{0x9D285CFF}, PAL16{0x983700FF}, PAL17{0x7F4C00FF}, 
		PAL18{0x5E6400FF}, PAL19{0x227700FF}, PAL1A{0x027E02FF}, PAL1B{0x007645FF}, 
		PAL1C{0x006E8AFF}, PAL1D{0x000000FF}, PAL1E{0x000000FF}, PAL1F{0x00000000},

		PAL20{0xFFFFFFFF}, PAL21{0x68A6FFFF}, PAL22{0x8C9CFFFF}, PAL23{0xB586F4FF},
		PAL24{0xD975FDFF}, PAL25{0xE377B9FF}, PAL26{0xE58D68FF}, PAL27{0xD49D29FF},
		PAL28{0xB3AF0CFF}, PAL29{0x7BC211FF}, PAL2A{0x55CA47FF}, PAL2B{0x46CB81FF},
		PAL2C{0x47C1C5FF}, PAL2D{0x4A4D4AFF}, PAL2E{0x000000FF}, PAL2F{0x00000000},

		PAL30{0xFFFFFFFF}, PAL31{0xCCEAFFFF}, PAL32{0xDDDEFFFF}, PAL33{0xECDAFFFF},
		PAL34{0xF8D7FEFF}, PAL35{0xFCD6D5FF}, PAL36{0xFDDBCFFF}, PAL37{0xF9E7B5FF},
		PAL38{0xF1F0AAFF}, PAL39{0xDAFAA9FF}, PAL3A{0xC9FFBCFF}, PAL3B{0xC3FBD7FF},
		PAL3C{0xC4F6F6FF}, PAL3D{0xBEC1BEFF}, PAL3E{0x000000FF}, PAL3F{0x00000000};
		
}

*/

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

void RaquetClear(NES pal)
{
	Uint32 palr  = (pal.color >> 24) & 0x000000FF;
	Uint32 palg  = (pal.color >> 16) & 0x000000FF;
	Uint32 palb  = (pal.color >> 8) & 0x000000FF;
	SDL_SetRenderDrawColor(gRenderer, palr, palg, palb, 255);
	SDL_RenderFillRect(gRenderer, NULL); 
}

void RaquetDrawRectangle(int x1, int y1, int width, int height, NES pal, int alpha, int fill)
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

SDL_Texture* LoadCHR(int id, NES palette[3])
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
					pixels[dest] = NES_PAL0F.color;
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
