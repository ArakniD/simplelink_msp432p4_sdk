### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---
# capturepwmdisplay

---

## Example Summary

This example will capture the output of a PWM signal and
periodically print out the captured period in microseconds.

## Peripherals Exercised

* `Board_CAPTURE1` - Capture instance that detects two consecutive edges
* `Board_PWM0` - Generates a PWM signal to blink a LED.
* `Board_PWM1` - Generates a PWM signal to be captured.

## Resources & Jumper Settings

* `Board_PWM0` pin must be jumpered to see the LED blink.

* Connect the `Board_PWM1` pin to the `Board_CAPTURE1` pin.

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

## Example Usage

* Example output is generated through use of Display driver APIs. Refer to the
Display driver documentation found in the  SimpleLink MCU SDK User's Guide.

* The example performs general initialization in `mainThread`.

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's
Homepage"), etc.) to the appropriate COM port to see the output.
    * The COM port can be determined via Device Manager in Windows or via
`ls /dev/tty*` in Linux.

The connection will have the following settings:
```
    Baud-rate:     115200
    Data bits:          8
    Stop bits:          1
    Parity:          None
    Flow Control:    None
```

## Application Design Details

This application uses one thread, `mainThread` , which performs the following
actions:

1. Opens a Display of type UART

2. Opens two PWMs. One to blink the LED and the second to drive the Capture
input.

3. Opens and initializes Capture driver objects.

4. Waits for two consecutive rising edges on the configured hardware port

5. Outputs the time in uS between edges of the input capture to the Display

> In some cases, the captured period may not be exactly equal to the pwm
period. This is due to the clock source and/or clock dividers used by the
capture and pwm drivers. For specific applications, an appropriate clock source
and divider should be selected.

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
