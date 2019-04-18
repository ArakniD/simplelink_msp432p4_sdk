### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---
# uartecho

---

## Example Summary

Example that uses the UART driver to echo back to the console.

## Peripherals Exercised

* `Board_GPIO_LED0` - Indicates that the board was initialized within `main()`
* `Board_UART0` - Used to echo characters from host serial session

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.


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

* Run the example. `Board_GPIO_LED0` turns ON to indicate driver
initialization is complete.

* The target echoes back any character that is typed in the serial session.

* If the serial session is started before the target completes initialization,
the following is displayed:
`Echoing characters:`

## Application Design Details

* This example shows how to initialize the UART driver in blocking read
and write mode with no data processing and echo characters back to a console.

* A single thread, `echo`, reads a character from `Board_UART0` and writes it
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

IAR:

* When using any SensorTag(STK) Board, the XDS110 debugger must be
selected with the 4-wire JTAG connection within your projects' debugger
configuration.
