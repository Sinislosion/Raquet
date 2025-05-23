// put your libs here.
#include "Raquet.h"


/*
 ********************
 *     PPF BANK     *
 ********************
*/
PPF_Bank ppf_main;


/**
 *******************
 *     SPRITES     *
 *******************
*/
int arr_logo_r[] = {
     0,  0,  0,  0,  0,  1, -1, -1,
     0,  0, -1, -1,  0,  0, -1, -1,
    16,  0, 19, -1,  0,  0, -1, -1,
     0,  0,  0,  0,  0,  3, -1, -1,
     0,  0, 17, 16,  0,  1, -1, -1,
     0,  0, -1, 17,  0, 16,  1, -1,
     0,  0, -1, -1, 18,  0,  0,  1,
    20, 19, -1, -1, -1, 17,  0, -1,
    -1,  0, -1, -1, -1, 17, 18, -1,
    -1, 18, -1, -1, -1, -1, 17, -1,
    -1, 18, -1, -1, -1, -1, -1, -1,
    -1, 17, -1, -1, -1, -1, -1, -1
};

int arr_logo_a[] = {
    -1, -1,  2,  0,  0, 18,
    -1, -1, 16,  0,  0, 19,
    -1,  2,  0,  3,  0,  0,
    -1,  0,  0, -1,  0,  0,
     2,  0,  0,  5,  0,  0,
     4,  0,  6, 17,  0, 16,
    -1,  4, -1, -1,  0,  0,
    -1, -1, -1, -1,  0, 17,
    -1, -1, -1, -1,  0, 17,
    -1, -1, -1, -1, 16, -1,
    -1, -1, -1, -1, 17, -1


};

int arr_logo_q[] = {
     2,  0,  0,  0,  0,  1,
     0,  0,  6, -1,  0,  0,
     0,  0, -1, -1, 16,  0,
     0,  0, -1, -1,  0,  0,
     0,  0, -1,  1,  0,  0,
    16,  0, 19,  4,  0,  3,
     4,  0,  0,  3,  4,  0,
    -1, 16,  0, -1, -1, 16,
    -1, -1, 17, -1, -1, -1,
    -1, -1, -1, -1, -1, -1,
    -1, -1,  7, -1, -1, -1
};

int arr_logo_u[] = {
     0,  0, -1, -1,  0,  0,
     0,  0, -1, -1,  0,  0,
     0,  0, -1, -1,  0,  0,
     0,  0, -1, -1,  0, 16,
     0,  0, -1, -1,  0,  0,
     0,  0,  1, -1,  0,  0,
     4,  0,  0,  0,  0,  4,
    -1,  0,  0,  8, 16, -1,
    -1,  0, 17,  0, 16, -1,
    -1,  9, -1, -1, -1, -1,
    -1, 17, -1, -1, -1, -1
};

int arr_logo_e[] = {
    10, 10,  0,  0,  0,  0,
     0,  0, -1, -1, -1, -1,
     0, 16, -1, -1, -1, -1,
     0,  0, 10, 10, -1, -1,
     0,  0, -1, -1, -1, -1,
     0,  0, -1, -1, -1, -1,
     0,  0,  0,  0,  0, 16,
    -1,  0,  0,  8,  0, 11,
    -1,  0, 18, 16, 20, -1,
    -1,  9, 18, -1, -1, -1,
    -1, 16, -1, -1, -1, -1,
    -1, -1, -1, -1,  7, -1
};

int arr_logo_t[] = {
    12, 12,  0, 16, 12, 12,
    -1, -1,  0,  0, -1, -1,
    -1, -1,  0,  0, -1, -1,
    -1, -1,  0,  0, -1, -1,
    -1, -1,  0,  0, -1, -1,
    -1, -1,  0,  0, -1, -1,
    -1, -1,  0,  0, -1, -1,
    -1, -1,  0, 17, -1, -1,
    -1, -1,  0, 17, -1, -1,
    -1, -1,  0, -1, -1, -1,
    -1, -1, 16, -1, -1, -1,
    -1, -1, 17, -1, -1, -1
};

int arr_logo_text[] = {'G', 'A', 'M', 'E', -1, 'E', 'N', 'G', 'I', 'N', 'E'};

Raquet_CHR chr_logo_r, chr_logo_a, chr_logo_q, chr_logo_u, chr_logo_e, chr_logo_t, chr_logo_text;
Raquet_CHR * chr_logo[] = {&chr_logo_r, &chr_logo_a, &chr_logo_q, &chr_logo_u, &chr_logo_e, &chr_logo_t};

Palette pal_logo[3][4];

/**
 *****************
 *     STARS     *
 *****************
*/
typedef struct Stars {
    float x;
    float y;
} Stars;

Stars stars[3][SCREEN_HEIGHT];

void Stars_Init() {
    for (int i = 0; i < 3; ++i) {
        for (int o = 0; o < SCREEN_HEIGHT; ++o) {
            stars[i][o].x = rand() % (SCREEN_WIDTH + 1);
            stars[i][o].y = o;
        }
    }
}

