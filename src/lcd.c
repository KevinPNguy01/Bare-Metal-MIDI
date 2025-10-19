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

    delay_ms(20);

    send_nibble(0b0011);
    delay_ms(5);
    send_nibble(0b0011);
    delay_us(150);
    send_nibble(0b0011);
    delay_us(150);
    send_nibble(0b0010);
    delay_us(150);

    lcd_write_instruction(0b00101000);
    lcd_write_instruction(0b00001000);
    lcd_clear_screen();
    lcd_write_instruction(0b00000110);
    lcd_write_instruction(0b00001111);
}

/**
 * Writes to instruction register on the LCD
 */
void lcd_write_instruction(char data) {
    GPIO_PORTD_DATA_R &= ~(1 << 6);
    delay_ns(75);
    send_byte(data);
    delay_us(1750);
}

/**
 * Writes to data register on the LCD
 */
void lcd_write_data(char data) {
    GPIO_PORTD_DATA_R |= 1 << 6;
    delay_ns(75);
    send_byte(data);
    delay_us(40);
}

/**
 * Clears the display
 */
void lcd_clear_screen() {
    lcd_write_instruction(0x1);
    delay_us(1600);
}

/**
 * Sets the cursor position to the given column and row
 */
void lcd_set_cursor_pos(uint8_t x, uint8_t y) {
    uint8_t address = x | (y ? (1 << 6) : 0);
    lcd_write_instruction((1 << 7) | address);

}

void lcd_write_str(char str[], uint8_t len) {
    uint8_t i;
    for (i = 0; str[i] != '\0'; ++i) {
        lcd_write_data(str[i]);
    }
}

/**
 * Pulses Enable pin on the LCD to initiate data transfer
 */
static void pulse_enable() {
    GPIO_PORTD_DATA_R |= (1 << 7);
    delay_us(1);
    GPIO_PORTD_DATA_R &= ~(1 << 7);
    delay_us(1);
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
