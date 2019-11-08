## Example Summary

All examples are now supported by SysConfig, a tool that will help you
graphically configure your software components.  Although we strongly
encourage users to develop applications with SysConfig, this legacy example
is provide for those that do not want to upgrade to SysConfig.  Legacy Board
files are provided with this example, that can be edited to configure TI
drivers.

## Peripherals Exercised

* `Board_GPIO_LED0`  - LED0

## BoosterPacks, Board Resources & Jumper Settings

For board specific jumper settings, resources and BoosterPack modifications,
refer to the __Board_legacy.html__ file.

> If you're using an IDE such as Code Composer Studio (CCS) or IAR, please
refer to Board_legacy.html in your project directory for resources used and
board-specific jumper settings.

The Board_legacy.html can also be found in your SDK installation:

        <SDK_INSTALL_DIR>/source/ti/boards/<BOARD>

## Example Usage

* The example lights `Board_GPIO_LED0` as part of the initialization in the
`mainThread()`. This thread then toggles the LED at a 1 second rate.

## Application Design Details

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
