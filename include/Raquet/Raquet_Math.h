#ifndef RAQUET_MATH_H
#define RAQUET_MATH_H

#include "Raquet.h"
#include <math.h>

// Returns -1 if a number is negative, 0 if 0, and 1 if positive
extern int Raquet_Sign(float comp);

// Returns the smallest number inputted
extern float Raquet_Min(float x, float y);

// Returns the largest number inputted
extern float Raquet_Max(float x, float y);

// its pi
extern float Raquet_PI;

// find angle in rads from 2 points
extern float Raquet_PointDir(float x1, float y1, float x2, float y2);

// distance to an x point
extern float Raquet_LengthDirX(float dist, float angle);

// distance to a y point
extern float Raquet_LengthDirY(float dist, float angle);

// Collision, with LINES
extern int Raquet_CollisionLine(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

// Collision with lines AND an actor
extern int Raquet_CollisionLineActor(int x1, int y1, int x2, int y2, Actor * act);

// Lerp
extern float Raquet_Lerp(float a, float b, float f);

#endif