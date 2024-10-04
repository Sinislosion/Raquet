#include "Raquet.h"

/* Define the camera*/
Raquet_Camera Camera = {0, 0};

/* PPF HEADER V1.0 */
const unsigned char PPFHEADER[8] = {
    0x50,
    0x50,
    0x46,
    0x76,
    0x01,
    0x00,
    0x00,
    0x00
};

/*
 * fixed bitmask we use to read CHR data
 * (this is probably sloppy, but im not smart enough to know another way)
 */
const unsigned int ppfbitmask[8] = {
    0x80,
    0x40,
    0x20,
    0x10,
    0x08,
    0x04,
    0x02,
    0x01
};

/* Load a PPF bank into memory. More info is in the wiki */
int Raquet_LoadPPFBank(PPF_Bank* targetarray, const char* dir) {

	// check if ppf data is a valid directory
	if (SDL_RWFromFile(dir, "rb") != NULL) {
        SDL_RWops* ppfdata = SDL_RWFromFile(dir, "rb");
		long long sizeoffile = SDL_RWseek(ppfdata, 0, RW_SEEK_END);
		SDL_RWseek(ppfdata, 0, RW_SEEK_SET);
		*targetarray = (char*)malloc(sizeoffile * sizeof(char));

		SDL_RWread(ppfdata, *targetarray, 8, 1024);

		for (int i = 0; i < 8; i++) {
			if ((*targetarray)[i] != PPFHEADER[i]) {
                #ifdef PRINT_DEBUG
				    printf("WARNING: HEADER DATA DOES NOT MATCH\n");
				    fflush(stdout);
                #endif
			}

		}

		SDL_RWclose(ppfdata);

        #ifdef PRINT_DEBUG
		    printf("Loaded PPF Data at: %s successfully\n", dir);
		    fflush(stdout);
		#endif

        return 1;

	} else {

        #ifdef PRINT_DEBUG
		    printf("Failed to load PPF at: %s\n", dir);
            fflush(stdout);
        #endif

        exit(1);
		return 0;
	}
}

/* Load a single-tile sprite. More info is in the wiki */

Raquet_CHR Raquet_LoadCHR(PPF_Bank ppfbank, int id, Palette pal[3]) {

    Raquet_CHR ret;

    ret.tex = SDL_CreateTexture(
        gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 8, 8);

    ret.data = (int*)malloc(sizeof(int) * 64);
    ret.width = 8;
    ret.height = 8;

    ret.palette[0] = pal[0];
    ret.palette[1] = pal[1];
    ret.palette[2] = pal[2];

    Uint32 pixels[64];

    /*
     * For every y row, write a pixel that corresponds to the palette
     * If our first palette index is 0, and our second palette index is 0, write transparency
     * If our first palette index is 1, and our second palette index is 0, write color 1
     * If our first palette index is 0, and our second palette index is 1, write color 2
     * If our first palette index is 1, and our second palette index is 1, write color 3
     */

    if (id == -1) // if our tile is transparency, make it transparent
    {
        Uint32 swag = 0x00000000;
        memset(pixels, swag, 64 * sizeof(Uint32));
        SDL_SetTextureBlendMode(ret.tex, SDL_BLENDMODE_BLEND);
        SDL_UpdateTexture(ret.tex, NULL, pixels, 8 * sizeof(Palette));
        return ret;
    }

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int dest = x + (y * 8); // dest is where in our array of pixels we will place the color
            int index = y + 8 + (id * 16); // index is the byte in the file we're reading for palette data
            int index2 = y + 16 + (id * 16); // index2 is the second byte in the file we read for palette data

            int check1 = ((ppfbank[index] & ppfbitmask[x]) > 0);
            int check2 = ((ppfbank[index2] & ppfbitmask[x]) > 0);
            int place = check1 + check2;

            switch (place) {
            case 0:
                pixels[dest] = 0x00000000;
                ret.data[dest] = 0;
                break;

            case 1:
                switch (check1) {
                case 1:
                    pixels[dest] = pal[0];
                    ret.data[dest] = 1;
                    break;

                default:
                    pixels[dest] = pal[1];
                    ret.data[dest] = 2;
                    break;
                }
                break;

            case 2:
                pixels[dest] = pal[2];
                ret.data[dest] = 3;
                break;
            }

        }

    }

    SDL_SetTextureBlendMode(ret.tex, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(ret.tex, NULL, pixels, 8 * sizeof(Palette));

    return ret;
}

