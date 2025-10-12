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
            lcd_write_data((pressed_key - '1') + '1');
            speaker_play_note(21 + (pressed_key - '1'), 1000);
        }
    }

	return 0;
}
