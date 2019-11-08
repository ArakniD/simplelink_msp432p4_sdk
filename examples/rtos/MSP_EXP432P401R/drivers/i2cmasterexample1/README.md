## Example Summary

Sample application to demonstrate I2C Master and I2C Slave communication.

## Peripherals & Pin Assignments

SysConfig generates the driver configurations into the __ti_drivers_config.c__
and __ti_drivers_config.h__ files. Information on pins and resources used
is present in both generated files. The SysConfig user interface can also be
utilized to determine pins and resources used.

* This example requires two LaunchPads.

* The first LaunchPad will run the __i2cmasterexample1__ example and act as the
I2C master.

* The second LaunchPad will run the __i2cslaveexample1__ example and act as the
I2C slave.

* `CONFIG_I2C_0` - I2C bus

## BoosterPacks, Board Resources & Jumper Settings

For board specific jumper settings, resources and BoosterPack modifications,
refer to the __Board.html__ file.

> If you're using an IDE such as Code Composer Studio (CCS) or IAR, please
refer to Board.html in your project directory for resources used and
board-specific jumper settings.

The Board.html can also be found in your SDK installation:

        <SDK_INSTALL_DIR>/source/ti/boards/<BOARD>

  | Master LaunchPad Pins | Pull-Up Resistor | Slave LaunchPad Pins |
  |-----------------------|:----------------:|----------------------|
  |       __`SDA`__       |     __10K__      |      __`SDA`__       |
  |       __`SCL`__       |     __10K__      |      __`SCL`__       |
  |       __`GND`__       |     __None__     |      __`GND`__       |

Note that two 10K pull-up resistors are needed, one for SDA, and one for SCL.

## Example Usage

* Example output is generated through use of Display driver APIs. Refer to the
Display driver documentation found in the SimpleLink MCU SDK User's Guide.

1. Download the __i2cslaveexample1__ example onto the first LaunchPad. This
LaunchPad will be the I2C slave device. Unplug the device.

2. Download the __i2cmasterexample1__ example onto the second LaunchPad. This
will be the I2C master device. Pause the code at `main()`.

3. Ensure pin connections between the two LaunchPads are correct. Refer to the
Master and Slave Pin connections in the table above.

4. Plug-in the I2C slave device into another USB port so that the
__i2cslaveexample1__ example is now running. Since the example was flashed onto
this device in step one, simply powering on the device will cause the
application to run.

5.  Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's
Homepage"), etc.) to the appropriate COM port.
    The COM port can be determined via Device Manager in Windows or via
`ls /dev/tty*` in Linux.
The connection will have the following settings:
```
    Baud-rate:     115200
    Data bits:          8
    Stop bits:          1
    Parity:          None
    Flow Control:    None
```

6.  Run the __i2cmasterexample1__ example from step two.

    Example of expected output:
```
    Master: x Hello this is master

    I2C master/slave transfer complete

    Slave: o Hello this is slave

    I2C closed!
```

## Application Design Details

* This application uses one task, 'mainThread', which performs the following
actions:
    * Opens and initializes I2C object.
    * Uses the I2C driver to communicate with the I2CSlave.

* The following commands are defined by the application code on top of I2C
operations:
    * GETSTATUS Cmd from master - command[0] = 0x01
    * SETSTATUS Cmd from master - command[0] = 0x02
    * READBLOCK Cmd from master - command[0] = 0x03
    * WRITEBLOCK Cmd from master - command[0] = 0x04

mirrorRegister is the memory block that is used to store sent/received data
mirrorRegister[0] holds the status byte that GETSTATUS and SETSTATUS point to.
* Master mirrorRegister begins with "x Hello this is master".
* Slave mirrorRegister begins with "x ".

1. The master will initially display the contents of its mirrorRegister.
2. The master will use the SETSTATUS command on the slave to 'x'.
3. The master will then use the GETSTATUS command, expecting "x" status byte from the slave.
An error message is displayed otherwise.
4. The master again uses SETSTATUS, this time overwriting the slave status byte from "x" to "o".
5. The WRITEBLOCK command is used to fill in the rest of the slave mirrorRegister with "Hello
this is slave".
6. The READBLOCK command is used to read the entire slave mirrorRegister and stored in the master's
mirrorRegister
7. The master's mirrorRegister is displayed which now contains "o Hello this
is slave".

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
