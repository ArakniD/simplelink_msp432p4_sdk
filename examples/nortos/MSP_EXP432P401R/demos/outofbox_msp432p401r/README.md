## Example Summary

The out-of-box demo software extends on a basic blink LED example and allows users
to control the blink rate and the color of a RGB LED on the MSP-EXP432P401R LaunchPad development kit.

>**NOTE:** The source code for this demo is available in 2 coding styles:
>
>1. No-RTOS
>  * Out of the box, LaunchPads are loaded with this version of firmware.
>  * Uses DriverLib APIs and CMSIS style register access
>2. RTOS
>  * This coding style is portable across devices in the SimpleLink MCU Family.
>  * Uses TI Drivers APIs and POSIX compatible RTOS APIs
>  * Compatible with TI-RTOS and FreeRTOS

## Example Usage

Upon running the demo, the RGB LED2 blinks Red at 1 Hz.

* Tapping switch S1 repeatedly at a constant rate will synchronize the LED2 blink frequency to the tap frequency.
* Pressing Switch S2 will cycle LED2 through four color settings: Red, Green, Blue and Random color. Each color
setting retains its own LED blink frequency.

A GUI accompanies the demo to allow control of color and blink rate of the RGB LED.

* Launch the out-of-box demo GUI in TI Resource Explorer
* Click on the Connect button and open the serial COM port.
* After the connection has been established and the GUI indicates "Target Status: Running...," the color wheel/sliders and BPM input box can be used to control the RGB LED color and blink rate.

## Application Design Details

### No-RTOS

Peripheral Configuration:

  * Clock System is configured with MCLK = 3 MHz.
  * EUSCI_A0 is used for UART communication with PC GUI.
  * SysTick is used to control LED blink frequency (LED toggles inside the SysTick ISR).
  * Timer_A0 is configured with PWM output for Red, Green, and Blue channels of LED.
  * Timer_A1 is used for button debounce.
  * Timer_A2 is used as a timeout timer for tap counter reset.

After initializing device peripherals, the device goes into low power mode sleep mode, only waking up to service hardware interrupts from SysTick, EUSCIA0 (UART), GPIO, and Timers.

### RTOS
>When building in Code Composer Studio, the kernel configuration project will be imported along with the example. The kernel configuration project is referenced by the example, so it will be built first.

Peripheral Configuration:

  * Clock System is configured by RTOS kernel configuration and does not need to be known by developer for timers to correctly function in this application.
  * EUSCI_A0 is used for UART communication with PC GUI.
  * Timer32_1 is used to control/keep track of LED blink rate.
  * Timer_A1 is configured with PWM output for Red, Green, and Blue channels of LED.
  * Timer_A2 is used for button debounce.
  * Timer32_0 is used as a timeout timer for tap counter reset.  This is also used for counting interval time between S1 button taps.

Application structure:

  * `led_thread` handles blinking the RGB LED and updating its color & blinkrate.
  * `lcd_thread` handles displaying texts, symbols, and battery icons on the segmented LCD display
  * `uart_thread` receives and parses incoming JSON data from the PC GUI
