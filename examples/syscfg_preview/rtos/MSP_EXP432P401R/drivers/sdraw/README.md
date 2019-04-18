### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---
# sdraw

---

## Example Summary

Sample application to read and write data onto an SD Card (SD interface).
> Running this application will cause any filesystem on the SD card to
  become corrupted!

## Peripherals Exercised

* `Board_GPIO_LED0` - Indicates that the board was initialized within
`mainThread()`
* `Board_SD0`  - Connection to the SD card

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

This example requires an external SD Card BoosterPack.

## Example Usage

* Example output is generated through use of Display driver APIs. Refer to the
Display driver documentation found in the  SimpleLink MCU SDK User's Guide.

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

* Edit the sdraw.c file and set WRITEENABLE to 1, and build the example.
  This will allow data to be written to the SD card.  __WARNING:__ Any file
  system present on the SD card will be overwritten and corrupted by this
  action!

* Run the example. `Board_GPIO_LED0` turns ON to indicate driver initialization
is complete.

* The example proceeds to write a buffer of data to the SD card, overwriting
  any filesystem currently present on the card.

* The raw data that was written to the file is then read and compared with
  the original data.  If the data read does not match what was written,
  an error message will be displayed.

* Set WRITEENABLE back to 0, build, and run the example.  The example will
  only read the contents of the file in this case.

## Application Design Details

* This application demonstrates how to use the SD Card driver without
  a filesystem to perform disk I/O operations on an SD Card using
  simple write/read calls.


This application uses one thread:

The `mainThread` performs the following actions:

1. Create and initialize the SD driver object.

2. Calculate the total card capacity from the sector size and number
   of sectors on the SD card.

3. Write the raw data in `textarray` to the file.

4. Read back the raw data from the SD card into a temporary buffer, `cpy_buff`
   and print its contents if the data read does not match what was written.

5. The SD driver is closed and the example is terminated.

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
