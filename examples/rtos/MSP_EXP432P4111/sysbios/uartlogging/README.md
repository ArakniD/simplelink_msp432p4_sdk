## Example Summary

Application that uses the UART driver and LoggerIdle to send Log data to the
UART.

## Peripherals & Pin Assignments

* `Board_GPIO_LED0` - Indicates that the board was initialized within `main()`
* `Board_UART0` - Used to send data to the RTOS Analyzer

## BoosterPacks, Board Resources & Jumper Settings

For board specific jumper settings, resources and BoosterPack modifications,
refer to the __Board.html__ file.

> If you're using an IDE such as Code Composer Studio (CCS) or IAR, please
refer to Board.html in your project directory for resources used and
board-specific jumper settings.

The Board.html can also be found in your SDK installation:

        <SDK_INSTALL_DIR>/source/ti/boards/<BOARD>

## Example Usage

* Run the example. `Board_GPIO_LED0` turns ON to indicate driver
initialization is complete.

* This example also demonstrates RTOS Analyzer in CCS. This is accomplished
by sending log records via UART from the target to the host.  First make
sure to close any terminal windows that are connected to the board's
application UART COM port.  Load and run the example in CCS.
In the CCS Tools menu, select RTOS Analyzer -> Execution
Analysis.  When the Analysis Configuration window pops up, configure the
COM port to the board's application UART port.  The name of the COM port
should show up in the Port Name drop-down list under Transport Settings.
The baud rate should be left as 115200. Then click the "Start" button on
the Analysis Configuration window.  The Execution Graph will pop up and
display thread execution.

* The UART uses the default 115200 baud rate.

## Application Design Details

* This example demonstrates how to use an application to send Log data through a
UART. Log data will go through `Board_UART0` which is connected to your PC
through the same USB as the JTAG.

* Log events will be sent during idle through `Board_UART0` and can be analyzed
by RTOS Analyzer, or the data can be captured by a program running on the
host. Your host program for collecting the Log data will need to use the serial
COM port that is connected to `Board_UART0`.

* Power management is disabled in this example to allow the Idle loop
to run sufficiently often so that the LoggerIdle circular buffer does not fill
up, causing records to be lost.

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
