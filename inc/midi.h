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
#include "song.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define NUM_NOTES 88
#define NUM_SINE_SAMPLES 256
#define NUM_TANH_SAMPLES 1024

extern uint16_t midi_note_index;
extern uint32_t midi_time;
extern uint8_t midi_instrument;

struct note_message {
    uint32_t time;
    uint8_t note;
    bool on;
};

extern bool midi_notes[NUM_NOTES];
extern float midi_notes_phases[NUM_NOTES];
extern float midi_notes_phases_inc[NUM_NOTES];
extern float midi_sine[NUM_SINE_SAMPLES];
extern float midi_tanh[NUM_TANH_SAMPLES];

void midi_init_trig(void);
void midi_init(void);

void midi_sample_note(void);

#endif /* INC_MIDI_H_ */
