/* If you for some reason include Raquet.h several times, don't worry, I've got your back. */
#ifndef RAQUET_GAME_ENGINE
#define SDL_MAIN_HANDLED

/* Comment this out if you dont want fullscreen */
#define ALLOW_FULLSCREN

/* other headers we need */
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <gme/gme.h>  // LGPL v2.0! Thank you!

/* WINDOW CONSTANTS */
#define SCREEN_WIDTH	      480
#define SCREEN_HEIGHT	      270
#define SCREEN_SCALE	      3
const double FRAMERATE_CAP = 144;
#define WINDOW_TITLE        "Raquet Game Engine"
#define AUDIO_SAMPLE_RATE   44100

SDL_Window* gWindow;
uint8_t gFullscreen = -1;
SDL_Renderer* gRenderer;

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
	SDL_PauseAudio(0);
}
static void soundStop()
{
  SDL_PauseAudio(1);

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
// Digital Prime Palette
const Palette PAL00 = {0x696969FF};
const Palette PAL01 = {0x00148FFF};
const Palette PAL02 = {0x1E029BFF};
const Palette PAL03 = {0x3F008AFF};
const Palette PAL04 = {0x600060FF};
const Palette PAL05 = {0x660017FF};
const Palette PAL06 = {0x570D00FF};
const Palette PAL07 = {0x451B00FF};
const Palette PAL08 = {0x243400FF};
const Palette PAL09 = {0x004200FF};
const Palette PAL0A = {0x004500FF};
const Palette PAL0B = {0x003C1FFF};
const Palette PAL0C = {0x00315CFF};
const Palette PAL0D = {0x000000FF};
const Palette PAL0E = {0x000000FF};
const Palette PAL0F = {0x00000000};	// TRANSPARENCY

const Palette PAL10 = {0xAFAFAFFF};
const Palette PAL11 = {0x0F51DDFF};
const Palette PAL12 = {0x442ff3FF};
const Palette PAL13 = {0x7220E2FF};
const Palette PAL14 = {0xA319B3FF};
const Palette PAL15 = {0xAE1C51FF};
const Palette PAL16 = {0xA43400FF};
const Palette PAL17 = {0x884D00FF};
const Palette PAL18 = {0x676D00FF};
const Palette PAL19 = {0x208000FF};
const Palette PAL1A = {0x008B00FF};
const Palette PAL1B = {0x007F42FF};
const Palette PAL1C = {0x006C97FF};
const Palette PAL1D = {0x000000FF};
const Palette PAL1E = {0x000000FF};
const Palette PAL1F = {0x00000000};	// TRANSPARENCY

const Palette PAL20 = {0xFFFFFFFF};
const Palette PAL21 = {0x65AAFFFF};
const Palette PAL22 = {0x8C96FFFF};
const Palette PAL23 = {0xB983FFFF};
const Palette PAL24 = {0xDD6FFFFF};
const Palette PAL25 = {0xEA6FBDFF};
const Palette PAL26 = {0xEB8466FF};
const Palette PAL27 = {0xDCA21FFF};
const Palette PAL28 = {0xBAB403FF};
const Palette PAL29 = {0x7ECB07FF};
const Palette PAL2A = {0x54D33EFF};
const Palette PAL2B = {0x3CD284FF};
const Palette PAL2C = {0x3EC7CCFF};
const Palette PAL2D = {0x4B4B4BFF};
const Palette PAL2E = {0x000000FF};
const Palette PAL2F = {0x00000000};	// TRANSPARENCY

const Palette PAL30 = {0xFFFFFFFF};
const Palette PAL31 = {0xBDE2FFFF};
const Palette PAL32 = {0xCECFFFFF};
const Palette PAL33 = {0xE6C2FFFF};
const Palette PAL34 = {0xF6bCFFFF};
const Palette PAL35 = {0xF9C2EDFF};
const Palette PAL36 = {0xFACFC6FF};
const Palette PAL37 = {0xF8DEACFF};
const Palette PAL38 = {0xEEE9A1FF};
const Palette PAL39 = {0xD0F59FFF};
const Palette PAL3A = {0xBBF5AFFF};
const Palette PAL3B = {0xB3F5CDFF};
const Palette PAL3C = {0xB9EDF0FF};
const Palette PAL3D = {0xB9B9B9FF};
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
const Uint8* sdlkeys;
Uint8 prevkeys[322];

/* Check if this key is being held down */
int Raquet_KeyCheck(SDL_Scancode nkey)
{
  return sdlkeys[nkey];
}

/* Will only return 1 if the key has been pressed down for 1 frame */
int Raquet_KeyCheck_Pressed(SDL_Scancode nkey)
{ 
  int check = (prevkeys[nkey] != sdlkeys[nkey] && sdlkeys[nkey] != 0);
  prevkeys[nkey] = sdlkeys[nkey];
  return check;
}

/* Will only return 1 if the key has been released for 1 frame */
int Raquet_KeyCheck_Released(SDL_Scancode nkey)
{ 
	int check = (prevkeys[nkey] != sdlkeys[nkey] && sdlkeys[nkey] != 1);
  prevkeys[nkey] = sdlkeys[nkey];
  return check;
}

/*
 ****************************
 *     RAQUET FUNCTIONS     *
 ****************************
*/

// DOG FUNCTIONS
void runthedog(); // put this somewhere in your program, the default code to run
void createthedog(); // put all your creation code for the program here

// FRAMERATE
int tick1;
int tick2;
float delta_time;

int initsdl()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("FAILED TO INITIALIZE SDL VIDEO.\n");
	  return 0;	
  }
	else
	{
		// Create window
		gWindow = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (gWindow == NULL)
		{
			printf("FAILED TO CREATE SDL WINDOW.\n");
			return 0;
    }
		else
		{
			printf("SDL Initialized\n");
			fflush(stdout);
			// Init Window Renderer
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			SDL_RenderSetViewport(gRenderer, NULL);
			SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
			SDL_GL_SetSwapInterval(1);	// VSYNC
			SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
			
		}

	}

	return 1;
	
}

