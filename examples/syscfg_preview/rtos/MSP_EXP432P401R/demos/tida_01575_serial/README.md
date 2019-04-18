### SysConfig Notice

All examples will soon be supported by SysConfig, a tool that will help you graphically configure your software components. A preview is available today in the examples/syscfg_preview directory. Starting in 3Q 2019, with SDK version 3.30, only SysConfig-enabled versions of examples will be provided. For more information, click [here](http://www.ti.com/sysconfignotice).

## Example Summary

The demo software is a code for the TI Reference Design TIDA-01575. The MSP432P4x Host MCU captures the analog output from the 3-axis Accelerometer, computes the FFT and transmits it using the serial COM port.

## Example Usage

Upon running the demo, the device captures **ADCBUFFERSIZE** number of samples for the Analog channels A11, A13 and A14 once every 1 minute at sampling frequency of **ADC_CH_SAMPLING_FREQ** per channel. The two parameters **ADCBUFFERSIZE** and **ADC_CH_SAMPLING_FREQ** is defined in the file vibrationCapture.h.

It then runs FFT conversion on the data and displays the information for every frequency point of the FFT on the serial COM port in comma separated vector (CSV) file format.
