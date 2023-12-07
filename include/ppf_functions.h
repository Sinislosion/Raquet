// the array we store our data in, with a max file size of 8KB
char CHARDATASET[8192];

// load the ppf file for the program and store it into a char
int LoadPPFData(const char* dir);

// load a ppf tile as a texture
SDL_Texture* LoadCHR(int id, NES palette[3]);

// place an 8x8 texture
void PlaceCHR(SDL_Texture* tex, int x, int y);

