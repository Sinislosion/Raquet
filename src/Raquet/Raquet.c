#include "Raquet.h"

SDL_Window * gWindow;
uint8_t gFullscreen = -1;
SDL_Renderer * gRenderer;
SDL_Texture * gFinalTexture;

const SDL_Rect gRectScreen = {
    0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
};

SDL_Rect gRectScreenScale = {
	0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
};

SDL_Event e;

/*
 ****************************
 *     RAQUET FUNCTIONS     *
 ****************************
 */

// DOG FUNCTIONS
extern void runthedog(void); // put this somewhere in your program, the default code to run
extern void createthedog(void); // put all your creation code for the program here

// FRAMERATE
Uint64 start_tick = 0;
Uint64 last_tick = 0;
double Raquet_DeltaTime = 1;

int Raquet_InitSDL(void) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        #ifdef PRINT_DEBUG
            printf("FAILED TO INITIALIZE SDL VIDEO.\n");
        #endif
        return 0;
    } else {
        // Init SDL_mixer
        if (Mix_OpenAudio(AUDIO_SAMPLE_RATE, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            #ifdef PRINT_DEBUG
                printf("FAILED TO INITIALIZE SDL MIXER.\n");
            #endif
            return 0;
        }
        // Create window
        gWindow = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (gWindow == NULL) {
            #ifdef PRINT_DEBUG
                printf("FAILED TO CREATE SDL WINDOW.\n");
            #endif
            return 0;
        } else {
            #ifdef PRINT_DEBUG
                printf("SDL Initialized\n");
            #endif
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
int Raquet_Init(void) {
    sdlkeys = SDL_GetKeyboardState(NULL);
    if (!Raquet_InitSDL()) {
        #ifdef PRINT_DEBUG
            printf("Failed to Initialize SDL\n");
        #endif
        return 0;
    }

    return 1;
}

/* This is used to update the Window within the Raquet_Main function */
void Raquet_Update(void) {
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

        gRectScreenScale.w = Raquet_Max(SCREEN_WIDTH, SCREEN_WIDTH * mult3);
        gRectScreenScale.h = Raquet_Max(SCREEN_HEIGHT, SCREEN_HEIGHT * mult3);

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
void Raquet_Main(void) {
    if (!Raquet_Init()) {
        #ifdef PRINT_DEBUG
            printf("Failed to Initialize Raquet\n");
        #endif
        return;
    } else {
        #ifdef PRINT_DEBUG
            printf("Raquet Initialized\n");
        #endif
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
            #ifdef ALLOW_FULLSCREEN
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

            #ifdef INTEGER_SCALING
                SDL_SetRenderTarget(gRenderer, gFinalTexture);
            #endif

            start_tick = SDL_GetTicks64();
            runthedog();
            Raquet_Update();

            for (int i = 0; i < 322; i++) {prevkeys[i] = sdlkeys[i];}
            prevmouse = sdlmouse;

            #ifdef DELTA_TIME
                Raquet_DeltaTime = double(start_tick - last_tick) * 0.060f;
                last_tick = start_tick;
                SDL_Delay(1);
            #else
                int cur_tick = SDL_GetTicks64();
                if ((1000.0f / FRAMERATE_CAP) > cur_tick - start_tick) {
                    SDL_Delay(1000.0f / FRAMERATE_CAP - (cur_tick - start_tick));
                }
            #endif

        }

    }

}
