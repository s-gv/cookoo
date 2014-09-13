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

#include <msp430.h>
#include <inttypes.h>

#include "cappush.h"
#include "cookoo.h"
#include "uart.h"
#include "hal.h"

int main(void) {
    uint16_t tempSensor, battery;
    uint8_t capPushA, capPushB;
    uint8_t it=1;

    InitWDT();
    InitCLOCK();
    InitUART();
    InitLED();
    InitADC();
    InitBuzzer();
    InitCapPush();
    
    EnableInterrupts();
    while (1) {
        /*
        capPushB = senseCapPushB(); 
        tempSensor = ReadTemp();
        battery = ReadBattery(); 
        
        //MainLoop(capPushA, capPushB, tempSensor, battery);
        */
        it--;
        if(it == 0) {
            it = 1;
            tempSensor = ReadTemp();
            battery = ReadBattery(); 
        }
        SetupWDTToWakeUpCPU(2); // Wake up in 16 mS
        Sleep();
    }
    return 0;
}
