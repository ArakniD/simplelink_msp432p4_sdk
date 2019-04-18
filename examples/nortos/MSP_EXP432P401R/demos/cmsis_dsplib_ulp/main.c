/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//****************************************************************************
//
// main.c - MSP-EXP432P401R
//
// Device toggles between low-power mode 3 (~980ms) and active mode (~20ms).
// During the active mode the ADC14 is in low power mode (12-bit resolution
// and 100ksps) and acquiring 2048 samples(~20ms) from differential ADC channel
// A0 (P5.4, P5.5).
//
// The ADC is used in conjunction with the DMA in scatter-gather mode.  The ADC
// memory is configured to measure channel A0 repeatedly and place the values
// sequentially into the 32-word memory space, ADC14MEM0-ADC14MEM31.  After
// each 32 words are written to the internal ADC14 memory space the ADC
// triggers the DMA to perform a task, which moves the 32 words from the ADC
// to the data array in SRAM.  This results in 32 tasks to move the 4096 
// samples.  In addition to these tasks, 4 additional tasks are added to
// trigger the CPU to begin processing the collected data.  In this way, a 
// portion of the processing is done in parallel with the data capture.  After
// the entire 2048 samples are collected the DMA ISR allows the CPU to return
// to main after the interrupt (does not return to sleep) where the final
// portion of the computation is performed to determine the RMS value.
//
// The RTC is used to provide a periodic wakeup of 1s from LPM3.  SRAM 
// configuration is managed dynamically to support measurements and low power
// requirements.
//
// SRAM management : Bank 0 is default, enabled and retained.  Banks 1 and 2
//                   will be turned off during LPM3 to save power, Banks 3
//                   through 7 will always be off
// Data in SRAM      :: 0x100 (DMA table) + 0x01C (usual .data)
// Functions in SRAM :: 0x3F0 
// Stack in SRAM     :: 0x200 
//
// Data in SRAM that is only 'used' during active mode and does not need to be
// preserved: 0x4014
//
//    Result in .map file
//    .bss       0    20000000    00000100     UNINITIALIZED
//    .data      0    20000100    0000001c     UNINITIALIZED
//    .stack     0    20001000    00000200     UNINITIALIZED
//    myData     0    20001800    00004014     UNINITIALIZED
//
//****************************************************************************

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <arm_math.h>
#include "main.h"
#include "preCompile.h"
#include "dmaTasks.h"

/*
 * Timer_A Compare Configuration Parameter
 * CCR1 is used to trigger the ADC14, conversion time
 * SMCLK = 12Mhz, Timer = 200Khz
 *
 * MODOSC ~ 5Mhz, and conversion time is 14 clocks in 12-bit mode +2CLK for
 * sync+1Clk for memory.  Negative duty cycle needs to be at least 17/5Mhz
 * 3.4uS or 40.8 SMCLKs, the positive duty cycle represents the sample and
 * hold time of (19/12Mhz) 1.58us (which is ~8 ADCCLK cycles).
 *
 */
const Timer_A_PWMConfig timerA_PWM =
{
    .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
    .clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1,
    .timerPeriod = (SAMPLE_CLOCK_FREQUENCY/SAMPLE_FREQEUNCY)-1,
    .compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1,
    .compareOutputMode = TIMER_A_OUTPUTMODE_SET_RESET,
    .dutyCycle = (SAMPLE_CLOCK_FREQUENCY/SAMPLE_FREQEUNCY)-19
};

/* DMA Control Table */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(MSP_EXP432P401RLP_DMAControlTable, 256)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=256
#elif defined(__GNUC__)
__attribute__ ((aligned (256)))
#elif defined(__CC_ARM)
__align(256)
#endif
static DMA_ControlTable MSP_EXP432P401RLP_DMAControlTable[16];
static DMA_ControlTable scatterGatherInit;

 /*****************************************************************************/
 /*
  * These variables are only viable during active and sleep modes.  During
  * deep sleep the sections of SRAM are turned off.
  */
