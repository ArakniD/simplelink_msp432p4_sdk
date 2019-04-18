### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

## Example Summary

The MSP-EXP432P4111 LaunchPad out-of-box demo software extends upon a basic blink LED example and allows users
to control the blink rate and the color of a RGB LED on the [MSP-EXP432P4111 LaunchPad development kit](http://www.ti.com/tool/msp-exp432p4111).

The demo showcases the on-board segmented LCD by displaying LED blinkrate and color. Moreover, buttons S1 and S2 can be held down simultaneously to display temperature measured from on-board LMT70 analog temp sensor.

A [cloud-based GUI](http://dev.ti.com/gallery/view/3491167/MSP-EXP432P4111_OOB/) is available to interact with the out-of-box demo, providing controls to manipulate RGB LED and update LCD display segments.

## Example Usage

Upon running the demo, a welcoming text message scrolls across the LCD display, and the RGB LED2 blinks RED at 60 Beats-per-minute (1 Hz).

* Tapping button S1 repeatedly at a constant rate will synchronize the LED2 blink frequency to the button tap frequency. This emulates a common "Tap mode" feature found on many musical metronomes, enabling user to enter a custom beat simply by tapping.
* Pressing button S2 will cycle LED2 through four color modes: Red, Green, Blue and Random color. Each color
setting retains its own LED blink frequency.
* The on-board LCD displays the LED frequency (beats-per-minutes) and the color mode that it's in.
* Briefly holding down both buttons S1 & S2 will put application into temperature mode. Before entering the temperature mode, the user is prompted with two options:
 * Press Left (S1) button to use the on-board External Temperature Sensor (connected to analog input pin P5.3)
 * Press Right (S2) button to use the MSP432's Internal ADC Temperature Sensor
> **Note:**
> The external temperature sensor requires the 0 Ohm Resistor **R8** to be populated. The resistor R8 is absent by default to allow for independent power measurement of the MSP432 microcontroller. Adding a 0 Ohm resistor or shorting across R8 will provide power to the external temperature sensor. However, subsequent EnergyTrace or board-level power measurement will then include the external temperature sensor's power consumption.
* Hold down both buttons S1 & S2 briefly to exit temperature mode

A [cloud-based PC GUI](http://dev.ti.com/gallery/view/3491167/MSP-EXP432P4111_OOB/) accompanies the demo to allow control of color and blink rate of the RGB LED.

* Launch the out-of-box demo GUI from TI Resource Explorer or by going [here](http://dev.ti.com/gallery/view/3491167/MSP-EXP432P4111_OOB/)
* Connect to the hardware by clicking on the link icon at the bottom left corner and making sure the serial connection has been established. (GUI status bar indicates "waiting for data...")
* The LED color sliders, color wheel, and BPM input box can be used to control the RGB LED color and blink rate of the MSP-EXP432P4111 LaunchPad.
 * When LED color and blinkrate are updated using the PC GUI, the demo enters PC color mode, which keeps a separate color and blinkrate apart from the other four default color modes.
* The LCD text input, symbol controls, and animation controls can be used to manipulate what's displayed on the segmented LCD of the MSP-EXP432P4111 LaunchPad.

## Application Design Details

**Peripheral Configuration:**

* `Clock System` is configured with MCLK = 48MHz
* `EUSCI_A0` is used for UART communication with PC GUI @ 115200 baudrate
* `Timer32_0` is used to setup `intervalTimer` to reset button tap counts after a 4 second timeout.
* `Timer32_1` is used to setup `blinkTimer`, which toggles the RGB LED every interval.
* `TimerA1` is configured with PWM output on its compare registers 1, 2, & 3 to control RED, GRN, & BLU RGB channels
* `TimerA2` is used to setup `debounceTimer` that debounces buttons S1 and S2.
* `ADC14` is configured to sample and convert from channel A2 (P5.3), connected to the on-board LMT70 analog temp sensor

**RTOS Threads:**

* Tasks
 * `led_thread` handles blinking the RGB LED and updating its color & blinkrate.
 * `lcd_thread` handles displaying texts, symbols, and battery icons on the segmented LCD display
 * `uart_thread` receives and parses incoming JSON data from the cloud-based PC GUI
 * `temperature_thread` handles triggering an ADC sample & conversion to obtain temperature measurement from the on-board LMP70 analog temp sensor.
* ISRs
 * `intervalTimer_Callback` handles resetting button tap counts after a 4 second timeout.
 * `blinkTimer_Callback` sends LED toggle command to the led_thread to toggle the RGB LED.
 * `debounceTimer_Callback` checks the push button states of S1 & S2 every 20 ms and resets button debounce variables when appropriate.
 * `gpioButton0_Callback` handles button S1 presses and updates RGB LED blinkrate.
 * `gpioButton1_Callback` handles button S2 presses and changes RGB LED colormode.

**JSON Serial Data Communication**

An open source library,  `cJSON`, is used to parse incoming JSON strings from the PC GUI. This demo utilized JSON data structure due to ease-of-use and readability of serial data. Real-world applications will more likely use compact binary data structures for optimal efficiency.

**LCD Design and Considerations:**

Despite each LCD pin being configurable as either COMMON or SEGMENT functionalities, particular care was taken to determine LCD pin connections between the microcontroller and the LCD on the MSP-EXP432P4111  LaunchPad. Pin connections were chosen to ensure simple and easy to read software while minimizing hardware layout complexity.

A custom `LCD_HAL` hardware abstraction layer was written to provide simple functions to manipulate the segmented LCD screen on the MSP-EXP432P4111 LaunchPad. This HAL library can be referenced for other segmented LCD designs, but it will most likely need to be drastically re-written depending on the chosen LCD and the particular pin connections in a different board design.

MSP432P4111's `LCD_F` peripheral features built-in animation controls (up-to 8 LCD segments & up-to 8 animation frames). The battery symbol on the segmented LCD is made of 8 LCD segments, and therefore serves as a perfect example to demonstrate `LCD_F` animation feature.


----------


> **Note:**
> * TI-RTOS
>  *  When building in Code Composer Studio, the kernel configuration project will be imported along with the example. The kernel configuration project is referenced by the example, so it will be built first.
> * FreeRTOS
>  *  Please view the `FreeRTOSConfig.h` header file for example configuration information.
