#ifndef RAQUET_PPF_H
#define RAQUET_PPF_H

/** @file */

/** @brief The header for the PPF format */
extern const unsigned char Raquet_PPFHeader[8];

/** @brief The bitmask used for loading the individual bits from the PPF file */
extern const unsigned int Raquet_PPFBitmask[8];

/**
 * @brief Loads a PPF Bank into a PPF_Bank variable
 * @param targetarray The pointer to the PPF_Bank we want to load the file into
 * @param dir The path to the file
 */
int Raquet_LoadPPFBank(PPF_Bank* targetarray, const char* dir);

#endif
