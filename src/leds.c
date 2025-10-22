/*
 * leds.c
 *
 *  Created on: Oct 22, 2025
 *      Author: kevin
 */

#include "leds.h"

void leds_init(void) {
    SYSCTL_RCGCGPIO_R |= 0b10010;  // Enable clock for Port B, E

    GPIO_PORTB_DIR_R |= 0x03;       // Output for pins 0-1
    GPIO_PORTB_DEN_R |= 0x03;       // Digital enable for pins 0-1

    GPIO_PORTE_DIR_R |= 0x30;       // Input for pins 4-5
    GPIO_PORTE_DEN_R |= 0x30;       // Digital enable for pins 4-5
}

void leds_turn_all_off(void) {
    GPIO_PORTB_DATA_R &= ~0x03;
    GPIO_PORTE_DATA_R &= ~0x30;
}

void leds_turn_on_one(uint8_t i) {
    if (i < 2) {
        GPIO_PORTB_DATA_R |= (1 << i);
    } else {
        GPIO_PORTE_DATA_R |= (1 << (i + 2));
    }
}