#if defined(__TI_COMPILER_VERSION__)
 #pragma SET_DATA_SECTION("myData")
 uint16_t sampleIndex;
 uint8_t direction;
 uint8_t zcIndex;
 uint16_t zeroCrossArray[NUMBER_OF_ZEROCROSSINGS];
 int32_t i32Data_array[SAMPLE_LENGTH];
 float32_t rmsCalculation;
 float32_t rmsFrequency;
 #pragma SET_DATA_SECTION()
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma section = "MYDATA"
#define MYRAMDATA(x) _Pragma("location=\"MYDATA\"") x
MYRAMDATA(uint16_t sampleIndex;)
MYRAMDATA(uint8_t direction;)
MYRAMDATA(uint8_t zcIndex;)
MYRAMDATA(uint16_t zeroCrossArray[NUMBER_OF_ZEROCROSSINGS];)
MYRAMDATA(int32_t i32Data_array[SAMPLE_LENGTH];)
MYRAMDATA(float32_t rmsCalculation;)
MYRAMDATA(float32_t rmsFrequency;)
#elif defined(__GNUC__)
uint16_t sampleIndex __attribute__((section("myData")));
uint8_t direction __attribute__((section("myData")));
uint8_t zcIndex __attribute__((section("myData")));
uint16_t zeroCrossArray[NUMBER_OF_ZEROCROSSINGS] __attribute__((section("myData")));
int32_t i32Data_array[SAMPLE_LENGTH] __attribute__((section("myData")));
float32_t rmsCalculation __attribute__((section("myData")));
float32_t rmsFrequency __attribute__((section("myData")));
#elif defined(__CC_ARM)
 __attribute__((section(".bss.myData"))) uint16_t sampleIndex;
 __attribute__((section(".bss.myData"))) uint8_t direction;
 __attribute__((section(".bss.myData"))) uint8_t zcIndex;
 __attribute__((section(".bss.myData"))) uint16_t zeroCrossArray[NUMBER_OF_ZEROCROSSINGS];
 __attribute__((section(".bss.myData"))) int32_t i32Data_array[SAMPLE_LENGTH];
 __attribute__((section(".bss.myData"))) float32_t rmsCalculation;
 __attribute__((section(".bss.myData"))) float32_t rmsFrequency;
#endif

/*****************************************************************************/

void calculateRMS(void);
void configureGPIO(void);
void configureCS(void);
void configureMemory(void);
void configurePower(void);
void configureAnalog(void);
void configureDMA(void);
void configureRTC(void);


int main(void)
{
    /* Halting WDT and disabling master interrupts */
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_disableMaster();

    configureGPIO();
    configureCS();
    configureMemory();
    configurePower();
    configureAnalog();
    configureDMA();
    configureRTC();

    while(1)
    {
        /*
         * Set to intermediate operating point: 3Mhz, LDO
         */
        MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_3);
        MAP_PCM_setPowerMode(PCM_LDO_MODE);  // Switch from DC-DC to LDO
        MAP_Interrupt_enableSleepOnIsrExit();
        /*
         * Turn off balance of SRAM, less Bank 0, before entry to LPM3
         */
//        MAP_SysCtl_disableSRAMBank(SYSCTL_SRAM_BANK4); // Bank 4 0x2000 8000 to 0x2000 9FFF
//        MAP_SysCtl_disableSRAMBank(SYSCTL_SRAM_BANK3); // Bank 3 0x2000 6000 to 0x2000 7FFF
        MAP_SysCtl_disableSRAMBank(SYSCTL_SRAM_BANK2); // Bank 2 0x2000 4000 to 0x2000 5FFF
        MAP_SysCtl_disableSRAMBank(SYSCTL_SRAM_BANK1); // Bank 1 0x2000 2000 to 0x2000 3FFF

        MAP_PCM_gotoLPM3();
        __no_operation();
        /*
         * Process data
         */
        if(zcIndex >= NUMBER_OF_ZEROCROSSINGS-1)
        {
            calculateRMS();
        }
        else
        {
            rmsCalculation = 0.0;
            rmsFrequency = 0.0;
        }
    }  // end of while
}  // end of main

