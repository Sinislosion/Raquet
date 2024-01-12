#define SDL_MAIN_HANDLED
#include "SDL.h"
#include <gme/gme.h>

// WINDOW CONSTANTS
const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 240;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Rect gRectScrn = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_Event e;

/*
************************
*     AUDIO STUFFS     *
************************
* big thanks to gme btw, the audio library this part of the code relies on
* https://github.com/libgme/game-music-emu for more info
*/

typedef struct sample_t
{
    short* buf;
    int size;
} sample_t;

typedef struct Raquet_Sound
{
	Music_Emu* emu_;
	short* scope_buf;
	int paused;
	gme_info_t* track_info_;
	int scope_buf_size;
	long sample_rate;
} Raquet_Sound;

Raquet_Sound* gAudioPlayer;

typedef void (*sound_callback_t)(void* data, short* out, int count);
static sound_callback_t sound_callback;
static void* sound_callback_data;

static void sdl_callback( void* data, Uint8* out, int count )
{
	if (sound_callback)
	{
		sound_callback(sound_callback_data, (short*) out, count / 2);
	}
}

static const char* soundInit(long sample_rate, int buf_size, sound_callback_t cb, void* data)
{
	sound_callback = cb;
	sound_callback_data = data;
	static SDL_AudioSpec as; 
	as.freq     = sample_rate;
	as.format   = AUDIO_S16SYS;
	as.channels = 2;
	as.callback = sdl_callback;
	as.samples  = buf_size;
	if (SDL_OpenAudio( &as, 0 ) < 0)
	{
		const char* err = SDL_GetError();
		if ( !err )
		{
			err = "Couldn't open SDL audio";
		}
		return err;
	}
	return 0;
}

static void soundStart()
{
	SDL_PauseAudio( 0 );
}
static void soundStop()
{
	SDL_PauseAudio( 1 );

	SDL_LockAudio();
	SDL_UnlockAudio();
}
static void soundCleanup()
{
	soundStop();
	SDL_CloseAudio();
}

Raquet_Sound* newRaquetSound() 
{
	Raquet_Sound* player = (Raquet_Sound*)malloc(sizeof(Raquet_Sound));
	player->emu_ = NULL;
	player->scope_buf = NULL;
	player->paused = 0;
	player->track_info_ = NULL;
	if (player != NULL)
	{
    	printf("GME Initialized\n");
    	fflush(stdout);
	}
	return player;
}

void RaquetSound_FillBuffer(void* data, short* out, int count) 
{
	Raquet_Sound* self = (Raquet_Sound*) data;
	if (self->emu_)
	{
		gme_play(self->emu_, count, out);
		if (self->scope_buf)
		{
			memcpy( self->scope_buf, out, self->scope_buf_size * sizeof *self->scope_buf );
		}
	}
}

gme_err_t Raquet_InitSound(long rate) 
{
	gAudioPlayer->sample_rate = rate;
 	int min_size = gAudioPlayer->sample_rate * 2;
	int buf_size = 512;
	while (buf_size < min_size) { buf_size *= 2; }
	return soundInit(gAudioPlayer->sample_rate, buf_size, RaquetSound_FillBuffer, gAudioPlayer);
}

void RaquetSound_StopSound() 
{
	soundStop();
	gme_delete(gAudioPlayer->emu_);
	gAudioPlayer->emu_ = NULL;
}

void RaquetSound_DestroySound() 
{
	RaquetSound_StopSound();
	soundCleanup();
	gme_free_info(gAudioPlayer->track_info_);
	free(gAudioPlayer);
}

gme_err_t RaquetSound_LoadAudio(const char* path) 
{
	RaquetSound_StopSound();
	gme_open_file(path, &(gAudioPlayer->emu_), gAudioPlayer->sample_rate);
	char m3u_path [256 + 5];
	strncpy(m3u_path, path, 256);
	m3u_path [256] = 0;
	char* p = strrchr(m3u_path, '.');
	if (!p)
	{
		p = m3u_path + strlen(m3u_path);
	}
	strcpy(p, ".m3u");
	gme_load_m3u(gAudioPlayer->emu_, m3u_path);
	return 0;
}

