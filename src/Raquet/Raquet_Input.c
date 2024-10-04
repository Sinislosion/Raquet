#include "Raquet_Input.h"

/*
 ***************************
 *     INPUT FUNCTIONS     *
 ***************************
 */

const Uint8 * sdlkeys;
uint8_t sdlmouse;
uint8_t prevmouse;
uint8_t prevkeys[SDL_NUM_SCANCODES];

/* Check if this key is being held down */
int Raquet_KeyCheck(unsigned int key) {
	return sdlkeys[key];
}

/* Will only return 1 for the first frame the key is being held down */
int Raquet_KeyCheck_Pressed(unsigned int key) {
	return (prevkeys[key] != sdlkeys[key] && sdlkeys[key] != 0);
}

/* Will only return 1 if the key has been released for 1 frame */
int Raquet_KeyCheck_Released(unsigned int key) {
	return (prevkeys[key] != sdlkeys[key] && sdlkeys[key] != 1);
}

/* Check if this mouse button is being held down */
int Raquet_MouseCheck(unsigned int sdlbutton) {
	return sdlbutton & sdlmouse;
}

/* Will only return 1 for the first frame the mouse button is being held down */
int Raquet_MouseCheck_Pressed(unsigned int sdlbutton) {
	return (prevmouse & sdlbutton) != (sdlmouse & sdlbutton) && (sdlmouse & sdlbutton) != 0;
}

/* Will only return 1 if the mouse button has been released for 1 frame */
int Raquet_MouseCheck_Released(unsigned int sdlbutton) {
	return (prevmouse & sdlbutton) != (sdlmouse & sdlbutton) && (sdlmouse & sdlbutton) != 1;
}
