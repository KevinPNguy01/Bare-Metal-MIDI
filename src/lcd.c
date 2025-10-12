/*
 * lcd.c
 *
 *  Created on: Oct 12, 2025
 *      Author: kevin
 */

#include "lcd.h"

void lcd_init() {
    SYSCTL_RCGCGPIO_R |= (3 << 2);  // Enable clock for Port C-D

    GPIO_PORTC_DIR_R = 0xF0;        // Output for pins 4-7
    GPIO_PORTC_DEN_R = 0xF0;        // Digital enable for pins 4-7

    GPIO_PORTD_DIR_R |= 0xCF;       // Output for pins 6-7
    GPIO_PORTD_DEN_R |= 0xCF;       // Digital enable for pins 6-7

    delay_ms(50);
    lcd_write_instruction(0x02);    // 4-bit mode
    lcd_write_instruction(0x28);    // 4-bit mode, 2 lines, 5x8 dots per character
    lcd_write_instruction(0x1);     // Clear screen
}

/**
 * Writes to instruction register on the LCD
 */
void lcd_write_instruction(char data) {
    GPIO_PORTD_DATA_R &= ~(1 << 6);
    send_byte(data);
    delay_ms(2);
}

/**
 * Writes to data register on the LCD
 */
void lcd_write_data(char data) {
    GPIO_PORTD_DATA_R |= 1 << 6;
    send_byte(data);
    delay_ms(2);
}


/**
 * Pulses Enable pin on the LCD to initiate data transfer
 */
static void pulse_enable() {
    GPIO_PORTD_DATA_R |= (1 << 7);
    delay_us(2);
    GPIO_PORTD_DATA_R &= ~(1 << 7);
    delay_us(2);
}

/**
 * Sends a nibble to the LCD
 */
static void send_nibble(char data) {
    GPIO_PORTC_DATA_R = data << 4;
    pulse_enable();
}

/**
 * Sends a byte to the LCD
 */
static void send_byte(char data) {
    send_nibble(data >> 4);
    send_nibble(data & 0xF);
}
