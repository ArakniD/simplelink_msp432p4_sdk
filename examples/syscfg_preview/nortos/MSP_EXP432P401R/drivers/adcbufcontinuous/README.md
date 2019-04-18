### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---
# adcbufcontinuous

---

## Example Summary

Example that uses the ADCBuf driver to make a number of samples
and echo them via UART. This example shows how to initialize the
ADCBuf driver in continuous mode.

## Peripherals Exercised

* `Board_ADCBUF0` - Buffer instance used to manage samples from ADCBuf channel.
* `Board_ADCBUF0CHANNEL0` - ADCBuf channel 0.
* `Board_UART0` - Used to echo samples to host serial session.

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

## Example Usage

* Connect the ADC channel to a sampling source.
    * For quick testing, connect `Board_ADCBUF0CHANNEL0` to `GND` or `3V3`.

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

* The target will send packages of 50 samples and a header to the serial
session. Adjusted raw values and microvolt values are displayed
on respective new lines.

## Application Design Details

* A single task sets up the UART connection to the serial console and starts
a continuous conversion with the ADCBuf driver. After this is completed, the
task sleeps forever.

* The ADCBuf driver performs 50 samples at 200 Hz each.
    * The driver supports higher sampling rates; however, 200 Hz was chosen for
easily interpretable output. The maximum sampling rates are device specific.

* The callback function `adcBufCallback` is called whenever an ADC buffer is
full. The contents of the buffer are first adjusted so that raw results are
comparable between devices of the same make. The adjusted values are then
converted into microvolts to be human-readable. Lastly, both the adjusted and
microvolt values are displayed via UART.

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
