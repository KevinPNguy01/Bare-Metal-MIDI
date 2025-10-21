/*
 * song.c
 *
 *  Created on: Oct 18, 2025
 *      Author: kevin
 */

#include "song.h"

char marquee_text[256] = {'\0'};
uint8_t marquee_len = 0;

uint8_t current_song_index;
const struct Song* current_song = NULL;
bool is_playing = false;

void prev_song(void) {
    current_song_index = current_song_index == 0 ? NUM_SONGS - 1 : current_song_index - 1;
    current_song = &songs[current_song_index];
    global_time = 0;
    lcd_clear_screen();
    midi_init();

    uint8_t i;
    for (i = 0; current_song->title[i] != '\0'; ++i);
    uint8_t title_strlen = i;
    for (i = 0; current_song->artist[i] != '\0'; ++i);
    uint8_t artist_strlen = i;

    const char title_artist_separator[] = " - ";

    marquee_len = title_strlen + artist_strlen + 3;
    for (i = 0; i < title_strlen; ++i) {
        marquee_text[i] = current_song->title[i];
    }
    for (i = 0; i < 3; ++i) {
        marquee_text[title_strlen+i] = title_artist_separator[i];
    }
    for (i = 0; i < artist_strlen; ++i) {
        marquee_text[title_strlen+3+i] = current_song->artist[i];
    }
    marquee_text[title_strlen+3+artist_strlen] = '\0';
}

void next_song(void) {
    current_song_index = current_song_index == NUM_SONGS - 1 ? 0 : current_song_index + 1;
    current_song = &songs[current_song_index];
    global_time = 0;
    lcd_clear_screen();
    midi_init();

    uint8_t i;
    for (i = 0; current_song->title[i] != '\0'; ++i);
    uint8_t title_strlen = i;
    for (i = 0; current_song->artist[i] != '\0'; ++i);
    uint8_t artist_strlen = i;

    const char title_artist_separator[] = " - ";

    marquee_len = title_strlen + artist_strlen + 3;
    for (i = 0; i < title_strlen; ++i) {
        marquee_text[i] = current_song->title[i];
    }
    for (i = 0; i < 3; ++i) {
        marquee_text[title_strlen+i] = title_artist_separator[i];
    }
    for (i = 0; i < artist_strlen; ++i) {
        marquee_text[title_strlen+3+i] = current_song->artist[i];
    }
    marquee_text[title_strlen+3+artist_strlen] = '\0';
}
