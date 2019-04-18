### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---
# adcsinglechannel

---

## Example Summary

Example that uses the ADC driver to make a number of samples
and print them via UART.

## Peripherals Exercised

* `Board_ADC0`
* `Board_ADC1`

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.


## Example Usage

* Example output is generated through use of Display driver APIs. Refer to the
Display driver documentation found in the SimpleLink MCU SDK User's Guide.

* Connect the ADC channels to the sampling sources.
    * For quick testing, connect `Board_ADC0` to `GND` and `Board_ADC1` to `3V3`.

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

Example snippet of outputs:
```
        ADC1 raw result (4): 230

        ADC1 convert result (4): 2500000 uV
                    .                 .     .
                    .                 .     .
                    .                 .     .
        ADC1 raw result (9): 280

        ADC1 convert result (9): 3000000 uV
```

* The example outputs one sample from `Board_ADC0` and ten samples from
`Board_ADC1`.

* The actual conversion result values may vary depending on the reference
voltage settings. Please refer to the board specific datasheet for more details.

## Application Design Details

This application uses two threads:

`threadFxn0` - performs the following actions:

1. Opens an ADC driver object.

2. Uses the ADC driver object to perform a single sample and outputs the result.

3. Closes the ADC driver object.

`threadFxn1` - performs the following actions:

1. Opens an ADC driver object.

2. Uses the ADC driver object to perform 10 samples and output the results.

3. Closes the ADC driver object.

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
