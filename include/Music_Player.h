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

typedef struct {
    short* buf;
    int size;
} sample_t;

typedef struct Music_Player{
    Music_Emu* emu_;
    short* scope_buf;
    int paused;
    gme_info_t* track_info_;
    int scope_buf_size;
    long sample_rate;
} Music_Player;

gme_err_t Music_Player_init(Music_Player* player, long sample_rate);
gme_err_t Music_Player_load_file(Music_Player* player, const char* path);
gme_err_t Music_Player_start_track(Music_Player* player, int track);
void Music_Player_stop(Music_Player* player);
int Music_Player_track_count(const Music_Player* player);
gme_info_t const* Music_Player_track_info(const Music_Player* player);
void Music_Player_pause(Music_Player* player, int);
int Music_Player_track_ended(const Music_Player* player);
Music_Emu* Music_Player_emu(const Music_Player* player);
void Music_Player_set_stereo_depth(Music_Player* player, double);
void Music_Player_enable_accuracy(Music_Player* player, int);
void Music_Player_set_tempo(Music_Player* player, double);
void Music_Player_mute_voices(Music_Player* player, int);
void Music_Player_set_fadeout(Music_Player* player, int);
void Music_Player_set_scope_buffer(Music_Player* player, sample_t* buf, int size);
void Music_Player_fill_buffer(void* data, short* out, int count);
