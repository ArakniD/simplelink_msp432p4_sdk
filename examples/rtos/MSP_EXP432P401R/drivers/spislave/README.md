### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---

# spislave

---

## Example Summary

Demonstrates how to use SPI driver in slave mode to communicate with another
SimpleLink device.  To run this example successfully, another SimpleLink
device running the `spimaster` example is required.

## Peripherals Exercised

* `Board_SPI_SLAVE` - SPI peripheral assigned as a slave
* `Board_MASTER_READY` - GPIO managed by master to notify the slave
Board_SPI_MASTER has been opened
* `Board_SLAVE_READY` - GPIO to notify the master the slave is ready for a
transfer
* `Board_GPIO_LED0` - Indicator LED
* `Board_GPIO_LED1` - Indicator LED

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

Before running the example the following pins must be connected between master
& slave devices.

  |Master Pins              |Slave Pins              |
  |-------------------------|------------------------|
  |`Board_SPI_MASTER` `CLK` |`Board_SPI_SLAVE` `CLK` |
  |`Board_SPI_MASTER` `MOSI`|`Board_SPI_SLAVE` `MOSI`|
  |`Board_SPI_MASTER` `MISO`|`Board_SPI_SLAVE` `MISO`|
  |`Board_SPI_MASTER` `CS`  |`Board_SPI_SLAVE` `CS`  |
  |`Board_SPI_MASTER_READY` |`Board_SPI_MASTER_READY`|
  |`Board_SPI_SLAVE_READY`  |`Board_SPI_SLAVE_READY` |

> The SPI can be used in 4-pin or 3-pin modes (chip select is optional).  When
running this example SPI peripherals on both SimpleLink devices must align on
chip select use.  Users must verify before running the example.

> Although not in the table above, a ground jumper must also be connected
between both boards.

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

* Run the example. `Board_GPIO_LED0` turns ON to show the example is running.

* Once the master is running, master & slave devices will exchange messages in
a loop.  While the SPI transaction is taking place, `Board_GPIO_LED1` will
toggle on/off indicating transfers are occuring.  After a transfer is complete,
the messages are printed via UART. The loop is repeated `MAX_LOOP` times.

Messages should appear as follows:
```
    Starting the SPI slave example
    This example requires external wires to be connected to the header pins. Please see the Board.html for details.

    Slave SPI initialized

    Slave received: Hello from master, msg#: 0
    Slave received: Hello from master, msg#: 1
    Slave received: Hello from master, msg#: 2
    Slave received: Hello from master, msg#: 3
    Slave received: Hello from master, msg#: 4
    Slave received: Hello from master, msg#: 5
    Slave received: Hello from master, msg#: 6
    Slave received: Hello from master, msg#: 7
    Slave received: Hello from master, msg#: 8
    Slave received: Hello from master, msg#: 9

    Done
```

> Note: The SPI peripheral on CC13x0, CC26x0, and CC26x0R2 devices have a bug
which may result in TX data being lost when operating in SPI slave mode
(SSI Advisory 04 & 05 in device errata). This bug causes the first SPI frame
sent from the slave to be 0 instead of the actual data.  When the master prints
the received message to the display, the 0 will be interpreted as a NULL
termination character & the message will not be printed.  When this occurs, halt
the master SPI device & check the contents of the receive buffer; the slave
message should have been received with the first frame as 0.

## Application Design Details

This application uses a single thread:

`slaveThread` - waits for master to open `Board_SPI_MASTER`, opens
`Board_SPI_SLAVE`, creates the slave message & prepares a transfer.  The
`Board_SPI_SLAVE_READY` pin is pulled low to notify the master it is ready.
Once the transfer complete, slave pulls `Board_SPI_SLAVE_READY` high, prepares
the next transaction & pulls `Board_SPI_SLAVE_READY` low again.  A total of
`MAX_LOOP` SPI transactions are performed in this manner.

The `slaveThread` performs the following actions:

1.  Before performing transfers, we must make sure both, `spimaster` &
`spislave` applications are synchronized with each other.  The slave will set
`Board_SPI_SLAVE_READY` to 1 to notify the master it is ready to synchronize.
The slave will then wait for the master to pull `Board_SPI_MASTER_READY` high in
acknowledgment.

2.  Waits until master has opened `Board_SPI_MASTER`.  Master will pull
`Board_SPI_MASTER_READY` low once it has opened its SPI peripheral.

3.  Opens `Board_SPI_SLAVE` in callback mode.

4. Creates a SPI transaction structure and sets txBuffer to `Hello from
slave, msg# n`, where n is the iteration number.

5. Toggles `Board_GPIO_LED1`.

6.  Performs the SPI transfer in callback mode.  If the transfer is successful,
`Board_SPI_SLAVE_READY` is pulled low to notify the master the slave is ready
for a transfer & the slave waits until the transfer has completed.  Otherwise, an
error message is printed to the UART.

7.  Once the transfer is completed, `Board_SPI_SLAVE_READY` is pulled high to
notify master slave is not ready for another transfer.

8. Repeats from step 4 for `MAX_LOOP` iterations.

9. Closes the SPI driver object and terminates execution.

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
