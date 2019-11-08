## Example Summary

Application that demonstrates power shutdown following a GPIO pin interrupt.

## Peripherals & Pin Assignments

SysConfig generates the driver configurations into the __ti_drivers_config.c__
and __ti_drivers_config.h__ files. Information on pins and resources used
is present in both generated files. The SysConfig user interface can also be
utilized to determine pins and resources used.


* `CONFIG_GPIO_LED_0` - Indicates that the board was initialized within
`mainThread()`, also turned off by `CONFIG_GPIO_BUTTON_0`
* `CONFIG_GPIO_BUTTON_0` - Triggers shutdown

## BoosterPacks, Board Resources & Jumper Settings

For board specific jumper settings, resources and BoosterPack modifications,
refer to the __Board.html__ file.

> If you're using an IDE such as Code Composer Studio (CCS) or IAR, please
refer to Board.html in your project directory for resources used and
board-specific jumper settings.

The Board.html can also be found in your SDK installation:

        <SDK_INSTALL_DIR>/source/ti/boards/<BOARD>

## Example Usage

* Build the program and download it to the device.

* Terminate the debug session.

* Disconnect and reconnect the board power, to start the program running.

* `CONFIG_GPIO_LED_0` turns ON to indicate driver initialization is complete.

* Pressing `CONFIG_GPIO_BUTTON_0` will turn off `CONFIG_GPIO_LED_0` and the device
will shutdown.

* Pressing `CONFIG_GPIO_BUTTON_0` again will wake the device from shutdown and
`CONFIG_GPIO_LED_0` will turn ON as the program restarts.


## Application Design Details

* On button press of `CONFIG_GPIO_BUTTON_0` the application requests shutdown by
the Power driver. As the application enters shutdown, a registered notification
function, `notifyFxn`, is invoked that puts all GPIOs (except for
 `CONFIG_GPIO_BUTTON_0`) in their lowest power configuration.

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
