#ifndef RAQUET_INPUT_H
#define RAQUET_INPUT_H

#include "Raquet.h"

extern const uint8_t * sdlkeys;
extern uint8_t prevkeys[SDL_NUM_SCANCODES];

extern uint8_t sdlmouse;
extern uint8_t prevmouse;

#define RAQUET_MOUSE_RIGHT      4
#define RAQUET_MOUSE_LEFT       1
#define RAQUET_MOUSE_MIDDLE     2

/* Check if this key is being held down */
extern int Raquet_KeyCheck(unsigned int nkey);

/* Will only return 1 for the first frame the key is being held down */
extern int Raquet_KeyCheck_Pressed(unsigned int nkey);

/* Will only return 1 if the key has been released for 1 frame */
extern int Raquet_KeyCheck_Released(unsigned int nkey);

/* Check if this mouse button is being held down */
extern int Raquet_MouseCheck(unsigned int sdlbutton);

/* Will only return 1 for the first frame the mouse button is being held down */
extern int Raquet_MouseCheck_Pressed(unsigned int sdlbutton);

/* Will only return 1 if the mouse button has been released for 1 frame */
extern int Raquet_MouseCheck_Released(unsigned int sdlbutton);

#endif