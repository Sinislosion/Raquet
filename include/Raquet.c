#include "Raquet.h"

SDL_Window * gWindow;
uint8_t gFullscreen;
SDL_Renderer * gRenderer;
SDL_Texture * gFinalTexture;
const SDL_Rect gRectScreen;
SDL_Rect gRectScreenScale;
SDL_Event e;

const SDL_Rect gRectScreen = {
    0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
};

SDL_Rect gRectScreenScale = {
	0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
};

/*
 ************************
 *     PALETTE INFO     *
 ************************
 */

Palette Raquet_GlobalPalette[0x40] = {
    /* 0x00 - 0x0F */
    0x696969FF,
    0x00148FFF,
    0x1E029BFF,
    0x3F008AFF,
    0x600060FF,
    0x660017FF,
    0x570D00FF,
    0x451B00FF,
    0x243400FF,
    0x004200FF,
    0x004500FF,
    0x003C1FFF,
    0x00315CFF,
    0x000000FF,
    0x000000FF,
    0x00000000,

    /* 0x10 - 0x1F */
    0xAFAFAFFF,
    0x0F51DDFF,
    0x442FF3FF,
    0x7220E2FF,
    0xA319B3FF,
    0xAE1C51FF,
    0xA43400FF,
    0x884D00FF,
    0x676D00FF,
    0x208000FF,
    0x008B00FF,
    0x007F42FF,
    0x006C97FF,
    0x000000FF,
    0x000000FF,
    0x00000000,

    /* 0x20 - 0x2F */
    0xFFFFFFFF,
    0x65AAFFFF,
    0x8C96FFFF,
    0xB983FFFF,
    0xDD6FFFFF,
    0xEA6FBDFF,
    0xEB8466FF,
    0xDCA21FFF,
    0xBAB403FF,
    0x7ECB07FF,
    0x54D33EFF,
    0x3CD284FF,
    0x3EC7CCFF,
    0x4B4B4BFF,
    0x000000FF,
    0x00000000,

    /* 0x30 - 0x3F */
    0xFFFFFFFF,
    0xBDE2FFFF,
    0xCECFFFFF,
    0xE6C2FFFF,
    0xF6bCFFFF,
    0xF9C2EDFF,
    0xFACFC6FF,
    0xF8DEACFF,
    0xEEE9A1FF,
    0xD0F59FFF,
    0xBBF5AFFF,
    0xB3F5CDFF,
    0xB9EDF0FF,
    0xB9B9B9FF,
    0x000000FF,
    0x00000000
};

/*
 ***************************
 *     AUDIO FUNCTIONS     *
 ***************************
 */

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

int Raquet_Sign(int comp) {
	return (0 < comp) - (comp < 0);
}

int Raquet_Min(int x, int y) {
	return (((x) < (y)) ? (x) : (y));
}

float Raquet_PI = 3.1415926535;

/*
 ***************************
 *     INPUT FUNCTIONS     *
 ***************************
 */

const Uint8 * sdlkeys;
uint8_t sdlmouse;
uint8_t prevmouse;
uint8_t prevkeys[SDL_NUM_SCANCODES];

/* Check if this key is being held down */
int Raquet_KeyCheck(unsigned int key) {
	return sdlkeys[key];
}

/* Will only return 1 for the first frame the key is being held down */
int Raquet_KeyCheck_Pressed(unsigned int key) {
	return (prevkeys[key] != sdlkeys[key] && sdlkeys[key] != 0);
}

/* Will only return 1 if the key has been released for 1 frame */
int Raquet_KeyCheck_Released(unsigned int key) {
	return (prevkeys[key] != sdlkeys[key] && sdlkeys[key] != 1);
}

/* Check if this mouse button is being held down */
int Raquet_MouseCheck(unsigned int sdlbutton) {
	return sdlbutton & sdlmouse;
}

/* Will only return 1 for the first frame the mouse button is being held down */
int Raquet_MouseCheck_Pressed(unsigned int sdlbutton) {
	return (prevmouse & sdlbutton) != (sdlmouse & sdlbutton) && (sdlmouse & sdlbutton) != 0;
}

/* Will only return 1 if the mouse button has been released for 1 frame */
int Raquet_MouseCheck_Released(unsigned int sdlbutton) {
	return (prevmouse & sdlbutton) != (sdlmouse & sdlbutton) && (sdlmouse & sdlbutton) != 1;
}

/* 
 **************************
 *     MISC FUNCTIONS     *
 **************************
 */
 
/* 1 for display cursor, 0 for disable cursor */
extern void Raquet_ShowCursor(int toggle) {
	SDL_ShowCursor(toggle);
}

/*
 ****************************
 *     RAQUET FUNCTIONS     *
 ****************************
 */

// DOG FUNCTIONS
extern void runthedog(); // put this somewhere in your program, the default code to run
extern void createthedog(); // put all your creation code for the program here

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

            // Init Window Renderer
            #ifdef VSYNC
            	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
				if (SDL_GL_SetSwapInterval(-1) < 0) {
        	        SDL_GL_SetSwapInterval(1);
                }
            #else
            	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
            #endif
			
            #ifdef INTEGER_SCALING
			    gFinalTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
            #else
                SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
            #endif

            SDL_RenderSetViewport(gRenderer, NULL);

            

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

/* This is used to update the Window within the Raquet_Main function */
void Raquet_Update() {
    SDL_UpdateWindowSurface(gWindow);
	SDL_SetRenderTarget(gRenderer, NULL);

    #ifdef INTEGER_SCALING
        /* Integer Scaling Math */
        int windowWidth, windowHeight;
        SDL_GetWindowSize(gWindow, &windowWidth, &windowHeight);
        
        int mult1, mult2, mult3;
        
        mult1 = (windowHeight / SCREEN_HEIGHT);
        mult2 = (windowWidth / SCREEN_WIDTH);

        mult3 = Raquet_Min(mult1, mult2);
        
        gRectScreenScale.w = SCREEN_WIDTH * mult3;
        gRectScreenScale.h = SCREEN_HEIGHT * mult3;

        gRectScreenScale.x = windowWidth/2 - (gRectScreenScale.w / 2);
        gRectScreenScale.y = windowHeight/2 - (gRectScreenScale.h / 2);
    
        SDL_RenderCopy(gRenderer, gFinalTexture, &gRectScreen, &gRectScreenScale);

    #endif

    SDL_RenderPresent(gRenderer);

    #ifndef BACKGROUND_CLEARING_COLOR
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    #else
        Raquet_SetDrawColor(gClearColor, 255);
    #endif

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
			
            #ifdef INTEGER_SCALING
                SDL_SetRenderTarget(gRenderer, gFinalTexture);
            #endif

            runthedog();
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