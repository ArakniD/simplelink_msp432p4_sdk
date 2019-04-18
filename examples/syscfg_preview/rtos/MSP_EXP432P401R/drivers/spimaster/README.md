### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---

# spimaster

---

## Example Summary

Demonstrates how to use SPI driver in master mode to communicate with another
SimpleLink device.  To run this example successfully, another SimpleLink
device running the `spislave` example is required.

## Peripherals Exercised

* `Board_SPI_MASTER` - SPI peripheral assigned as a master
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

* Once the slave is running, master & slave devices will exchange messages in
a loop.  While the SPI transactions are taking place, `Board_GPIO_LED1` will
toggle on/off indicating transfers are occurring. After a transfer is complete,
the messages are printed via UART. The loop is repeated `MAX_LOOP` times.

Messages should appear as follows:
```
    Starting the SPI master example
    This example requires external wires to be connected to the header pins. Please see the Board.html for details.

    Master SPI initialized

    Master received: Hello from slave, msg#: 0
    Master received: Hello from slave, msg#: 1
    Master received: Hello from slave, msg#: 2
    Master received: Hello from slave, msg#: 3
    Master received: Hello from slave, msg#: 4
    Master received: Hello from slave, msg#: 5
    Master received: Hello from slave, msg#: 6
    Master received: Hello from slave, msg#: 7
    Master received: Hello from slave, msg#: 8
    Master received: Hello from slave, msg#: 9

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

`masterThread` - creates the master SPI message, opens `Board_SPI_MASTER` and
waits for the slave to be ready for a SPI transfer.  When the slave is ready it
starts the transfer.  Once complete, the master prepares the next transaction
& waits for the slave once more.  A total of `MAX_LOOP` SPI transactions are
performed in this manner.

The `masterThread` performs the following actions:

1.  Before performing transfers, we must make sure both, `spimaster` &
`spislave` applications are synchronized with each other.  The master will set
`Board_SPI_MASTER_READY` to 1 to notify the slave it is ready to synchronize.
The master will then wait for the slave to pull `Board_SPI_SLAVE_READY` high in
acknowledgment.

2.  Opens `Board_SPI_MASTER` & sets `Board_SPI_MASTER_READY` to 0.  Setting
`Board_SPI_MASTER_READY` will notify the slave `Board_SPI_MASTER` has been
opened.

3.  Waits for the slave to be ready for the SPI transfer.  The slave will pull
`Board_SPI_SLAVE_READY` low when ready for a transfer.

4. Creates a SPI transaction structure and sets txBuffer to `Hello from
master, msg# n`, where n is the iteration number.

5. Toggles `Board_GPIO_LED1`.

6. Transfers the message. If the transfer is successful, the message
received from the slave SPI is printed. Otherwise, an error message
is printed.

7. Sleeps for 3 seconds.

8. Repeats from step 3 for `MAX_LOOP` iterations.

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
