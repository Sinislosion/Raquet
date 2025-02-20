#ifndef RAQUET_VIDEO_H
#define RAQUET_VIDEO_H

/** @file */

/**
 * @brief Initializes a palette during runtime in a single funciton
 * @param dest The palette to load the colors into
 * @param pal1 The first palette value to load into the array
 * @param pal2 The second palette value to load into the array
 * @param pal3 The third palette value to load into the array
 * @param pal4 The fourth palette value to load into the array
 */
extern void Raquet_SetPalette(Palette dest[4], Uint32 pal1, Uint32 pal2, Uint32 pal3, Uint32 pal4);

/**
 * @brief Copies a palette array to another palette array
 * @param dest The palette array to copy to
 * @param origin The palette array to copy from
 */
extern void Raquet_CopyPalette(Palette dest[4], Palette origin[4]);

/**
 * @brief Wrapper for SDL's Render Set Draw Color, that converts a Raquet Palette Value to an RGBA8 value
 * @param pal The palette color to set the Draw Color to
 * @param alpha The opacity of the color ( 0 to 255 )
 */
extern void Raquet_SetDrawColor(Palette pal, int alpha);

/* Clear the screen with a solid color */
#ifdef BACKGROUND_CLEARING_COLOR
    extern Palette Raquet_ClearColor;
#endif

/**
 * @brief Clears the screen with a specified color
 * @param pal The Palette color to clear the screen with.
 */
extern void Raquet_Clear(Palette pal);

/**
 * @brief Draws a rectangle of a given color onto the screen
 * @param x The X position of the rectangle in Screen space
 * @param y The Y position of the rectangle in Screen space
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @param pal The Palette color of the rectangle
 * @param alpha The transparecny of the rectangle, with 255 being the max and 0 being the minimum
 * @param fill Whether or not to fill in the rectangle (1 or yes, 0 for no)
 */
extern void Raquet_DrawRectangle(int x1, int y1, int width, int height, Palette pal, int alpha, int fill);

/**
 * @brief Draws a single pixel onto the specified coordinates
 * @param x The screen X coordinate to draw the pixel to
 * @param y The screen Y coordinate to draw the pixel to
 * @param pal The palatte color to draw the pixel as
 * @param alpha The transpareny of the pixel
 */
void Raquet_DrawPoint(int x, int y, Palette pal, int alpha);

/**
 * @brief Draws a line onto the screen
 * @param x1 The first X coordinate
 * @param y1 The first Y coordinate
 * @param x2 The second X coordinate
 * @param y2 The second Y coordinate
 * @param pal palette color to draw the line as
 * @param alpha The transparency of the line
 */
void Raquet_DrawLine(int x1, int y1, int x2, int y2, Palette pal, int alpha);

#endif
