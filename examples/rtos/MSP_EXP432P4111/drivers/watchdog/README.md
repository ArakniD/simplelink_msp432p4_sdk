### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---
# watchdog

---

## Example Summary

This application demonstrates how to use the Watchdog driver.

## Peripherals Exercised

* `Board_GPIO_LED0`      - Indicator LED
* `Board_GPIO_BUTTON0`   - Used to control the application
* `Board_WATCHDOG0`      - Watchdog driver instance.

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.


## Example Usage

1. After loading the example application to the board, disconnect the device
from the debug session. Then physically disconnect the device from power for 5
seconds. Reconnect the device and the application should run automatically.

2. The application turns on `Board_GPIO_LED0` to indicate watchdog driver
initialization is complete.

3. `Board_GPIO_LED0` is toggled periodically. With each toggle, the watchdog
timer is cleared.

4. Pressing `Board_GPIO_BUTTON0` will enter an interrupt service routine which
turns `Board_GPIO_LED0` on and enters an infinite loop.

5. The watchdog expires and the device is reset. For some devices,
the application will restart.

## Application Design Details

* The application opens a Watchdog driver instance configured to reset
(`Watchdog_RESET_ON`) upon the watchdog timer expiring. After
`Watchdog_open()`, the watchdog timer is running with the reload value
specified in the board file.

* The application demonstrates how to configure the reload value at runtime.
Not all devices support `Watchdog_convertMsToTicks()` and
`Watchdog_setReload()`. `Watchdog_convertMsToTicks()` will return `0` if the
APIs are not support or if the specified reloadValue is not valid. This example
configures the reload value to be the same value used in the board file.

* The application's `mainThread` enters an infinite loop which periodically
clears the watchdog timer using `Watchdog_clear()`. Each time the watchdog is
cleared, `Board_GPIO_LED0` is toggled.

* `Power_disablePolicy()` and `Power_enablePolicy()` are used to disable and
enable the power driver respectively. Doing so affects how the device behaves
when idle. If the power policy is enabled, the device is allowed to enter a
low power state when the CPU is idle. Otherwise, the device will stay awake
when the CPU is idle. This demonstrates the watchdog driver's flexibility in an
application regardless of power driver usage.

* Pressing `Board_GPIO_BUTTON0` will cause the application to get stuck in the
GPIO callback interrupt service routine (ISR), `gpioButtonIsr()`. This is
intended to simulate the application getting stuck unintentionally.

* The `watchdogCallback()` is called by some watchdog drivers upon a watchdog
timeout event. User's should refer to the device specific watchdog driver
documentation for their device's behavior. The watchdog interrupt is a
non-maskable interrupt (NMI) and therefore will preempt the `gpioButtonIsr()`.
This application's `watchdogCallback()` will loop infinitely until the watchdog
peripheral issues a device reset.

* When the watchdog peripheral issues a device reset, the behavior is
equivalent to the user manually pressing the LaunchPad reset button. Therefore,
behavior after a reset is device specific. In most instances, the example
application will restart.

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
