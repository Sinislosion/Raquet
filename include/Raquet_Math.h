#include "Raquet.h"
#include <math.h>

// find angle in rads from 2 points
extern float Raquet_PointDir(float x1, float y1, float x2, float y2);

// distance to an x point
extern float Raquet_LengthDirX(float dist, float angle);

// distance to a y point
extern float Raquet_LengthDirY(float dist, float angle);

extern int Raquet_CollisionLine(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

extern int Raquet_CollisionLineRect(int x1, int y1, int x2, int y2, Actor * act);

extern float Raquet_Lerp(float a, float b, float f);