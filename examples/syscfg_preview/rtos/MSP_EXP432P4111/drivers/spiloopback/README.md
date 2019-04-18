---
# spiloopback

---

## Example Summary

Application to demonstrate use of the SPI driver to create a simple external
loop-back.

## Peripherals Exercised

* `Board_GPIO_LED0` - Indicator LED
* `Board_GPIO_LED1` - Indicator LED
* `Board_SPI0` - SPI peripheral assigned as a Master
* `Board_SPI1` - SPI peripheral assigned as a Slave

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

  |Master Pins|Slave Pins|
  |---|---|
  |`Board_SPI0` `CLK`|`Board_SPI1` `CLK`|
  |`Board_SPI0` `MOSI`|`Board_SPI1` `MOSI`|
  |`Board_SPI0` `MISO`|`Board_SPI1` `MISO`|
  |`Board_SPI0` `CS`|`Board_SPI1` `CS`|

* Some devices support SPI in 3 pin mode. These devices may not require a `CS`
pin.

## Example Usage

* Example output is generated through use of Display driver APIs. Refer to the
Display driver documentation found in the SimpleLink MCU SDK User's Guide.

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

* Run the example. `Board_GPIO_LED0` turns ON to indicate driver
initialization is complete.

* Once initialized, the Slave SPI will send a message to the Master SPI
in a loop. The Master SPI will also send a message to the Slave SPI in a loop.
After the transfer is complete, the messages are printed via UART.  While the
SPI transaction is taking place,`Board_GPIO_LED1` will be turned on, and will
remain on for half a second (so that it appears as a flash). After each SPI
transaction, the Master will sleep for a few seconds.  The loop is repeated
MAX_LOOP times.

Messages should appear as follows:
```
        Starting the SPI loop-back example
        This example requires external wires to be connected to the header pins. Please see the Board.html for details.
        Slave SPI initialized
        Master SPI initialized
        Slave: Hello from master, msg#: 0
        Master: Hello from slave, msg#: 0
        Slave: Hello from master, msg#: 1
        Master: Hello from slave, msg#: 1
        Slave: Hello from master, msg#: 2
        Master: Hello from slave, msg#: 2
        Slave: Hello from master, msg#: 3
        Master: Hello from slave, msg#: 3
        Slave: Hello from master, msg#: 4
        Master: Hello from slave, msg#: 4
        Done
```

## Application Design Details

This application uses two threads:

`masterThread` - creates the Master SPI message and initiates the first SPI
transfer.  When the transaction completes, the Master sleeps for 3 seconds,
before sending the next transaction.  The Master initiates a total of MAX_LOOP
SPI transactions.
`slaveThread` - creates the Slave SPI message and waits for the Master to
start the SPI transaction. This thread runs at a higher priority
than the masterThread, since the Slave SPI must be ready before the
Master SPI starts the first transaction.  The Slave also initiates its
SPI transactions in a loop of MAX_LOOP iterations.

The `masterThread` performs the following actions:

1. Opens and initializes an SPI driver object.

2. Creates a SPI transaction structure and sets txBuffer to `Hello from
master, msg# n`, where n is the iteration number.

3. Turns on Board_GPIO_LED1.

4. Transfers the message. If the transfer is successful, the message
received from the Slave SPI is printed. Otherwise, an error message
is printed.

5. Sleeps for 500 milliseconds before turning off Board_GPIO_LED1.

6. Sleeps for 3 seconds, then repeats from step 2 for MAX_LOOP iterations.

7. Closes the SPI driver object and terminates execution.

The `slaveThread` performs the following actions:

1. Opens and initializes an SPI driver object.

2. Creates a SPI transaction structure and sets txBuffer to `Hello from
slave, msg# n`, where n is the iteration number.

3. Waits for the Master SPI before sending the message. If the transfer is
successful, the message received from the Master SPI is printed.
Otherwise, an error message is printed.

4. Repeats steps 2-3 for MAX_LOOP iterations.

5. Closes the SPI driver object.

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
