/* If you for some reason include Raquet.h several times, don't worry, I've got your back. */
#ifndef RAQUET_GAME_ENGINE
#define SDL_MAIN_HANDLED

/* Comment this out if you dont want fullscreen */
#define ALLOW_FULLSCREN

/* other headers we need */
#include <stdio.h>

#include <string.h>

#include <SDL2/SDL.h>

#include <SDL2/SDL_mixer.h>

/* WINDOW CONSTANTS */
#define SCREEN_WIDTH 480 // Internal screen width
#define SCREEN_HEIGHT 360 // Internal screen height
#define SCREEN_SCALE 2 // How much we scale the window by default
const float FRAMERATE_CAP = 60.0f; // Constant framerate
#define GAME_NAME "Raquet Game Engine" // Window Title
#define AUDIO_SAMPLE_RATE 44100 // How high quality our sound is, decrease if you want moldy mp3 sound :)
#define VSYNC

SDL_Window * gWindow;
uint8_t gFullscreen = -1;
SDL_Renderer * gRenderer;
SDL_Texture * gFinalTexture;

SDL_Rect gRectScrn = {
    0,
    0,
    SCREEN_WIDTH,
    SCREEN_HEIGHT
};
SDL_Event e;

/*
 ************************
 *     PALETTE INFO     *
 ************************
 */

typedef Uint32 Palette;

// Digital Prime Palette
#define PAL00   0x696969FF
#define PAL01 	0x00148FFF
#define PAL02 	0x1E029BFF 
#define PAL03 	0x3F008AFF
#define PAL04 	0x600060FF
#define PAL05 	0x660017FF
#define PAL06 	0x570D00FF
#define PAL07 	0x451B00FF
#define PAL08 	0x243400FF
#define PAL09 	0x004200FF
#define PAL0A 	0x004500FF
#define PAL0B 	0x003C1FFF
#define PAL0C 	0x00315CFF
#define PAL0D 	0x000000FF
#define PAL0E 	0x000000FF
#define PAL0F 	0x00000000  // TRANSPARENCY

#define PAL10 	0xAFAFAFFF
#define PAL11 	0x0F51DDFF
#define PAL12 	0x442FF3FF
#define PAL13 	0x7220E2FF
#define PAL14 	0xA319B3FF
#define PAL15 	0xAE1C51FF
#define PAL16 	0xA43400FF
#define PAL17 	0x884D00FF
#define PAL18 	0x676D00FF
#define PAL19 	0x208000FF
#define PAL1A 	0x008B00FF
#define PAL1B 	0x007F42FF
#define PAL1C 	0x006C97FF
#define PAL1D 	0x000000FF
#define PAL1E 	0x000000FF
#define PAL1F 	0x00000000  // TRANSPARENCY

#define PAL20 	0xFFFFFFFF
#define PAL21 	0x65AAFFFF
#define PAL22 	0x8C96FFFF
#define PAL23 	0xB983FFFF
#define PAL24 	0xDD6FFFFF
#define PAL25 	0xEA6FBDFF
#define PAL26 	0xEB8466FF
#define PAL27 	0xDCA21FFF
#define PAL28 	0xBAB403FF
#define PAL29 	0x7ECB07FF
#define PAL2A 	0x54D33EFF
#define PAL2B 	0x3CD284FF
#define PAL2C 	0x3EC7CCFF
#define PAL2D 	0x4B4B4BFF
#define PAL2E 	0x000000FF
#define PAL2F 	0x00000000  // TRANSPARENCY

#define PAL30 	0xFFFFFFFF
#define PAL31 	0xBDE2FFFF
#define PAL32 	0xCECFFFFF
#define PAL33 	0xE6C2FFFF
#define PAL34 	0xF6bCFFFF
#define PAL35 	0xF9C2EDFF
#define PAL36 	0xFACFC6FF
#define PAL37 	0xF8DEACFF
#define PAL38 	0xEEE9A1FF
#define PAL39 	0xD0F59FFF
#define PAL3A 	0xBBF5AFFF
#define PAL3B 	0xB3F5CDFF
#define PAL3C 	0xB9EDF0FF
#define PAL3D 	0xB9B9B9FF
#define PAL3E 	0x000000FF
#define PAL3F 	0x00000000  // TRANSPARENCY

