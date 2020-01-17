## Example Summary

This demo initializes BQ27441 Fuel Gauge configurations suitable for the included battery and periodically prints information to serial backchannel uart.

## BoosterPack Requirements

This demo requires the [Fuel Tank MKII Battery BoosterPack Plug-In Module (BOOSTXL-BATPAKMKII)](http://www.ti.com/tool/BOOSTXL-BATPAKMKII) paired with the MSP-EXP432P401R LaunchPad

## Example Usage

After loading the demo program onto the Launchpad, follow the following steps to run the demo:

1. Attach the BOOSTXL-BATPAKMKII Battery BoosterPack plug-in module to the LaunchPad development kit.
2. Flip the switch to the "ON" position on the side of the BOOSTXL-BATPAKMKII Battery BoosterPack plug-in module.
3. Connect the MSP-EXP432 P401R LaunchPad development kit to a computer via micro-USB cable.
4. Launch a serial terminal application and connect to the COM port for "XDS110 Class Application/User UART" at 115200 baud rate.
5. Press the reset button on the MSP-EXP432P401R LaunchPad development kit.
6. Observe serial data displaying Fuel Gauge configuration and Battery Information.

## Application Design Details

* No-RTOS
  * The demo program begins by initializing a number of configuration parameters in the bq27441-G1 to
match the target battery.
  * Four important parameters are Design Capacity, Design Energy, Terminate Voltage, and Taper Rate. Values are determined based on the target battery properties and bq27441-G1
documentations.
  * Next, the host MSP432P401R MCU clears the BIE (Batter Insert Enable) bit in the fuel gauge operation
configuration register.
  * When BIE is cleared, the battery detection will rely on the host to issue a BAT_INSERT subcommand to indicate battery presence, bypassing the J6 BIN jumper on the BOOSTXLBATPAKMKII BoosterPack plug-in module that the fuel gauge relies on for battery detection by default when BIE is set (J6 shorted = Battery Inserted; J6 open = Battery Removed). This is done to ensure the demo application works regardless if J6 is connected or not.
  *In end user applications, a switch or the host MCU would more likely be used control the BIN state of the
fuel gauge depending on battery connection. However, this is not implemented on the BoosterPack plug-in
module and a jumper is used to manually toggle between battery insertion and removal.
Once the bq27441-G1 has been configured properly, the main loop repeatedly reads back
DESIGN_CAPACITY, REMAINING_CAPACITY, STATE_OF_CHARGE, TEMPERATURE, VOLTAGE,
and AVERAGE_CURRENT from the fuel gauge. Results are formatted and transmitted through
Application/User UART.
