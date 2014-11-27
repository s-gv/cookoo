/*
Copyright (c) 2014, Sagar G V (sagar.writeme@gmail.com) and Dhruv Saxena (dhruvrai23@gmail.com)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the names of the copyright holders nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "hal.h"

static void __attribute__((__interrupt__(WDT_VECTOR))) WDT_ISR(void) {
    LPM3_EXIT; // Return to active mode
}

static void __attribute__((__interrupt__(ADC10_VECTOR))) ADC10_ISR(void) {
    LPM3_EXIT; // Return to active mode
}


/*
// If you're using TI's Code Composer for MSP430, replace the above two ISRs with the following.

#pragma vector=WDT_VECTOR,ADC10_VECTOR
__interrupt void ISR(void) {
    LPM3_EXIT; // Return to active mode
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////
void InitBuzzer() {
    P1DIR |= 1 << 0;
    P1OUT &= ~(1 << 0);
}
void InitWDT() {
    WDTCTL = WDTPW | WDTHOLD;
    IE1 |= WDTIE;
}
void InitUART() {
    P1DIR |= (1 << 1);
    P1OUT |= (1 << 1);
}

void InitCLOCK() {
    BCSCTL3 |= XCAP_3; // Set ACLK to 32 kHZ crystal
    BCSCTL1 = CALBC1_1MHZ; // Set DCO to 1MHz
    DCOCTL = CALDCO_1MHZ; // Set DCO to 1MHz
}

void InitLED() {
    // P1.2 is led 0, P1.3 is led 1
    // P2.2 is led 2, P2.3 is led 3, P2.4 is led 4, P2.5 is led 5
    // P1.6 is led 6, P1.7 is led 7
    P2DIR |= (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2);
    P1DIR |= (1 << 7) | (1 << 6) | (1 << 3) | (1 << 2);

    // Turn off the LEDs
    AllLEDOff();
}

void InitCapPush() {
    P2DIR |= (1 << 1) | (1 << 0); // P2.0 and P2.1 are cap touch pins
}

void InitADC() {
    ADC10AE0 |= BIT4; //ADC Input pin P1.4
    P1DIR |= (1 << 5); // P1.5 is connected to upper side of resistor divider
    P1OUT &= ~(1 << 5); // Turn off power to the resistor divider
}

void EnableInterrupts() {
    __enable_interrupt();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void UARTsb(uint8_t val) {
    // This works at 2400 baud, 1 MHz CPU
#define UART_BIT_CYCLES 415
#define UART_SEND_ZERO P1OUT &= ~(1 << 1);
#define UART_SEND_ONE P1OUT |= (1 << 1);

    // Send 0 (start bit)
    UART_SEND_ZERO
    ;
    __delay_cycles(UART_BIT_CYCLES);

    /*
     UART_SEND_ONE;
     __delay_cycles(UART_BIT_CYCLES);
     UART_SEND_ZERO;
     __delay_cycles(UART_BIT_CYCLES);
     UART_SEND_ZERO;
     __delay_cycles(UART_BIT_CYCLES);
     UART_SEND_ZERO;
     __delay_cycles(UART_BIT_CYCLES);

     UART_SEND_ZERO;
     __delay_cycles(UART_BIT_CYCLES);
     UART_SEND_ZERO;
     __delay_cycles(UART_BIT_CYCLES);
     UART_SEND_ONE;
     __delay_cycles(UART_BIT_CYCLES);
     UART_SEND_ZERO;
     __delay_cycles(UART_BIT_CYCLES);
     */

    // Send Bit 0 (lsb)
    if (val & 0x01) {
        UART_SEND_ONE
        ;
    } else {
        UART_SEND_ZERO
        ;
    }
    __delay_cycles(UART_BIT_CYCLES);

    // Send Bit 1
    if (val & 0x02) {
        UART_SEND_ONE
        ;
    } else {
        UART_SEND_ZERO
        ;
    }
    __delay_cycles(UART_BIT_CYCLES);

    // Send Bit 2
    if (val & 0x04) {
        UART_SEND_ONE
        ;
    } else {
        UART_SEND_ZERO
        ;
    }
    __delay_cycles(UART_BIT_CYCLES);

    // Send Bit 3
    if (val & 0x08) {
        UART_SEND_ONE
        ;
    } else {
        UART_SEND_ZERO
        ;
    }
    __delay_cycles(UART_BIT_CYCLES);

    // Send Bit 4
    if (val & 0x10) {
        UART_SEND_ONE
        ;
    } else {
        UART_SEND_ZERO
        ;
    }
    __delay_cycles(UART_BIT_CYCLES);

    // Send Bit 5
    if (val & 0x20) {
        UART_SEND_ONE
        ;
    } else {
        UART_SEND_ZERO
        ;
    }
    __delay_cycles(UART_BIT_CYCLES);

    // Send Bit 6
    if (val & 0x40) {
        UART_SEND_ONE
        ;
    } else {
        UART_SEND_ZERO
        ;
    }
    __delay_cycles(UART_BIT_CYCLES);

    // Send Bit 7 (msb)
    if (val & 0x80) {
        UART_SEND_ONE
        ;
    } else {
        UART_SEND_ZERO
        ;
    }
    __delay_cycles(UART_BIT_CYCLES);

    // Send 1 (stop bits)
    UART_SEND_ONE
    ;
    /*
     uint8_t i;
     for (i = 0; i < 20; i++)
     __delay_cycles(UART_BIT_CYCLES);
     */

}

