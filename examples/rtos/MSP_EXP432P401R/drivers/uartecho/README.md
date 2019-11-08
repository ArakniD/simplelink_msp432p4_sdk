## Example Summary

Example that uses the UART driver to echo back to the console.

## Peripherals & Pin Assignments

SysConfig generates the driver configurations into the __ti_drivers_config.c__
and __ti_drivers_config.h__ files. Information on pins and resources used
is present in both generated files. The SysConfig user interface can also be
utilized to determine pins and resources used.

* `CONFIG_GPIO_LED_0` - Indicates that the board was initialized within `main()`
* `CONFIG_UART_0` - Used to echo characters from host serial session

## BoosterPacks, Board Resources & Jumper Settings

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

* Run the example. `CONFIG_GPIO_LED_0` turns ON to indicate driver
initialization is complete.

* The target echoes back any character that is typed in the serial session.

* If the serial session is started before the target completes initialization,
the following is displayed:
`Echoing characters:`

## Application Design Details

* This example shows how to initialize the UART driver in blocking read
and write mode with no data processing and echo characters back to a console.

* A single thread, `echo`, reads a character from `CONFIG_UART_0` and writes it
back.

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
