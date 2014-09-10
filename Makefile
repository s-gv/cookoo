# Copyright (c) 2014, Sagar G V (sagar.writeme@gmail.com) and Dhruv Saxena (dhruvrai23@gmail.com)
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:

# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.

# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.

# * Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


TARGET = msp430cookoo

MCU = msp430g2232

PROGRAMMER = rf2500 # Use TI MSP430 launchpad as flash programmer

OBJDIR = obj
INCDIR = inc
BINDIR = bin
SRCDIR = src

CC = msp430-gcc
OBJCOPY = msp430-objcopy
SIZE = msp430-size

CFLAGS = -mmcu=$(MCU) -Wall -Wunused -O2 -I$(INCDIR)
LFLAGS = -mmcu=$(MCU) -O2

SRCS := $(wildcard $(SRCDIR)/*.c)
INCS := $(wildcard $(INCDIR)/*.h)
OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(BINDIR)/$(TARGET).hex

program: $(BINDIR)/$(TARGET).hex
	# As of Sept. 2014, MSP430G2232 is not supported by mspdebug
	mspdebug $(PROGRAMMER) "prog $(BINDIR)/$(TARGET).hex" exit

$(BINDIR)/$(TARGET).hex: $(BINDIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(BINDIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@
	$(SIZE) $(BINDIR)/$(TARGET).elf

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONEY: clean
clean:
	rm -f $(OBJS) $(BINDIR)/$(TARGET).hex $(BINDIR)/$(TARGET).elf

