/* WINDOW CONSTANTS */
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240
#define SCREEN_SCALE 3
#define ALLOW_FULLSCREEN

/* RENDERING SETTINGS */
#define INTEGER_SCALING
#define VSYNC
//#define DELTA_TIME
//#define BACKGROUND_CLEARING_COLOR

/* MISC SETTINGS */
#define GAME_NAME "Raquet Game Engine"  // Window Title
#define AUDIO_SAMPLE_RATE 44100         // How high quality our sound is, decrease if you want moldy mp3 sound :)
#define FRAMERATE_CAP 60                // Internal framerate cap. Disable VSYNC if this is higher than 60

/* DEBUG SETTINGS */
#define VISUALIZE_BBOX
#define PRINT_DEBUG

#ifdef VISUALIZE_BBOX
    #define BBOX_DEBUG_COLOR    0xFF00FFFF
#endif
