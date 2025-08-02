#ifndef RAQUET_TYPES_H
#define RAQUET_TYPES_H

/** @file */

#include "Raquet.h"

/** @brief The Camera structure used to control where actors are drawn on the screen */
typedef struct Raquet_Camera {
    float x;    /**< The x position of the Camera */
    float y;    /**< The y position of the Camera */
} Raquet_Camera;

/** @brief The camera used by all of the built-in Raquet functions */
extern Raquet_Camera Camera;

/** @brief Contains an x and y coordinate */
typedef SDL_FPoint Raquet_Point;

/** @brief The 2-bit color container for graphical data. Holds an array of 8x8 tiles */
typedef char * PPF_Bank;

/** @brief RGBA value, each color uses 8 bits */
typedef Uint32 Palette;

/** @brief a CHR contains a texture, width and height data, pixel data, and a copy of its current palette */
typedef struct Raquet_CHR {
    SDL_Texture * tex;      /**< The internal texture data used to draw the CHR onto the screen */
    int width;              /**< The width in pixels of the texture */
    int height;             /**< The height in pixels of the texture */
    int * data;             /**< The cached data of palette indexes */
    Palette palette[4];     /**< The cached Palette */
} Raquet_CHR;

/** @brief The standard AABB collision structure */
typedef struct Raquet_BoundingBox {
    int x1;     /**< The first x position */
    int y1;     /**< The first y position */
    int x2;     /**< The second x position */
    int y2;     /**< The second y position */
}
Raquet_BoundingBox;

/** @brief The "objects" of Raquet. Can hold graphical and logical information */
typedef struct Raquet_Actor {
    Raquet_Point position;  /**< The position in virtual space */

    Raquet_CHR chr;         /**< The Raquet_CHR texture drawn onto the screen */
    Raquet_Point origin;    /**< Our graphic origin point in which we are rotated around */
    int width;              /**< How wide we are in pixels (Defaults to the width of the Raquet_CHR the actor is initialized with) */
    int height;             /**< How tall we are in pixels (Defaults to the height of the Raquet_CHR the actor is initialized with) */
    int angle;              /**< The angle of the object, rorated around its origin point */

    Raquet_BoundingBox bbox;    /**< The Raquet_BoundingBox used to check for collision */

    SDL_FlipMode flip;      /**< How we should be flipped. Uses an SDL_FlipMode value */

} Raquet_Actor;

#endif
