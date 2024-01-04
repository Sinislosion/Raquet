#include "Raquet.h"

// Math and Graphical LIBS
#include "math_functions.c"
#include "ppf_functions.c"

// GAME LIBS
#include "input.h"

// CONSTANTS
SDL_Texture* placeface;

// GAME VARS
SDL_Color backgroundColorControl = {255, 0, 255, 255};

uint8_t demox = 0;
uint8_t demoy = 0;

int initRaquet()
{	
	int ready = 1;
	if (!initsdl())
	{
		printf("Failed to Initialize SDL\n");
		ready = 0;
	}
	// PUT YOUR SETUP CODE HERE
	LoadPPFData("./assets/test.ppf");
	
	
	return ready;
}

int runthedog()
{

	int move = key_right - key_left;
	int vmove = key_down - key_up;
	demox += move;
	demoy += vmove;

	// Draw our stuffs
	RaquetClear(NES_PALINVALID);

	if (placeface == NULL) {
		NES paltest[3] = {NES_PAL0D, NES_PAL00, NES_PAL20};
		placeface = LoadCHR(0, paltest);
	}

	for (int y = 0; y < SCREEN_WIDTH/8; y++) {
		for (int x = 0; x < SCREEN_WIDTH/8; x++)
		{
			PlaceCHR(placeface, demox + (x * 8), demoy + (y * 8));
			PlaceCHR(placeface, demox - (x * 8) - 8, demoy + (y * 8));

			PlaceCHR(placeface, demox + (x * 8), demoy - (y * 8) - 8);
			PlaceCHR(placeface, demox - (x * 8) - 8, demoy - (y * 8) - 8);
		}
	}

	// Reset the Window
	RaquetUpdate();

	return 1;
}

int main() {

	if (!initRaquet())
	{
		printf("Failed to Initialize\n");
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
			runthedog();
			
		}
			
	}
	
	quitit();

	return 0;
	
}
