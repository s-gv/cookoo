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

#ifndef COOKOO_H_
#define COOKOO_H_

#include <inttypes.h>
#include "hal.h"
#include "uart.h"

#define MAX_WHISTLE_COUNT 8

#define STATUS_CANCEL_THRESH 28 // When no activity, waits for this time before shutting off status
#define STATUS_BLINK_THRESH 31 // When no activity, waits upto this time to blink status.
#define STATUS_CANCEL_THRESH2 40 // When no activity, shuts off status beyond this time
#define BATTERY_LOW_COUNT_THRESH 5 // When the battery is low, wait for this time before disabling the device
#define BATTERY_LOW_THRESH 850

#define NLOW 4
#define NHI 32

#define OVER_TEMP_THRESH_LO (700*NLOW)
#define OVER_TEMP_THRESH_HI (715*NLOW)

// This function gets called once every 0.1 sec
void MainLoop(uint8_t capPushA, uint8_t capPushB, uint16_t tempSensor, uint16_t battery);

void ShowStatus(); // Show current status (number of whistles yet to be done)
void CancelStatus(); // Turn off status LEDs

#endif /* COOKOO_H_ */
