// put your libs here.
#include "Raquet.h"
#include "Raquet_Math.h"

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
void drawStars() {	

	// for every star we know exists, and for every 4 points we need to draw.
	for (int i = 0; i < PARTICLE_AMOUNT; i++) {
		stararray[i].x--;
		for (int o = 0; o < 4; o++) {
			int tempx = stararray[i].x;
			Raquet_DrawPoint(PAL30, tempx + o, stararray[i].y, 255 - (64 * o));
		}

		// if a point exceeds 0, plus its length, loop it back to the screen width
		if (stararray[i].x <= -4) {
			stararray[i] = createStars();
			stararray[i].x = SCREEN_WIDTH;
		}
	} 
}

/*
 *********************
 *     PLACEFACE     *
 *********************
*/
Actor* act_placeface;
Actor* act_placeface2;
void bePlaceface()
{
	int key_up = Raquet_KeyCheck(SDL_SCANCODE_UP);
	int key_down = Raquet_KeyCheck(SDL_SCANCODE_DOWN);
	int key_left = Raquet_KeyCheck(SDL_SCANCODE_LEFT);
	int key_right = Raquet_KeyCheck(SDL_SCANCODE_RIGHT);

	int move_x = key_right - key_left;
	int move_y = key_down - key_up; 

	if (Raquet_ActorColliding(act_placeface->x + (move_x * 2), act_placeface->y, act_placeface, act_placeface2)) {
		while (!Raquet_ActorColliding(act_placeface->x + Raquet_Sign(move_x), act_placeface->y, act_placeface, act_placeface2)) {
			act_placeface->x += Raquet_Sign(move_x);
		}
		move_x = 0;
	}

	act_placeface->x += move_x * 2;

	if (Raquet_ActorColliding(act_placeface->x, act_placeface->y + (move_y * 2), act_placeface, act_placeface2))  {
		while (!Raquet_ActorColliding(act_placeface->x, act_placeface->y + Raquet_Sign(move_y), act_placeface, act_placeface2)) {
			act_placeface->y += Raquet_Sign(move_y);
		}
		move_y = 0;
	}
	act_placeface->y += move_y * 2;
	act_placeface->angle++;

	Raquet_DrawActor(act_placeface);
	Raquet_DrawActor(act_placeface2);
}

/*
 **************************
 *     MAIN FUNCTIONS     *
 **************************
*/

void createthedog()
{
	/* Graphical */
	Raquet_LoadPPFBank(&ppf_main, "./assets/main.ppf");
  
	Raquet_SetPalette(pal_face, PAL0D, PAL00, PAL20);
	Raquet_SetPalette(pal_logo, PAL20, PAL20, PAL20);
	
	chr_placeface = Raquet_LoadCHR(ppf_main, 0, pal_face);
	int arr_raquetlogo_R[2] = {1, 7};
	int arr_raquetlogo_A[2] = {2, 8};
	int arr_raquetlogo_Q[2] = {3, 9};
	int arr_raquetlogo_U[2] = {4, 10};
	int arr_raquetlogo_E[2] = {5, 11};
	int arr_raquetlogo_T[2] = {6, 12};
	chr_raquetlogo_R = Raquet_LoadCHRMult(ppf_main, arr_raquetlogo_R, 1, 2, pal_logo);
	chr_raquetlogo_A = Raquet_LoadCHRMult(ppf_main, arr_raquetlogo_A, 1, 2, pal_logo);
	chr_raquetlogo_Q = Raquet_LoadCHRMult(ppf_main, arr_raquetlogo_Q, 1, 2, pal_logo);
	chr_raquetlogo_U = Raquet_LoadCHRMult(ppf_main, arr_raquetlogo_U, 1, 2, pal_logo);
	chr_raquetlogo_E = Raquet_LoadCHRMult(ppf_main, arr_raquetlogo_E, 1, 2, pal_logo);
	chr_raquetlogo_T = Raquet_LoadCHRMult(ppf_main, arr_raquetlogo_T, 1, 2, pal_logo);

	initStars();
  	
	/* Actors */
	act_placeface = Raquet_AllocateActor();
	Raquet_CreateActor(act_placeface, chr_placeface);
	act_placeface->origin.x = 4;
	act_placeface->origin.y = 4;

	act_placeface->x = SCREEN_WIDTH/2;
	act_placeface->y = 64;

	act_placeface2 = Raquet_AllocateActor();
	Raquet_CreateActor(act_placeface2, chr_placeface);
	act_placeface2->width = 32;	
	act_placeface2->height = 32;
	act_placeface2->bbox.x2 = act_placeface2->width;
	act_placeface2->bbox.y2 = act_placeface2->height;
	act_placeface2->x = 100;
	act_placeface2->y = 100;

	/* Audio */
	Raquet_Sound snd_placeface = Raquet_LoadSound("./assets/2A03_Kevvviiinnn-Superfusion.wav");
	Raquet_PlaySound(snd_placeface, 0);

}

void runthedog()
{
	demotime++;
  
	// Draw our stuffs
	Raquet_Clear(PAL12); 

	drawStars();
	
	for (int i = 0; i < 6; i++) {
		Raquet_CHR arr[6] = {
			chr_raquetlogo_R, chr_raquetlogo_A, chr_raquetlogo_Q, 
			chr_raquetlogo_U, chr_raquetlogo_E, chr_raquetlogo_T
		};
		Raquet_PlaceCHR(arr[i], ((SCREEN_WIDTH/2) - 24) + (8 * i), ((SCREEN_HEIGHT/2) - 8) + (sin((demotime + (i * 8)) * .1) * 4));
	}
  
	bePlaceface(); 

}

int main(int argc, char *argv[]) {

	Raquet_Main();
	return 0;
	
}
