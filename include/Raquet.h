/* If you for some reason include Raquet.h several times, don't worry, I've got your back. */
#ifndef RAQUET_GAME_ENGINE
#define SDL_MAIN_HANDLED

/* Comment this out if you dont want fullscreen */
#define ALLOW_FULLSCREEN

/* headers we need */
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

/* WINDOW CONSTANTS */
#define SCREEN_WIDTH 256                // Internal screen width
#define SCREEN_HEIGHT 240               // Internal screen height
#define SCREEN_SCALE 2                  // How much we scale the window by default
#define FRAMERATE_CAP 60      			// Constant framerate
#define GAME_NAME "Raquet Game Engine"  // Window Title
#define AUDIO_SAMPLE_RATE 44100         // How high quality our sound is, decrease if you want moldy mp3 sound :)

/* VISUAL STUFF */
#define VSYNC               // DSIABLE FOR NO VSYNC
#define INTEGER_SCALING     // DISABLE FOR NO INTEGER SCALING
//#define BACKGROUND_CLEARING_COLOR   // Makes Raquet_Clear also change the window background color

/* More Constants */
extern SDL_Window * gWindow;
extern uint8_t gFullscreen;
extern SDL_Renderer * gRenderer;
extern SDL_Texture * gFinalTexture;

extern const SDL_Rect gRectScreen;
extern SDL_Rect gRectScreenScale;
extern SDL_Event e;

/*
 ************************
 *     PALETTE INFO     *
 ************************
 */

typedef Uint32 Palette;
extern Palette Raquet_GlobalPalette[];

/*
 ***************************
 *     AUDIO FUNCTIONS     *
 ***************************
 */
typedef Mix_Chunk * Raquet_Sound;
typedef Mix_Music * Raquet_BGM;

Raquet_Sound Raquet_LoadSound(const char * file);

extern void Raquet_PlaySound(Raquet_Sound wav, int loops);

extern void Raquet_DestroySound(Raquet_Sound wav);

/*
 *********************************
 *     CUSTOM MATH FUNCTIONS     *
 *********************************
 */

extern int Raquet_Sign(float comp);
extern float Raquet_Min(float x, float y);
extern float Raquet_Max(float x, float y);
extern float Raquet_PI;

/*
 ***************************
 *     INPUT FUNCTIONS     *
 ***************************
 */

extern const uint8_t * sdlkeys;
extern uint8_t prevkeys[SDL_NUM_SCANCODES];

extern uint8_t sdlmouse;
extern uint8_t prevmouse;

#define RAQUET_MOUSE_RIGHT      4
#define RAQUET_MOUSE_LEFT       1
#define RAQUET_MOUSE_MIDDLE     2

/* Check if this key is being held down */
extern int Raquet_KeyCheck(unsigned int nkey);

/* Will only return 1 for the first frame the key is being held down */
extern int Raquet_KeyCheck_Pressed(unsigned int nkey);

/* Will only return 1 if the key has been released for 1 frame */
extern int Raquet_KeyCheck_Released(unsigned int nkey);

/* Check if this mouse button is being held down */
extern int Raquet_MouseCheck(unsigned int sdlbutton);

/* Will only return 1 for the first frame the mouse button is being held down */
extern int Raquet_MouseCheck_Pressed(unsigned int sdlbutton);

/* Will only return 1 if the mouse button has been released for 1 frame */
extern int Raquet_MouseCheck_Released(unsigned int sdlbutton);

/*
 **************************
 *     MISC FUNCTIONS     *
 **************************
 */

/* 1 for display cursor, 0 for disable cursor */
extern void Raquet_ShowCursor(int toggle);

/* Get an absolute string to an asset from the assets folder */
extern const char* Raquet_AbsoluteToAsset(const char* relativepath);

/*
 ****************************
 *     RAQUET FUNCTIONS     *
 ****************************
 */

// FRAMERATE
extern Uint64 start_tick;

/* Initialize SDL2 */
extern int Raquet_InitSDL();

/* Initialize the Raquet Engine */
extern int Raquet_Init();

/* Set a palette variable within runtime */
extern void Raquet_SetPalette(Palette dest[3], Uint32 pal1, Uint32 pal2, Uint32 pal3);

/* Copy a Palette to another array */
extern void Raquet_CopyPalette(Palette dest[3], Palette origin[3]);

/* I'm actually not sure what you can use this for yet, but you can use it */
extern void Raquet_SetDrawColor(Palette pal, int alpha);

/* Clear the screen with a solid color */
#ifdef BACKGROUND_CLEARING_COLOR
    Palette gClearColor;
#endif

/* Clear the screen */
extern void Raquet_Clear(Palette pal);

/* Draw a rectangle (x position, y position, width, height, color, alpha, fill) */
extern void Raquet_DrawRectangle(int x1, int y1, int width, int height, Palette pal, int alpha, int fill);

/* This is used to update the Window within the Raquet_Main function */
extern void Raquet_Update();

/* The main Raquet function. Everything runs from here. */
extern void Raquet_Main();

/*
 *************************
 *     PPF FUNCTIONS     *
 *************************
 */
typedef SDL_Point Raquet_Point;
typedef char * PPF_Bank;

// PPF HEADER V1.0
extern const unsigned char PPFHEADER[8];

/* 
 * fixed bitmask we use to read CHR data 
 * (this is probably sloppy, but im not smart enough to know another way)
 */
extern const unsigned int ppfbitmask[8];

/* Load a PPF bank into memory. More info is in the wiki */
int Raquet_LoadPPFBank(PPF_Bank* targetarray, const char* dir);

/* Load a single-tile sprite. More info is in the wiki */
//typedef SDL_Texture * Raquet_CHR;

typedef struct Raquet_CHR {
    SDL_Texture * tex;
    int width;
    int height;
    int * data;
    Palette palette[3];
} Raquet_CHR;

Raquet_CHR Raquet_LoadCHR(PPF_Bank ppfbank, int id, Palette pal[3]);

/* Load a multi-tile sprite. More info is in the wiki */
Raquet_CHR Raquet_LoadCHRMult(PPF_Bank ppfbank, int * id, int xwrap, int ywrap, Palette palette[3]);

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
void Raquet_SwapCHRPalette(Raquet_CHR* chr, Palette pal[3]);

/* Destroy a CHR after we're done using it */
void Raquet_DestroyCHR(SDL_Texture * tex);

void Raquet_DrawPoint(Palette pal, int x, int y, int alpha);

void Raquet_DrawLine(Palette pal, int x1, int y1, int x2, int y2, int alpha);

/*
 ******************
 *     CAMERA     *
 ******************
 */

typedef struct Raquet_Camera {
    int x;
    int y;
} Raquet_Camera;

extern Raquet_Camera Camera;

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
    Raquet_CHR chr; // Current CHR
    Raquet_Point origin; // Our Orgigin Point (x, y) default is (0, 0)
    int width; // How wide we are (default is the width of the sprite)
    int height; // How tall we are (default is the height of the sprite)
    int angle; // angle of the object, rotated around its origin

    Palette color; // used in very specific circumstances (barriers in B-TRON)

    // collision info
    Raquet_BoundingBox bbox; // Bounding box

    SDL_RendererFlip flip;

} Actor;

Actor * Raquet_AllocateActor();

void Raquet_CreateActor(Actor * act, Raquet_CHR chr);

void Raquet_DestroyActor(Actor * act);

void Raquet_DrawActor(Actor * act);

int Raquet_ActorColliding(int x, int y, Actor * act1, Actor * act2);

#define RAQUET_GAME_ENGINE
#endif
