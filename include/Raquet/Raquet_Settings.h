#ifndef RAQUET_SETTINGS_H
#define RAQUET_SETTINGS_H

/** @file */

/** @brief The internal screen width */
#define SCREEN_WIDTH 640

/** @brief The internal screen height */
#define SCREEN_HEIGHT 360

/** @brief The integer scale of the window at startup */
#define SCREEN_SCALE 3

/** @brief Whether or not to allow the user to fullscreen by pressing F11 */
#define ALLOW_FULLSCREEN

/** @brief Whether or not to force Integer Scaling */
#define INTEGER_SCALING

/** @brief Whether or not to use Hardware VSYNC */
#define VSYNC

// Whether or not to enable the Raquet_DeltaTime variable
//#define DELTA_TIME

// Whether or not to make Raquet_Clear also change the window background */
//#define BACKGROUND_CLEARING_COLOR

/** @brief The initial window title */
#define GAME_NAME "Raquet Game Engine"

/** @brief The audio sample rate. Lower value gives lower quality sound */
#define AUDIO_SAMPLE_RATE 44100

/** @brief The internal framerate cap. Can be disabled if using Delta Time */
#define FRAMERATE_CAP 60

// When defined, allows you to see any actors' bounding boxes */
//#define VISUALIZE_BBOX

/** @brief When enabled, will output debug messages to the console */
#define PRINT_DEBUG

#ifdef VISUALIZE_BBOX
    #define BBOX_DEBUG_COLOR    0xFF00FFFF
#endif

#endif
