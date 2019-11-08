## Example Summary

This example is the same as the "portable" example but instead of using
POSIX APIs, it uses the native RTOS APIs. This example is still portable
across devices (because of TI Drivers). This project requires less code/data
space since there is no POSIX layer. This example is described in more detail
in the _SDK User's Guide_ at
&lt;SDK_INSTALL_DIR&gt;/docs/simplelink_mcu_sdk/Users_Guide.html.

## Peripherals & Pin Assignments

SysConfig generates the driver configurations into the __ti_drivers_config.c__
and __ti_drivers_config.h__ files. Information on pins and resources used
is present in both generated files. The SysConfig user interface can also be
utilized to determine pins and resources used.


* `CONFIG_GPIO_LED_0` - Indicates that temperature has exceeded the alert
threshold
* `CONFIG_GPIO_BUTTON_1` - Used to restart the UART console
* `CONFIG_UART_0` - Used for a simple console
* `CONFIG_I2C_TMP` - Used to read temperature

If using the Building Automation Sensors BoosterPack, `CONFIG_GPIO_TMP116_EN`
is used to power on the TMP116 temperature sensor.

## BoosterPacks, Board Resources & Jumper Settings

* For boards with an on-board TMP sensor (e.g. CC32XX LaunchPads), the on-board
TMP sensor is used.

* For boards without an on-board TMP sensor, This example requires a
[__BOOSTXL-BASSENSORS BoosterPack__][boostxl-bassensors]. It has the TMP116
sensor along with several I2C sensors. CC32XX LaunchPads will default to the
BoosterPack's TMP116 sensor if present.

For board specific jumper settings, resources and BoosterPack modifications,
refer to the __Board.html__ file.

> If you're using an IDE such as Code Composer Studio (CCS) or IAR, please
refer to Board.html in your project directory for resources used and
board-specific jumper settings.

The Board.html can also be found in your SDK installation:

        <SDK_INSTALL_DIR>/source/ti/boards/<BOARD>

## Example Usage

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's
Homepage"), etc.) to the appropriate COM port.
    * The COM port can be determined via Device Manager in Windows or via
`ls /dev/tty*` in Linux.

The connection should have the following settings
```
    Baud-rate:  115200
    Data bits:       8
    Stop bits:       1
    Parity:       None
    Flow Control: None
```

* Run the example.

* The target displays a simple console on the serial session.
```
    Valid Commands
    --------------
    h: help
    q: quit and shutdown UART
    c: clear the screen
    t: display current temperature
```

* The DIE temperature is displayed. You'll need to change the temperature of
the actual TMP sensor to see a change. Please note to keep the example short
and sweet, the TMP sensor was not calibrated.

* If you quit the console, the device will go into a lower power mode.
To start the console back up, hit `CONFIG_GPIO_BUTTON_1`.

## Application Design Details

* This example shows how to initialize the UART driver in blocking read
and write mode.

* A thread, `consoleThread`, is a simple console. When the UART is closed
the device can go into a lower power mode.

* A thread, `temperatureThread`, reads the temperature via I2C. The thread reads
the sensor every second. The timing is controlled by a timer and
semaphore. This approach was used to eliminate time drift (which would have
occurred with simply using sleep(1)).

* The disabling of interrupts/tasking (depending on the kernel you are using)
is used to protect the reading/writing of the
temperature variables. The reading/writing of these two variables must be
done atomically. For example without critical region management, the
temperature thread (which is higher priority) could interrupt the reading of
the variables by the console thread. This could result with the console thread
printing a Celsius value that did not match up with the Fahrenheit value.

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

* This example uses a TMP006 or TMP116 sensor. It interfaces via the
I2C Driver. For a richer API set for the TMP sensors, please refer the SimpleLink
Sensor and Actuator Plugin.

[boostxl-bassensors]: http://www.ti.com/tool/BOOSTXL-BASSENSORS