## Example Summary

Application that demonstrates the different device performance levels that are
possible in the application. The application steps between the performance
levels, and also displays the CPU clock speed at each performance level

## Peripherals & Pin Assignments

SysConfig generates the driver configurations into the __ti_drivers_config.c__
and __ti_drivers_config.h__ files. Information on pins and resources used
is present in both generated files. The SysConfig user interface can also be
utilized to determine pins and resources used.


* `CONFIG_GPIO_LED_0` - Indicates that the board was initialized within
`mainThread()`

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

Messages should appear via UART as follows:
```
Initial perf level = 2
MCLK = 48000000
HSMCLK = 24000000
SMCLK = 12000000
ACLK = 32768
= = = = = = = = =
Set perf 1 status = 0
Perf level = 1
MCLK = 24000000
HSMCLK = 6000000
SMCLK = 6000000
ACLK = 32768
= = = = = = = = =
Set perf 0 status = 0
Perf level = 0
MCLK = 12000000
HSMCLK = 3000000
SMCLK = 3000000
ACLK = 32768
= = = = = = = = =
Set perf 3 status = 0
Perf level = 3
MCLK = 48000000
HSMCLK = 48000000
SMCLK = 24000000
ACLK = 32768
= = = = = = = = =
Set perf 2 status = 0
Perf level = 2
MCLK = 48000000
HSMCLK = 24000000
SMCLK = 12000000
ACLK = 32768
```

## Application Design Details

This application uses one thread:

`mainThread` - perfroms the following actions:

1. Turns on `CONFIG_GPIO_LED_0`

2. Prints the initial performance level and clock frequencies.

3. Sets up power notifications
    * `notifyFxn` is called when a performance level change is initiated or
completed.
    * `CONFIG_GPIO_LED_0` turns OFF when Performace Change Starts and turns OFF
when Performance change Ends

4. `runPerfLevel` is called consecutively; adjusting the performance level
three times.

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
