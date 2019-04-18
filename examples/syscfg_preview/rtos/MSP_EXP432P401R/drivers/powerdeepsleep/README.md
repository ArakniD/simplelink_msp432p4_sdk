---
# powerdeepsleep

---

## Example Summary

This example demonstrates usage of a Power policy to put the device into a deep
sleep state.  The application initializes and then waits for a button press.
When the button is pressed there will be some preparation of I/O pins, and
then the deep sleep Power policy will be selected.  When the button ISR finishes
and the CPU idle loop runs again, the Power policy puts the device into deep
sleep.

## Peripherals Exercised

* `Board_GPIO_LED1` - Indicates that the board was initialized within
`mainThread()`. It is turned OFF when the button is pressed.
* `Board_GPIO_BUTTON0` - Enables the deep sleep power policy

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.


## Example Usage

* Run the example. `Board_GPIO_LED1` turns ON to indicate driver
initialization is complete.

* Press `Board_GPIO_BUTTON0`.  `Board_GPIO_LED1` will be turned OFF, and the
device will enter deep sleep.


## Application Design Details

* When the application starts the LED is turned ON, and a button callback
function is registered with the GPIO driver.

* When the button is pressed, the LED will be turned OFF, I/O pins will be
prepared for deep sleep, and the PSS high-side supervisor is disabled to
eliminate its consumption during deep sleep.

* Next the Power driver function `Power_setPolicy()` is used to explicitly
select `PowerMSP432_deepSleepPolicy()` to be called when the CPU becomes idle.

* When it is invoked, `PowerMSP432_deepSleepPolicy()` calls to the Power driver
to transition the device to deep sleep.

* Note that by default, `.enablePolicy = true` in set in the
`PowerMSP432_config` structure in the board file.  This enables the configured
Power policy to be invoked when the CPU becomes idle.  If this configuration
parameter is set to false instead, then the policy needs to be enabled
explicitly at runtime.  The application can do this by calling
`Power_enablePolicy();`.

* Note that also by default, the lighter-weight sleep policy
`PowerMSP432_sleepPolicy()` is selected in the `PowerMSP432_config` structure
in the board file.  This enables the CPU to enter sleep after initializing,
while waiting for the button to be pressed.

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
