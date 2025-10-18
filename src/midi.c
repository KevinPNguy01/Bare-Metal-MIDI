/*
 * midi.c
 *
 *  Created on: Oct 12, 2025
 *      Author: kevin
 */

#include "midi.h"

uint16_t midi_note_index = 0;
uint32_t midi_time = 0;

bool midi_notes[NUM_NOTES] = {false};
float midi_notes_phases[NUM_NOTES] = {0};
float midi_notes_phases_inc[NUM_NOTES] = {0};
float midi_sine[NUM_SINE_SIMPLES] = {0};

void midi_init(void) {
    midi_note_index = 0;
    midi_time = 0;

    uint16_t i;
    for (i = 0; i < NUM_NOTES; ++i) {
        midi_notes[i] = false;
        midi_notes_phases[i] = 0;
        midi_notes_phases_inc[i] = 440.0 * pow(2.0, (21 + i - 69) / 12.0) / 8000;
    }

    for (i = 0; i < NUM_SINE_SIMPLES; ++i) {
        midi_sine[i] = sinf(2 * M_PI * i / NUM_SINE_SIMPLES);
    }
}

void midi_sample_note(void) {
    midi_time += 125;
    while (midi_note_index < NUM_MIDI_NOTES && midi_time >= note_messages[midi_note_index].time) {
        struct note_message note = note_messages[midi_note_index];
        if (note.on) {
            midi_notes[note.note-21] = 1;
        } else {
            midi_notes[note.note-21] = 0;
        }
        ++midi_note_index;
    }
    if (midi_note_index < NUM_MIDI_NOTES) {
        speaker_play_notes();
    } else {
        PWM0_1_CMPA_R = 0;
    }
}
