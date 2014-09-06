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

#include "cookoo.h"
#include "fir.h"

#define NLOW 4
#define NHI 32

uint8_t whistleCount, time, noButtonPressTime;
uint8_t downSample;
uint8_t whistleOneCount;
uint16_t beepTime;
uint16_t tempLow[NLOW];
uint16_t tempHi[NHI];

void ShowStatus() {
    uint8_t i;
    AllLEDOff();
    for (i = 0; i < whistleCount; i++) {
        LEDOn(i);
    }
}
void CancelStatus() {
    AllLEDOff();
}

void MainLoop(uint8_t capPushA, uint8_t capPushB, uint16_t tempSensor) {
    if ((capPushA == 1) || (capPushB == 1)) {
        beepTime = 0; // Stop beeping the buzzer when the user touches either of the capacitive touch buttons
        if (noButtonPressTime < STATUS_CANCEL_THRESH) { // Was a cap touch button pressed recently ?
            // If yes, this press must have been to increase (button A) or decrease (button B) the whistle count
            if (capPushA == 1) {
                if (whistleCount < MAX_WHISTLE_COUNT) {
                    whistleCount++; // increase the target whistle count
                }
            } else if (capPushB == 1) {
                if (whistleCount > 0) {
                    whistleCount--; // decrease the target whistle count
                }
            }
        }
        ShowStatus(); // When a cap touch button is pressed, show status (number of whistles left) immediately
        noButtonPressTime = 0; // Keep track of how much time since a cap touch button was pressed
    } else {
        if (noButtonPressTime < 250)
            noButtonPressTime++;
        // After showing the status, blink the LED and shut it off to conserve power
        if ((noButtonPressTime > STATUS_BLINK_THRESH
                && noButtonPressTime < STATUS_CANCEL_THRESH2)
                || noButtonPressTime < STATUS_CANCEL_THRESH) {
            ShowStatus();
        } else {
            CancelStatus();
        }
    }
    time++;
    downSample++;
    if (downSample == 4) {
        downSample = 0;
        uint16_t val = tempSensor; // Process thermistor samples once every ~0.25 sec

        uint32_t val_lo = mafilt(tempLow, val, NLOW); // low-pass filter the thermistor (ADC) readings to suppress noise
        int32_t val_hi = (NHI) * val_lo - mafilt(tempHi, val_lo, NHI); // high-pass filter to detect only changes in temperature
        if (val > 100 && val_hi < -1000) {
            // detect whistle when the temperature is high (> ~100C), and the temperature has sharply fallen
            whistleOneCount++;
        } else {
            if (whistleOneCount > 12) { // See if the temperature fell for a duration greater than 3 secs before rising again
                // Whistle detection complete !
                //sb(128);
                if (whistleCount > 0) { 
                    whistleCount--;
                    if (whistleCount == 0) { // Number of whistles == Target number of whistles ?
                        beepTime = 767; // Start beeping the buzzer for 45 seconds or until the user touches a cap touch button
                    }
                }
                ShowStatus(); // after each whistle, show how many whistles are left on the LEDs
                noButtonPressTime = 0; // To blink the LED, invoke the same code that shows status when a cap button is touched
            }
            whistleOneCount = 0;
        }
        //sb(10);
    }

    if (beepTime > 0) {
        if( (beepTime & 0x0F) > 10 ) {
            BuzzerOn(); // Alternately beep the buzzer
        } else {
            BuzzerOff();
        }
        beepTime--;
    } else {
        BuzzerOff();
    }

}
