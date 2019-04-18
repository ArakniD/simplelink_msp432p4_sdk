### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---
# adcbuftemperature

---

## Example Summary

Example that uses the ADCBuf driver to take one hundred temperature samples when the user presses button 0
and echo them via UART. This example shows how to initialize the
ADCBuf driver in one-shot mode using the temperature module.

## Peripherals Exercised

* `Board_ADCBUF0` - Buffer instance used to manage samples from ADCBuf channel.
* `Board_ADCBUF0CHANNEL2` - ADCBuf channel 2.
* `Board_UART0` - Used to echo samples to host serial session.
* `Board_GPIO_BUTTON0` - Triggers ADCBuf to collect data

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

## Example Usage

* Connect the ADC channel to a sampling source.
    * For quick testing, connect `Board_ADCBUF0CHANNEL2` to `GND` or `3V3`.

>__Important:__ Caution should be used when connecting the pins to analog inputs greater than 3VDC.

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

* The target will send packages of 20 samples and a header to the serial
session twice per second. The values displayed on the console are an average of the samples and convert to degrees Celsius.

## Application Design Details

* A single task sets up the UART connection to the serial console and initializes
the ADCBuf driver. Pressing button 0 creates a task that will start ADCBuf conversion. After this is completed, the
task sleeps forever.

* The ADCBuf driver performs 20 samples at 200 Hz each.
    * The driver supports higher sampling rates; however, 200 Hz was chosen for
easily interpretable output. The maximum sampling rates are device specific.

* The callback function `adcBufCallback` is called whenever an ADC buffer is
full. The contents of the buffer are converted into human-readable temperature format (degrees Celsius) and
sent via UART.

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
