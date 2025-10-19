/*
 * song.h
 *
 *  Created on: Oct 18, 2025
 *      Author: kevin
 */

#ifndef INC_SONG_H_
#define INC_SONG_H_

#include "midi.h"
#include <stddef.h>

struct Song {
    const struct note_message* note_messages;
    const uint16_t num_notes;
    const char* title;
    const char* artist;
};

extern const struct Song* current_song;

#endif /* INC_SONG_H_ */
