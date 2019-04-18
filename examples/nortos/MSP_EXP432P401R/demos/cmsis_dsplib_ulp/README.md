## Example Summary

This demo software demonstrates the low power analog features of the MSP432 SimpleLink MCU. For more details read the Application Report http://www.ti.com/lit/an/slaa741/slaa741.pdf

## Example Usage

Upon running the demo, the device samples the differential analog channel A0 once every second. The valid voltage range on each pin is 0 to 1.2V.  This example provides no demonstrable output but the IDE can be used to verify/display the RMS and frequency calculations.

## Application Design Details

* No-RTOS
  * Clock System is configured to switch between MCLK = 3Mhz and 48 MHz.
  * Uses the CMSIS-DSP library in SDK for RMS calculation.