void Sleep() {
    LPM3; // Goto sleep
}

void LEDOn(uint8_t led) {
    switch (led) {
    case 0:
        P1OUT &= ~(1 << 2);
        break;
    case 1:
        P1OUT &= ~(1 << 3);
        break;
    case 2:
        P2OUT &= ~(1 << 2);
        break;
    case 3:
        P2OUT &= ~(1 << 3);
        break;
    case 4:
        P2OUT &= ~(1 << 4);
        break;
    case 5:
        P2OUT &= ~(1 << 5);
        break;
    case 6:
        P1OUT &= ~(1 << 6);
        break;
    case 7:
        P1OUT &= ~(1 << 7);
        break;
    default:
        break;
    }
}
void LEDOff(uint8_t led) {
    switch (led) {
    case 0:
        P1OUT |= (1 << 2);
        break;
    case 1:
        P1OUT |= (1 << 3);
        break;
    case 2:
        P2OUT |= (1 << 2);
        break;
    case 3:
        P2OUT |= (1 << 3);
        break;
    case 4:
        P2OUT |= (1 << 4);
        break;
    case 5:
        P2OUT |= (1 << 5);
        break;
    case 6:
        P1OUT |= (1 << 6);
        break;
    case 7:
        P1OUT |= (1 << 7);
        break;
    default:
        break;
    }
}
void LEDToggle(uint8_t led) {
    switch (led) {
    case 0:
        P1OUT ^= (1 << 2);
        break;
    case 1:
        P1OUT ^= (1 << 3);
        break;
    case 2:
        P2OUT ^= (1 << 2);
        break;
    case 3:
        P2OUT ^= (1 << 3);
        break;
    case 4:
        P2OUT ^= (1 << 4);
        break;
    case 5:
        P2OUT ^= (1 << 5);
        break;
    case 6:
        P1OUT ^= (1 << 6);
        break;
    case 7:
        P1OUT ^= (1 << 7);
        break;
    default:
        break;
    }
}

void AllLEDOn() {
    uint8_t i;
    for (i = 0; i < 8; i++)
        LEDOn(i);
}
void AllLEDOff() {
    uint8_t i;
    for (i = 0; i < 8; i++)
        LEDOff(i);
}
void AllLEDToggle() {
    uint8_t i;
    for (i = 0; i < 8; i++)
        LEDToggle(i);
}

void SetupWDTToWakeUpCPU(uint8_t clkdiv) {
    WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDTSSEL | clkdiv;
}

static uint16_t readCapPush() {
    uint16_t val;

    TACTL = TASSEL_3 | MC_2 | TACLR; // INCLK, Up-mode to 0xFFFF, clear timer
    WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDTSSEL | WDTIS1 | WDTIS0; // About 2 ms with 32 kHz clock
    LPM3; // Goto Sleep to conserve power
    WDTCTL = WDTPW | WDTHOLD; // After wake-up, disable WDT
    val = TAR; // Read timer
    TACTL = MC_0; // stop timer
    return val;
}

uint16_t readCapPushA() {
    uint16_t val;
    
    P2SEL2 |= (1 << 1); // P2.1 is the capacitive touch sensor
    val = readCapPush();
    P2SEL2 &= ~(1 << 1);
    
    return val;
}

uint16_t readCapPushB() {
    uint16_t val;

    P2SEL2 |= (1 << 0); // P2.0 is the capacitive touch sensor
    val = readCapPush();
    P2SEL2 &= ~(1 << 0);

    return val;
}
void BuzzerOn() {
    P1OUT |= (1 << 0);
}
void BuzzerOff() {
    P1OUT &= ~(1 << 0);
}

uint16_t ReadTemp() {
    uint16_t value;

    P1OUT |= (1 << 5); // Turn ON power to the resistor divider
    ADC10CTL1 = INCH_4 + ADC10DIV_3;
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE + ENC + ADC10SC; // Sampling and conversion start
    LPM3; // Goto sleep
    value = ADC10MEM;
    ADC10CTL0 &= ~ENC; // Disable ADC
    P1OUT &= ~(1 << 5); // Turn off power to the resistor divider

    return value;
}

uint16_t ReadBattery() {
    uint16_t value;

    ADC10CTL1 = INCH_11 + ADC10DIV_3;
    ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE; // Turn on ADC and reference
    __delay_cycles(30); // Wait while the reference settles (30 uS)
    ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion start
    LPM3; // Goto sleep
    value = ADC10MEM;
    ADC10CTL0 &= ~ENC; // Disable ADC
    ADC10CTL0 &= ~REFON; // Power down reference vg

    return value;
}
