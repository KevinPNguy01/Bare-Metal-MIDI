/*
 * song.c
 *
 *  Created on: Oct 18, 2025
 *      Author: kevin
 */

#include "song.h"

const struct Song* current_song = NULL;

char marquee_text[256] = {'\0'};
uint8_t marquee_len = 0;
