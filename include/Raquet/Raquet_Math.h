#ifndef RAQUET_MATH_H
#define RAQUET_MATH_H

/** @file */

#include "Raquet.h"
#include <math.h>

/**
 * @brief Returns -1 if a number is negative, 0 if 0, and 1 if positive
 * @param comp The value to get the sign from
 */
extern int Raquet_Sign(float comp);

/**
 * @brief Returns the smallest number inputted
 * @param x The first value to check
 * @param y The second value to check
 */
extern float Raquet_Min(float x, float y);

/**
 * @brief Returns the largest number inputted
 * @param x The first value to check
 * @param y The second value to check
 */
extern float Raquet_Max(float x, float y);

/**
 * @brief It's PI.
 */
extern float Raquet_PI;

/**
 * @brief Returns the angle from 2 points in radians
 * @param x1 The x value of the first point
 * @param y1 The y value of the first point
 * @param x2 The x value of the second point
 * @param y2 The y value of the second point
 */
extern float Raquet_PointDir(float x1, float y1, float x2, float y2);

/**
 * @brief Returns the length on the X axis in a given direction
 * @param dist The distance to move in the direction
 * @param angle The direction in radians
 */
extern float Raquet_LengthDirX(float dist, float angle);

/**
 * @brief Returns the length on the Y axis in a given direction
 * @param dist The distance to move in the direction
 * @param angle The direction in radians
 */
extern float Raquet_LengthDirY(float dist, float angle);

/**
 * @brief Returns 1 if the 2 specified lines are colliding
 * @param x1 The first x value of the first line
 * @param y1 The first y value of the first line
 * @param x2 The second x value of the first line
 * @param y2 The second y value of the first line
 * @param x3 The first x value of the second line
 * @param y3 The first y value of the second line
 * @param x4 The second x value of the second line
 * @param y4 The second y value of the second line
 */
extern int Raquet_CollisionLine(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

/**
 * @brief Returns 1 if the line is colliding with the specified actor
 * @param x1 The first x value of the line
 * @param y1 The first y value of the line
 * @param x2 The second x value of the line
 * @param y2 The second y value of the line
 * @param act The Actor to check
 */
extern int Raquet_CollisionLineActor(int x1, int y1, int x2, int y2, Actor * act);

/**
 * @brief It's lerp, Linear inerpolation
 * @param a from
 * @param b to
 * @param f amount
 */
extern float Raquet_Lerp(float a, float b, float f);

#endif
