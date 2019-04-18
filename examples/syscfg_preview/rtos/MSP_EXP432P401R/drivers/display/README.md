### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---
# display

---

## Example Summary

Writes to the UART and the available LCD display using the Display driver.

## Peripherals Exercised

* `Board_GPIO_LED0` - Toggled by example task periodically
* `Board_UART` - Display UART output
* `Board_SPI` - Display LCD output

## Resources & Jumper Settings

> This example requires an external display device.  If you're using an IDE
(such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

## Example Usage

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

* Run the example.

* A splash screen will be displayed on the LCD and the string "Hello Serial!"
on UART

* `Board_GPIO_LED0` is toggled periodically

* The state of LED0 is output periodically on the UART and the LCD

## Application Design Details

> The board file is configured to use the BOOSTXL-SHARP128 BoosterPack which
 has a 128x128 pixel display. This example is compatible with the
 430BOOST-SHARP96 which has a 96x96 pixel display. Update the
 `BOARD_DISPLAY_SHARP_SIZE` definition in the _Board.c_ file to 96 if you
 want to use the 430BOOST-SHARP96 display. SysConfig users may simply select
 the 430BOOST-SHARP96 from the "Use Hardware" option of the Display module or
 alternatively, make the following change in the _display.syscfg_ manually:
```
var SharpLCD = scripting.addHardware("/ti/boards/boosterpacks/MSP430BOOST-SHARP96");
```

TI-RTOS:

* When building in Code Composer Studio, the kernel configuration project will
be imported along with the example. The kernel configuration project is
referenced by the example, so it will be built first. The "release" kernel
configuration is the default project used. It has many debug features disabled.
These features include assert checking, logging and runtime stack checks. For a
detailed difference between the "release" and "debug" kernel configurations and
how to switch between them, please refer to the SimpleLink MCU SDK User's
Guide. The "release" and "debug" kernel configuration projects can be found
under &lt;SDK_INSTALL_DIR&gt;/kernel/tirtos/builds/&lt;BOARD&gt;/(release|debug)/(ccs|gcc).

FreeRTOS:

* Please view the `FreeRTOSConfig.h` header file for example configuration
information.
