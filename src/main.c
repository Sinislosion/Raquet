// put your libs here.
#include "Raquet.h"

// TODO: WRITE FPS FUNCTIONS
// TODO: COMMENT MORE STUFF IN THE Raquet.h FILE

// Demo Global Variables
uint8_t demoy;
uint8_t demox;

// PPF Banks
PPF_Bank ppf_main;

// Palettes
Palette palface[3];
Palette pallogo[3];

// Characters
Raquet_CHR chr_placeface;
Raquet_CHR chr_raquetlogo[12];

void createthedog()
{
	LoadPPFData(&ppf_main, "./assets/main.ppf");

	Raquet_SetPalette(palface, PAL0D, PAL00, PAL20);
	Raquet_SetPalette(pallogo, PAL20, PAL20, PAL20);
	
	chr_placeface = LoadCHR(ppf_main, 0, palface);

	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			int offset = x + (6 * y);
			chr_raquetlogo[offset] = LoadCHR(ppf_main, offset + 1, pallogo);
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
	Raquet_Clear(PAL12);

	for (int y = 0; y < SCREEN_WIDTH/8; y++) 
	{
		for (int x = 0; x < SCREEN_WIDTH/8; x++)
		{
			PlaceCHR(chr_placeface, demox + (x * 8), demoy + (y * 8));
			PlaceCHR(chr_placeface, demox - (x * 8) - 8, demoy + (y * 8));

			PlaceCHR(chr_placeface, demox + (x * 8), demoy - (y * 8) - 8);
			PlaceCHR(chr_placeface, demox - (x * 8) - 8, demoy - (y * 8) - 8);
		}
	}

	Raquet_DrawRectangle(96, 104, 56, 24, PAL12, 255, 1);
	
	for (int y = 0; y < 2; y++) 
	{
		for (int x = 0; x < 6; x++)
		{
			PlaceCHR(chr_raquetlogo[x + (6 * y)], 0 + x * 8 + 101, 0 + y * 8 + 109);
		}
	}

	// Reset the Window
	Raquet_Update();

}

int main(int argc, char *argv[]) {

	Raquet_Main();
	return 0;
	
}
