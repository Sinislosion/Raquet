#include "Music_Player.h"

/* Copyright (C) 2005-2010 by Shay Green. Permission is hereby granted, free of
charge, to any person obtaining a copy of this software module and associated
documentation files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and
to permit persons to whom the Software is furnished to do so, subject to the
following conditions: The above copyright notice and this permission notice
shall be included in all copies or substantial portions of the Software. THE
SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

// This code was modified to work within C by JESSIE J. TAYLOR of Sinislosion

const int fill_rate = 80;

typedef void (*sound_callback_t)( void* data, short* out, int count );
static const char* sound_init( long sample_rate, int buf_size, sound_callback_t, void* data );
static void sound_start();
static void sound_stop();
static void sound_cleanup();

Music_Player* new_Music_Player() {
    Music_Player* player = (Music_Player*)malloc(sizeof(Music_Player));
    player->emu_ = NULL;
    player->scope_buf = NULL;
    player->paused = 0;
    player->track_info_ = NULL;
    return player;
}

gme_err_t Music_Player_init(Music_Player* player, long rate) {
    player->sample_rate = rate;
    int min_size = player->sample_rate * 2 / fill_rate;
    int buf_size = 512;
    while ( buf_size < min_size )
        buf_size *= 2;
    return sound_init( player->sample_rate, buf_size, Music_Player_fill_buffer, player );
}

void Music_Player_stop(Music_Player* player) {
    sound_stop();
    gme_delete( player->emu_ );
    player->emu_ = NULL;
}

void Music_Player_destroy(Music_Player* player) {
    Music_Player_stop(player);
    sound_cleanup();
    gme_free_info( player->track_info_ );
    free(player);
}

gme_err_t Music_Player_load_file(Music_Player* player, const char* path) {
    Music_Player_stop(player);
    gme_open_file( path, &(player->emu_), player->sample_rate );
    char m3u_path [256 + 5];
    strncpy( m3u_path, path, 256 );
    m3u_path [256] = 0;
    char* p = strrchr( m3u_path, '.' );
    if ( !p )
        p = m3u_path + strlen( m3u_path );
    strcpy( p, ".m3u" );
    if ( gme_load_m3u( player->emu_, m3u_path ) ) { } 
    return 0;
}

int Music_Player_track_count(const Music_Player* player) {
    return player->emu_ ? gme_track_count( player->emu_ ) : 0;
}

gme_err_t Music_Player_start_track(Music_Player* player, int track) {
    if ( player->emu_ )
    {
        gme_free_info( player->track_info_ );
        player->track_info_ = NULL;
        gme_track_info( player->emu_, &(player->track_info_), track );

        sound_stop();
        gme_start_track( player->emu_, track );

        if ( player->track_info_->length <= 0 )
            player->track_info_->length = player->track_info_->intro_length +
                        player->track_info_->loop_length * 2;
        if ( player->track_info_->length <= 0 )
            player->track_info_->length = (long) (2.5 * 60 * 1000);
        //gme_set_fade( player->emu_, player->track_info_->length );
        player->paused = 0;
        sound_start();
    }
    return 0;
}

void Music_Player_pause(Music_Player* player, int b) {
    player->paused = b;
    if ( b )
        sound_stop();
    else
        sound_start();
}

void Music_Player_suspend(Music_Player* player) {
    if ( !player->paused )
        sound_stop();
}

void Music_Player_resume(Music_Player* player) {
    if ( !player->paused )
        sound_start();
}

int Music_Player_track_ended(const Music_Player* player) {
    return player->emu_ ? gme_track_ended( player->emu_ ) : 0;
}

void Music_Player_set_stereo_depth(Music_Player* player, double tempo) {
    Music_Player_suspend(player);
    gme_set_stereo_depth( player->emu_, tempo );
    Music_Player_resume(player);
}

void Music_Player_enable_accuracy(Music_Player* player, int b) {
    Music_Player_suspend(player);
    gme_enable_accuracy( player->emu_, b );
    Music_Player_resume(player);
}

void Music_Player_set_tempo(Music_Player* player, double tempo) {
    Music_Player_suspend(player);
    gme_set_tempo( player->emu_, tempo );
    Music_Player_resume(player);
}

void Music_Player_mute_voices(Music_Player* player, int mask) {
    Music_Player_suspend(player);
    gme_mute_voices( player->emu_, mask );
    gme_ignore_silence( player->emu_, mask != 0 );
    Music_Player_resume(player);
}

void Music_Player_set_fadeout(Music_Player* player, int fade) {
    gme_set_fade( player->emu_, fade ? player->track_info_->length : -1 );
}

void Music_Player_fill_buffer(void* data, short* out, int count) {
    Music_Player* self = (Music_Player*) data;
    if ( self->emu_ )
    {
        if ( gme_play( self->emu_, count, out ) ) { }
        if ( self->scope_buf )
            memcpy( self->scope_buf, out, self->scope_buf_size * sizeof *self->scope_buf );
    }
}

#include "SDL.h"
static sound_callback_t sound_callback;
static void* sound_callback_data;
static void sdl_callback( void* data, Uint8* out, int count )
{
    if ( sound_callback )
        sound_callback( sound_callback_data, (short*) out, count / 2 );
}
static const char* sound_init( long sample_rate, int buf_size,
        sound_callback_t cb, void* data )
{
    sound_callback = cb;
    sound_callback_data = data;
    static SDL_AudioSpec as; 
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

    SDL_LockAudio();
    SDL_UnlockAudio();
}
static void sound_cleanup()
{
    sound_stop();
    SDL_CloseAudio();
}


