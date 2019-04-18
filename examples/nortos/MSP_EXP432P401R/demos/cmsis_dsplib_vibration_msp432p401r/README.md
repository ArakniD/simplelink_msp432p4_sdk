## Example Summary

This demo software demonstrates detecting vibration noise in sub-sonic, audio-band and ultra-sonic spectrum on the MSP-EXP432P401R LaunchPad development kit. For more details read the Application Report http://www.ti.com/lit/an/slaa735/slaa735.pdf

## Example Usage

Upon running the demo, the device samples the Analog channels A0, A1 and A2 once every 5 minutes. If the the device detects a frequency above the specified threshold

1. In the sub-sonic band it will light up the RED LED on the LaunchPad.
2. In the audio band it will light up the GREEN LED on the LaunchPad.
3. In the ultra-sonic band it will light up the BLUE LED on the LaunchPad.

At the same time it will change the sampling duration from 5 minutes to 2 minutes, unless the issue is resolved.

## Application Design Details

* No-RTOS
  * Clock System is configured for MCLK = 48 MHz.
  * Uses the CMSIS-DSP library in SDK for FFT transform functions.
