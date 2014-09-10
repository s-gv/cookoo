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

#ifndef HAL_H_
#define HAL_H_

#include <msp430.h>
#include <inttypes.h>

#include "uart.h"

// Call once before the main loop
void InitWDT();
void InitCLOCK();
void InitUART();
void InitLED();
void InitBuzzer();
void StopWDT();
void InitADC();
void EnableInterrupts(); // Do after all other inits are done
void InitCapPushButtons();

// Call in the main loop
uint16_t ReadBattery(); // With 1.5v band-gap, VCC/2 is read. 

void BuzzerOn();
void BuzzerOff();

void SetupWDTToWakeUpCPU(); // If CPU is sleeping, it is woken up once after about 16 ms at 32 kHz

uint16_t readCapPushA(); // read capacitive push button A (returns 1 when button has just been pushed)
uint16_t readCapPushB(); // read capacitive push button B (returns 1 when button has just been pushed)

void UARTsb(uint8_t val); // Send byte over UART

void Sleep();

uint16_t ReadTemp(); // Read temperature by digitizing the voltage of the potential divider formed by the thermistor

void LEDOn(uint8_t led); // led is in the range 0-7
void LEDOff(uint8_t led); // led is in the range 0-7
void LEDToggle(uint8_t led); // led is in the range 0-7
void AllLEDOn();
void AllLEDOff();
void AllLEDToggle();

#endif /* HAL_H_ */
