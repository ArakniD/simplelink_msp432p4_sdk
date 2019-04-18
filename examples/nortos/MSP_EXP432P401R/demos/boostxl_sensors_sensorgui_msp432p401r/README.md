## Example Summary

This demo example works with a PC GUI to visualize real-time data from the following sensors on the BOOSTXL-SENSORS BoosterPack:

* OPT3001 digital ambient light sensor
* TMP007 digital infrared thermopile contactless temperature sensor
* Bosch® BMI160 Inertial Measurement Sensor
  * 3-axis digital accelerometer
  * 3-axis gyroscope sensor
* Bosch® BMM150 Geomagnetic Sensor
  * 3-axis digital magnetometer sensor
* Bosch® BME280 Environmental Sensor
  * Digital pressure
  * Ambient temperature
  * Relative humidity sensor

## BoosterPack Requirements

This demo requires the [Sensors BoosterPack Plug-In Module (BOOSTXL-SENSORS)](http://www.ti.com/tool/BOOSTXL-SENSORS) paired with the MSP-EXP432P401R LaunchPad

## Example Usage

Refer to [BOOSTXL-SENSORS BoosterPack Plug-in Module User's Guide](http://www.ti.com/lit/pdf/slau666) for more comprehensive detail on the demo.

1. Download [BOOSTXL-SENSORS_GUI+ET.zip](http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/BOOSTXL-SENSORS/latest/exports/BOOSTXL-SENSORS_GUI_ET.zip), and extract its contents.
2. Launch BOOSTXL-SENSORS_GUI+ET.
3. Plug in your MSP432P401R LaunchPad development kit with Sensors BoosterPack into a USB port.
4. Click the connect icon in the lower left corner of the GUI.
  * If needed, go to "Options" and select the proper COM Port for the Application UART and the baud
rate as 115200

	>NOTE: For Windows, you can find the port number by opening Device Manager and looking for
	"XDS110 Class Application/User UART" under "Ports (COM & LPT)". It will be listed as
	COMnn, where nn is the number of the port.

5. Click "File" and click "Program Device". A pre-built firmware should download to the LaunchPad
development kit

## Application Design Details

* No-RTOS
  * Clock System is configured with MCLK = 48 MHz.
  * EUSCI_A0 is used for UART communication with PC GUI.
  * EUSCI_B1 is used for I2C communication with various digital sensors.
