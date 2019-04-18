### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---
# timerled

---

## Example Summary

Sample application to periodically toggle an LED based off a timer.

## Peripherals Exercised

* `Board_TIMER0` - Timer instance that toggles the LED.
* `Board_GPIO_LED0` - LED toggled in the callback function.

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.


## Example Usage

* The example performs general initialization in `mainThread`.

## Application Design Details

This application uses one thread, `mainThread`. The timer is operating in
Timer_CONTINUOUS_CALLBACK mode which causes the callback function,
`timerCallback`, to be called at a rate specified by `period`. A `period` of
1,000,000 microseconds or 1 second is used. Because the LED is toggled each
time `timerCallback` is called, the observed frequency the LED blinks is once
every 2 seconds.

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
