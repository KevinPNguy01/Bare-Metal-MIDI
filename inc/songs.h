#ifndef INC_SONGS_H_
#define INC_SONGS_H_

#include "midi.h"
#include "song.h"

#define NUM_SONGS 9


#define SONG_0_NUM_NOTES 1750
extern const char song_0_title[];
extern const char song_0_artist[];
extern const struct note_message song_0_note_messages[SONG_0_NUM_NOTES];

#define SONG_1_NUM_NOTES 1850
extern const char song_1_title[];
extern const char song_1_artist[];
extern const struct note_message song_1_note_messages[SONG_1_NUM_NOTES];

#define SONG_2_NUM_NOTES 2334
extern const char song_2_title[];
extern const char song_2_artist[];
extern const struct note_message song_2_note_messages[SONG_2_NUM_NOTES];

#define SONG_3_NUM_NOTES 1648
extern const char song_3_title[];
extern const char song_3_artist[];
extern const struct note_message song_3_note_messages[SONG_3_NUM_NOTES];

#define SONG_4_NUM_NOTES 3902
extern const char song_4_title[];
extern const char song_4_artist[];
extern const struct note_message song_4_note_messages[SONG_4_NUM_NOTES];

#define SONG_5_NUM_NOTES 4090
extern const char song_5_title[];
extern const char song_5_artist[];
extern const struct note_message song_5_note_messages[SONG_5_NUM_NOTES];

#define SONG_6_NUM_NOTES 1022
extern const char song_6_title[];
extern const char song_6_artist[];
extern const struct note_message song_6_note_messages[SONG_6_NUM_NOTES];

#define SONG_7_NUM_NOTES 2442
extern const char song_7_title[];
extern const char song_7_artist[];
extern const struct note_message song_7_note_messages[SONG_7_NUM_NOTES];

#define SONG_8_NUM_NOTES 416
extern const char song_8_title[];
extern const char song_8_artist[];
extern const struct note_message song_8_note_messages[SONG_8_NUM_NOTES];

extern const struct Song songs[NUM_SONGS];

#endif /* INC_SONGS_H_ */
