#include <math.h>
#include "Raquet.h"

// find angle in rads from 2 points
extern float point_dir(float x1, float y1, float x2, float y2);

// distance to an x point
extern float lendir_x(float dist, float angle);

// distance to a y point
extern float lendir_y(float dist, float angle);

extern int collision_line(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

extern int collision_line_rect(int x1, int y1, int x2, int y2, Actor * act);

extern float lerp(float a, float b, float f);