/*
 * speaker.c
 *
 *  Created on: Oct 12, 2025
 *      Author: kevin
 */

#include "speaker.h"

void speaker_init(void) {
    SYSCTL_RCGCGPIO_R |= (1 << 1);  // Enable clock for Port B
    SYSCTL_RCGCPWM_R |= 0x1;        // Enable clock for PWM0

    GPIO_PORTB_DIR_R |= (1 << 4);   // Output for PB4
    GPIO_PORTB_AFSEL_R |= (1 << 4); // Alternate function for PB4
    GPIO_PORTB_PCTL_R |= (4 << 16); // Configure PB4 as M0PWM2
    GPIO_PORTB_DEN_R |= (1 << 4);   // Enable digital output for PB4

    PWM0_1_CTL_R &= ~(0x1);         // Disable while configuring
    PWM0_1_CTL_R &= ~(1 << 1);      // Count down mode
    PWM0_1_GENA_R |= (0x2 << 0);    // Go low when 0
    PWM0_1_GENA_R |= (0x3 << 6);    // Go high when CMPA
    PWM0_1_CTL_R |= 0x1;            // Enable PWM0
    PWM0_ENABLE_R = (1 << 2);       // Enable PWM0M2
}

/**
 * Play a tone based off the character c. Must be between '1'-'9'.
 */
void speaker_play_tone(char c) {
    if ('1' <= c && c <= '9') {
        PWM0_1_LOAD_R = 65000 - 5000 * (c - '1');
        PWM0_1_CMPA_R = PWM0_1_LOAD_R / 2;
        delay_ms(1000);
    } else {
        PWM0_1_CMPA_R = 0;
    }
}