/*
 *	Return the amount of tracks in the currently playing file
 *	@return int				returns the integer count of the tracks in the file
*/
int RaquetSound_TrackCount() {
	return gAudioPlayer->emu_ ? gme_track_count(gAudioPlayer->emu_) : 0;
}

/*
 *	Start the track in a given gme-compatible file
 *	@param int track 		The track ID in the file
 *	@param int fadeout 		0 to not fadeout at the end, anything else to fadeout
 *	@return gme_err_t		returns nothing because this is basically a void
*/
gme_err_t RaquetSound_StartTrack(int track, int fadeout) {
	if (gAudioPlayer->emu_)
	{
		gme_free_info(gAudioPlayer->track_info_);
		gAudioPlayer->track_info_ = NULL;
		gme_track_info(gAudioPlayer->emu_, &(gAudioPlayer->track_info_), track);

		soundStop();
		gme_start_track(gAudioPlayer->emu_, track);

		if ( gAudioPlayer->track_info_->length <= 0 )
		{
			gAudioPlayer->track_info_->length = gAudioPlayer->track_info_->intro_length + 
				gAudioPlayer->track_info_->loop_length * 2;
		}
		              
		if ( gAudioPlayer->track_info_->length <= 0 )
		{
			gAudioPlayer->track_info_->length = (long) (2.5 * 60 * 1000);
		}
		
		if (fadeout)
		{
			gme_set_fade( gAudioPlayer->emu_, gAudioPlayer->track_info_->length );
		}
		gAudioPlayer->paused = 0;
		soundStart();
	}
	return 0;
}

void RaquetSound_PauseSound(int b) 
{
	gAudioPlayer->paused = b;
	if (b) { soundStop(); } else { soundStart(); }
}

void RaquetSound_SuspendSound() 
{
	if (!gAudioPlayer->paused) {soundStop();}
}

void RaquetSound_ResumeSound() 
{
	if (!gAudioPlayer->paused) {soundStart();}
}

int RaquetSound_TrackEnded() 
{
	return gAudioPlayer->emu_ ? gme_track_ended( gAudioPlayer->emu_ ) : 0;
}

void RaquetSound_SetStereoDepth(double tempo) 
{
	RaquetSound_SuspendSound();
	gme_set_stereo_depth( gAudioPlayer->emu_, tempo );
	RaquetSound_ResumeSound();
}

void RaquetSound_EnableAccuracy(int b) 
{
	RaquetSound_SuspendSound();
	gme_enable_accuracy( gAudioPlayer->emu_, b );
	printf("Audio accuracy set to: %d\n", b);
	fflush(stdout);
	RaquetSound_ResumeSound();
}

void RaquetSound_SetTempo(double tempo) 
{
	RaquetSound_SuspendSound();
	gme_set_tempo( gAudioPlayer->emu_, tempo );
	RaquetSound_ResumeSound();
}

void RaquetSound_MuteVoices(int mask) {
	RaquetSound_SuspendSound();
	gme_mute_voices( gAudioPlayer->emu_, mask );
	gme_ignore_silence( gAudioPlayer->emu_, mask != 0 );
	RaquetSound_ResumeSound();
}

void RaquetSound_SetFadeout(int fade) 
{
	gme_set_fade( gAudioPlayer->emu_, fade ? gAudioPlayer->track_info_->length : -1 );
}

/*
 ************************
 *     PALETTE INFO     *
 ************************
*/

typedef Uint32 Palette;

