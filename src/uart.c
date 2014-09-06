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

#include "uart.h"

void sb(unsigned char c) {
	UARTsb(c);
}

void ss(char* str) {
	unsigned char c = *str++;
	while (c != '\0') {
		sb(c);
		c = *str++;
	}
}
void s4hex(unsigned char val) {
	unsigned char c = val + 0x30;
	if (val > 9)
		c += 0x07;
	sb(c);
}
void s16hex(unsigned int val) {
	unsigned char i, c;

	sb('0');
	sb('x');

	for (i = 0; i < 4; i++) {
		c = (val >> 4 * (3 - i)) & 0x0F;
		s4hex(c);
	}
}
void sval(int16_t val) {
	uint8_t tmp;
	if (val < -126)
		tmp = 0;
	else if (val > 126)
		tmp = 255;
	else
		tmp = 127 + val;
	sb(tmp);
}
void s32val(int32_t val) {
	uint8_t tmp;
	if (val < -126)
		tmp = 0;
	else if (val > 126)
		tmp = 255;
	else
		tmp = 127 + val;
	sb(tmp);
}
