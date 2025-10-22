#include "tm4c123gh6pm.h"
#include "timers.h"
#include "delays.h"
#include "keypad.h"
#include "lcd.h"
#include "midi.h"
#include "song.h"
#include "leds.h"
#include <stdint.h>
#include <stdbool.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

void set_system_clock_80MHz(void) {
    // 1. Use RCC2 for advanced features
    SYSCTL_RCC2_R |= 0x80000000;   // USERCC2

    // 2. Bypass PLL while configuring
    SYSCTL_RCC2_R |= 0x00000800;   // BYPASS2

    // 3. Configure crystal and oscillator source
    SYSCTL_RCC_R  &= ~0x000007C0;  // Clear XTAL field
    SYSCTL_RCC_R  += 0x00000540;   // Configure for 16 MHz crystal (XTAL = 0x15)
    SYSCTL_RCC2_R &= ~0x00000070;  // MOSC main oscillator

    // 4. Activate PLL by clearing PWRDN
    SYSCTL_RCC2_R &= ~0x00002000;

    // 5. Set system divider and enable 400 MHz PLL
    SYSCTL_RCC2_R |= 0x40000000;   // DIV400 -> use 400 MHz PLL
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000) | (4 << 22); // SYSDIV2 = 4 -> 400/(4+1)=80 MHz

    // 6. Wait for PLL to lock
    while((SYSCTL_RIS_R & 0x00000040) == 0);

    // 7. Enable PLL by clearing BYPASS
    SYSCTL_RCC2_R &= ~0x00000800;
}

/**
 * main.c
 */
int main(void)
{
    set_system_clock_80MHz();

    timer0_init();

    leds_init();
    leds_turn_on_one(0);
    keypad_init();
    lcd_init();
    speaker_init();
    midi_init_trig();
    midi_init();

    timer1_init();

    lcd_set_cursor_pos(0, 0);
    lcd_write_str("Bare Metal MIDI");
    lcd_set_cursor_pos(0, 1);
    lcd_write_str("by Kevin Nguy");


    uint32_t marquee_delay_start = 3000;
    uint32_t marquee_delay_end = 3000;
    uint32_t marquee_delay_shift = 500;

    while (1) {
        if (current_song == NULL) continue;

        lcd_set_cursor_pos(0, 0);

        uint8_t marquee_index = 0;
        if (marquee_len > 16) {
            uint32_t marquee_text_time = (marquee_len - 16) * marquee_delay_shift;
            uint32_t marquee_duration = marquee_delay_start + marquee_text_time + marquee_delay_end;
            uint32_t marquee_time = global_time % marquee_duration;
            if (marquee_delay_start <= marquee_time) {
                if (marquee_text_time + marquee_delay_start <= marquee_time ) {
                    marquee_index = marquee_len - 16;
                } else {
                    marquee_index = (marquee_time - marquee_delay_start) / marquee_delay_shift;
                }
            }

        }
        lcd_write_str(&(marquee_text[marquee_index]));


        uint16_t seconds = midi_time / 10 / 1000 / 1000;
        uint16_t minute = seconds / 60 % 10;
        uint16_t seconds_tens = seconds / 10 % 6;
        uint16_t seconds_ones = seconds % 10;
        const char time_str[] = {
            '0' + minute,
            ':',
            '0' + seconds_tens,
            '0' + seconds_ones,
            '\0'
        };
        lcd_set_cursor_pos(0, 1);
        lcd_write_str(time_str);

        // Display media controls
        lcd_set_cursor_pos(5, 1);
        lcd_write_data(3);
        lcd_write_data(' ');
        lcd_write_data(is_playing ? 1 : 0);
        lcd_write_data(' ');
        lcd_write_data(2);

        seconds = current_song->note_messages[current_song->num_notes-1].time / 10 / 1000 / 1000;
        minute = seconds / 60 % 10;
        seconds_tens = seconds / 10 % 6;
        seconds_ones = seconds % 10;
        const char duration_str[] = {
            '0' + minute,
            ':',
            '0' + seconds_tens,
            '0' + seconds_ones,
            '\0'
        };
        lcd_set_cursor_pos(11, 1);
        lcd_write_str(duration_str);

        delay_ms(50);
    }

	return 0;
}
