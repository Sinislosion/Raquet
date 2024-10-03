#include "Raquet_Math.h"

int Raquet_Sign(float comp) {
	return (0 < comp) - (comp < 0);
}

float Raquet_Min(float x, float y) {
	return (((x) < (y)) ? (x) : (y));
}

float Raquet_Max(float x, float y) {
    return (((x) > (y)) ? (x) : (y));
}

float Raquet_PI = 3.1415926535;

float Raquet_PointDir(float x1, float y1, float x2, float y2) {
    const float dx = x2 - x1;
    const float dy = y2 - y1;
    return atan2(dy, dx);
}

float Raquet_LengthDirX(float dist, float angle) {
    return dist * cos(angle);
}

float Raquet_LengthDirY(float dist, float angle) {
    return dist * sin(angle);
}

int Raquet_CollisionLine(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {

    const float check1 = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3);
    const float check2 = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
    const float check3 = (x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3);
    const float check4 = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);

    const float line1 = (check1 == 0 && check2 == 0) ? -1 : (check1 / check2);
    const float line2 = (check3 == 0 && check4 == 0) ? -1 : (check3 / check4);

    return (line1 >= 0 && line1 <= 1 && line2 >= 0 && line2 <= 1);

}

int Raquet_CollisionLineActor(int x1, int y1, int x2, int y2, Actor * act) {
    const int rx = (act -> position.x - act -> origin.x) + act -> bbox.x1;
    const int ry = (act -> position.y - act -> origin.y) + act -> bbox.y1;
    const int rw = act -> bbox.x2 - act -> bbox.x1;
    const int rh = act -> bbox.y2 - act -> bbox.y1;

    return Raquet_CollisionLine(x1, y1, x2, y2, rx, ry, rx, ry + rh) ||
        Raquet_CollisionLine(x1, y1, x2, y2, rx + rw, ry, rx + rw, ry + rh) ||
        Raquet_CollisionLine(x1, y1, x2, y2, rx, ry, rx + rw, ry) ||
        Raquet_CollisionLine(x1, y1, x2, y2, rx, ry + rh, rx + rw, ry + rh);

}

float lerp(float a, float b, float f) {
    return a + f * (b - a);
}