/*
 ***************************
 *     AUDIO FUNCTIONS     *
 ***************************
 */
typedef Mix_Chunk * Raquet_Sound;
typedef Mix_Music * Raquet_BGM;

Raquet_Sound Raquet_LoadSound(const char * file) {
    return Mix_LoadWAV(file);
}

void Raquet_PlaySound(Raquet_Sound wav, int loops) {
    Mix_PlayChannel(-1, wav, loops);
}

void Raquet_DestroySound(Raquet_Sound wav) {
    Mix_FreeChunk(wav);
}

/*
 *********************************
 *     CUSTOM MATH FUNCTIONS     *
 *********************************
 */

#define sign(comp)  (0 < comp) - (comp < 0)
#define pi 3.1415926535

/*
 ***************************
 *     INPUT FUNCTIONS     *
 ***************************
 */

const Uint8 * sdlkeys;
uint8_t sdlmouse;
uint8_t prevmouse;

#define RAQUET_MOUSE_RIGHT      4
#define RAQUET_MOUSE_LEFT       1
#define RAQUET_MOUSE_MIDDLE     2

Uint8 prevkeys[SDL_NUM_SCANCODES];

/* Check if this key is being held down */
#define Raquet_KeyCheck(nkey) sdlkeys[nkey]

/* Will only return 1 for the first frame the key is being held down */
#define Raquet_KeyCheck_Pressed(nkey) (prevkeys[nkey] != sdlkeys[nkey] && sdlkeys[nkey] != 0)

/* Will only return 1 if the key has been released for 1 frame */
#define Raquet_KeyCheck_Released(nkey) (prevkeys[nkey] != sdlkeys[nkey] && sdlkeys[nkey] != 1)

/* Check if this mouse button is being held down */
#define Raquet_MouseCheck(sdlbutton) sdlbutton & sdlmouse

/* Will only return 1 for the first frame the mouse button is being held down */
#define Raquet_MouseCheck_Pressed(sdlbutton) (prevmouse & sdlbutton) != (sdlmouse & sdlbutton) && (sdlmouse & sdlbutton) != 0

/* Will only return 1 if the mouse button has been released for 1 frame */
#define Raquet_MouseCheck_Released(sdlbutton) (prevmouse & sdlbutton) != (sdlmouse & sdlbutton) && (sdlmouse & sdlbutton) != 1

/* 
 **************************
 *     MISC FUNCTIONS     *
 **************************
 */
 
/* 1 for display cursor, 0 for disable cursor */
#define Raquet_ShowCursor(toggle) SDL_ShowCursor(toggle)

/*
 ****************************
 *     RAQUET FUNCTIONS     *
 ****************************
 */

// DOG FUNCTIONS
void runthedog(); // put this somewhere in your program, the default code to run
void createthedog(); // put all your creation code for the program here

// FRAMERATE
Uint64 start_tick = 0;

int Raquet_InitSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("FAILED TO INITIALIZE SDL VIDEO.\n");
        return 0;
    } else {
        // Init SDL_mixer
        if (Mix_OpenAudio(AUDIO_SAMPLE_RATE, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            printf("FAILED TO INITIALIZE SDL MIXER.\n");
            return 0;
        }
        // Create window
        gWindow = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (gWindow == NULL) {
            printf("FAILED TO CREATE SDL WINDOW.\n");
            return 0;
        } else {
            printf("SDL Initialized\n");
            fflush(stdout);

            gFinalTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

            // Init Window Renderer
            #ifdef VSYNC
            	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            #else
            	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            #endif

            SDL_RenderSetViewport(gRenderer, NULL);
            SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);

            #ifdef VSYNC
                if (SDL_GL_SetSwapInterval(-1) < 0) {
        	        SDL_GL_SetSwapInterval(1);
                }
            #endif

            SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

        }

    }

    return 1;

}

