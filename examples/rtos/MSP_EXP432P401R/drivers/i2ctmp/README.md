### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---
# i2ctmp

---

## Example Summary

Sample application that reads the temperature from a TMP sensor.

## Peripherals Exercised

* `Board_GPIO_LED0` - Indicator LED
* `Board_I2C_TMP` - I2C used to communicate with the TMP sensor.

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

## Example Usage

* Example output is generated through use of Display driver APIs. Refer to the
Display driver documentation found in the SimpleLink MCU SDK User's Guide.

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's
 Homepage"), etc.) to the appropriate COM port.
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

* Run the example. `Board_GPIO_LED0` turns ON to indicate driver
initialization is complete.

* The example will request temperature samples from an available TMP and display
them via the UART. A total of 20 temperature samples are read and printed before
the example exits. Terminal output should resemble:
```
    I2C Initialized!
    Sample 0: 24 (C)
    Sample 1: 24 (C)
           .
           .
           .
    Sample 18: 24 (C)
    Sample 19: 24 (C)
    I2C closed!
```

*  If the I2C communication fails, the following error message is displayed:
```
    I2C Bus fault.
```

## Application Design Details

This application uses one task:

`'getTempTask'` - performs the following actions:

1. Opens and initializes an I2C driver object.

2. Determines which TMP sensor is available. Attempts to find a TMP116 first,
   and then a TMP006.

3. Uses the I2C driver to get data from the TMP sensor.

4. Extracts the temperature (in Celsius) and prints the value via the UART.

5. The task sleeps for 1 second.

6. After 20 temperature samples are recovered, the I2C peripheral is closed
and the example exits.

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

Code Composer Studio:

* When using in CCS for the CC32XX devices, it's recommended you do
a  "Free Run" instead of "Run". This is because when the CC32XX device goes
into low power mode, emulation is lost.

SimpleLink Sensor and Actuator Plugin:

* This example uses a TMP sensor. It interfaces via the I2C Driver. For a
richer API set for the TMP sensors, please refer the SimpleLink Sensor and
Actuator Plugin. This plugin also offers a Button module to handle
debounce also.