/* Set a palette variable within runtime */
void Raquet_SetPalette(Palette dest[3], Uint32 pal1, Uint32 pal2, Uint32 pal3) {
    dest[0] = pal1;
    dest[1] = pal2;
    dest[2] = pal3;
}

void Raquet_CopyPalette(Palette dest[3], Palette origin[3]) {
    dest[0] = origin[0];
    dest[1] = origin[1];
    dest[2] = origin[2];
}

/* I'm actually not sure what you can use this for yet, but you can use it */
void Raquet_SetDrawColor(Palette pal, int alpha) {
    Uint32 palr = (pal >> 24) & 0x000000FF;
    Uint32 palg = (pal >> 16) & 0x000000FF;
    Uint32 palb = (pal >> 8) & 0x000000FF;
    SDL_SetRenderDrawColor(gRenderer, palr, palg, palb, alpha);
}

/* Clear the screen with a solid color */
#ifdef BACKGROUND_CLEARING_COLOR
    Palette gClearColor;
#endif

void Raquet_Clear(Palette pal) {
    Raquet_SetDrawColor(pal, 255);
    SDL_RenderFillRect(gRenderer, NULL);

    #ifdef BACKGROUND_CLEARING_COLOR
        gClearColor = pal;
    #endif

}

/* Draw a rectangle (x position, y position, width, height, color, alpha, fill) */
void Raquet_DrawRectangle(int x1, int y1, int width, int height, Palette pal, int alpha, int fill) {
    SDL_Rect rect = {
        x1,
        y1,
        width,
        height
    };
    Raquet_SetDrawColor(pal, alpha);

    switch (fill) {
    default:
        SDL_RenderDrawRect(gRenderer, & rect);
        break;

    case 1:
        SDL_RenderFillRect(gRenderer, & rect);
        break;
    }

}

/* Load a multi-tile sprite. More info is in the wiki */
Raquet_CHR Raquet_LoadCHRMult(PPF_Bank ppfbank, int * id, int xwrap, int ywrap, Palette palette[3]) {
    Raquet_CHR ret;

    ret.width = xwrap * 8;
    ret.height = ywrap * 8;

    ret.tex = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, ret.width, ret.height);



    ret.data = (int*)malloc(sizeof(int) * (ret.width * ret.height));

    Uint32 pixels[ret.width * ret.height];

    /*
     * For every y row, write a pixel that corresponds to the palette
     * If our first palette index is 0, and our second palette index is 0, write transparency
     * If our first palette index is 1, and our second palette index is 0, write color 1
     * If our first palette index is 0, and our second palette index is 1, write color 2
     * If our first palette index is 1, and our second palette index is 1, write color 3
     */

    for (int chrcounty = 0; chrcounty < ywrap; chrcounty++) {
        for (int y = 0; y < 8; y++) {
            for (int chrcountx = 0; chrcountx < xwrap; chrcountx++) {
                for (int x = 0; x < 8; x++) {
                    int dest = x + (y * ret.width) + (chrcountx * 8) + (chrcounty * (ret.width * 8));
                    int curid = chrcountx + (chrcounty * xwrap);
                    int index = y + 8 + (id[curid] * 16);
                    int index2 = y + 16 + (id[curid] * 16);

                    int check1 = ((ppfbank[index] & ppfbitmask[x]) > 0);
                    int check2 = ((ppfbank[index2] & ppfbitmask[x]) > 0);
                    int place = check1 + check2;
                    if (id[curid] < 0) {
                        pixels[dest] = 0x00000000; // if our tile is transparency, make it transparent.
                        ret.data[dest] = 0;
                    } else {
                        switch (place) {
                        case 0:
                            pixels[dest] = 0x00000000;
                            ret.data[dest] = 0;
                            break;
                        case 1:
                            switch (check1) {
                            case 1:
                                pixels[dest] = palette[0];
                                ret.data[dest] = 1;
                                break;

                            default:
                                pixels[dest] = palette[1];
                                ret.data[dest] = 2;
                                break;
                            }
                            break;

                        case 2:
                            pixels[dest] = palette[2];
                            ret.data[dest] = 3;
                            break;
                        }
                    }
                }
            }
        }
    }

    SDL_SetTextureBlendMode(ret.tex, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(ret.tex, NULL, pixels, ret.width * sizeof(Palette));

    return ret;

}