/* Initialize the Raquet Engine */
int Raquet_Init()
{	
	sdlkeys = SDL_GetKeyboardState(NULL);
	if (!initsdl())
	{
		printf("Failed to Initialize SDL\n");
		return 0;
	}

	// Init Audio
	gAudioPlayer = newRaquetSound();
	Raquet_InitSound(AUDIO_SAMPLE_RATE);

	return 1;
}

/* Set a palette variable within runtime */
void Raquet_SetPalette(Palette dest[3], Uint32 pal1, Uint32 pal2, Uint32 pal3)
{
	dest[0] = pal1;
	dest[1] = pal2;
	dest[2] = pal3;
}

/* I'm actually not sure what you can use this for yet, but you can use it */
void Raquet_SetDrawColor(Palette pal, int alpha)
{
	Uint32 palr  = (pal >> 24) & 0x000000FF;
	Uint32 palg  = (pal >> 16) & 0x000000FF;
	Uint32 palb  = (pal >> 8) & 0x000000FF;
	SDL_SetRenderDrawColor(gRenderer, palr, palg, palb, alpha);
}

/* Clear the screen with a solid color */
void Raquet_Clear(Palette pal)
{
	Raquet_SetDrawColor(pal, 255);
	SDL_RenderFillRect(gRenderer, NULL); 
}