/* Initialize the Raquet Engine */
int Raquet_Init() {
    sdlkeys = SDL_GetKeyboardState(NULL);
    if (!Raquet_InitSDL()) {
        printf("Failed to Initialize SDL\n");
        return 0;
    }

    return 1;
}

/* Set a palette variable within runtime */
void Raquet_SetPalette(Palette dest[3], Uint32 pal1, Uint32 pal2, Uint32 pal3) {
    dest[0] = pal1;
    dest[1] = pal2;
    dest[2] = pal3;
}

/* I'm actually not sure what you can use this for yet, but you can use it */
void Raquet_SetDrawColor(Palette pal, int alpha) {
    Uint32 palr = (pal >> 24) & 0x000000FF;
    Uint32 palg = (pal >> 16) & 0x000000FF;
    Uint32 palb = (pal >> 8) & 0x000000FF;
    SDL_SetRenderDrawColor(gRenderer, palr, palg, palb, alpha);
}

/* Clear the screen with a solid color */
void Raquet_Clear(Palette pal) {
    Raquet_SetDrawColor(pal, 255);
    SDL_RenderFillRect(gRenderer, NULL);
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

/* This is used to update the Window within the Raquet_Main function */
void Raquet_Update() {
    SDL_UpdateWindowSurface(gWindow);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderPresent(gRenderer);
    SDL_RenderClear(gRenderer);
}

/* The main Raquet function. Everything runs from here. */
void Raquet_Main() {
    if (!Raquet_Init()) {
        printf("Failed to Initialize Raquet\n");
        return;
    } else {
        printf("Raquet Initialized\n");
        fflush(stdout);
        createthedog(); // run our creation code

        /* SDL While loop, and frame counter */
        while (1) {
            while (SDL_PollEvent( & e)) {
                switch (e.type) {
                case SDL_QUIT:
                    return;
                    break;
                }
            }

            // If we allow fullscreen, then let us use fullscreen with F11
            #ifdef ALLOW_FULLSCREN
            if (Raquet_KeyCheck_Pressed(SDL_SCANCODE_F11)) {
                gFullscreen = -gFullscreen;
            }

            switch (gFullscreen) {
            default:
                SDL_SetWindowFullscreen(gWindow, 0);
                break;

            case 1:
                SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
                break;
            }
            #endif

            sdlmouse = SDL_GetMouseState(NULL, NULL);

            start_tick = SDL_GetTicks64();
            SDL_SetRenderTarget(gRenderer, gFinalTexture);
            runthedog();
            SDL_SetRenderTarget(gRenderer, NULL);
            Raquet_Update();
            if ((1000.0f / FRAMERATE_CAP) > SDL_GetTicks64() - start_tick) {
                SDL_Delay(1000.0f / FRAMERATE_CAP - (SDL_GetTicks64() - start_tick));
            }
            for (int i = 0; i < 322; i++)(prevkeys[i] = sdlkeys[i]);
            prevmouse = sdlmouse;

            
        }

    }

}

/*
 *************************
 *     PPF FUNCTIONS     *
 *************************
 */
typedef SDL_Point Raquet_Point;
typedef char * PPF_Bank;

// PPF HEADER V1.0
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
				printf("WARNING: HEADER DATA DOES NOT MATCH\n");
				fflush(stdout);
			}
			
		} 
		
		SDL_RWclose(ppfdata);
		printf("Loaded PPF Data at: %s successfully\n", dir);
		fflush(stdout);
		return 1;
		
	} else {
		printf("Failed to load PPF at: %s\n", dir);
        fflush(stdout);
        exit(1);
		return 0;
	}
}

/* Load a single-tile sprite. More info is in the wiki */
//typedef SDL_Texture * Raquet_CHR;

typedef struct Raquet_CHR {
  SDL_Texture * tex;
  int width;
  int height;
  int * data;
  Palette palette[3];
} Raquet_CHR;

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



