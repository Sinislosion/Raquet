
#include <gme/gme.h>
typedef short sample_t;
typedef struct Music_Player {
    sample_t* scope_buf;
    long sample_rate;
    int scope_buf_size;
    gme_info_t* track_info_;
} Music_Player;

Music_Emu* gEmu_;
long sample_rate = 44100;
const int fill_rate = 80;
gme_info_t* track_info_ = NULL;
int audiopaused = 0;

// SDL STUFFS
typedef void (*sound_callback_t)( void* data, short* out, int count );
static sound_callback_t sound_callback;
static void* sound_callback_data;

static void sdl_callback( void* /* data */, Uint8* out, int count )
{
	if ( sound_callback )
		sound_callback( sound_callback_data, (short*) out, count / 2 );
}

static const char* sound_init( long sample_rate, int buf_size,
		sound_callback_t cb, void* data )
{
	sound_callback = cb;
	sound_callback_data = data;

	static SDL_AudioSpec as; // making static clears all fields to 0
	as.freq     = sample_rate;
	as.format   = AUDIO_S16SYS;
	as.channels = 2;
	as.callback = sdl_callback;
	as.samples  = buf_size;
	if ( SDL_OpenAudio( &as, 0 ) < 0 )
	{
		const char* err = SDL_GetError();
		if ( !err )
			err = "Couldn't open SDL audio";
		return err;
	}

	return 0;
}

static void sound_start()
{
	SDL_PauseAudio( 0 );
}

static void sound_stop()
{
	SDL_PauseAudio( 1 );

	// be sure audio thread is not active
	SDL_LockAudio();
	SDL_UnlockAudio();
}

static void sound_cleanup()
{
	sound_stop();
	SDL_CloseAudio();
}

// NSF STUFFS

void fill_buffer( void* data, sample_t* out, int count )
{
	Music_Player* self = (Music_Player*) data;
	if ( gEmu_ )
	{
		if ( gme_play( gEmu_, count, out ) ) { } // ignore error

		if ( self->scope_buf )
			memcpy( self->scope_buf, out, self->scope_buf_size * sizeof *self->scope_buf );
	}
}

gme_err_t nsf_init( long rate, Music_Player* player)
{
    sample_rate = rate;
    int min_size = sample_rate * 2 / fill_rate;
    int buf_size = 512;
    while (buf_size < min_size)
        buf_size *= 2;
    return sound_init(sample_rate, buf_size, fill_buffer, player);
}

gme_err_t load_nsf(const char* path , int by_mem, Music_Player* player)
{

    if ( by_mem )
	{
		printf( "Loading file %s by memory...\n", path );
		fflush( stdout );

		SDL_RWops *file = SDL_RWFromFile(path, "rb");

		if ( !file )
			return "Can't load the file";

		size_t fileSize = SDL_RWsize(file);
		Uint8 *buf = (Uint8 *)SDL_malloc(fileSize);

		if ( !buf )
			return "Out of memory";

		if ( SDL_RWread(file, buf, 1, fileSize) < fileSize)
		{
			SDL_free(buf);
			SDL_RWclose(file);
			return "Can't read a file";
		}

		SDL_RWclose(file);

		gme_open_data( buf, (long)fileSize, &gEmu_, player->sample_rate );
		SDL_free(buf);
	}
	else
	{
		printf( "Loading file %s by file path...\n", path );
		fflush( stdout );
		gme_open_file( path, &gEmu_, player->sample_rate );
	}

    return 0;
}

gme_err_t nsf_start_track( int track, Music_Player* player )
{
	if ( gEmu_ )
	{
		gme_free_info( player->track_info_ );
		player->track_info_ = NULL;
		gme_track_info( gEmu_, &player->track_info_, track );

		// Sound must not be running when operating on emulator
		sound_stop();
		gme_start_track( gEmu_, track );

		// Calculate track length
		if ( player->track_info_->length <= 0 )
			player->track_info_->length = player->track_info_->intro_length +
						player->track_info_->loop_length * 2;

		/*if ( track_info_->length <= 0 )
			track_info_->length = (long) (2.5 * 60 * 1000);
		gme_set_fade_msecs( gEmu_, track_info_->length, 8000 );*/

		audiopaused = 0;
		sound_start();
	}
	return 0;
}

