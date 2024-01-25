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
Palette pal_face[3];
Palette pal_logo[3];

// Characters
Raquet_CHR chr_placeface;
Raquet_CHR chr_raquetlogo;

void createthedog()
{
	LoadPPFBank(&ppf_main, "./assets/main.ppf");

	Raquet_SetPalette(pal_face, PAL0D, PAL00, PAL20);
	Raquet_SetPalette(pal_logo, PAL20, PAL20, PAL20);
	
	chr_placeface = LoadCHR(ppf_main, 0, pal_face);
	int arr_raquetlogo[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,};
	chr_raquetlogo = LoadCHRMult(ppf_main, arr_raquetlogo, 6, 2, pal_logo);
	
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
	PlaceCHR(chr_raquetlogo, 101, 109);

	// Reset the Window
	Raquet_Update();

}

int main(int argc, char *argv[]) {

	Raquet_Main();
	return 0;
	
}
