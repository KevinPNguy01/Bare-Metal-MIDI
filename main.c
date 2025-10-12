#include "tm4c123gh6pm.h"
#include "timers.h"
#include "delays.h"
#include "keypad.h"
#include "lcd.h"
#include "speaker.h"
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

    while (1) {
        if (pressed_key != ' ') {
            lcd_write_instruction(0x1);
            lcd_write_data(pressed_key);
            speaker_play_tone(pressed_key);
        }
    }

	return 0;
}
