#include "tm4c123gh6pm.h"
#include "timers.h"
#include "delays.h"
#include "keypad.h"
#include "lcd.h"
#include "midi.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * main.c
 */
int main(void)
{
    timer0_init();

    keypad_init();
    lcd_init();
    speaker_init();

    timer1_init();

    uint16_t prev = 0;

    while (1) {
        if (pressed_key == 'A') {
            midi_note_index = 0;
            midi_time = 0;
            midi_notes_active = 0;
        }
        if (pressed_key == 'B') {
            midi_note_index = 834;
            PWM0_1_CMPA_R = 0;
        }
        if (midi_note_index == prev) continue;
        prev = midi_note_index;
        lcd_write_instruction(0x1);
        uint16_t hundreds = (midi_note_index / 100) % 10;
        uint16_t tens = (midi_note_index / 10) % 10;
        uint16_t ones = midi_note_index % 10;
        lcd_write_data('0' + hundreds);
        lcd_write_data('0' + tens);
        lcd_write_data('0' + ones);
    }

	return 0;
}
