#include <math.h>

// find angle in rads from 2 points
float point_dir(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float angle = atan2(dy, dx);
    return angle;
}

// distance to an x point
float lendir_x(float dist, float angle) {
    return dist * cos(angle);
}

// distance to a y point
float lendir_y(float dist, float angle) {
    return dist * sin(angle);
}

int collision_line(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    float line1;
    float line2;

    float check1 = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3);
    float check2 = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
    float check3 = (x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3);
    float check4 = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);

    if (check1 == 0 || check2 == 0) {
        line1 = -1; // set to undefined
    } else {
        line1 = (check1 / check2);
    }

    if (check3 == 0 || check4 == 0) {
        line2 = -1; // set to undefined
    } else {
        line2 = (check3 / check4);
    }
    return (line1 >= 0 && line1 <= 1 && line2 >= 0 && line2 <= 1);

}

int collision_line_rect(int x1, int y1, int x2, int y2, Actor * act) {
    int rx = (act -> x - act -> origin.x) + act -> bbox.x1;
    int ry = (act -> y - act -> origin.y) + act -> bbox.y1;
    int rw = act -> bbox.x2 - act -> bbox.x1;
    int rh = act -> bbox.y2 - act -> bbox.y1;

    return collision_line(x1, y1, x2, y2, rx, ry, rx, ry + rh) ||
        collision_line(x1, y1, x2, y2, rx + rw, ry, rx + rw, ry + rh) ||
        collision_line(x1, y1, x2, y2, rx, ry, rx + rw, ry) ||
        collision_line(x1, y1, x2, y2, rx, ry + rh, rx + rw, ry + rh);

}

float lerp(float a, float b, float f) {
    return a + f * (b - a);
}
