#include "Raquet.h"

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


void Raquet_PlaceCHR(Raquet_CHR chr, int x, int y) {
    SDL_Point size = { chr.width, chr.height };
    const SDL_FRect dstrect = {
        x,
        y,
        size.x,
        size.y
    };
    SDL_RenderTexture(Raquet_Renderer, chr.tex, NULL, & dstrect);
}

/* Place a CHR sprite, with additional control (chr data, x, y, width in pixels, height in pixels, SDL flip mode) */
void Raquet_PlaceCHR_ext(Raquet_CHR chr, int x, int y, int xsize, int ysize, double angle, Raquet_Point center, SDL_FlipMode flip) {
    const SDL_FRect dstrect = {
        x - center.x,
        y - center.y,
        xsize,
        ysize
    };
    SDL_RenderTextureRotated(Raquet_Renderer, chr.tex, NULL, & dstrect, angle, & center, flip);
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

Raquet_CHR * Raquet_AllocateCHR(void) {
    return ( Raquet_CHR * ) malloc(sizeof(Raquet_CHR));
}

void Raquet_FreeCHR(Raquet_CHR* chr) {
    SDL_DestroyTexture(chr -> tex);
    free(chr);
}
