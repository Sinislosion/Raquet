// put your libs here.
#include "Raquet.h"
// TODO: WRITE FPS FUNCTIONS
// TODO: COMMENT MORE STUFF IN THE Raquet.h FILE
// TODO: MAKE CREATING VARIABLES MORE SIMPLE

uint8_t demoy;
uint8_t demox;
Palette palface[3];
Palette pallogo[3];

SDL_Texture* placeface;
SDL_Texture* RaquetLogo[12];

void createthedog()
{
	LoadPPFData("./assets/main.ppf");

	 palface[0] = PAL0D;
	 palface[1] = PAL00;
	 palface[2] = PAL20;

	 pallogo[0] = PAL20;
	 pallogo[1] = PAL0D;
	 pallogo[2] = PAL11;
	
	placeface = LoadCHR(0, palface);

	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			int offset = x + (6 * y);
			RaquetLogo[offset] = LoadCHR(offset + 1, pallogo);
		}
	}
	const char* nsfpath = "./assets/2A03_Kevvviiinnn-Superfusion.nsf";
	RaquetSound_LoadAudio(nsfpath);
	RaquetSound_EnableAccuracy(1);
	RaquetSound_StartTrack(0, 0);

	demox = 0;
	demoy = 0;
	
}

void runthedog()
{
	int move = key_right - key_left;
	demox += move;
	demoy++;

	// Draw our stuffs
	RaquetClear(PAL12);

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

	RaquetDrawRectangle(96, 104, 56, 24, PAL12, 255, 1);
	
	for (int y = 0; y < 2; y++) 
	{
		for (int x = 0; x < 6; x++)
		{
			PlaceCHR(RaquetLogo[x + (6 * y)], 0 + x * 8 + 101, 0 + y * 8 + 109);
		}
	}

	// Reset the Window
	RaquetUpdate();

}

int main() {

	RaquetMain();
	return 0;
	
}
