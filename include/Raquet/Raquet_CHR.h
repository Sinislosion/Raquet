#ifndef RAQUET_CHR_H
#define RAQUET_CHR_H

/** @file */

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
void Raquet_PlaceCHR_ext(Raquet_CHR chr, int x, int y, int xsize, int ysize, double angle, Raquet_Point center, SDL_FlipMode flip);

/**
 * @brief Swaps a CHR's color palette with a different one
 * @param chr The pointer to the Raquet_CHR to modify
 * @param pal The new palette to use with the texture
 */
void Raquet_SwapCHRPalette(Raquet_CHR* chr, Palette pal[4]);

/**
 * @brief Allocates the memory for a CHR
 * @returns The allocated memory for the CHR
 */
Raquet_CHR * Raquet_AllocateCHR(void);

/**
 * @brief Frees a Raquet_CHR from memory
 * @param chr The pointer to the CHR you want to destroy
 */
void Raquet_FreeCHR(Raquet_CHR* chr);

#endif

