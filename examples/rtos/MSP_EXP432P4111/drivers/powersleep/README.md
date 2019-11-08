## Example Summary

This example demonstrates usage of Power driver constraints, along with a
Power policy, to enable or disable entry into sleep during CPU idle time.
The application sets a constraint so the Power policy does not choose sleep,
and then later releases the constraint to allow the policy to choose to enter
sleep.

## Peripherals & Pin Assignments

SysConfig generates the driver configurations into the __ti_drivers_config.c__
and __ti_drivers_config.h__ files. Information on pins and resources used
is present in both generated files. The SysConfig user interface can also be
utilized to determine pins and resources used.

* `CONFIG_GPIO_LED_0` - Indicates that the board was initialized within
`mainThread()`, and later to indicate the state of the sleep constraint
* `CONFIG_GPIO_BUTTON_0` - Alternately sets/releases the sleep constraint

## BoosterPacks, Board Resources & Jumper Settings

For board specific jumper settings, resources and BoosterPack modifications,
refer to the __Board.html__ file.

* If you're using an IDE such as Code Composer Studio (CCS) or IAR, please
refer to Board.html in your project directory for resources used and
board-specific jumper settings.

* The Board.html can also be found in your SDK installation:

        <SDK_INSTALL_DIR>/source/ti/boards/<BOARD>

## Example Usage

* Run the example. `CONFIG_GPIO_LED_0` turns ON to indicate driver
initialization is complete.

* Press the `CONFIG_GPIO_BUTTON_0` button.  Each button press will alternately set or release the `PowerMSP432_DISALLOW_SLEEP` constraint.

* When the constraint is set `CONFIG_GPIO_LED_0` will be lit and the CPU will remain active.  When the constraint is released the LED will be turned OFF, and the CPU will transition to sleep until the next interrupt occurs.

## Application Design Details

* When the application starts the LED is turned ON.

* A notification function is registered with the Power driver, to be called later by the Power driver, when the CPU is being transitioned to sleep. When this function is called it will simply turn OFF the LED so that power consumption can be measured.

* Next, unused device I/Os are set to low-power (i.e., non-floating) states so that they do not contribute power consumption when the CPU is sleeping.

* The button callback function maintains a 'sleeping' flag, to alternately choose to set or release the `PowerMSP432_DISALLOW_SLEEP` constraint, so the Power policy will alternately keep the CPU active, or put it to sleep during idle time.


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
