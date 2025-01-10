#ifndef RAQUET_ACTOR_H
#define RAQUET_ACTOR_H

/** @file */

#include "Raquet_Types.h"

/**
 * @brief Allocates the memory for an Actor
 * @returns The allocated memory for the Actor
 */
Raquet_Actor * Raquet_AllocateActor(void);

/**
 * @brief Initializes an Actor along with a texture
 * @param act The Actor to initialize
 * @param chr The CHR texture to initialize the Actor with
 */
void Raquet_CreateActor(Raquet_Actor * act, Raquet_CHR chr);

/**
 * @brief Draws an Actor onto the screen
 * @param act The Actor to draw
 */
void Raquet_DrawActor(Raquet_Actor * act);

/**
 * @brief Checks if 2 actors are colliding
 * @param x The x coordinate to check with the bounding box of the first Actor
 * @param y The y coordinate to check with the bounding box of the first Actor
 * @param act1 The first actor to check
 * @param act2 The second actor to check
 */
int Raquet_ActorColliding(int x, int y, Raquet_Actor * act1, Raquet_Actor * act2);

#endif