const Palette PAL00 = {0x6A6A6AFF};
const Palette PAL01 = {0x001380FF};
const Palette PAL02 = {0x1E008AFF};
const Palette PAL03 = {0x39007AFF};
const Palette PAL04 = {0x550056FF};
const Palette PAL05 = {0x5A0018FF};
const Palette PAL06 = {0x4F1000FF};
const Palette PAL07 = {0x3D1C00FF};
const Palette PAL08 = {0x253200FF};
const Palette PAL09 = {0x003D00FF};
const Palette PAL0A = {0x004000FF};
const Palette PAL0B = {0x003924FF};
const Palette PAL0C = {0x002E55FF};
const Palette PAL0D = {0x000000FF};
const Palette PAL0E = {0x000000FF};
const Palette PAL0F = {0x00000000};	// TRANSPARENCY

const Palette PAL10 = {0xB9BCB9FF};
const Palette PAL11 = {0x1850C7FF};
const Palette PAL12 = {0x4B30E3FF};
const Palette PAL13 = {0x9B22D6FF};
const Palette PAL14 = {0x951FA9FF};
const Palette PAL15 = {0x9D285CFF};
const Palette PAL16 = {0x983700FF};
const Palette PAL17 = {0x7F4C00FF};
const Palette PAL18 = {0x5E6400FF};
const Palette PAL19 = {0x227700FF};
const Palette PAL1A = {0x027E02FF};
const Palette PAL1B = {0x007645FF};
const Palette PAL1C = {0x006E8AFF};
const Palette PAL1D = {0x000000FF};
const Palette PAL1E = {0x000000FF};
const Palette PAL1F = {0x00000000};	// TRANSPARENCY

const Palette PAL20 = {0xFFFFFFFF};
const Palette PAL21 = {0x68A6FFFF};
const Palette PAL22 = {0x8C9CFFFF};
const Palette PAL23 = {0xB586F4FF};
const Palette PAL24 = {0xD975FDFF};
const Palette PAL25 = {0xE377B9FF};
const Palette PAL26 = {0xE58D68FF};
const Palette PAL27 = {0xD49D29FF};
const Palette PAL28 = {0xB3AF0CFF};
const Palette PAL29 = {0x7BC211FF};
const Palette PAL2A = {0x55CA47FF};
const Palette PAL2B = {0x46CB81FF};
const Palette PAL2C = {0x47C1C5FF};
const Palette PAL2D = {0x4A4D4AFF};
const Palette PAL2E = {0x000000FF};
const Palette PAL2F = {0x00000000};	// TRANSPARENCY

const Palette PAL30 = {0xFFFFFFFF};
const Palette PAL31 = {0xCCEAFFFF};
const Palette PAL32 = {0xDDDEFFFF};
const Palette PAL33 = {0xECDAFFFF};
const Palette PAL34 = {0xF8D7FEFF};
const Palette PAL35 = {0xFCD6D5FF};
const Palette PAL36 = {0xFDDBCFFF};
const Palette PAL37 = {0xF9E7B5FF};
const Palette PAL38 = {0xF1F0AAFF};
const Palette PAL39 = {0xDAFAA9FF};
const Palette PAL3A = {0xC9FFBCFF};
const Palette PAL3B = {0xC3FBD7FF};
const Palette PAL3C = {0xC4F6F6FF};
const Palette PAL3D = {0xBEC1BEFF};
const Palette PAL3E = {0x000000FF};
const Palette PAL3F = {0x00000000};	// TRANSPARENCY

const Palette PALINVALID = {0xFF00FFFF};

/*
 *********************************
 *     CUSTOM MATH FUNCTIONS     *
 *********************************
*/

int sign(float comp) 
{
	return ((0) < comp) - (comp < (0));
}

/*
 ***************************
 *     INPUT FUNCTIONS     *
 ***************************
*/


/*
 * NES KEYMAP
 * UP, DOWN, LEFT, RIGHT
 * B, A, SELECT, START
 * 
 * If a key is being pressed, it is 1
 * If it is not being pressed, it is 0
 * TODO: make this rely on a configuration file for key remapping
 */

int key_up = 0, key_down = 0, key_left = 0, key_right = 0, 
key_a = 0, key_b = 0, key_select = 0, key_start = 0,
key_zoomin = 0, key_zoomout = 0;

