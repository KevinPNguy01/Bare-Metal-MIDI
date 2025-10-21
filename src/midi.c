/*
 * midi.c
 *
 *  Created on: Oct 12, 2025
 *      Author: kevin
 */

#include "midi.h"

uint16_t midi_note_index = 0;
uint32_t midi_time = 0;         // 0.1 microsecond units
uint8_t midi_instrument = 0;

bool midi_notes[NUM_NOTES] = {false};
float midi_notes_phases[NUM_NOTES] = {0};
float midi_notes_phases_inc[NUM_NOTES] = {0};
float midi_sine[NUM_SINE_SAMPLES] = {0};
float midi_tanh[NUM_TANH_SAMPLES] = {0};


void midi_init_trig(void) {
    uint16_t i;
    for (i = 0; i < NUM_NOTES; ++i) {
        midi_notes_phases_inc[i] = 440.0 * pow(2.0, (21 + i - 69) / 12.0) / 16000;
    }

    for (i = 0; i < NUM_SINE_SAMPLES; ++i) {
        midi_sine[i] = sinf(2 * M_PI * i / NUM_SINE_SAMPLES);
    }

    for (i = 0; i < NUM_TANH_SAMPLES; ++i) {
        midi_tanh[i] = tanh(M_PI * i / NUM_TANH_SAMPLES);
    }
}

void midi_init(void) {
    midi_note_index = 0;
    midi_time = 0;

    uint16_t i;
    for (i = 0; i < NUM_NOTES; ++i) {
        midi_notes[i] = false;
        midi_notes_phases[i] = 0;
    }
}

void midi_sample_note(void) {
    if (!is_playing) return;

    uint16_t num_notes = current_song->num_notes;
    const struct note_message* note_messages = current_song->note_messages;

    midi_time += 625;
    while (midi_note_index < num_notes && midi_time >= note_messages[midi_note_index].time) {
        struct note_message note = note_messages[midi_note_index];
        if (note.on) {
            midi_notes[note.note-21] = 1;
        } else {
            midi_notes[note.note-21] = 0;
        }
        ++midi_note_index;
    }
    if (midi_note_index < num_notes) {
        speaker_play_notes();
    } else {
        PWM0_1_CMPA_R = 0;
        is_playing = false;
    }
}
