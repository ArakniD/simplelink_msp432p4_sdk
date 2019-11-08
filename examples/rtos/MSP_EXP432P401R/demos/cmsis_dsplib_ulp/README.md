---
# cmsis_dsplib_ulp_rtos

---

## Example Summary

This example is intended to demonstrate the low power mode of the precision
ADC.  The RTC is used to wake the device from deep sleep 0 (LPM3) and start
a repeated single-channel conversion 4096 times at 200ksps.  After each 1K
sample the CPU wakes from LPM0 to compute RMS and frequency while continuing
to collect the remaining samples.  After all samples are collected the final
result is displayed via UART and then the device returns to deep sleep 0.
The RTC interval is 1s.

## Peripherals Exercised

* 'Board_ADCBUF0CHANNEL3' 
* RTC as defined in ulpMeasure.c
* 'Board_UART0' Display result

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.


## Example Usage

* The example measures the differential voltage between P5.4 and P5.5 and
  calculates the frequency and RMS voltage of the signal over a 4K sample, 
  200ksps period (~20ms).

## Application Design Details

TI-RTOS:

* When building in Code Composer Studio, the kernel configuration project will
be imported along with the example. The kernel configuration project is
referenced by the example, so it will be built first. The "release" kernel
configuration is the default project used. It has many debug features disabled.
These feature include assert checking, logging and runtime stack checks. For a
detailed difference between the "release" and "debug" kernel configurations and
how to switch between them, please refer to the SimpleLink MCU SDK User's
Guide. The "release" and "debug" kernel configuration projects can be found
under &lt;SDK_INSTALL_DIR&gt;/kernel/tirtos/builds/&lt;BOARD&gt;/(release|debug)/(ccs|gcc).

FreeRTOS:

* Please view the `FreeRTOSConfig.h` header file for example configuration
information.
