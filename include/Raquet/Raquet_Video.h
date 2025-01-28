#ifndef RAQUET_VIDEO_H
#define RAQUET_VIDEO_H

/** @file */

/** @brief The header for the PPF format */
extern const unsigned char Raquet_PPFHeader[8];

/** @brief The bitmask used for loading the individual bits from the PPF file */
extern const unsigned int Raquet_PPFBitmask[8];

/**
 * @brief Loads a PPF Bank into a PPF_Bank variable
 * @param targetarray The pointer to the PPF_Bank we want to load the file into
 * @param dir The path to the file
 */
int Raquet_LoadPPFBank(PPF_Bank* targetarray, const char* dir);

/**
 * @brief Loads a CHR texture from a PPF_Bank into a Raquet_CHR
 * @param ppfbank The PPF_Bank to load the texture from
 * @param id The CHR ID to load
 * @param pal The initial palette to use with the texture
 */
Raquet_CHR Raquet_LoadCHR(PPF_Bank ppfbank, int id, Palette pal[4]);

/**
 * @brief Loads a CHR texture from a PPF_Bank into a Raquet_CHR
 * @param ppfbank The PPF_Bank to load the texture from
 * @param id The array of CHR IDs to load ( -1 will load a blank 8x8 tile )
 * @param xwrap The amount of 8x8 tiles on the x axis
 * @param ywrap The amount of 8x8 tiles on the y axis
 * @param pal The initial palette to use with the texture
 */
Raquet_CHR Raquet_LoadCHRMult(PPF_Bank ppfbank, int* id, int xwrap, int ywrap, Palette palette[4]);

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
 * @brief Places a CHR onto the screen with the x and y coordinates specified
 * @param chr The Raquet_CHR to place
 * @param x The Screen X Coordinate to place the texture
 * @param y The Screen Y Coordinate to place the texture
 */
void Raquet_PlaceCHR(Raquet_CHR chr, int x, int y);

/**
 * @brief Places a CHR onto the screen with the x and y coordinates specified
 * @param chr The Raquet_CHR to place
 * @param x The Screen X Coordinate to place the texture
 * @param y The Screen Y Coordinate to place the texture
 * @param xsize The width you wish to draw the texture in pixels
 * @param ysize The height you wish to draw the texture in pixels
 * @param angle The angle to rotate the sprite in radians
 * @param center The Raquet_Point to use as an origin point to rotate and scale the sprite around
 * @param flip The SDL_RendererFlip value of how the sprite should be flipped
 */
void Raquet_PlaceCHR_ext(Raquet_CHR chr, int x, int y, int xsize, int ysize, double angle, Raquet_Point center, SDL_RendererFlip flip);

/**
 * @brief Swaps a CHR's color palette with a different one
 * @param chr The pointer to the Raquet_CHR to modify
 * @param pal The new palette to use with the texture
 */
void Raquet_SwapCHRPalette(Raquet_CHR* chr, Palette pal[4]);

/**
 * @brief Copies a palette array to another palette array
 * @param dest The palette array to copy to
 * @param origin The palette array to copy from
 */
extern void Raquet_CopyPalette(Palette dest[4], Palette origin[4]);

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

/**
 * @brief Frees a Raquet_CHR from memory
 * @param chr The pointer to the CHR you want to destroy
 */
void Raquet_FreeCHR(Raquet_CHR* chr);

#endif
