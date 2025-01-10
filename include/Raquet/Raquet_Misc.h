#ifndef RAQUET_MISC_H
#define RAQUET_MISC_H

/** @file */

/**
 * @brief Toggles the cursor on or off
 * @param toggle 1 turns on the cursor, 0 turns it off.
 */
extern void Raquet_ShowCursor(int toggle);

/**
 * @brief Returns a string to the absolute path to a file within the "assets" folder
 * @param relativepath The relative path to the file from the assets folder
 */
extern const char* Raquet_AbsoluteToAsset(const char* relativepath);

#endif
