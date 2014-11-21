cookoo
======

Pressure cooker whistle counter

Dependencies
------------

- MSPGCC and GNU Make (only if you want to re-build)
- Flash Programmer (TI Launchpad will do)
- Flash Programmer Tool (TI Code Composer Studio / FET-Pro430 / TI MSP430 Flasher/ mspdebug)

On Ubuntu 12.04, the dependencies can be installed with `sudo apt-get install binutils-msp430 gcc-msp430 msp430-libc mspdebug`

How-to-use
----------

- Build the circuit in the [schematic](schematic/whistle_counter.png) and program the MSP430 with the [hex file](bin/msp430cookoo.hex)
- [Gerber files](pcb/gerber) for the PCB are available along with images to [assist manual assembly](pcb/)
- The [STL files](enclosure/) may be used to 3D-print an enclosure

Notes
-----

- Pour hot glue over the crystal pins, the 22pF caps, and all over the MSP430 pins to prevent moisture from interfering with the low-power 32kHz crystal oscillator
- The [PCB design](pcb/) included has a minor inconvenience. For the wire-terminated thermistor, SMD0402 pads have been provided (rather than two through-holes)
