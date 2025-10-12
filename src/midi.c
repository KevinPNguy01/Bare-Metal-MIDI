/*
 * midi.c
 *
 *  Created on: Oct 12, 2025
 *      Author: kevin
 */

#include "midi.h"

uint8_t midi_notes_active = 0;
uint16_t midi_note_index = 0;
uint32_t midi_time = 0;

struct note_message note_messages[26] = { {0, 68, 1},{960000, 68, 0},{960000, 70, 1},{1680000, 70, 0},{1680000, 72, 1},{2640000, 72, 0},{2640000, 68, 1},{2880000, 68, 0},{2880000, 73, 1},{3240000, 73, 0},{3240000, 72, 1},{3600000, 72, 0},{3600000, 65, 1},{4560000, 65, 0},{4560000, 63, 1},{4800000, 63, 0},{4800000, 65, 1},{5520000, 65, 0},{5520000, 63, 1},{5760000, 63, 0},{5760000, 65, 1},{6120000, 65, 0},{6120000, 68, 1},{6480000, 68, 0},{6480000, 67, 1},{7680000, 67, 0}, };

void midi_sample_note() {
    midi_time += 125;
    while (midi_note_index < 26 && midi_time >= note_messages[midi_note_index].time) {
        struct note_message note = note_messages[midi_note_index];
        if (note.on) {
            ++midi_notes_active;
            speaker_play_note(note.note);
        } else {
            --midi_notes_active;
            if (midi_notes_active == 0) {
                PWM0_1_CMPA_R = 0;
            }
        }
        ++midi_note_index;
    }
}