/*
 **************************
 *     LOGO VARIABLES     *
 **************************
*/

int logoX = (SCREEN_WIDTH / 2) - 168;
const int logoY = (SCREEN_HEIGHT / 2) - 66;

const int textX = (SCREEN_WIDTH / 2) - 44;
const int textY = logoY + 108;

uint8_t playAnim = 0;
int animTick = 0;
int tick = 2;
float sinTick = 0;
float sinOffset = 16;

/*
 **************************
 *     MAIN FUNCTIONS     *
 **************************
*/

void createthedog(void)
{

    /* Graphical */
    Raquet_LoadPPFBank(&ppf_main, Raquet_AbsoluteToAsset("main.ppf"));
    Stars_Init();

    Raquet_SetPalette(pal_logo[0], Raquet_GlobalPalette[0x0F], Raquet_GlobalPalette[0x27], Raquet_GlobalPalette[0x00], Raquet_GlobalPalette[0x00]);
    Raquet_SetPalette(pal_logo[1], Raquet_GlobalPalette[0x0F], Raquet_GlobalPalette[0x37], Raquet_GlobalPalette[0x10], Raquet_GlobalPalette[0x10]);
    Raquet_SetPalette(pal_logo[2], Raquet_GlobalPalette[0x0F], Raquet_GlobalPalette[0x20], Raquet_GlobalPalette[0x20], Raquet_GlobalPalette[0x20]);

    chr_logo_r = Raquet_LoadCHRMult(ppf_main, arr_logo_r, 8, 12, pal_logo[2]);
    chr_logo_a = Raquet_LoadCHRMult(ppf_main, arr_logo_a, 6, 11, pal_logo[2]);
    chr_logo_q = Raquet_LoadCHRMult(ppf_main, arr_logo_q, 6, 11, pal_logo[2]);
    chr_logo_u = Raquet_LoadCHRMult(ppf_main, arr_logo_u, 6, 11, pal_logo[2]);
    chr_logo_e = Raquet_LoadCHRMult(ppf_main, arr_logo_e, 6, 12, pal_logo[2]);
    chr_logo_t = Raquet_LoadCHRMult(ppf_main, arr_logo_t, 6, 12, pal_logo[2]);

    chr_logo_text = Raquet_LoadCHRMult(ppf_main, arr_logo_text, 11, 1, pal_logo[2]);

}

void runthedog(void) {
    Raquet_Clear(Raquet_GlobalPalette[0x0F]);

    for (int i = 0; i < 3; ++i) {
        for (int o = 0; o < SCREEN_HEIGHT; ++o) {
            Raquet_DrawPoint(stars[i][o].x, stars[i][o].y, Raquet_GlobalPalette[0x12], 255 / (i + 1));
            stars[i][o].x -= 1.0 / (i + 1);
            if (stars[i][o].x < 0) {
                stars[i][o].x += SCREEN_WIDTH;
            }
        }
    }

    int move = Raquet_KeyCheck(SDL_SCANCODE_RIGHT) - Raquet_KeyCheck(SDL_SCANCODE_LEFT);
    
    logoX += move * 2;

    Raquet_PlaceCHR(chr_logo_r, logoX, logoY + (sin(sinTick / 32) * 16));
    Raquet_PlaceCHR(chr_logo_a, logoX + 56, logoY + (sin((sinTick - sinOffset) / 32) * 16));
    Raquet_PlaceCHR(chr_logo_q, logoX + 112, logoY + (sin((sinTick - sinOffset * 2) / 32) * 16));
    Raquet_PlaceCHR(chr_logo_u, logoX + 168, logoY + (sin((sinTick - sinOffset * 3) / 32) * 16));
    Raquet_PlaceCHR(chr_logo_e, logoX + 224, logoY + (sin((sinTick - sinOffset * 4) / 32) * 16));
    Raquet_PlaceCHR(chr_logo_t, logoX + 280, logoY + (sin((sinTick - sinOffset * 5) / 32) * 16));

    Raquet_PlaceCHR(chr_logo_text, textX, textY);

    ++sinTick;

    if (Raquet_KeyCheck_Pressed(SDL_SCANCODE_UP) && playAnim == 0) {
        playAnim = 1;
    }

    switch (playAnim) {
        case 1:
            animTick = 0;
            tick = 1;
            playAnim = 2;
            break;

        case 2:
            ++animTick;

            if (animTick > 5) {
                animTick = 0;
                --tick;
            }

            for (int i = 0; i < 6; ++i) {
                Raquet_SwapCHRPalette(chr_logo[i], pal_logo[tick]);
            }

            if (tick == 0) {
                playAnim = 3;
                animTick = 0;
            }
            break;

        case 3:
            ++animTick;

            if (animTick > 5) {
                animTick = 0;
                ++tick;
            }

            for (int i = 0; i < 6; ++i) {
                Raquet_SwapCHRPalette(chr_logo[i], pal_logo[tick]);
            }

            if (tick == 2) {
                playAnim = 0;
                animTick = 0;
            }
            break;

    }

}

int main(void) {
    Raquet_Main();
    return 0;
}
