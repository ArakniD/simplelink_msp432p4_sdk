### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

---
# clock

---

## Example Summary

This application demonstrates how to use the `ti.sysbios.knl.Clock` module to
construct one-shot and periodic Clock instances. Clock instances are
essentially functions that run after a certain number of Clock ticks.

## Example Usage

* Run the application, the two Clock instances will each print messages to the
console indicating tick counts when the functions are run.

## Application Design Details

* The application is designed to run two Clock instances, the periodic
instance runs twice while the one-shot is only triggered once. The
functions that are passed into the Clock instances, `clk0Fxn` and `clk1Fxn`
respectively are run when the instance reaches its prescribed timeout.

> To configure the Clock module, such as to set the system's `tickPeriod` in
microseconds, please see the application's corresponding configuration file
*clock.cfg*.

> Note that the timeout value configured for each Clock instance is scaled
by the Clock 'tickPeriod' value, so the test should run for the same duration
across different processor and timer types.  Consequently, the displayed tick
counts will vary based on the specified Clock 'tickPeriod' and the rate and
accuracy of the underlying timer.

For example:

For CC13xx/CC26xx devices with a system `tickPeriod` of 10 microseconds,
the Clock instances will trigger after approximately 500, 1000 and
1100 ticks.

For an MSP432 running with a system `tickPeriod` of 1000 microseconds, the
Clock instances will trigger after approximately 5, 10 and 11 ticks.

## References
* For GNU and IAR users, please read the following website for details about
enabling [semi-hosting](http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting)
in order to view console output.

* For more help, search the SYS/BIOS User Guide.
