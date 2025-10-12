/*
 * midi.h
 *
 *  Created on: Oct 12, 2025
 *      Author: kevin
 */

#ifndef INC_MIDI_H_
#define INC_MIDI_H_

#include "tm4c123gh6pm.h"
#include "delays.h"
#include "speaker.h"
#include "lcd.h"
#include <stdint.h>
#include <stdbool.h>

extern uint8_t midi_notes_active;
extern uint16_t midi_note_index;
extern uint32_t midi_time;

struct note_message {
    uint32_t time;
    uint8_t note;
    bool on;
};

extern struct note_message note_messages[26];

void midi_sample_note();

#endif /* INC_MIDI_H_ */
