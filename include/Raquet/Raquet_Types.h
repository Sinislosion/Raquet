#ifndef RAQUET_TYPES_H
#define RAQUET_TYPES_H

#include "Raquet.h"

/* Raquet_Point contains an x and a y variable */
typedef SDL_Point Raquet_Point;

/* PPF_Bank contains a whole bunch of CHRs */
typedef char * PPF_Bank;

/* RGBA value */
typedef Uint32 Palette;

/* a CHR contains a texture, width and height data, pixel data, and a copy of its current palette */
typedef struct Raquet_CHR {
    SDL_Texture * tex;
    int width;
    int height;
    int * data;
    Palette palette[3];
} Raquet_CHR;

/* Bounding Boxes */
typedef struct Raquet_BoundingBox {
    int x1;
    int y1;
    int x2;
    int y2;
}
Raquet_BoundingBox;

/* Actors */
typedef struct Actor {
    // where we are in virtual space
    // can be accessed with position.x or position.y
    Raquet_Point position;

    // how we're displayed
    Raquet_CHR chr; // Current CHR
    Raquet_Point origin; // Our Orgigin Point (x, y) default is (0, 0)
    int width; // How wide we are (default is the width of the sprite)
    int height; // How tall we are (default is the height of the sprite)
    int angle; // angle of the object, rotated around its origin

    Palette color; // used in very specific circumstances (barriers in B-TRON)

    // collision info
    Raquet_BoundingBox bbox; // Bounding box

    SDL_RendererFlip flip;

} Actor;

#endif