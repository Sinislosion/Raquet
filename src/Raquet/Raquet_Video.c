#include "Raquet.h"

/* Define the camera*/
Raquet_Camera Camera = {0, 0};

/* PPF HEADER V1.0 */
const unsigned char Raquet_PPFHeader[8] = {
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
const unsigned int Raquet_PPFBitmask[8] = {
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
            if ((*targetarray)[i] != Raquet_PPFHeader[i]) {
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

Raquet_CHR Raquet_LoadCHR(PPF_Bank ppfbank, int id, Palette pal[4]) {

    Raquet_CHR ret;

    ret.tex = SDL_CreateTexture(Raquet_Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 8, 8);
    ret.data = (int*)malloc(sizeof(int) * 64);
    ret.width = 8;
    ret.height = 8;

    for (uint8_t i = 0; i < 4; i++) {
        ret.palette[i] = pal[i];
    }

    Uint32 pixels[64];

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
            uint8_t v1, v2, v3;
            int dest = x + (y * 8);
            v1 = (ppfbank[y + 8 + (id * 16)] & Raquet_PPFBitmask[x]) >> (7 - x);
            v2 = (ppfbank[y + 16 + (id * 16)] & Raquet_PPFBitmask[x]) >> (7 - x);
            v3 = v1 + v2 == 2 ? 3 : (v1 == 1 ? 1 : (v2 == 1 ? 2 : 0));
            pixels[dest] = pal[v3];
            ret.data[dest] = v3;
        }
    }

    SDL_SetTextureBlendMode(ret.tex, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(ret.tex, NULL, pixels, 8 * sizeof(Palette));

    return ret;
}

/* Load a multi-tile sprite. More info is in the wiki */
Raquet_CHR Raquet_LoadCHRMult(PPF_Bank ppfbank, int * id, int xwrap, int ywrap, Palette pal[4]) {

    Raquet_CHR ret;

    ret.width = xwrap * 8;
    ret.height = ywrap * 8;

    ret.tex = SDL_CreateTexture(Raquet_Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, ret.width, ret.height);

    ret.data = (int*)malloc(sizeof(int) * (ret.width * ret.height));

    for (uint8_t i = 0; i < 4; i++) {
        ret.palette[i] = pal[i];
    }

    Palette* pixels = (Palette*)malloc(sizeof(Palette) * (ret.width * ret.height));

    for (int chrcounty = 0; chrcounty < ywrap; chrcounty++) {
        for (int y = 0; y < 8; y++) {
            for (int chrcountx = 0; chrcountx < xwrap; chrcountx++) {
                for (int x = 0; x < 8; x++) {
                    int dest = x + (y * ret.width) + (chrcountx * 8) + (chrcounty * (ret.width * 8));
                    int curid = chrcountx + (chrcounty * xwrap);
                    if (id[curid] >= 0) {
                        const uint8_t v1 = (ppfbank[y + 8 + (id[curid] * 16)] & Raquet_PPFBitmask[x]) >> (7 - x);
                        const uint8_t v2 = (ppfbank[y + 16 + (id[curid] * 16)] & Raquet_PPFBitmask[x]) >> (7 - x);
                        const uint8_t v3 = v1 + v2 == 2 ? 3 : (v1 == 1 ? 1 : (v2 == 1 ? 2 : 0));
                        pixels[dest] = pal[v3];
                        ret.data[dest] = v3;
                    } else if (id[curid] < 0){
                        pixels[dest] = pal[0];
                        ret.data[dest] = 0;
                    }
                }
            }
        }
    }

    SDL_SetTextureBlendMode(ret.tex, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(ret.tex, NULL, pixels, ret.width * sizeof(Palette));

    return ret;

}

/* Set a palette variable within runtime */
void Raquet_SetPalette(Palette dest[4], Uint32 pal1, Uint32 pal2, Uint32 pal3, Uint32 pal4) {
    dest[0] = pal1;
    dest[1] = pal2;
    dest[2] = pal3;
    dest[3] = pal4;
}

void Raquet_CopyPalette(Palette dest[4], Palette origin[4]) {
    dest[0] = origin[0];
    dest[1] = origin[1];
    dest[2] = origin[2];
    dest[3] = origin[3];
}

/* I'm actually not sure what you can use this for yet, but you can use it */
void Raquet_SetDrawColor(Palette pal, int alpha) {
    Uint32 palr = (pal >> 24) & 0x000000FF;
    Uint32 palg = (pal >> 16) & 0x000000FF;
    Uint32 palb = (pal >> 8) & 0x000000FF;
    SDL_SetRenderDrawColor(Raquet_Renderer, palr, palg, palb, alpha);
}

/* Clear the screen with a solid color */
#ifdef BACKGROUND_CLEARING_COLOR
    Palette gClearColor;
#endif

void Raquet_Clear(Palette pal) {
    Raquet_SetDrawColor(pal, 255);
    SDL_RenderFillRect(Raquet_Renderer, NULL);

    #ifdef BACKGROUND_CLEARING_COLOR
        Raquet_ClearColor = pal;
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
        SDL_RenderDrawRect(Raquet_Renderer, & rect);
        break;

    case 1:
        SDL_RenderFillRect(Raquet_Renderer, & rect);
        break;
    }

}

void Raquet_PlaceCHR(Raquet_CHR chr, int x, int y) {
    SDL_Point size = { chr.width, chr.height };
    SDL_Rect dstrect = {
        x,
        y,
        size.x,
        size.y
    };
    SDL_RenderCopy(Raquet_Renderer, chr.tex, NULL, & dstrect);
}

/* Place a CHR sprite, with additional control (chr data, x, y, width in pixels, height in pixels, horizontal flip, vertical flip) */
void Raquet_PlaceCHR_ext(Raquet_CHR chr, int x, int y, int xsize, int ysize, double angle, Raquet_Point center, SDL_RendererFlip flip) {
    SDL_Rect dstrect = {
        x - center.x,
        y - center.y,
        xsize,
        ysize
    };
    SDL_RenderCopyEx(Raquet_Renderer, chr.tex, NULL, & dstrect, angle, & center, flip);
}

/* Swap a CHR's Palette */
void Raquet_SwapCHRPalette(Raquet_CHR* chr, Palette pal[4]) {

    if (memcmp(chr->palette, pal, sizeof(Palette[4])) == 0) {
        return;
    }

    Palette* pixels = (Palette*)malloc(sizeof(Palette) * (chr->width * chr->height));

    chr->palette[0] = pal[0];
    chr->palette[1] = pal[1];
    chr->palette[2] = pal[2];
    chr->palette[3] = pal[3];

    for (int y = 0; y < chr->height; y++) {
        for (int x = 0; x < chr->width; x++) {
            int dest = (y * chr->width) + x;
            pixels[dest] = pal[chr->data[dest]];
        }
    }
    SDL_UpdateTexture(chr->tex, NULL, pixels, chr->width * sizeof(Palette));

    free(pixels);

}

void Raquet_DrawPoint(int x, int y, Palette pal, int alpha) {
    Raquet_SetDrawColor(pal, alpha);
    SDL_RenderDrawPoint(Raquet_Renderer, x, y);
}

void Raquet_DrawLine(int x1, int y1, int x2, int y2, Palette pal, int alpha) {
    Raquet_SetDrawColor(pal, alpha);
    SDL_RenderDrawLine(Raquet_Renderer, x1, y1, x2, y2);
}

void Raquet_FreeCHR(Raquet_CHR* chr) {
    SDL_DestroyTexture(chr -> tex);
    free(chr);
}
