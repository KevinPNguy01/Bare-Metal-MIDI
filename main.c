#include "tm4c123gh6pm.h"
#include "timers.h"
#include "delays.h"
#include "keypad.h"
#include "lcd.h"
#include "midi.h"
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

    uint16_t prev = 0;

    while (1) {
        if (midi_note_index == prev) continue;
        lcd_init();
        prev = midi_note_index;
        lcd_write_instruction(0x1);
        uint16_t hundreds = (midi_note_index / 100) % 10;
        uint16_t tens = (midi_note_index / 10) % 10;
        uint16_t ones = midi_note_index % 10;
        lcd_write_data('0' + hundreds);
        lcd_write_data('0' + tens);
        lcd_write_data('0' + ones);
        delay_ms(50);
    }

	return 0;
}
