## Example Summary

Sample application that reads the temperature from a TMP sensor.

## Peripherals & Pin Assignments

SysConfig generates the driver configurations into the __ti_drivers_config.c__
and __ti_drivers_config.h__ files. Information on pins and resources used
is present in both generated files. The SysConfig user interface can also be
utilized to determine pins and resources used.

* `CONFIG_GPIO_LED_0` - Indicator LED
* `CONFIG_I2C_TMP` - I2C bus used to communicate with the TMP sensor.
* `CONFIG_GPIO_TMP116_EN` - TMP116 power pin

## BoosterPacks, Board Resources & Jumper Settings

This example requires a
[__BOOSTXL-BASSENSORS BoosterPack__][boostxl-bassensors].

For board specific jumper settings, resources and BoosterPack modifications,
refer to the __Board.html__ file.

> If you're using an IDE such as Code Composer Studio (CCS) or IAR, please
refer to Board.html in your project directory for resources used and
board-specific jumper settings.

The Board.html can also be found in your SDK installation:

        <SDK_INSTALL_DIR>/source/ti/boards/<BOARD>

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

* Run the example. `CONFIG_GPIO_LED_0` turns ON to indicate driver
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

SimpleLink Sensor and Actuator Plugin:

* This example uses a TMP sensor. It interfaces via the I2C Driver. For a
richer API set for the TMP sensors, please refer the SimpleLink Sensor and
Actuator Plugin. This plugin also offers a Button module to handle
debounce also.

[boostxl-bassensors]: http://www.ti.com/tool/BOOSTXL-BASSENSORS