#include "Raquet_Input.h"

/*
 ***************************
 *     INPUT FUNCTIONS     *
 ***************************
 */

const uint8_t * Raquet_SDLKeys;
uint8_t Raquet_PrevSDLKeys[SDL_NUM_SCANCODES];
uint8_t Raquet_SDLMouse;
uint8_t Raquet_PrevSDLMouse;

/* Check if this key is being held down */
int Raquet_KeyCheck(unsigned int key) {
	return Raquet_SDLKeys[key];
}

/* Will only return 1 for the first frame the key is being held down */
int Raquet_KeyCheck_Pressed(unsigned int key) {
	return (Raquet_PrevSDLKeys[key] != Raquet_SDLKeys[key] && Raquet_SDLKeys[key] != 0);
}

/* Will only return 1 if the key has been released for 1 frame */
int Raquet_KeyCheck_Released(unsigned int key) {
	return (Raquet_PrevSDLKeys[key] != Raquet_SDLKeys[key] && Raquet_SDLKeys[key] != 1);
}

/* Check if this mouse button is being held down */
int Raquet_MouseCheck(unsigned int mouse_button) {
	return mouse_button & Raquet_SDLMouse;
}

/* Will only return 1 for the first frame the mouse button is being held down */
int Raquet_MouseCheck_Pressed(unsigned int mouse_button) {
	return (Raquet_PrevSDLMouse & mouse_button) != (Raquet_SDLMouse & mouse_button) && (Raquet_SDLMouse & mouse_button) != 0;
}

/* Will only return 1 if the mouse button has been released for 1 frame */
int Raquet_MouseCheck_Released(unsigned int mouse_button) {
	return (Raquet_PrevSDLMouse & mouse_button) != (Raquet_SDLMouse & mouse_button) && (Raquet_SDLMouse & mouse_button) != 1;
}
