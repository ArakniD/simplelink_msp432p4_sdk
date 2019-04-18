## Example Summary

This demo visualizes 512-point real FFT results calculated from audio samples gathered in real-time from the onboard microphone of the Educational BoosterPack plug-in module MKII. The demo uses the real FFT function contained in the CMSIS DSP Software Library.

For more information on Signal Processing with the CMSIS-DSP library please
refer to our [Application Note](http://www.ti.com/lit/an/slaa707/slaa707.pdf).

## BoosterPack Requirements

This demo requires the [Educational BoosterPack MKII (BOOSTXL-EDUMKII)](http://www.ti.com/tool/BOOSTXL-EDUMKII) paired with the MSP-EXP432P401R LaunchPad

## Example Usage

The program uses the MSP432's built-in Precision ADC module to sample audio signal at 8 KHz from the microphone on the Educational BoosterPack plug-in module MKII. 512-point real
FFT calculation is done in real-time on the collected audio samples, and the resulting frequency bin data is displayed in a bar graph on the BoosterPack plug-in module's colored 128x128 dot-matrix LCD.

* Try generating a pure tone ranging from 1 Hz to 4000 Hz and see if the demo shows the correct frequency.

## Application Design Details

* No-RTOS
  * Precision ADC is configured to sample accelerometer data from A10 analog input channel.
  * Clock System is configured with MCLK = 48 MHz.
  * EUSCI_B0 is used for SPI communication with the LCD controller.
  * DMA is setup in Ping-Pong mode to alternate between two data buffers.
