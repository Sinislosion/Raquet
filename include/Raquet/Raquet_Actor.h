#ifndef RAQUET_ACTOR_H
#define RAQUET_ACTOR_H

#include "Raquet_Types.h"

Actor * Raquet_AllocateActor(void);

void Raquet_CreateActor(Actor * act, Raquet_CHR chr);

void Raquet_DestroyActor(Actor * act);

void Raquet_DrawActor(Actor * act);

int Raquet_ActorColliding(int x, int y, Actor * act1, Actor * act2);

#endif
