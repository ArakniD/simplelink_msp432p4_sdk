---
# stairstep

---

## Example Summary

This application exercises the System Analyzer's CPU Load, Task Load
Execution Graph Analyis Features.

## Example Usage

* Run and pause the application. To view the load data that was logged, open the
RTOS Analyzer through `Tools->RTOS Analyzer->Printf and Error Logs` and switch
to the `Live Session` tab. In addition to the logged load data, you can view
the Execution Graph through `Tools->RTOS Analyzer->Execution Analysis`, which
depicts all the threads and transition events.

## Application Design Details

* Following SYS/BIOS object construction within main,
`cpuLoadInit()` gets the CPU frequency using an API and
fills the `loadValue` arrays with load values corresponding to
0, 25, 50, 75 and 95 percent of the CPU load.

* `timerFunc()`, a Hwi thread, runs every 100ms and launches
3 types of threads Hwi, Swi, and Task. Each respective thread
then performs a corresponding load function before relinquishing the CPU.

* After dwelling for 5 seconds at each load setting, `timerFunc()`
calls `step()` which advances to the next set of Hwi, Swi, and
Task load values. The cycle repeats forever.

> The loads of the 3 thread types can be seen separately using
> Task Load view, and cumulatively with the CPU Load view

## References

* For GNU and IAR users, please read the following website for details about
enabling [semi-hosting](http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting)
in order to view console output.

* For more help, search the SYS/BIOS User Guide.