void calculateRMS(void)
{
    volatile uint16_t ii;
    uint16_t sampleSize;
    float32_t localMath;

    localMath = 0.0;
    /*
     * Determine is one whole cycle is captured (three zero crossings) or if
     * only the half cycle is captured.
     */
    if(zcIndex == NUMBER_OF_ZEROCROSSINGS)
    {
        sampleSize = zeroCrossArray[2] - zeroCrossArray[0];
    }
    else
    {
        //Only captured two zero crossings
        sampleSize = zeroCrossArray[1] - zeroCrossArray[0];
        if(sampleSize < SAMPLE_LENGTH/2)
        {
            sampleSize = sampleSize*2;
        }
        else
        {
            /*
             *  Bad result
             */
            sampleSize = 0;
            rmsCalculation = 0;
        }
    }
    rmsFrequency = (SAMPLE_FREQEUNCY)/((float32_t)sampleSize);
    for(ii=0;ii<sampleSize;ii++)
    {
        localMath+= (float32_t)i32Data_array[ii];
    }
    localMath = localMath/((float32_t)sampleSize);
    arm_sqrt_f32(localMath,&rmsCalculation);
    rmsCalculation = rmsCalculation*(1.2/2047);
}

void configureGPIO(void)
{
/*****************************************************************************/
    /*
     * Initialize all pins (except JTAG) for power consumption
     */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PA, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PB, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PC, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PD, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PE, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PA, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PB, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PC, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PD, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PE, PIN_ALL16);

    MAP_GPIO_setAsOutputPin(GPIO_PORT_PJ, GPIO_PIN0+GPIO_PIN1+GPIO_PIN2+
                          GPIO_PIN3);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PJ, GPIO_PIN0+GPIO_PIN1+GPIO_PIN2+
                               GPIO_PIN3);
/*****************************************************************************/
    /*
     * Configuration of GPIO for application.
     */

    /* Setup LFXT1*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);
    /*
     * Non-conventional usage of P1.2 as 'internal' interrupt triggered from
     * DMA.  In dmaTask.c, the ADC conversion results are moved 32 measurements
     * at a time and after 32 tasks (1024 measurements) the DMA writes to the
     * port to trigger the CPU to wake up and process the corresponding 1024
     * measurements.
     */
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN2);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN2);
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P1,GPIO_PIN2,
                                    GPIO_LOW_TO_HIGH_TRANSITION);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1,GPIO_PIN2);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1,GPIO_PIN2);
    /*
     * Differential ADC inputs A0+/A0- are on P5.5 and P5.4 respectively.
     * Configuring GPIOs for tertiary (analog) input.
     */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
                                                    GPIO_PIN5+GPIO_PIN4,
                                                    GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_Interrupt_enableInterrupt(INT_PORT1);
		 
};
void configureCS(void)
{
    /*****************************************************************************/
        /*
         * Initializes Clock System.
         *
         * An intermediate state, 3Mhz DCO and LDO mode,
         * is used to transition between the LPM3 LDO mode and the 24Mhz DC-DC
         * mode.
         */
        MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_3);
        MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );

        MAP_CS_startLFXT(false);
        /*
         * HSMCLK and SMCLK share the same source selection register, so the last
         * API will determine the clock source for both the HSMCLK and the SMCLK.
         * Neither SMCLK nor HSMCLK are used in this application but shown for
         * discussion purposes.
         */
        MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);
        MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);
        /*
         * Assign LFXT to BCLK or ACLK
         */
        MAP_CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
        MAP_CS_initClockSignal(CS_BCLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
};

