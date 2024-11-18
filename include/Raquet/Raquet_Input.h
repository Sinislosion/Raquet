#ifndef RAQUET_INPUT_H
#define RAQUET_INPUT_H

/** @file */

#include "Raquet.h"

extern const uint8_t * Raquet_SDLKeys;
extern uint8_t Raquet_PrevSDLKeys[SDL_NUM_SCANCODES];

extern uint8_t Raquet_SDLMouse;
extern uint8_t Raquet_PrevSDLMouse;

#define RAQUET_MOUSE_RIGHT      4
#define RAQUET_MOUSE_LEFT       1
#define RAQUET_MOUSE_MIDDLE     2

/**
 * @brief Returns if the provided key is currently being pressed
 * @param key The SDL_SCANCODE value to check for
 * @returns 1 if true, 0 if false
 */
extern int Raquet_KeyCheck(unsigned int key);

/**
 * @brief Returns if the provided key has just been pressed
 * @param key The SDL_SCANCODE value to check for
 * @returns 1 if true, 0 if false
 */
extern int Raquet_KeyCheck_Pressed(unsigned int key);

/**
 * @brief Returns if the provided key has just been released
 * @param key The SDL_SCANCODE value to check for
 * @returns 1 if true, 0 if false
 */
extern int Raquet_KeyCheck_Released(unsigned int key);

/**
 * @brief Returns if the provided mouse button is currently being pressed
 * @param mouse_button The RAQUET_MOUSE value to check for
 * @returns 1 if true, 0 if false
 */
extern int Raquet_MouseCheck(unsigned int mouse_button);

/**
 * @brief Returns if the provided mouse button has just been pressed
 * @param mouse_button The RAQUET_MOUSE value to check for
 * @returns 1 if true, 0 if false
 */
extern int Raquet_MouseCheck_Pressed(unsigned int mouse_button);

/**
 * @brief Returns if the provided mouse button has just been released
 * @param mouse_button The RAQUET_MOUSE value to check for
 * @returns 1 if true, 0 if false
 */
extern int Raquet_MouseCheck_Released(unsigned int mouse_button);

#endif
