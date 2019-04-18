### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---
# boostxl\_capkeypad\_msp432p401r

---

## BoosterPack Requirements

* This demo requires the [MSP430 CapTIvate™ Touch Keypad BoosterPack](http://www.ti.com/tool/boostxl-capkeypad) paired with the MSP-EXP432P401R LaunchPad
* Program the [MSP430 CapTIvate™ Touch Keypad BoosterPack](http://www.ti.com/tool/boostxl-capkeypad) with the npi\_BOOSTXL-CAPKEYPAD-Demo.txt firmware example. This binary can be found in &lt;SDK\_INSTALL\_DIR&gt;/examples/demos/&lt;BOARD&gt;/boostxl\_capkeypad\_msp432p401r/boostxl_capkeypad_source
* The binary source code (npi\_BOOSTXL-CAPKEYPAD-Demo.txt) is part of the [CapTIvate Design Center](http://www.ti.com/tool/mspcaptdsnctr) (npi\_BOOSTXL-CAPKEYPAD project)
* The I2C slave interface that is being in this example is IRQB. The default is IRQA, please make sure to move jumper J9 to IRQB

## Example Summary

This example demonstrates how to mate the BOOSTXL-CAPKEYPAD to an MSP-EXP432P401R LaunchPad through a communication interface heavily based on [TI’s Unified Network Processor Interface (UNPI/NPI)](http://processors.wiki.ti.com/index.php/Unified_Network_Processor_Interface). Because of the resemblance to UNPI, many of the software files have been named similarly.

### Message Definition
#### TypeID
Only one type value is used for the CapTIvate device (0xCA). Any transaction between the host and CapTIvate device must use this value. 
Another user application can choose to use different or more values if needed. This can be done by adding values into the `npi_type_t` enumeration in `npi_message.h` for both host and slave. However, proper checking of the type value must be performed to validate the authenticity of the message

#### CommandID

They are defined as INFO\_CMD (0x01) (not used by this example) and CAPT\_MSG (0xAA). The values for these definitions were chosen randomly and can be modified by the user in the `npi_cmd_t` enumeration in `npi_message.h` for both host and slave. 

When an event has occurred on the CapTIvate device, a CapTIvate message type will be sent from the slave with CAPT\_MSG as the command field. The payload has been formatted like the following:

<table>
<thead>
<tr>
  <th>Index</th>
  <th>0</th>
  <th>1</th>
  <th>2</th>
  <th>3</th>
  <th>4</th>
  <th>5</th>
  <th>6</th>
  <th>7</th>
</tr>
</thead>
<tbody>
<tr>
  <td>Description</td>
  <td>Keypad sensor activity flag</td>
  <td>Key number</td>
  <td>Key pressed flag</td>
  <td>Proximity sensor activity flag</td>
  <td>Proximity touch flag</td>
  <td>Proximity sensor flag</td>
  <td>Wheel button activityflag</td>
  <td>Wheel button touch flag</td>
</tr>
</tbody>
</table>

At the same time this example demonstrates the low power consumption on both devices.

## Peripherals Exercised

* `Board_GPIO_LED1` - Indicates that the board was initialized within
`SLEEP_taskFxn()`
* `Board_GPIO_BUTTON0` - Switches from sleep to deep sleep
* `Board_UART0` - Log the keystrokes from the the BOOSTXL-CAPKEYPAD
* `Board_I2C_CAPTIVATE` - I2CB1 interface with the BOOSTXL-CAPKEYPAD
* `Board_CAPTIVATE_IRQ` - IRQ from the BOOSTXL-CAPKEYPAD

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.


## Example Usage

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/) "PuTTY's
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

* Connect the MSP-EXP432P401R LaunchPad to the BOOSTXL-CAPKEYPAD to create a 2-board stack

* Run the example. `Board_GPIO_LED1` turns ON to indicate driver
initialization is complete.

*  Then, touch any of the buttons on the BOOSTXL-CAPKEYPAD to see the keystrokes logged onto the serial interface. Touching the enter key displays the number `10` and the cancel key displays the number `11`. If the keypad is not touched for a period of time, it will shut down into is wake-on-proximity mode.

* Pressing `Board_GPIO_BUTTON0` will turn off `Board_GPIO_LED1` and the MSP432 device
will enter deep sleep. 

## Application Design Details
There are 3 threads on the example.

* `SLEEP_taskFxn` - Configures the sleep policy and `Board_GPIO_BUTTON0` in order to switch to deep sleep
* `NPI_taskFxn` - Handles the communication (process the message) between the Host and the Slave 
* `CAPTIVATE_taskFxn` - Displays the keystrokes received from the `NPI_taskFxn` to the serial interface

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
