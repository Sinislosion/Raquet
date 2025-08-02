#include "Raquet.h"

/* PPF HEADER V1.0 */
const unsigned char Raquet_PPFHeader[8] = {
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
const unsigned int Raquet_PPFBitmask[8] = {
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
    if (SDL_IOFromFile(dir, "rb") != NULL) {
        SDL_IOStream* ppfdata = SDL_IOFromFile(dir, "rb");
        long long sizeoffile = SDL_SeekIO(ppfdata, 0, SDL_IO_SEEK_END);
        SDL_SeekIO(ppfdata, 0, SDL_IO_SEEK_SET);
        *targetarray = (char*)malloc(sizeoffile * sizeof(char));

        SDL_ReadIO(ppfdata, *targetarray, sizeoffile / sizeof(char));

        for (int i = 0; i < 8; i++) {
            if ((*targetarray)[i] != Raquet_PPFHeader[i]) {
                #ifdef PRINT_DEBUG
                    printf("WARNING: HEADER DATA DOES NOT MATCH\n");
                    fflush(stdout);
                #endif
            }

        }

        SDL_CloseIO(ppfdata);

        #ifdef PRINT_DEBUG
            printf("Loaded PPF Data at: %s successfully\n", dir);
            fflush(stdout);
        #endif

        return 1;

    } else {

        #ifdef PRINT_DEBUG
            printf("Failed to load PPF at: %s\n", dir);
            fflush(stdout);
        #endif

        exit(1);
        return 0;
    }
}

