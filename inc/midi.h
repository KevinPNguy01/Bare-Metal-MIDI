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
#include "songs.h"
#include <stdint.h>
#include <stdbool.h>

#define NUM_NOTES 88

extern uint16_t midi_note_index;
extern uint32_t midi_time;

struct note_message {
    uint32_t time;
    uint8_t note;
    bool on;
};

extern bool midi_notes[NUM_NOTES];
extern float midi_notes_phases[NUM_NOTES];
extern float midi_notes_phases_inc[NUM_NOTES];

void midi_init(void);

void midi_sample_note(void);

#endif /* INC_MIDI_H_ */
