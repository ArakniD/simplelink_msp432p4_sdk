### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---
# nvsinternal

---

## Example Summary

This example shows how to use the Non-Volatile Storage (NVS) driver
to read and write data to on-chip flash memory.

##Peripherals Exercised

* `Board_UART0` - Used for console output.
* `Board_NVSINTERNAL` - Non-volatile storage used by a NVS driver instance.

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
 your project directory for resources used and board-specific jumper settings.
 Otherwise, you can find Board.html in the directory
 &lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

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

* Run the example.

* The example will output the region attributes as defined by `Board_NVSINTERNAL`.

* The example checks if the string, "SimpleLink SDK Non-Volatile Storage
 (NVS) Example" is present in non-volatile storage.
    * If present, the string is displayed to the UART console. The entire flash
    sector is then erased.
    * If not present, the string is written to the non-volatile storage.

 * Disconnect the device from the debug session.

 * When prompted, reset the device. This will cause the application to
 start over.

 * The region base address, sector size and region size will vary depending on
 the device specific definitions in the board file.

The following is example output assuming the string was not present in non-volatile
storage:
```
    Region Base Address: 0x1B000
    Sector Size: 0x1000
    Region Size: 0x4000

    Writing signature to flash...
    Reset the device.
    ==================================================
```
The following is example output assuming the string is present in non-volatile
storage:
```
    Region Base Address: 0x1B000
    Sector Size: 0x1000
    Region Size: 0x4000

    SimpleLink SDK Non-Volatile Storage (NVS) Example.
    Erasing flash sector...
    Reset the device.
    ==================================================
```

## Application Design Details

* The application utilizes a single thread to demonstrate using the
 non-volatile storage region defined by `Board_NVSINTERNAL`.

 * `Board_NVSINTERNAL` defines a non-volatile storage located on the device's on-chip
 internal flash memory. The memory definitions may be found in the board file.
 After `NVS_open()`, the `nvsHandle` is associated with the memory region
 defined by `Board_NVSINTERNAL`.

* A block of memory the size of `signature` bytes is read from flash and
copied into RAM (`buffer`). An offset of 0 is used with the `NVS_read()` call.
The NVS memory location read is therefore equal to the NVS region base address,
`NVS_Attrs.baseAddress`. The contents copied into `buffer` are compared to
`signature`.
  * If equal, the application displays the signature to the UART console. It is
    important to note that the string was copied into RAM (`buffer`)
    during `NVS_read()`. This example chooses to display the string directly
    from the NVS region in flash. After the string is output to the UART
    console, the first sector in the NVS region is erased.
  * If not equal, the application writes the string, `signature` to the
    NVS region. An offset of 0 is specified with the `NVS_write` call. The
    NVS memory location written to is therefore equal to the NVS region base
    address, `NVS_Attrs.baseAddress`.

* At the end of execution, the application prompts the user to reset the
device. Upon a reset, the contents of volatile memory (RAM) are lost. The
example application restarts.

* The `NVS_WRITE_ERASE` flag is used with the `NVS_write()` API to ensure the
flash sector is erased prior to performing a write. This flag will erase
`NVS_Attrs.sectorSize` bytes of memory starting at the offset specified with
`NVS_write()`.

* The `NVS_WRITE_POST_VERIFY` flag is also used with the `NVS_write()` API to
ensure the flash memory was successfully written.

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
