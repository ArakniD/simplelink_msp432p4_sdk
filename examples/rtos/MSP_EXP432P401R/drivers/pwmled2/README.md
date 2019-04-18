### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---
# pwmled2

---

## Example Summary

Sample application to control on-board LEDs with the PWM driver.

## Peripherals Exercised

* `Board_PWM0` - PWM instance used to control brightness of LED connected to `Board_PWM0`
* `Board_PWM1` - PWM instance used to control brightness of LED connected to `Board_PWM1`

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.


## Example Usage

* Run the example.

* The onboard LEDs will slowly vary in intensity.

* Both LEDs connected to `Board_PWM0` and `Board_PWM1` will fade-in and fade-out
when running the application.

## Application Design Details

This application uses one thread, `mainThread` , which performs the following
actions:

1. Opens and initializes PWM driver objects.

2. Uses the PWM driver to change the intensity of the LEDs.

3. The thread sleeps for 50 milliseconds before changing LED intensity again.

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