void configureMemory(void)
{
/*****************************************************************************/
    /*
     * Memory Configuration
     */

    /* Flash wait state of '0' */
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 0);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 0);
    /*
     *  Only a portion of the SRAM is used in this application.  The linker
     *  command file (msp432p401r_LPMRAM.cmd) is also configured to exclude
     *  banks 3-7.
     *
     *  Bank 7 0x2000 E000 to 0x2000 FFFF
     *  Bank 6 0x2000 C000 to 0x2000 DFFF
     *  Bank 5 0x2000 A000 to 0x2000 BFFF
     *  Bank 4 0x2000 8000 to 0x2000 9FFF
     *  Bank 3 0x2000 6000 to 0x2000 7FFF
     */
    MAP_SysCtl_disableSRAMBank(SYSCTL_SRAM_BANK7);
    MAP_SysCtl_disableSRAMBank(SYSCTL_SRAM_BANK6);
    MAP_SysCtl_disableSRAMBank(SYSCTL_SRAM_BANK5);
    MAP_SysCtl_disableSRAMBank(SYSCTL_SRAM_BANK4);
    MAP_SysCtl_disableSRAMBank(SYSCTL_SRAM_BANK3);
};
void configurePower(void)
{
/*****************************************************************************/
    /*
     * Power Control Manager and Power Supply Supervisor Configuration
     *
     * Set the core voltage level to VCORE0, this application does not exceed
     * 24Mhz and therefore the Vcore and wait states are set for the entire
     * application here.  It would be possible to transition to 48Mhz after
     * waking from LPM3, however this would also require additional
     * instructions to move the device to Vcore1 and a flash wait state of 1.
     */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE0);
    /*
     * The 'rude' mode is used to ensure that the device enters LPM3
     * independent of clock requests.
     */
    MAP_PCM_enableRudeMode();
    /* Turn off PSS high-side supervisors to conserve power. */
    MAP_PSS_disableHighSide();
};
void configureAnalog(void)
{
/*****************************************************************************/
    /*
     * Configure Analog, ADC and Reference
     */

    /* 1.2V reference */
    MAP_REF_A_setReferenceVoltage(REF_A_VREF1_2V);
    /*
     * Initializing ADC (SYSCLK/1/1), SYSCLK = 5Mhz
     * Sample Time = 19 Timer_A clock cycles (12Mhz)
     * 12-bit Conversion time = 14 clocks
     * Sync+memory+autoMode = 1+1+1 clocks 
     *
     */
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_SYSOSC, ADC_PREDIVIDER_1, ADC_DIVIDER_1, ADC_NOROUTE);
    MAP_ADC14_setResolution(ADC_12BIT);

    /* Trigger from TA0CCR1, false = no inversion of trigger */
    MAP_ADC14_disableSampleTimer();
    MAP_ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false);

    /* 
     * Configuring ADC Memory 
     * Start Memory, Stop Memory, Repeat Mode = true
     */
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM31, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM1,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM2,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM3,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM4,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM5,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM6,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM7,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM8,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM9,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM10,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM11,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM12,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM13,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM14,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM15,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM16,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM17,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM18,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM19,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM20,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM21,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM22,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM23,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM24,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM25,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM26,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM27,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM28,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM29,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM30,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);
    MAP_ADC14_configureConversionMemory(ADC_MEM31,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, ADC_DIFFERENTIAL_INPUTS);

    /*
     * Configure low power mode.  Limited to 12-bit resolution and 
     * 200ksps
     */
    MAP_ADC14_setPowerMode(ADC_ULTRA_LOW_POWER_MODE);
};
void configureDMA(void)
{
/*****************************************************************************/
    /* Configuring DMA module */
    MAP_DMA_enableModule();
    MAP_DMA_setControlBase(MSP_EXP432P401RLP_DMAControlTable);
    /*
     * Setup the DMA + ADC14 interface
     *  Disabling channel attributes
     */
    MAP_DMA_disableChannelAttribute(DMA_CH7_ADC14,
                                     UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST |
                                     UDMA_ATTR_HIGH_PRIORITY |
                                     UDMA_ATTR_REQMASK);
    MAP_DMA_enableChannelAttribute(DMA_CH7_ADC14, UDMA_ATTR_USEBURST);
    /*
     * Set the DMAs primary channel for peripheral scatter gather mode
     */
    MAP_DMA_setChannelScatterGather(DMA_CH7_ADC14, NUMBER_OF_TASKS, (void*)&altTaskList[0], false);
    scatterGatherInit = MSP_EXP432P401RLP_DMAControlTable[7];

};
void configureRTC(void)
{
/*****************************************************************************/
    /*
     * RTC, 1s period,
     * Use Q7 (32768, 1s) of RT1PS, with a RT0PS+RT1PS 'pre-value' of
     * (1.0-1.0)*32768 = 000h -> RT0PS = 00h, RT1PS = 00h
     */
    MAP_RTC_C_setPrescaleValue(RTC_C_PRESCALE_0,0x00);
    MAP_RTC_C_setPrescaleValue(RTC_C_PRESCALE_1,0x00);
/* TODO: This API is not functional.  Bug filed. */
//    MAP_RTC_C_definePrescaleEvent(RTC_C_PRESCALE_1,RTC_C_PSEVENTDIVIDER_32);
    RTC_C->PS1CTL = RTC_C_PS1CTL_RT1IP_7;
    MAP_RTC_C_enableInterrupt(RTC_C_PRESCALE_TIMER1_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_RTC_C);
/*****************************************************************************/
    MAP_Interrupt_enableMaster();
    /* Start RTC Clock */
    MAP_RTC_C_startClock();
};

