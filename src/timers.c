/*
 * timers.c
 *
 *  Created on: Oct 11, 2025
 *      Author: kevin
 */

#include "timers.h"

// Flag indicating to poll the keypad
volatile bool checkKeys = false;

// Flag indicating delay timer is finished
volatile bool delayDone = false;

/**
 * Init Timer0A in one-shot mode for arbitrary delays
 * Init Timer0B in periodic mode for keypad polling
 */
void Timer0_Init(void) {
    SYSCTL_RCGCTIMER_R |= 0x1;      // Enable Timer0 clock
    while(!(SYSCTL_PRTIMER_R & 0x1));
    TIMER0_CTL_R = 0x0;             // Disable timer during setup
    TIMER0_CFG_R = 0x4;             // 16-bit timer

    // Timer 0A
    TIMER0_TAMR_R = 0x1;            // One-shot mode
    NVIC_EN0_R |= 1 << 19;          // Enable Timer0A IRQ in NVIC
    TIMER0_IMR_R |= 0x1;            // Enable timeout interrupt

    // Timer 0B
    TIMER0_TBMR_R = 0x2;            // Periodic mode
    TIMER0_TBPR_R = 5;              // Prescalar
    TIMER0_TBILR_R = 65306;         // Initcount
    NVIC_EN0_R |= (1 << 20);        // Enable Timer0B IRQ in NVIC
    TIMER0_IMR_R |= (1 << 8);       // Enable timeout interrupt
    TIMER0_CTL_R |= (1 << 8);       // Start Timer0B
}

void Timer0A_Handler(void) {
    TIMER0_ICR_R = 0x1; // Clear interrupt
    delayDone = true;   // Set flag to indicate timer is done
}

void Timer0B_Handler() {
    checkKeys = true;
    TIMER0_ICR_R |= (1 << 8);
}
