### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---
# cmsis_dsplib_overSample_rtos_msp432p401r

---

## Example Summary

This example is intended to demonstrate oversampling with the precision
ADC.  A GPIO button press is used to start the conversion of 256K samples.  After each 1K
sample the CPU uses the CMSIS decimation filter to reduce to 32 samples.  After all 256K samples 
are collected the final 8K sample result is displayed via UART and then the device returns to 
sleep waiting for another GPIO event.

## Peripherals Exercised

* 'Board_ADCBUF0CHANNEL3' 
* GPIO as defined in overSample.c
* 'Board_UART0' Display result

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.


## Example Usage

* The example measures the differential voltage between P5.4 and P5.5 256K times
at 1Msps and decimates the output to 8K sample with an effective sampling rate of 
31.25Ksps.

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