/*
 * Use GPIO as DMA callback for processing the RMS calculation.  This will be
 * called every 32 measures. In this case we do not want to use the driverLib
 * APIs in FLASH or ROM but keep everything in SRAM 
 */
#if defined(__TI_COMPILER_VERSION__)
__attribute__((ramfunc))
#elif defined(__IAR_SYSTEMS_ICC__)
__ramfunc
#elif defined(__GNUC__)
__attribute__((section(".TI.ramfunk")))
#elif defined(__CC_ARM)
__attribute__((section(".TI.ramfunk")))
#endif
void PORT1_IRQHandler(void)
{
    uint8_t status;
    volatile uint16_t   ii;

    //status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    status = P1-> IE;
    status &= P1->IFG;
    //MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
    P1->IFG &= ~status;
    
    if(status == GPIO_PIN2)
    {
        /*
         * Debug
         */
        //MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);
        P2->OUT ^= BIT1;
        //MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN2);
        P1->OUT &= ~BIT2;
        if(sampleIndex == 0)
        {
            if(i32Data_array[sampleIndex] > ZERO_CROSS)
            {
                direction = DOWN;
            }
            else
            {
                direction = UP;
            }
        }
        /*
         * find zero-crossings
         * Expecting 0 to 0x0FFF range.  Where 0 is negative full scale.
         */
        for(ii=0;ii<INTERMEDIATE_PROC_SIZE;ii++)
        {
            if(zcIndex < NUMBER_OF_ZEROCROSSINGS)
            {
                if(direction == DOWN)
                {
                    if(i32Data_array[sampleIndex] < ZERO_CROSS &&
                            i32Data_array[sampleIndex+1] < ZERO_CROSS)
                    {
                        zeroCrossArray[zcIndex] = sampleIndex;
                        direction = UP;
                        zcIndex++;
                    }
                }
                else
                {
                    if(i32Data_array[sampleIndex] > ZERO_CROSS &&
                            i32Data_array[sampleIndex+1] > ZERO_CROSS)
                    {
                        zeroCrossArray[zcIndex] = sampleIndex;
                        direction = DOWN;
                        zcIndex++;
                    }
                }
            }
            /*
             *  Convert to absolute value and square
             */
            if(i32Data_array[sampleIndex] > ZERO_CROSS)
            {
                i32Data_array[sampleIndex] = i32Data_array[sampleIndex] - (ZERO_CROSS+1);
            }
            else
            {
                i32Data_array[sampleIndex] = (ZERO_CROSS+1) - i32Data_array[sampleIndex];
            }
            i32Data_array[sampleIndex] = i32Data_array[sampleIndex]*i32Data_array[sampleIndex];
            sampleIndex++;
        }
        if(sampleIndex == SAMPLE_LENGTH)
        {
            //Finished
            /*
             * TODO: Clearing ADC14ON here instead of in the DMA.
             */
            //MAP_ADC14_disableModule();
            BITBAND_PERI(ADC14->CTL0, ADC14_CTL0_ON_OFS) = 0;
            //MAP_Interrupt_disableSleepOnIsrExit();
            SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
            /*
             * Reset primary and alternate DMA structures
             */
            //MAP_DMA_setChannelScatterGather(DMA_CH7_ADC14, NUMBER_OF_TASKS, (void*)&altTaskList[0], false);
            MSP_EXP432P401RLP_DMAControlTable[7] = scatterGatherInit;
            DMA_Control->ALTCLR = BIT7;
    
            //MAP_DMA_disableModule();
            DMA_Control->CFG = 0;
            DMA_Channel->CH_SRCCFG[7] = 0;
            /*
             * Debug
             */
            //MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
            P2->OUT &= ~BIT2;
        }
    }
}