void handleInput(SDL_Event e)
{ 
	switch (e.type)
	{
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym) 
			{
				case SDLK_UP:
					key_up = 1;
				break;

				case SDLK_DOWN:
					key_down = 1;
				break;
					
				case SDLK_LEFT:
					key_left = 1;
				break;
				
				case SDLK_RIGHT:
					key_right = 1;
				break;

				case SDLK_x:
					key_a = 1;
				break;

				case SDLK_w:
					key_zoomin = 1;
				break;

				case SDLK_s:
					key_zoomout = 1;
				break;
			}
		break;

		case SDL_KEYUP:
			switch(e.key.keysym.sym) 
			{
				case SDLK_UP:
					key_up = 0;
				break;

				case SDLK_DOWN:
					key_down = 0;
				break;
					
				case SDLK_LEFT:
					key_left = 0;
				break;
				
				case SDLK_RIGHT:
					key_right = 0;
				break;

				case SDLK_x:
					key_a = 0;
				break;

				case SDLK_w:
					key_zoomin = 0;
				break;

				case SDLK_s:
					key_zoomout = 0;
				break;
			}
		break;
		
	}
}


/*
 ****************************
 *     RAQUET FUNCTIONS     *
 ****************************
*/

// DOG FUNCTIONS
void runthedog(); // put this somewhere in your program, the default code to run
void createthedog(); // put all your creation code for the program here

int initsdl()
{
	
	// Init flag
	int succ = 1;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("FAILED TO INITIALIZE SDL VIDEO.\n");
		succ = 0;
	}
	else
	{
		// Create window
		gWindow = SDL_CreateWindow("Raquet Game Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (gWindow == NULL)
		{
			printf("rip :p\n");
			succ = 0;
		}
		else
		{
			printf("SDL Initialized\n");
			fflush(stdout);
			// Init Window Renderer
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			SDL_RenderSetViewport(gRenderer, NULL);
			SDL_RenderSetLogicalSize(gRenderer, 256, 240);
			SDL_GL_SetSwapInterval(-1);	// Uncomment this for VSYNC
			SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
			
		}

	}

	return succ;
	
}

int Raquet_Init()
{	
	int ready = 1;
	if (!initsdl())
	{
		printf("Failed to Initialize SDL\n");
		ready = 0;
	}

	// Init Audio
	gAudioPlayer = newRaquetSound();
	Raquet_InitSound(44800);

	// PUT YOUR SETUP CODE HERE
	
	return ready;
}

void Raquet_SetPalette(Palette dest[3], Uint32 pal1, Uint32 pal2, Uint32 pal3)
{
	dest[0] = pal1;
	dest[1] = pal2;
	dest[2] = pal3;
}

void quitit()
{
	printf("Done.\n");
	// Deallocate surface
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	soundStop();
	soundCleanup();
	gme_free_info( gAudioPlayer->track_info_ );

	SDL_Quit();
}

void Raquet_Clear(Palette pal)
{
	Uint32 palr  = (pal >> 24) & 0x000000FF;
	Uint32 palg  = (pal >> 16) & 0x000000FF;
	Uint32 palb  = (pal >> 8) & 0x000000FF;
	SDL_SetRenderDrawColor(gRenderer, palr, palg, palb, 255);
	SDL_RenderFillRect(gRenderer, NULL); 
}

void Raquet_DrawRectangle(int x1, int y1, int width, int height, Palette pal, int alpha, int fill)
{
	SDL_Rect rect = {x1, y1, width,height};
	Uint32 palr  = (pal >> 24) & 0x000000FF;
	Uint32 palg  = (pal >> 16) & 0x000000FF;
	Uint32 palb  = (pal >> 8) & 0x000000FF;

	SDL_SetRenderDrawColor(gRenderer, palr, palg, palb, alpha);
	
	switch (fill)
	{
		default:
			SDL_RenderDrawRect(gRenderer, &rect);
		break;

		case 1:
			SDL_RenderFillRect(gRenderer, &rect);
		break;
	}

}

