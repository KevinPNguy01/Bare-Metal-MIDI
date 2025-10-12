#include "tm4c123gh6pm.h"
#include "timers.h"
#include "delays.h"
#include "keypad.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * Pulses E pin on the LCD to initiate data transfer
 */
void E_pulse() {
    GPIO_PORTD_DATA_R |= (1 << 7);
    delay_us(2);
    GPIO_PORTD_DATA_R &= ~(1 << 7);
    delay_us(2);
}

/**
 * Sends a nibble to the LCD
 */
void SendNibble(char data) {
    GPIO_PORTC_DATA_R = data << 4;
    E_pulse();
}

/**
 * Sends a byte to the LCD
 */
void SendByte(char data) {
    SendNibble(data >> 4);
    SendNibble(data & 0xF);
}

/**
 * Writes to instruction register on the LCD
 */
void WriteToIR(char data) {
    GPIO_PORTD_DATA_R &= ~(1 << 6);
    SendByte(data);
    delay_ms(2);
}

/**
 * Writes to data register on the LCD
 */
void WriteToDR(char data) {
    GPIO_PORTD_DATA_R |= 1 << 6;
    SendByte(data);
    delay_ms(2);
}

/**
 * Play a tone based off the character c. Must be between '1'-'8'.
 */
void playTone(char c) {
    if ('1' <= c && c <= '8') {
        PWM0_1_LOAD_R = 65000 - 5000 * (c - '1');
        PWM0_1_CMPA_R = PWM0_1_LOAD_R / 2;
        delay_ms(1000);
    } else {
        PWM0_1_CMPA_R = 0;
    }
}

/**
 * main.c
 */
int main(void)
{
    timer0_init();
    keypad_init();

    SYSCTL_RCGCGPIO_R |= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);  // Enable clock for Port B-F
    SYSCTL_RCGCPWM_R |= 0x1;    // Enable clock for PWM0

    GPIO_PORTB_DIR_R |= (1 << 4);     // Output for PB4
    GPIO_PORTB_AFSEL_R |= (1 << 4);   // Alternate function for PB4
    GPIO_PORTB_PCTL_R |= (4 << 16);   // Configure PB4 as M0PWM2
    GPIO_PORTB_DEN_R |= (1 << 4);     // Enable digital output for PB4

    GPIO_PORTC_DIR_R = 0xF0;      // Output for pins 4-7
    GPIO_PORTC_DEN_R = 0xF0;      // Digital enable for pins 4-7

    GPIO_PORTD_DIR_R |= 0xCF;      // Output for pins 6-7
    GPIO_PORTD_DEN_R |= 0xCF;      // Digital enable for pins 6-7

    PWM0_1_CTL_R &= ~(0x1);          // Disable while configuring
    PWM0_1_CTL_R &= ~(1 << 1);       // Count down mode
    PWM0_1_GENA_R |= (0x2 << 0);     // Go low when 0
    PWM0_1_GENA_R |= (0x3 << 6);     // Go high when CMPA
    PWM0_1_CTL_R |= 0x1;             // Enable
    PWM0_ENABLE_R = (1 << 2);       // Enable PWM0

    delay_ms(50);
    WriteToIR(0x02);    // 4-bit mode
    WriteToIR(0x28);    // 4-bit mode, 2 lines, 5x8 dots per character
    WriteToIR(0x1);     // Clear screen

    while (1) {
        if (pressed_key != ' ') {
            WriteToIR(0x1);
            WriteToDR(pressed_key);
        }
        playTone(pressed_key);
    }

	return 0;
}
