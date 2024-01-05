// put your libs here.
#include "Raquet.h"
// TODO: WRITE FPS FUNCTIONS
// TODO: COMMENT MORE STUFF IN THE Raquet.h FILE
// TODO: MAKE CREATING VARIABLES MORE SIMPLE

uint8_t demoy;
uint8_t demox;
NES palface[3];
SDL_Texture* placeface;

void createthedog()
{
	LoadPPFData("./assets/main.ppf");

	 palface[0] = NES_PAL00;
	 palface[1] = NES_PAL0D;
	 palface[2] = NES_PAL20;
	
	placeface = LoadCHR(0, palface);

	demox = 0;
	demoy = 0;
	
}

void runthedog()
{
	int move = key_right - key_left;
	int vmove = key_down - key_up;
	demox += move;
	demoy += vmove;

	// Draw our stuffs
	RaquetClear(NES_PALINVALID);

	for (int y = 0; y < SCREEN_WIDTH/8; y++) 
	{
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

}

int main() {

	RaquetMain();
	return 0;
	
}