/* Draw a rectangle (x position, y position, width, height, color, alpha, fill) */
void Raquet_DrawRectangle(int x1, int y1, int width, int height, Palette pal, int alpha, int fill)
{
	SDL_Rect rect = {x1, y1, width,height};
	Raquet_SetDrawColor(pal, alpha);
	
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

/* This is used to update the Window within the Raquet_Main function */
void Raquet_Update()
{
	SDL_UpdateWindowSurface(gWindow);
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(gRenderer);
	SDL_RenderClear(gRenderer);
}

/* The main Raquet function. Everything runs from here. */
void Raquet_Main() {
	if (!Raquet_Init())
	{
		printf("Failed to Initialize Raquet\n");
    return;
	}
	else
	{
		printf("Raquet Initialized\n");
		fflush(stdout);
		createthedog(); // run our creation code
		
		/* SDL While loop, and frame counter */ 
		while(1)
		{ 
      tick1 = SDL_GetTicks64();
			delta_time = tick1 - tick2;
      double sleep_time = ((1000/FRAMERATE_CAP));
			tick2 = tick1;
			while(SDL_PollEvent(&e))
			{
        switch (e.type)
        {
          case SDL_QUIT:
            return;
          break;
        }
			}
        // If we allow fullscreen, then let us use fullscreen with F11
				#ifdef ALLOW_FULLSCREN 
				  if (Raquet_KeyCheck_Pressed(SDL_SCANCODE_F11))
				  {
            gFullscreen = -gFullscreen;
          }

          switch (gFullscreen)
          {
           default:
              SDL_SetWindowFullscreen(gWindow, 0);
            break;

            case 1:
              SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
            break;
          }
			#endif

			runthedog();  // Main loop event 
      SDL_Delay(sleep_time); // Give back some CPU time, wait for next frame
			
		}
			
	}	

}

/*
 *************************
 *     PPF FUNCTIONS     *
 *************************
*/

typedef SDL_Point Raquet_Point;
typedef char* PPF_Bank;

// PPF HEADER V1.0
const unsigned char PPFHEADER[8] =
{
    0x50, 0x50, 0x46, 0x76, 0x01, 0x00, 0x00, 0x00
};

/* 
 * fixed bitmask we use to read CHR data 
 * (this is probably sloppy, but im not smart enough to know another way)
*/
const unsigned int ppfbitmask[8] =
{
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 
};

/* Load a PPF bank into memory. More info is in the wiki */
int LoadPPFBank(PPF_Bank* targetarray, const char* dir)
{
	
	SDL_RWops* ppfdata = SDL_RWFromFile(dir, "rb");
	
	// check if ppf data is a valid directory
	if (ppfdata != NULL) 
	{
		long long sizeoffile = SDL_RWseek(ppfdata, 0, RW_SEEK_END);
		SDL_RWseek(ppfdata, 0, RW_SEEK_SET);
		*targetarray = (char*)malloc(sizeoffile * sizeof(char));
		
		SDL_RWread(ppfdata, *targetarray, 8, 1024);

		for (int i = 0; i < 8; i++)
		{
			if ((*targetarray)[i] != PPFHEADER[i])
			{
				printf("WARNING HEADER DATA DOES NOT MATCH\n");
				fflush(stdout);
			}
			
		} 
		
		SDL_RWclose(ppfdata);
		printf("Loaded PPF Data at: %s successfully\n", dir);
		fflush(stdout);
		return 1;
		
	} 
	else 
	{
		printf("Failed to load PPF");
		return 0;
	}

}

/* Load a single-tile sprite. More info is in the wiki */
typedef SDL_Texture* Raquet_CHR;
Raquet_CHR LoadCHR(PPF_Bank ppfbank, int id, Palette palette[3])
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
			int dest = x + (y * 8);				// dest is where in our array of pixels we will place the color
			int index = y + 8 + (id * 16);		// index is the byte in the file we're reading for palette data
			int index2 = y + 16 + (id * 16);	// index2 is the second byte in the file we read for palette data

			int check1 = ((ppfbank[index] & ppfbitmask[x]) > 0);
			int check2 = ((ppfbank[index2] & ppfbitmask[x]) > 0);
			int place =  check1 + check2;
			
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

/* Load a multi-tile sprite. More info is in the wiki */
Raquet_CHR LoadCHRMult(PPF_Bank ppfbank, int *id, int xwrap, int ywrap, Palette palette[3])
{
	int xsize = xwrap * 8;
	int ysize = ywrap * 8;

	SDL_Texture* tex = SDL_CreateTexture(
			gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, xsize, ysize);
	
	Uint32 pixels[xsize * ysize];

	for (int chrcounty = 0; chrcounty < ywrap; chrcounty++)
	{
		for (int y = 0; y < 8; y++)
		{
			for (int chrcountx = 0; chrcountx < xwrap; chrcountx++)
			{
				for (int x = 0; x < 8; x++)
				{
					int dest = x + (y * xsize) + (chrcountx * 8) + (chrcounty * (xsize * 8));
					int index = y + 8 + (id[chrcountx + (chrcounty * xwrap)] * 16);
					int index2 = y + 16 + (id[chrcountx + (chrcounty * xwrap)] * 16);
		
					int check1 = ((ppfbank[index] & ppfbitmask[x]) > 0);
					int check2 = ((ppfbank[index2] & ppfbitmask[x]) > 0);
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
		}
	}

	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	SDL_UpdateTexture(tex, NULL, pixels, xsize * sizeof(Palette));

	return tex;
	
}

/* Returns a struct of the width and height of the CHR, accessable with x and y */
Raquet_Point Raquet_SizeofCHR(SDL_Texture *tex)
{
	Raquet_Point size;
	SDL_QueryTexture(tex, NULL, NULL, &size.x, &size.y);
	return size;
}

/* Returns the width of a CHR */
int Raquet_WidthofCHR(SDL_Texture *tex)
{
	Raquet_Point size;
	SDL_QueryTexture(tex, NULL, NULL, &size.x, &size.y);
	return size.x;
}

/* returns the height of a CHR */
int Raquet_HeightofCHR(SDL_Texture *tex)
{
	Raquet_Point size;
	SDL_QueryTexture(tex, NULL, NULL, &size.x, &size.y);
	return size.y;
}

/* Place a CHR sprite (chr data, x, y) */
void PlaceCHR(SDL_Texture* tex, int x, int y) {
	SDL_Point size = Raquet_SizeofCHR(tex);
	SDL_Rect dstrect = {x, y, size.x, size.y};
	SDL_RenderCopy(gRenderer, tex, NULL, &dstrect);
}

/* Place a CHR sprite, with additional control (chr data, x, y, width in pixels, height in pixels) */
void PlaceCHR_ext(SDL_Texture* tex, int x, int y, int xsize, int ysize) {
	SDL_Rect dstrect = {x, y, xsize, ysize};
	SDL_RenderCopy(gRenderer, tex, NULL, &dstrect);
}

/* Destroy a CHR after we're done using it */
void DestroyCHR(SDL_Texture* tex)
{
	SDL_DestroyTexture(tex);
}

void Raquet_DrawPoint(Palette pal, int x, int y, int alpha)
{
	Raquet_SetDrawColor(pal, alpha);
	SDL_RenderDrawPoint(gRenderer, x, y);
}

/*
 *************************
 *     ACTORS SYSTEM     *
 *************************
*/

// TODO: Make a new example program to showcase and test the WIP Actor system

typedef struct Actor
{
	// where we are in virtual space
	int x;
	int y;

	// where we are on the screen
	int screen_x;
	int screen_y;

	// how we're displayed
	Raquet_CHR cur_image;	// Current CHR
	int origin_x;		// Our Orgigin Point (x) (default is 0, left)
	int origin_y;		// Our Orgigin Point (y) (default is 0, top)

	float width;		// How wide we are (default is 1, is multiplied)
	float height;		// How tall we are (default is 1, is multiplied)

	// collision info
	int bbox_x1;		// default is 0 (left)
	int bbox_x2;		// default is 0 (top)
	int bbox_y1;		// default is the virt width
	int bbox_y2;		// default is the virt width
	
} Actor;

void Raquet_CreateActor(Actor act)
{
	SDL_Point size = Raquet_SizeofCHR(act.cur_image);
	act.x = 0;
	act.y = 0;
	act.screen_x = 0;
	act.screen_y = 0;
	act.origin_x = 0;
	act.origin_y = 0;
	act.width = 1.0;
	act.height = 1.0;
	act.bbox_x1 = 0;
	act.bbox_y1 = 0;
	act.bbox_x2 = size.x;
	act.bbox_y2 = size.y;
}

void Raquet_DrawActor(Actor act)
{
	SDL_Point size = Raquet_SizeofCHR(act.cur_image);
	SDL_Rect dstrect = {act.screen_x, act.screen_y, size.x, size.y};
	SDL_RenderCopy(gRenderer, act.cur_image, NULL, &dstrect);
}

#define RAQUET_GAME_ENGINE
#endif
