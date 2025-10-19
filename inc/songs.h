#ifndef INC_SONGS_H_
#define INC_SONGS_H_

#include "midi.h"
#include "song.h"

#define NUM_SONGS 4


#define SONG_0_NUM_NOTES 1750
extern const char song_0_title[];
extern const char song_0_artist[];
extern const struct note_message song_0_note_messages[SONG_0_NUM_NOTES];

#define SONG_1_NUM_NOTES 2334
extern const char song_1_title[];
extern const char song_1_artist[];
extern const struct note_message song_1_note_messages[SONG_1_NUM_NOTES];

#define SONG_2_NUM_NOTES 2106
extern const char song_2_title[];
extern const char song_2_artist[];
extern const struct note_message song_2_note_messages[SONG_2_NUM_NOTES];

#define SONG_3_NUM_NOTES 4999
extern const char song_3_title[];
extern const char song_3_artist[];
extern const struct note_message song_3_note_messages[SONG_3_NUM_NOTES];

extern const struct Song songs[NUM_SONGS];

#endif /* INC_SONGS_H_ */
