// the array we store our data in, with a max file size of 8KB
char CHARDATASET[8192];

// load the ppf file for the program and store it into a char
void LoadPPFData(const char* dir);

// place 1 8x8 tile
void PlaceCHR(int id, uint8_t x, uint8_t y, NES palette[3]);

// place 4 8x8 tiles in a 16x16 formation
void PlaceCHR16(int id, uint8_t x, uint8_t y, NES palette[3]);