/* Returns a struct of the width and height of the CHR, accessable with x and y */
Raquet_Point Raquet_SizeofCHR(SDL_Texture * tex) {
    Raquet_Point size;
    SDL_QueryTexture(tex, NULL, NULL, & size.x, & size.y);
    return size;
}

/* Returns the width of a CHR */
int Raquet_WidthofCHR(SDL_Texture * tex) {
    Raquet_Point size;
    SDL_QueryTexture(tex, NULL, NULL, & size.x, & size.y);
    return size.x;
}

/* returns the height of a CHR */
int Raquet_HeightofCHR(SDL_Texture * tex) {
    Raquet_Point size;
    SDL_QueryTexture(tex, NULL, NULL, & size.x, & size.y);
    return size.y;
}

void Raquet_PlaceCHR(Raquet_CHR chr, int x, int y) {
    SDL_Point size = Raquet_SizeofCHR(chr.tex);
    SDL_Rect dstrect = {
        x,
        y,
        size.x,
        size.y
    };
    SDL_RenderCopy(gRenderer, chr.tex, NULL, & dstrect);
}

/* Place a CHR sprite, with additional control (chr data, x, y, width in pixels, height in pixels, horizontal flip, vertical flip) */
void Raquet_PlaceCHR_ext(Raquet_CHR chr, int x, int y, int xsize, int ysize, double angle, Raquet_Point center, SDL_RendererFlip flip) {
    SDL_Rect dstrect = {
        x - center.x,
        y - center.y,
        xsize,
        ysize
    };
    SDL_RenderCopyEx(gRenderer, chr.tex, NULL, & dstrect, angle, & center, flip);
}

/* Swap a CHR's Palette */ 
void Raquet_SwapCHRPalette(Raquet_CHR* chr, Palette pal[3]) {

    if (memcmp(chr->palette, pal, sizeof(Palette[3])) == 0) {
        return;
    }

    Palette pixels[chr->width * chr->height];

    chr->palette[0] = pal[0];
    chr->palette[1] = pal[1];
    chr->palette[2] = pal[2];

    for (int y = 0; y < chr->height; y++) {
        for (int x = 0; x < chr->width; x++) {
            int dest = (y * chr->width) + x;
            switch (chr->data[dest]) {
                case 0:
                    pixels[dest] = 0x00000000;
                    break;

                default:
                    pixels[dest] = pal[chr->data[dest] - 1];
                    break;
           } 

        }
    }
    SDL_UpdateTexture(chr->tex, NULL, pixels, chr->width * sizeof(Palette));

}

/* Destroy a CHR after we're done using it */
void Raquet_DestroyCHR(SDL_Texture * tex) {
    SDL_DestroyTexture(tex);
}

void Raquet_DrawPoint(Palette pal, int x, int y, int alpha) {
    Raquet_SetDrawColor(pal, alpha);
    SDL_RenderDrawPoint(gRenderer, x, y);
}

void Raquet_DrawLine(Palette pal, int x1, int y1, int x2, int y2, int alpha) {
    Raquet_SetDrawColor(pal, alpha);
    SDL_RenderDrawLine(gRenderer, x1, y1, x2, y2);
}
