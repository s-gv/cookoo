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

#include "cappush.h"

uint16_t capPushABuf[N_CAP_PUSH_BUF], capPushBBuf[N_CAP_PUSH_BUF];
uint8_t backOffA0, backOffA1, backOffB0, backOffB1, downSampleA, downSampleIgnoreCountA, downSampleB, downSampleIgnoreCountB;

// DRY (Don't Repeat Yourself) principle violated here for performance reasons. 

uint8_t senseCapPushA() {
    uint8_t status = 0;
    uint16_t val = capPushABuf[0];//readCapPushA();
    downSampleA++; // down sample the cap touch sensing
    if(downSampleIgnoreCountA > 0) {
        downSampleIgnoreCountA--; // A while after a button is touched, start down-sampling again
    }
    if((downSampleA == CAP_PUSH_DOWN_SAMPLE_FACTOR) || (downSampleIgnoreCountA > 0)) {
        downSampleA = 0;
        val = readCapPushA();
    }
    uint16_t filt = mafilt(capPushABuf, val, N_CAP_PUSH_BUF);
    int16_t hp_filt = N_CAP_PUSH_BUF * val - filt; // high pass filter the frequency of the cap touch relaxation osc.
    //sval(hp_filt);

    if (hp_filt < -THRESH1) { // Fire 'onPress' event when relaxation osc. frequency falls rapidly
        downSampleIgnoreCountA = RAPID_SAMPLE_DURATION; // Keep sensing rapidly (don't downsample) for the next 2 secs
        downSampleIgnoreCountB = RAPID_SAMPLE_DURATION;
        if (!backOffA0) {
            status = 1;
            backOffA0 = 1; // don't fire 'onPress' again until relaxation osc. frequency stops falling
        }
    } else if (hp_filt > -THRESH2) {
        backOffA0 = 0; // Now, 'onPress' can be fired again
    }

    if (hp_filt > THRESH1) { // Fire 'onRelease' event when relaxation osc. frequency falls rapidly
        downSampleIgnoreCountA = RAPID_SAMPLE_DURATION; // Keep sensing rapidly (don't downsample) for the next 2 secs
        downSampleIgnoreCountB = RAPID_SAMPLE_DURATION;
        if (!backOffA1) {
            status = 2;
            backOffA1 = 1; // don't fire 'onRelease' again until relaxation osc. frequency stops falling
        }
    } else if (hp_filt < THRESH2) {
        backOffA1 = 0; // Now, 'onRelease' can be fired again
    }

    return status;
}
uint8_t senseCapPushB() {
    uint8_t status = 0;
    uint16_t val = capPushBBuf[0];//readCapPushB();
    downSampleB++;
    if(downSampleIgnoreCountB > 0) {
        downSampleIgnoreCountB--;
    }
    if((downSampleB == CAP_PUSH_DOWN_SAMPLE_FACTOR) || (downSampleIgnoreCountB > 0)) {
        downSampleB = 0;
        val = readCapPushB();
    }
    uint16_t filt = mafilt(capPushBBuf, val, N_CAP_PUSH_BUF);
    int16_t hp_filt = N_CAP_PUSH_BUF * val - filt;

    //sval(hp_filt);

    if (hp_filt < -THRESH1) {
        downSampleIgnoreCountA = RAPID_SAMPLE_DURATION; // Keep sensing rapidly (don't downsample) for the next 2 secs
        downSampleIgnoreCountB = RAPID_SAMPLE_DURATION;
        if (!backOffB0) {
            status = 1;
            backOffB0 = 1;
        }
    } else if (hp_filt > -THRESH2) {
        backOffB0 = 0;
    }

    if (hp_filt > THRESH1) {
        downSampleIgnoreCountA = RAPID_SAMPLE_DURATION; // Keep sensing rapidly (don't downsample) for the next 2 secs
        downSampleIgnoreCountB = RAPID_SAMPLE_DURATION;
        if (!backOffB1) {
            status = 2;
            backOffB1 = 1;
        }
    } else if (hp_filt < THRESH2) {
        backOffB1 = 0;
    }

    return status;
}
