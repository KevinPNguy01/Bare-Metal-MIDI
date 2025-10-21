/*
 * keypad.c
 *
 *  Created on: Oct 12, 2025
 *      Author: kevin
 */

#include "keypad.h"

// The last key pressed. Space if no key was pressed.
char pressed_key = ' ';

// Keypad button to character mapping
const char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

/**
 * Matrix for storing keypad state
 * -1: is not pressed
 * 0: maybe pressed
 * 1: is pressed
 */
int pressed[4][4] = {
     {-1, -1, -1, -1},
     {-1, -1, -1, -1},
     {-1, -1, -1, -1},
     {-1, -1, -1, -1},
};

/**
 * Init the GPIO pins used for keypad scanning
 * GPIO D0-D3 for row output
 * GPIO E1-E3, F1 for column input
 */
void keypad_init(void) {
    SYSCTL_RCGCGPIO_R |= (7 << 3);  // Enable clock for Port D-F

    GPIO_PORTD_LOCK_R = 0x4C4F434B; // Unlock PD7
    GPIO_PORTD_CR_R |= (1 << 7);    // Allow changes to PD7

    GPIO_PORTD_DIR_R |= 0x0F;       // Output for pins 0-3
    GPIO_PORTD_DEN_R |= 0x0F;       // Digital enable for pins 0-3

    GPIO_PORTE_DIR_R &= ~0x0E;      // Input for pins 1-3
    GPIO_PORTE_DEN_R = 0x0E;        // Digital enable for pins 1-3
    GPIO_PORTE_PDR_R = 0x0E;        // Enable PDR for pins 1-3

    GPIO_PORTF_DIR_R &= ~0x02;      // Input for pin 1
    GPIO_PORTF_DEN_R = 0x02;        // Digital enable for pin 1
    GPIO_PORTF_PDR_R = 0x02;        // Enable PDR for pin 1
}

/**
 * Detects if a key was just pressed and stores it.
 * A key was just pressed if it transition from 0 to 1 state.
 */
void poll_keypad_handler(void) {
    pressed_key = ' ';
    int y, x;
    for (y = 0; y < 4; ++y) {
        GPIO_PORTD_DATA_R |= (1 << y);
        for (x = 0; x < 4; ++x) {
            int state = pressed[y][x];
            uint32_t bit = x < 3 ? GPIO_PORTE_DATA_R & (1 << (x + 1)) : GPIO_PORTF_DATA_R & 0x02;
            if (bit) {
                if (state <= 0) {
                    if (state == 0) {
                        pressed_key = keys[y][x];
                    }
                    ++pressed[y][x];
                }
            } else {
                if (state >= 0) {
                    --pressed[y][x];
                }
            }
        }
        GPIO_PORTD_DATA_R &= ~(1 << y);
    }

    if ('A' <= pressed_key && pressed_key <= 'D') {
        midi_instrument = pressed_key - 'A';
    }

    if ('1' <= pressed_key && pressed_key <= '9') {
        uint8_t index = pressed_key - '1';
        if (index < NUM_SONGS) {
            current_song = &songs[index];
            global_time = 0;
            is_playing = true;
            lcd_clear_screen();
            midi_init();

            uint8_t i;
            for (i = 0; current_song->title[i] != '\0'; ++i);
            uint8_t title_strlen = i;
            for (i = 0; current_song->artist[i] != '\0'; ++i);
            uint8_t artist_strlen = i;

            const char title_artist_separator[] = " - ";

            marquee_len = title_strlen + artist_strlen + 3;
            for (i = 0; i < title_strlen; ++i) {
                marquee_text[i] = current_song->title[i];
            }
            for (i = 0; i < 3; ++i) {
                marquee_text[title_strlen+i] = title_artist_separator[i];
            }
            for (i = 0; i < artist_strlen; ++i) {
                marquee_text[title_strlen+3+i] = current_song->artist[i];
            }
            marquee_text[title_strlen+3+artist_strlen] = '\0';
        }
    }

    if (current_song == NULL) return;

    if (pressed_key == '*') {
        if (midi_time < 2500 * 1000 * 10) {
            prev_song();
        } else {
            midi_init();
        }
    }
    else if (pressed_key == '0') {
        is_playing = !is_playing;
    }
    else if (pressed_key == '#') {
        next_song();
    }
}