void RTC_C_IRQHandler(void)
{
    uint32_t status;
    volatile uint32_t ii;
    /*
     * Debug
     */
    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0);

    status = MAP_RTC_C_getEnabledInterruptStatus();
    MAP_RTC_C_clearInterruptFlag(status);
    MAP_RTC_C_holdClock();
    /*
     * Enabling Bank 2 automatically enables bank 1 to prevent memory holes
     */
    MAP_SysCtl_enableSRAMBank(SYSCTL_SRAM_BANK2);
    // Bank 2 0x2000 4000 to 0x2000 5FFF
    // Bank 1 0x2000 2000 to 0x2000 3FFF
    // Bank 0 0x2000 0000 to 0x2000 1FFF
    while (!(SYSCTL->SRAM_BANKEN & SYSCTL_SRAM_BANKEN_SRAM_RDY));

    if (status & RTC_C_PRESCALE_TIMER1_INTERRUPT)
    {
        /*
         * Reload for next interval
         */
        MAP_RTC_C_setPrescaleValue(RTC_C_PRESCALE_0,0x00);
        MAP_RTC_C_setPrescaleValue(RTC_C_PRESCALE_1,0x00);
        /* Re-start RTC Clock */
        MAP_RTC_C_startClock();
        /*
         * Turn on SYSOSC
         */
        ADC14->CTL0 &= ~ADC14_CTL0_SSEL_MASK;
        ADC14->CTL0 |= ADC14_CTL0_SSEL__SYSCLK;

        MAP_REF_A_enableReferenceVoltage();

        MAP_DMA_enableModule();
        MAP_ADC14_enableModule();
        MAP_ADC14_enableConversion();
        MAP_DMA_assignChannel(DMA_CH7_ADC14);
        MAP_DMA_enableChannel(7);
        /*
         * Move to a faster/more efficient operating point
         *   24Mhz, DC-DC
         */
        MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);
        MAP_PCM_setPowerMode(PCM_DCDC_MODE);
        /*
         * Disable deepSleep, lpm3->lpm0
         */
        SCB->SCR &= ~(SCB_SCR_SLEEPDEEP_Msk);
        /*
         * Init variables after re-enable SRAM
         */
        sampleIndex = 0;
        zcIndex = 0;
        /*
         * wait ~70us for reference to settle
         * 24Mhz operation,
         */
#if defined(__TI_COMPILER_VERSION__)
        // Intrinsic for delay
        __delay_cycles(SETTLE_TIME_DELAY);
#else
        for(ii=0;ii<SETTLE_TIME_DELAY/4;ii++)
        {
            __no_operation();	
        }
#endif
        /* Start ADC+DMA conversions */
        MAP_Timer_A_generatePWM(TIMER_A0_BASE, &timerA_PWM);
        /*
         * Debug
         */
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
    }
}
