## Example Summary

This example will capture the output of a PWM signal and
periodically print out the captured period in microseconds.

## Peripherals & Pin Assignments

SysConfig generates the driver configurations into the __ti_drivers_config.c__
and __ti_drivers_config.h__ files. Information on pins and resources used
is present in both generated files. The SysConfig user interface can also be
utilized to determine pins and resources used.

* `CONFIG_CAPTURE_0` - Capture instance that detects two consecutive edges
* `CONFIG_PWM_0` - Generates a PWM signal to blink a LED.
* `CONFIG_PWM_1` - Generates a PWM signal to be captured.

## BoosterPacks, Board Resources & Jumper Settings

For board specific jumper settings, resources and BoosterPack modifications,
refer to the __Board.html__ file.

> If you're using an IDE such as Code Composer Studio (CCS) or IAR, please
refer to Board.html in your project directory for resources used and
board-specific jumper settings.

The Board.html can also be found in your SDK installation:

        <SDK_INSTALL_DIR>/source/ti/boards/<BOARD>

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

* `CONFIG_PWM_0` pin must be jumpered to see the LED blink.

* Connect the `CONFIG_PWM_1` pin to the `CONFIG_CAPTURE_0` pin.

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