/* Load a multi-tile sprite. More info is in the wiki */
Raquet_CHR Raquet_LoadCHRMult(PPF_Bank ppfbank, int * id, int xwrap, int ywrap, Palette palette[3]) {
    Raquet_CHR ret;
    
    ret.width = xwrap * 8;
    ret.height = ywrap * 8;
    
    ret.tex = SDL_CreateTexture(
        gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, ret.width, ret.height);
 
    

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
Raquet_CHR Raquet_SwapCHRPalette(Raquet_CHR chr, Palette pal[3]) {
  
    Uint32 pixels[chr.width * chr.height];

    if (pal[0] == chr.palette[0] && pal[1] == chr.palette[1] && pal[2] == chr.palette[2]) {
        return chr;
      
    }

    chr.palette[0] = pal[0];
    chr.palette[1] = pal[1];
    chr.palette[2] = pal[2];

    for (int y = 0; y < chr.height; y++) {
        for (int x = 0; x < chr.width; x++) {
            int dest = (y * chr.width) + x;
            switch (chr.data[dest]) {
                case 0:
                    pixels[dest] = 0x00000000;
                    break;

                case 1:
                    pixels[dest] = pal[0];
                    break;

                case 2:
                    pixels[dest] = pal[1];
                    break;

                case 3:
                    pixels[dest] = pal[2];
                    break;
           } 

        }
    }
    SDL_UpdateTexture(chr.tex, NULL, pixels, chr.width * sizeof(Palette));
    return chr;

}

/* Destroy a CHR after we're done using it */
void DestroyCHR(SDL_Texture * tex) {
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

/*
 ******************
 *     CAMERA     *
 ******************
 */
struct Camera {
    int x;
    int y;
} Camera;

/*
 *************************
 *     ACTORS SYSTEM     *
 *************************
 */

typedef struct Raquet_BoundingBox {
    int x1;
    int y1;
    int x2;
    int y2;

}
Raquet_BoundingBox;

typedef struct Actor {
    // where we are in virtual space
    int x;
    int y;

    // how we're displayed
    Raquet_CHR cur_image; // Current CHR
    Raquet_Point origin; // Our Orgigin Point (x, y) default is (0, 0)
    int width; // How wide we are (default is the width of the sprite)
    int height; // How tall we are (default is the height of the sprite)
    int angle; // angle of the object, rotated around its origin
    
    Palette color; // used in very specific circumstances (barriers in B-TRON)

    // collision info
    Raquet_BoundingBox bbox; // Bounding box                  

    SDL_RendererFlip flip;

}
Actor;

Actor * Raquet_AllocateActor() {
    return (Actor * ) malloc(sizeof(Actor));
}

void Raquet_CreateActor(Actor * act, Raquet_CHR chr) {
    act -> x = 0;
    act -> y = 0;
    act -> origin.x = 0;
    act -> origin.y = 0;
    act -> angle = 0;
    act -> flip = SDL_FLIP_NONE;
    Raquet_Point size = Raquet_SizeofCHR(chr.tex);
    act -> cur_image = chr;
    act -> width = size.x;
    act -> height = size.y;
    act -> bbox.x1 = 0;
    act -> bbox.y1 = 0;
    act -> bbox.x2 = size.x;
    act -> bbox.y2 = size.y;

}

void Raquet_DestroyActor(Actor * act) {
    free(act);
}

void Raquet_DrawActor(Actor * act) {
    Raquet_PlaceCHR_ext(act -> cur_image, act -> x - Camera.x, act -> y - Camera.y, act -> width, act -> height, act -> angle, act -> origin, act -> flip);
}

int Raquet_ActorColliding(int x, int y, Actor * act1, Actor * act2) {
    return (x - act1 -> origin.x + act1 -> bbox.x2 > act2 -> x - act2 -> origin.x + act2 -> bbox.x1) && (x - act1 -> origin.x + act1 -> bbox.x1 < act2 -> x - act2 -> origin.x + act2 -> bbox.x2) && (y - act1 -> origin.y + act1 -> bbox.y2 > act2 -> y - act2 -> origin.y + act2 -> bbox.y1) && (y - act1 -> origin.y + act1 -> bbox.y1 < act2 -> y - act2 -> origin.y + act2 -> bbox.y2);
}

#define RAQUET_GAME_ENGINE
#endif
