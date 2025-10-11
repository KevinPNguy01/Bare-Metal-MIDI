#include "tm4c123gh6pm.h"
#include <stdint.h>

/**
 * Delay in microseconds
 */
void delay_us(uint32_t us) {
    NVIC_ST_RELOAD_R = 16 - 1;   // 1 µs @ 16 MHz
    NVIC_ST_CURRENT_R = 0;       // Clear current count
    NVIC_ST_CTRL_R = 0x05;       // Enable SysTick, use system clock

    uint32_t i;
    for (i = 0; i < us; i++) {
        while ((NVIC_ST_CTRL_R & 0x10000) == 0) {}  // Wait for count flag
    }

    NVIC_ST_CTRL_R = 0;          // Disable SysTick
}

/**
 * Delay in milliseconds
 */
void delay_ms(uint32_t ms) {
    NVIC_ST_RELOAD_R = 16000 - 1;   // 1 ms @ 16 MHz
    NVIC_ST_CURRENT_R = 0;          // Clear current
    NVIC_ST_CTRL_R = 0x05;          // Enable, use system clock

    uint32_t i;
    for (i = 0; i < ms; i++) {
        while ((NVIC_ST_CTRL_R & 0x10000) == 0) {}  // Wait for count flag
    }
    NVIC_ST_CTRL_R = 0;             // Disable
}

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
 * main.c
 */
int main(void)
{
    SYSCTL_RCGCGPIO_R |= (1 << 2) | (1 << 3);  // Enable clock for Port C and D

    GPIO_PORTC_DIR_R = 0xF0;      // Output for pins 4-7
    GPIO_PORTC_DEN_R = 0xF0;      // Digital enable for pins 4-7

    GPIO_PORTD_LOCK_R = 0x4C4F434B;   // Unlock PD7
    GPIO_PORTD_CR_R |= (1 << 7);      // Allow changes to PD7

    GPIO_PORTD_DIR_R = 0xC0;      // Output for pins 6-7
    GPIO_PORTD_DEN_R = 0xC0;      // Digital enable for pins 6-7

    delay_ms(20);
    WriteToIR(0x02);    // 4-bit mode
    WriteToIR(0x28);    // 4-bit mode, 2 lines, 5x8 dots per character
    WriteToIR(0x1);     // Clear screen

    WriteToDR('H');
    WriteToDR('e');
    WriteToDR('l');
    WriteToDR('l');
    WriteToDR('o');
    WriteToDR(' ');
    WriteToDR('W');
    WriteToDR('o');
    WriteToDR('r');
    WriteToDR('l');
    WriteToDR('d');

	return 0;
}
