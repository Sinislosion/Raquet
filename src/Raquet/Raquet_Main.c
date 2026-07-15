#include "Raquet.h"

SDL_Window* Raquet_Window;
uint8_t Raquet_Fullscreen = -1;
SDL_Renderer* Raquet_Renderer;

const SDL_FRect Raquet_RectScreen = {
    0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
};

SDL_FRect Raquet_RectScreenScale = {
	0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
};

SDL_Event Raquet_Event;
Palette Raquet_ClearColor;

/*
 ****************************
 *     RAQUET FUNCTIONS     *
 ****************************
 */

extern void runthedog(void);

extern void createthedog(void); // put all your creation code for the program here

int Raquet_InitVideo(void) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == false) {

        #ifdef PRINT_DEBUG
            puts("FAILED TO INITIALIZE SDL VIDEO.");
            fflush(stdout);
        #endif

        return 0;
    } else {
        // Create window
        Raquet_Window = SDL_CreateWindow(GAME_NAME, SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE, SDL_WINDOW_RESIZABLE);
        if (Raquet_Window == NULL) {
            #ifdef PRINT_DEBUG
                printf("FAILED TO CREATE SDL WINDOW.\n");
            #endif
            return 0;
        }

        #ifdef PRINT_DEBUG
            printf("SDL Initialized\n");
            fflush(stdout);
        #endif

        // Init Window Renderer
        Raquet_Renderer = SDL_CreateRenderer(Raquet_Window, NULL);
        if (SDL_GL_SetSwapInterval(-1) == false) {
            SDL_GL_SetSwapInterval(1);
        }

        SDL_SetHint("SDL_RENDER_SCALE_QUALITY", 0);

        SDL_SetRenderViewport(Raquet_Renderer, NULL);

        SDL_SetRenderLogicalPresentation(Raquet_Renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
        SDL_SetRenderDrawBlendMode(Raquet_Renderer, SDL_BLENDMODE_BLEND);

    }

    return 1;

}

/* Initialize the Raquet Engine */
int Raquet_Init(void) {
    Raquet_SDLKeys = SDL_GetKeyboardState(NULL);
    if (!Raquet_InitVideo()) {

        #ifdef PRINT_DEBUG
            printf("Failed to Initialize Video\n");
            fflush(stdout);
        #endif

        return 0;
    }

    if (!Raquet_InitAudio()) {
        #ifdef PRINT_DEBUG
            printf("Failed to Initialize Audio\n");
            fflush(stdout);
        #endif
    }

    return 1;
}

/* This is used to update the Window within the Raquet_Main function */
void Raquet_Render(void) {
    SDL_UpdateWindowSurface(Raquet_Window);

    SDL_RenderPresent(Raquet_Renderer);

    #ifndef BACKGROUND_CLEARING_COLOR
        SDL_SetRenderDrawColor(Raquet_Renderer, 0, 0, 0, 255);
    #else
        Raquet_SetDrawColor(Raquet_ClearColor, 255);
    #endif

    SDL_RenderClear(Raquet_Renderer);

}

// FRAMERATE
Uint64 Raquet_StartTick = 0;
Uint64 Raquet_LastTick = 0;
double Raquet_DeltaTime = 0;

/* The main Raquet function. Everything runs from here. */
void Raquet_Main(void) {
    if (!Raquet_Init()) {

        #ifdef PRINT_DEBUG
            puts("Failed to Initialize Raquet");
            fflush(stdout);
        #endif

        return;
    } else {

        #ifdef PRINT_DEBUG
            puts("Raquet Initialized");
            fflush(stdout);
        #endif

        createthedog(); // run our creation code

        Raquet_StartTick = SDL_GetPerformanceCounter();
        Raquet_LastTick = Raquet_StartTick;

        /* SDL While loop, and frame counter */
        while (true) {

            Uint64 cpuFramerate = FRAMERATE_CAP / SDL_GetPerformanceCounter();

            Raquet_LastTick = SDL_GetPerformanceCounter();
            Raquet_DeltaTime = ((double)(Raquet_LastTick - Raquet_StartTick)) / (double)SDL_GetPerformanceFrequency();

            Raquet_StartTick = Raquet_LastTick;

            while (SDL_PollEvent(&Raquet_Event)) {
                switch (Raquet_Event.type) {
                case SDL_EVENT_QUIT:
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
                SDL_SetWindowFullscreen(Raquet_Window, false);
                break;

            case 1:
                SDL_SetWindowFullscreen(Raquet_Window, true);
                break;
            }
            #endif

            Raquet_SDLMouse = SDL_GetMouseState(NULL, NULL);

            runthedog();
            Raquet_Render();

            for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
                Raquet_PrevSDLKeys[i] = Raquet_SDLKeys[i];
            }
            Raquet_PrevSDLMouse = Raquet_SDLMouse;

            #if FRAMERATE_CAP == 0
                SDL_Delay(2);
            #else

                double msPastLastTick = ((double)SDL_GetPerformanceCounter() - (double)Raquet_StartTick) / (double)SDL_GetPerformanceFrequency();
                double delayAmonut = (1.0 / (double)FRAMERATE_CAP) - msPastLastTick;

                if (delayAmonut > 0) {
                    SDL_Delay((Uint32)(delayAmonut * 1000.0));
                }

            #endif


        }

    }

}