void Raquet_Update()
{
	SDL_UpdateWindowSurface(gWindow);
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(gRenderer);
	SDL_RenderClear(gRenderer);
}

void Raquet_Main() {
	if (!Raquet_Init())
	{
		printf("Failed to Initialize\n");
	}
	else
	{
		printf("Raquet Initialized\n");
		fflush(stdout);
		createthedog();
		
		// hack 
		int quit = 0; 
		while(quit == 0)
		{ 
			while(SDL_PollEvent(&e))
			{
				handleInput(e);
				if(e.type == SDL_QUIT)
				{
					quit = 1;
				}
				
			}

			// do our game stuff
			runthedog();
			
		}
			
	}
		
	quitit();

}

/*
 *************************
 *     PPF FUNCTIONS     *
 *************************
*/

// the array we store our data in, with a max file size of 8KB
char CHARDATASET[8192];

// PPF HEADER V1.0
const unsigned char PPFHEADER[8] =
{
    0x50, 0x50, 0x46, 0x76, 0x01, 0x00, 0x00, 0x00
};

const unsigned int ppfbitmask[8] =
{
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 
};

int LoadPPFData(const char* dir)
{
	
	SDL_RWops* ppfdata = SDL_RWFromFile(dir, "rb");

	// check if ppf data is a valid directory
	if (ppfdata != NULL) 
	{
		
		SDL_RWread(ppfdata, CHARDATASET, 8, 1024);

		for (int i = 0; i < 8; i++)
		{
			if (CHARDATASET[i] != PPFHEADER[i])
			{
				printf("WARNING HEADER DATA DOES NOT MATCH\n");
			}
			
		} 

		SDL_RWclose(ppfdata);
		return 1;
		
	} 
	else 
	{
		printf("Failed to load PPF");
		return 0;
	}

}

// @function Load a CHR To an SDL Texture
// @param PPF index starting at 0
// @param Palette of 3 colors
typedef SDL_Texture* Raquet_CHR;
Raquet_CHR LoadCHR(int id, Palette palette[3])
{
	
	SDL_Texture* tex = SDL_CreateTexture(
		gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 8, 8);

	Uint32 pixels[64];

	/*
	 * For every y row, write a pixel that corresponds to the palette
	 * If our first palette index is 0, and our second palette index is 0, write transparency
	 * If our first palette index is 1, and our second palette index is 0, write color 1
	 * If our first palette index is 0, and our second palette index is 1, write color 2
	 * If our first palette index is 1, and our second palette index is 1, write color 3
	*/
	
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			int dest = x + (y * 8);
			int index = y + 8 + (id * 16);
			int index2 = y + 16 + (id * 16);

			int check1 = sign(CHARDATASET[index] & ppfbitmask[x]);
			int check2 = sign(CHARDATASET[index2] & ppfbitmask[x]);
			int place =  check1 +  check2;

			switch (place)
			{
				case 0:
					pixels[dest] = PAL0F;
				break;

				case 1:
					switch (check1) 
					{
						case 1:
							pixels[dest] = palette[0];
						break;
						
						default:
							pixels[dest] = palette[1];
						break;
					}
				break;

				case 2:
					pixels[dest] = palette[2];
				break;
			}

		}
		
	}

	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	SDL_UpdateTexture(tex, NULL, pixels, 8 * sizeof(Palette));
	
	return tex;
}

void PlaceCHR(SDL_Texture* tex, int x, int y) {
	SDL_Rect dstrect;
		dstrect.x = x;
		dstrect.y = y;
		dstrect.w = 8;
		dstrect.h = 8;

	SDL_RenderCopy(gRenderer, tex, NULL, &dstrect);
}

void DestroyCHR(SDL_Texture* tex)
{
	SDL_DestroyTexture(tex);
}
