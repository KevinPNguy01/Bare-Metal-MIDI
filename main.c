#include "tm4c123gh6pm.h"
#include "timers.h"
#include "delays.h"
#include "keypad.h"
#include "lcd.h"
#include "midi.h"
#include "song.h"
#include <stdint.h>
#include <stdbool.h>

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

    keypad_init();
    lcd_init();
    speaker_init();
    midi_init();

    timer1_init();

    while (1) {
        if (current_song == NULL) continue;
        lcd_init();
        lcd_write_instruction(0x1);
        uint16_t seconds = midi_time / 10 / 1000 / 1000;
        uint16_t minute = seconds / 60 % 10;
        uint16_t seconds_tens = seconds / 10 % 6;
        uint16_t seconds_ones = seconds % 10;
        lcd_write_data('0' + minute);
        lcd_write_data(':');
        lcd_write_data('0' + seconds_tens);
        lcd_write_data('0' + seconds_ones);

        lcd_write_data(' ');
        lcd_write_data(' ');
        lcd_write_data(' ');
        lcd_write_data(' ');
        lcd_write_data(' ');
        lcd_write_data(' ');
        lcd_write_data(' ');
        lcd_write_data(' ');

        seconds = current_song->note_messages[current_song->num_notes-1].time / 10 / 1000 / 1000;
        minute = seconds / 60 % 10;
        seconds_tens = seconds / 10 % 6;
        seconds_ones = seconds % 10;
        lcd_write_data('0' + minute);
        lcd_write_data(':');
        lcd_write_data('0' + seconds_tens);
        lcd_write_data('0' + seconds_ones);

        delay_ms(50);
    }

	return 0;
}
