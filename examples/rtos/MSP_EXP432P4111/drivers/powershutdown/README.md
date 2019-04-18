---
# powershutdown

---

## Example Summary

Application that demonstrates power shutdown following a GPIO pin interrupt.

## Peripherals Exercised

* `Board_GPIO_LED0` - Indicates that the board was initialized within
`mainThread()`, also turned off by `Board_GPIO_BUTTON0`
* `Board_GPIO_BUTTON0` - Triggers shutdown

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

## Example Usage

* Build the program and download it to the device.

* Terminate the debug session.

* Disconnect and reconnect the board power, to start the program running.

* `Board_GPIO_LED0` turns ON to indicate driver initialization is complete.

* Pressing `Board_GPIO_BUTTON0` will turn off `Board_GPIO_LED0` and the device
will shutdown.

* Pressing `Board_GPIO_BUTTON0` again will wake the device from shutdown and
`Board_GPIO_LED0` will turn ON as the program restarts.


## Application Design Details

* On button press of Board_GPIO_BUTTON0 the application requests shutdown by
the Power driver. As the application enters shutdown, a registered notification
function, `notifyFxn`, is invoked that puts all GPIOs (except for
 Board_GPIO_BUTTON0) in their lowest power configuration.

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
