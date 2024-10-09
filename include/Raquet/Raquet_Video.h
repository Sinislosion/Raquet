/*
 *************************
 *     PPF FUNCTIONS     *
 *************************
 */

// PPF HEADER V1.0
extern const unsigned char PPFHEADER[8];

/*
 * fixed bitmask we use to read CHR data
 * (this is probably sloppy, but im not smart enough to know another way)
 */
extern const unsigned int ppfbitmask[8];

/* Load a PPF bank into memory. More info is in the wiki */
int Raquet_LoadPPFBank(PPF_Bank* targetarray, const char* dir);

Raquet_CHR Raquet_LoadCHR(PPF_Bank ppfbank, int id, Palette pal[4]);

/* Load a multi-tile sprite. More info is in the wiki */
Raquet_CHR Raquet_LoadCHRMult(PPF_Bank ppfbank, int * id, int xwrap, int ywrap, Palette palette[4]);

/* Set a palette variable within runtime */
extern void Raquet_SetPalette(Palette dest[4], Uint32 pal1, Uint32 pal2, Uint32 pal3, Uint32 pal4);

/* Returns a struct of the width and height of the CHR, accessable with x and y */
Raquet_Point Raquet_SizeofCHR(SDL_Texture * tex);

/* Returns the width of a CHR */
int Raquet_WidthofCHR(SDL_Texture * tex);

/* returns the height of a CHR */
int Raquet_HeightofCHR(SDL_Texture * tex);

void Raquet_PlaceCHR(Raquet_CHR chr, int x, int y);

/* Place a CHR sprite, with additional control (chr data, x, y, width in pixels, height in pixels, horizontal flip, vertical flip) */
void Raquet_PlaceCHR_ext(Raquet_CHR chr, int x, int y, int xsize, int ysize, double angle, Raquet_Point center, SDL_RendererFlip flip);

/* Swap a CHR's Palette */ 
void Raquet_SwapCHRPalette(Raquet_CHR* chr, Palette pal[4]);

/* Copy a Palette to another array */
extern void Raquet_CopyPalette(Palette dest[4], Palette origin[4]);

/* Destroy a CHR after we're done using it */
void Raquet_DestroyCHR(SDL_Texture * tex);

void Raquet_DrawPoint(int x, int y, Palette pal, int alpha);

void Raquet_DrawLine(int x1, int y1, int x2, int y2, Palette pal, int alpha);
