// put your libs here.
#include "../include/Raquet.h"
#include <math.h>
// TODO: COMMENT MORE STUFF IN THE Raquet.h FILE

// PPF Banks
PPF_Bank ppf_main;

// Global Demo Vars
float demotime = 0;

// BG Stars
#define PARTICLE_AMOUNT		SCREEN_WIDTH/4
typedef struct Star
{
  double x;
	int y;
} Star;

Star stararray[PARTICLE_AMOUNT];

// Palettes
Palette pal_face[3];
Palette pal_logo[3];

// Characters
Raquet_CHR chr_placeface;
Raquet_CHR chr_raquetlogo_R;
Raquet_CHR chr_raquetlogo_A;
Raquet_CHR chr_raquetlogo_Q;
Raquet_CHR chr_raquetlogo_U;
Raquet_CHR chr_raquetlogo_E;
Raquet_CHR chr_raquetlogo_T;

/*
 **************************
 *     STAR FUNCTIONS     *
 **************************
*/

// Init each star particle
Star createStars() 
{
	int pos  = (rand() % (SCREEN_HEIGHT + 1));
	Star par;
	par.x = 0;
	par.y = pos;

	return par;
}

// init all our stars
void initStars() 
{
	for (int i = 0; i < PARTICLE_AMOUNT; i++) {
		stararray[i] = createStars();
		stararray[i].x += 4.0 * i;
	}
}

// draw the stars
void drawStars() 
{	
	// for every star we know exists, and for every 4 points we need to draw.
	for (int i = 0; i < PARTICLE_AMOUNT; i++) {
		stararray[i].x -= 0.05 * (delta_time);
		for (int o = 0; o < 4; o++)
		{
      int tempx = stararray[i].x;
			Raquet_SetDrawColor(PAL30, 255 - (64 * o));
			SDL_RenderDrawPoint(gRenderer, tempx + o, stararray[i].y);
		}
		// if a point exceeds 0, plus its length, loop it back to the screen width
		if (stararray[i].x <= -4.0) {
			stararray[i] = createStars();
      stararray[i].x = SCREEN_WIDTH;
		}
	}
  printf("%f\n",stararray[0].x);
  fflush(stdout);
}

/*
 **************************
 *     MAIN FUNCTIONS     *
 **************************
*/

void createthedog()
{
	LoadPPFBank(&ppf_main, "./assets/main.ppf");

	Raquet_SetPalette(pal_face, PAL0D, PAL00, PAL20);
	Raquet_SetPalette(pal_logo, PAL20, PAL20, PAL20);
	
	chr_placeface = LoadCHR(ppf_main, 0, pal_face);
	int arr_raquetlogo_R[2] = {1, 7};
	int arr_raquetlogo_A[2] = {2, 8};
	int arr_raquetlogo_Q[2] = {3, 9};
	int arr_raquetlogo_U[2] = {4, 10};
	int arr_raquetlogo_E[2] = {5, 11};
	int arr_raquetlogo_T[2] = {6, 12};
	chr_raquetlogo_R = LoadCHRMult(ppf_main, arr_raquetlogo_R, 1, 2, pal_logo);
	chr_raquetlogo_A = LoadCHRMult(ppf_main, arr_raquetlogo_A, 1, 2, pal_logo);
	chr_raquetlogo_Q = LoadCHRMult(ppf_main, arr_raquetlogo_Q, 1, 2, pal_logo);
	chr_raquetlogo_U = LoadCHRMult(ppf_main, arr_raquetlogo_U, 1, 2, pal_logo);
	chr_raquetlogo_E = LoadCHRMult(ppf_main, arr_raquetlogo_E, 1, 2, pal_logo);
	chr_raquetlogo_T = LoadCHRMult(ppf_main, arr_raquetlogo_T, 1, 2, pal_logo);

	initStars();
	
	const char* nsfpath = "./assets/2A03_Kevvviiinnn-Superfusion.nsf";
	RaquetSound_LoadAudio(nsfpath);	
	RaquetSound_StartTrack(0, 0);
	
}

void runthedog()
{
	demotime += 0.05 * delta_time;
  
	// Draw our stuffs
	Raquet_Clear(PAL12);

	drawStars();
	
	for (int i = 0; i < 6; i++)
	{
		Raquet_CHR arr[6] = {
      chr_raquetlogo_R, chr_raquetlogo_A, chr_raquetlogo_Q, 
      chr_raquetlogo_U, chr_raquetlogo_E, chr_raquetlogo_T
    };
		PlaceCHR(arr[i], ((SCREEN_WIDTH/2) - 24) + (8 * i), ((SCREEN_HEIGHT/2) - 8) + (sin((demotime + (i * 8)) * .1) * 4));
	}

	// Reset the Window
	Raquet_Update();

}

int main(int argc, char *argv[]) {

	Raquet_Main();
	return 0;
	
}
