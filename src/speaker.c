/*
 * speaker.c
 *
 *  Created on: Oct 12, 2025
 *      Author: kevin
 */

#include "speaker.h"

void speaker_init(void) {
    SYSCTL_RCGCGPIO_R |= (1 << 1);  // Enable clock for Port B
    SYSCTL_RCGCPWM_R  |= (1 << 0);  // Enable clock for PWM0
    SYSCTL_RCC_R |= (1 << 20);      // USEPWMDIV = 1
    SYSCTL_RCC_R = (SYSCTL_RCC_R & ~0x000E0000) | (0x2 << 17); // Divide by 8

    while(!(SYSCTL_PRGPIO_R & (1 << 1))); // Wait for GPIOB ready

    // --- Configure PB4 for M0PWM2 ---
    GPIO_PORTB_AFSEL_R |= (1 << 4);
    GPIO_PORTB_PCTL_R  = (GPIO_PORTB_PCTL_R & ~0x000F0000) | (0x4 << 16);
    GPIO_PORTB_DEN_R  |= (1 << 4);
    GPIO_PORTB_DIR_R  |= (1 << 4);

    // --- Configure PWM0, Generator 1 (controls PWM2 & PWM3) ---
    PWM0_1_CTL_R = 0;               // Disable generator during setup
    PWM0_1_GENA_R = (0x3 << 2) | (0x2 << 6); // Set when CMPA down, clear at LOAD
    PWM0_1_LOAD_R = 0xFFFF;         // Default period (safe)
    PWM0_1_CMPA_R = 0;              // Start silent
    PWM0_1_CTL_R |= 1;              // Enable generator
    PWM0_ENABLE_R |= (1 << 2);      // Enable M0PWM2 output (PB4)
}

void speaker_play_note(uint8_t note, uint32_t duration_ms) {
    if (note < 21 || note > 108) { // piano key range
        PWM0_1_CMPA_R = 0; // silence
        return;
    }

    double freq = 440.0 * pow(2.0, (note - 69) / 12.0);
    uint32_t load = (uint32_t)(2000000.0 / freq) - 1; // assuming 16 MHz
    PWM0_1_LOAD_R = load;
    PWM0_1_CMPA_R = load / 2; // 50% duty for square wave

    delay_ms(duration_ms);

    PWM0_1_CMPA_R = 0; // stop tone
}
