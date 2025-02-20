// put your libs here.
#include "Raquet.h"

/*
 ********************
 *     PPF BANK     *
 ********************
*/
PPF_Bank ppf_main;

/*
 **************************
 *     MAIN FUNCTIONS     *
 **************************
*/
void createthedog(void)
{
	/* Graphical */
	Raquet_LoadPPFBank(&ppf_main, Raquet_AbsoluteToAsset("main.ppf"));

	/* Audio */
	Raquet_Sound snd_placeface = Raquet_LoadSound(Raquet_AbsoluteToAsset("2A03_Kevvviiinnn-Superfusion.wav"));
	Raquet_PlaySound(snd_placeface, 0);

}

void runthedog(void) {
    Raquet_Clear(Raquet_GlobalPalette[0x0D]);
}

int main(void) {
	Raquet_Main();
	return 0;
}
