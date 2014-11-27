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

static uint8_t whistleCount, time, noButtonPressTime;
static uint8_t downSample;
static uint8_t whistleOneCount;
static uint16_t beepTime;
static uint16_t tempLow[TEMP_LOW_PASS_FILTER_N];
static uint16_t tempHi[TEMP_HI_PASS_FILTER_N];
static uint8_t batteryLowCounter;
static uint8_t overTemp;
static uint8_t statusChange;
static uint8_t LEDStatus;
static uint8_t lastWhistleCount;

static void ShowStatus() {
    uint8_t i;
    if(LEDStatus == 0 || lastWhistleCount != whistleCount) {
        statusChange += 1;
        AllLEDOff();
        for (i = 0; i < whistleCount; i++) {
            LEDOn(i);
        }
    }
    LEDStatus = 1;
    lastWhistleCount = whistleCount;
}
static void CancelStatus() {
    AllLEDOff();
    if(LEDStatus == 1)
        statusChange += 1;
    LEDStatus = 0;
}

void MainLoop(uint8_t capPushA, uint8_t capPushB, uint16_t tempSensor, uint16_t battery) {
    if(statusChange) { // Don't detect cap touch when the LED status has just changed
        capPushA = 0;
        capPushB = 0;
        statusChange--;
    }
    if (battery < BATTERY_LOW_THRESH) {
        //About 2.5v
        if (batteryLowCounter < 200)
            batteryLowCounter++;
    }
    if ((capPushA == 1) || (capPushB == 1)) {
        beepTime = 0; // Stop beeping the buzzer when the user touches either of the capacitive touch buttons
        if (noButtonPressTime < STATUS_CANCEL_THRESH) { // Was a cap touch button pressed recently ?
            // If yes, this press must have been to increase (button A) or decrease (button B) the whistle count
            if (capPushA == 1) {
                if (whistleCount < MAX_WHISTLE_COUNT && batteryLowCounter < BATTERY_LOW_COUNT_THRESH) {
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
    if (downSample == TEMP_DOWNSAMPLE_FACTOR) {
        downSample = 0;
        uint16_t val = tempSensor; // Process thermistor samples once every ~0.25 sec

        uint32_t val_lo = mafilt(tempLow, val, TEMP_LOW_PASS_FILTER_N); // low-pass filter the thermistor (ADC) readings to suppress noise
        //sb(val_lo >> 4);
        if(val_lo > OVER_TEMP_THRESH_HI)
            overTemp = 1;
        else if(val_lo < OVER_TEMP_THRESH_LO)
            overTemp = 0;

        int32_t val_hi = (TEMP_HI_PASS_FILTER_N) * val_lo - mafilt(tempHi, val_lo, TEMP_HI_PASS_FILTER_N); // high-pass filter to detect only changes in temperature
        if (val > MIN_TEMP_WHISTLE_DETECTION && val_hi < -MIN_TEMP_FALL_WHISTLE_DETECTION) {
            // detect whistle when the temperature is high (> ~100C), and the temperature has sharply fallen
            whistleOneCount++;
        } else {
            if (whistleOneCount > MIN_TEMP_FALL_DURATION_WHISTLE_DETECTION) { // See if the temperature fell for a duration greater than 3 secs before rising again
                // Whistle detection complete !
                //sb(128);
                if (whistleCount > 0) { 
                    whistleCount--;
                    if (whistleCount == 0) { // Number of whistles == Target number of whistles ?
                        beepTime = WHISTLE_BEEP_TIME; // Start beeping the buzzer for 45 seconds or until the user touches a cap touch button
                    }
                    statusChange += 6; // to prevent false positives in cap touch after a whistle is detected and the LEDs are flashed
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
            statusChange += 1;
        } else {
            BuzzerOff();
        }
        beepTime--;
    } else {
        BuzzerOff();
    }

    if(overTemp)
        BuzzerOn();

    //sb(battery >> 2);
    //sb(tempSensor >> 2);

}
