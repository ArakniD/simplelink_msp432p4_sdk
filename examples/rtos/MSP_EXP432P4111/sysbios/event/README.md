## Example Summary

The event example shows how to use the `ti.sysbios.knl.Event module` which
addresses the problem of a task needing to Wait on Multiple events prior to
continuing its' execution.

## Example Usage

* Run the application, two tasks the, `writertask` and `readertask`, will print
messages being passed between them to the console.

## Application Design Details

* A reader task, `readertask`, waits on several events. It needs to wake up
when events with Ids `Event_Id_0` and `Event_Id_1` or if`Event_Id_2` occurs.
The `ti.sysbios.knl.Event` module provides an `Event_pend()` API that allows a
task to specify an `AND` mask and an `OR`mask. These masks determine which
event(s) must occur before returning from `Event_pend()`.

* The event related to `Event_Id_0` is posted by a Clock Instance using
a call to `Event_post()`. This is an example of the *Explicit*
posting of events.

* `Event_Id_1` is posted from within a Semaphore Instance. This is an example
of the *Implicit* posting of events. This is useful when a task needs to wait on
a semaphore and an event which is explicitly posted. `Event_id_2` is posted from
within a Mailbox Instance which occurs wihtin the context of the `writertask`.
This is also an example of *Implicit* posting of events.

> The Semaphore Module does not support events by default, as such, the
`Semaphore.supportsEvents` flag is set to `true` within the applications
configuration file *event.cfg*.

## References
* For GNU and IAR users, please read the following website for details about
enabling [semi-hosting](http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting)
in order to view console output.

* For more help, search the SYS/BIOS User Guide.