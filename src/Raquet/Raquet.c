#include "Raquet.h"

SDL_Window * Raquet_Window;
uint8_t Raquet_Fullscreen = -1;
SDL_Renderer * Raquet_Renderer;
SDL_Texture * Raquet_FinalTexture;

const SDL_Rect Raquet_RectScreen = {
    0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
};

SDL_Rect Raquet_RectScreenScale = {
	0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
};

SDL_Event Raquet_Event;

/*
 ****************************
 *     RAQUET FUNCTIONS     *
 ****************************
 */

extern void runthedog(void);

extern void createthedog(void); // put all your creation code for the program here

// FRAMERATE
Uint64 Raquet_StartTick = 0;
Uint64 Raquet_LastTick = 0;
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
        Raquet_Window = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (Raquet_Window == NULL) {
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
            	Raquet_Renderer = SDL_CreateRenderer(Raquet_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
				if (SDL_GL_SetSwapInterval(-1) < 0) {
        	        SDL_GL_SetSwapInterval(1);
                }
            #else
            	Raquet_Renderer = SDL_CreateRenderer(Raquet_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
            #endif

            #ifdef INTEGER_SCALING
			    Raquet_FinalTexture = SDL_CreateTexture(Raquet_Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
            #else
                SDL_RenderSetLogicalSize(Raquet_Renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
            #endif

            SDL_RenderSetViewport(Raquet_Renderer, NULL);



            SDL_SetRenderDrawBlendMode(Raquet_Renderer, SDL_BLENDMODE_BLEND);

        }

    }

    return 1;

}

/* Initialize the Raquet Engine */
int Raquet_Init(void) {
    Raquet_SDLKeys = SDL_GetKeyboardState(NULL);
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
    SDL_UpdateWindowSurface(Raquet_Window);
	SDL_SetRenderTarget(Raquet_Renderer, NULL);

    #ifdef INTEGER_SCALING
        /* Integer Scaling Math */
        int windowWidth, windowHeight;
        SDL_GetWindowSize(Raquet_Window, &windowWidth, &windowHeight);

        int mult1, mult2, mult3;

        mult1 = (windowHeight / SCREEN_HEIGHT);
        mult2 = (windowWidth / SCREEN_WIDTH);

        mult3 = Raquet_Min(mult1, mult2);

        Raquet_RectScreenScale.w = Raquet_Max(SCREEN_WIDTH, SCREEN_WIDTH * mult3);
        Raquet_RectScreenScale.h = Raquet_Max(SCREEN_HEIGHT, SCREEN_HEIGHT * mult3);

        Raquet_RectScreenScale.x = windowWidth/2 - (Raquet_RectScreenScale.w / 2);
        Raquet_RectScreenScale.y = windowHeight/2 - (Raquet_RectScreenScale.h / 2);

        SDL_RenderCopy(Raquet_Renderer, Raquet_FinalTexture, &Raquet_RectScreen, &Raquet_RectScreenScale);

    #endif

    SDL_RenderPresent(Raquet_Renderer);

    #ifndef BACKGROUND_CLEARING_COLOR
        SDL_SetRenderDrawColor(Raquet_Renderer, 0, 0, 0, 255);
    #else
        Raquet_SetDrawColor(Raquet_ClearColor, 255);
    #endif

    SDL_RenderClear(Raquet_Renderer);

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
            while (SDL_PollEvent(&Raquet_Event)) {
                switch (Raquet_Event.type) {
                case SDL_QUIT:
                    return;
                    break;
                }
            }

            // If we allow fullscreen, then let us use fullscreen with F11
            #ifdef ALLOW_FULLSCREEN
            if (Raquet_KeyCheck_Pressed(SDL_SCANCODE_F11)) {
                Raquet_Fullscreen = -Raquet_Fullscreen;
            }

            switch (Raquet_Fullscreen) {
            default:
                SDL_SetWindowFullscreen(Raquet_Window, 0);
                break;

            case 1:
                SDL_SetWindowFullscreen(Raquet_Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                break;
            }
            #endif

            Raquet_SDLMouse = SDL_GetMouseState(NULL, NULL);

            #ifdef INTEGER_SCALING
                SDL_SetRenderTarget(Raquet_Renderer, Raquet_FinalTexture);
            #endif

            Raquet_StartTick = SDL_GetTicks64();
            runthedog();
            Raquet_Update();

            for (int i = 0; i < SDL_NUM_SCANCODES; i++) {Raquet_PrevSDLKeys[i] = Raquet_SDLKeys[i];}
            Raquet_PrevSDLMouse = Raquet_SDLMouse;

            #ifdef DELTA_TIME
                Raquet_DeltaTime = double(Raquet_StartTick - Raquet_LastTick) * 0.060f;
                Raquet_LastTick = Raquet_StartTick;
                SDL_Delay(1);
            #else
                int cur_tick = SDL_GetTicks64();
                if ((1000.0f / FRAMERATE_CAP) > cur_tick - Raquet_StartTick) {
                    SDL_Delay(1000.0f / FRAMERATE_CAP - (cur_tick - Raquet_StartTick));
                }
            #endif

        }

    }

}
