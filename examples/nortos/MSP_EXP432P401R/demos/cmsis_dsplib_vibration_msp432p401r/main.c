/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
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
//***************************************************************************************
//  Captures analog data on A0, A1 and A2 with different sampling rates and then
//  performs FFT using CMSIS Library function.
//
//                MSP432P4xx
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST          P4.7|<--A2
//            |             P5.4|<--A1
//            |             P5.5|<--A0
//
//  Amit Ashara
//  Texas Instruments, Inc
//  Aug 2017
//  Built with Code Composer Studio v7
//***************************************************************************************
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#include "arm_math.h"
#include "arm_const_structs.h"

/* -------------------------------------------------------------------
 * DMA Control Table
 * ------------------------------------------------------------------- */
#if defined(__ICCARM__)
#pragma data_alignment=1024
uint8_t controlTable[1024];
#elif defined(__TI_ARM__)
#pragma DATA_ALIGN(controlTable, 1024)
uint8_t controlTable[1024];
#else
uint8_t controlTable[1024] __attribute__ ((aligned(1024)));
#endif

/* -------------------------------------------------------------------
 * Define used for achieving low current consumption in LPM3.5
 * ------------------------------------------------------------------- */
#define POWER_MODE_OPT           1
#define SSF_HANN_WINDOW          0
#define USF_HANN_WINDOW          0
#define ABF_HANN_WINDOW          0

/* -------------------------------------------------------------------
 * Define used for the Alarm function
 * ------------------------------------------------------------------- */
#define SSF_ALARM_PIN            GPIO_PIN0
#define ABF_ALARM_PIN            GPIO_PIN1
#define USF_ALARM_PIN            GPIO_PIN2

/* -------------------------------------------------------------------
 * Define used for the Averaging window
 * ------------------------------------------------------------------- */
#define AVG_WINDOW               10

/* -------------------------------------------------------------------
 * Counter for 10 minute and the 1 minute alarms
 * ------------------------------------------------------------------- */
uint32_t set5minAlarm = 0;
uint32_t set2minAlarm  = 0;

/* -------------------------------------------------------------------
 * Variable for counting the averaging window and setting up the
 * alarm for both monitor range and alarm range
 * ------------------------------------------------------------------- */
uint8_t  avgWindowCounter = 0;
uint16_t setAlarmLED      = 0;
bool     setMonitorMode   = false;
uint8_t  incMonitorCount  = 0;

/* -------------------------------------------------------------------
 * Define used for controlling the number of samples to be taken for FFT
 * analysis
 * ------------------------------------------------------------------- */
#define USF_SAMPLES              1024
#define ABF_SAMPLES              1024
#define SSF_SAMPLES              512

/* -------------------------------------------------------------------
 * Array for holding the ultrasonic frequency, audio-band frequency and
 * sub-sonic frequency bands
 * ------------------------------------------------------------------- */
int32_t ui32USFArray[USF_SAMPLES];
int32_t ui32ABFArray[ABF_SAMPLES];
uint32_t ui32SSFArray[SSF_SAMPLES];

/* -------------------------------------------------------------------
 * Variable to count the number of sub-sonic samples taken by the CPU
 * ------------------------------------------------------------------- */
uint16_t ui16SSFArrayIndex = 0;
bool     bProcessFFT       = false;
bool     bSSFDataAcquired  = false;

/* -------------------------------------------------------------------
* External Input and Output buffer Declarations for FFT Bin Example
* ------------------------------------------------------------------- */
int16_t fftOutput[USF_SAMPLES*2];

/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */
uint32_t ifftFlag     = 0;
uint32_t doBitReverse = 1;

/* -------------------------------------------------------------------
 * Structure to store the iteration count, range of bin of interest,
 * value to monitor in the range of bin, alarm condition to monitor
 * across the spectrum and energy value of each bin.
 * ------------------------------------------------------------------- */
typedef struct _Energy_Bin512
{
    uint8_t  iteration;
    uint16_t lowBin;
    uint16_t hiBin;
    uint16_t monitorValue;
    uint16_t alarmValue;
    uint32_t energyValue[512];
} Energy_Bin512;

typedef struct _Energy_Bin256
{
    uint8_t  iteration;
    uint16_t lowBin;
    uint16_t hiBin;
    uint16_t monitorValue;
    uint16_t alarmValue;
    uint32_t energyValue[256];
} Energy_Bin256;

Energy_Bin512 USFEnergyBin;
Energy_Bin512 ABFEnergyBin;
Energy_Bin256 SSFEnergyBin;

/* -------------------------------------------------------------------
 * Set up of the current RTC time when the system is first powered up.
 * This is set for April 28th 2017 11:30:00 PM
 * ------------------------------------------------------------------- */
const RTC_C_Calendar currentTime =
{
        00,
        30,
        23,
        06,
        28,
        04,
        2017
};

/* -------------------------------------------------------------------
 * Values used by the DMA Scatter Gather structure to program the
 * peripherals
 * ------------------------------------------------------------------- */
static uint16_t TimerStopCommand       = 0x0200;
static uint16_t TimerUpModeCommand     = 0x0210;
static uint16_t TimerCCR1Update40K     = 0x04B0;
static uint16_t TimerCCR1Update400K    = 0x0078;
static uint32_t ADCStopCommand         = 0x0C6C2210;
static uint32_t ADCStartCommand        = 0x0C6C2212;
static uint32_t ADCConfigProcSampling  = 0x046C2210;
static uint32_t ADCConfigABFSampling   = 0x00010030;
static uint32_t ADCConfigABFChannel    = 0x00000101;
static uint32_t ADCConfigSSFSampling   = 0x00000030;
static uint32_t ADCConfigSSFChannel    = 0x00000100;

/* -------------------------------------------------------------------
 * DMA Task Structure
 * ------------------------------------------------------------------- */
DMA_ControlTable TaskList[] =
{
 /* 1. Transfer the 1024 samples of 400 KHz */
 DMA_TaskStructEntry(USF_SAMPLES, UDMA_SIZE_32,
                     UDMA_SRC_INC_NONE, &ADC14->MEM[2],
                     UDMA_DST_INC_32, (void *)&ui32USFArray[0],
                     UDMA_ARB_1, UDMA_MODE_PER_SCATTER_GATHER
                     ),
 /* 2. Stop the Timer-A0 */
 DMA_TaskStructEntry(1, UDMA_SIZE_16,
                     UDMA_SRC_INC_NONE, (void *)&TimerStopCommand,
                     UDMA_DST_INC_NONE, &TIMER_A0->CTL,
                     UDMA_ARB_1, UDMA_MODE_MEM_SCATTER_GATHER
                     ),
 /* 3. Re configure the Timer-A0 CCR for 40 KHz */
 DMA_TaskStructEntry(1, UDMA_SIZE_16,
                     UDMA_SRC_INC_NONE, (void *)&TimerCCR1Update40K,
                     UDMA_DST_INC_NONE, &TIMER_A0->CCR[0],
                     UDMA_ARB_1, UDMA_MODE_MEM_SCATTER_GATHER
                    ),
 /* 4. Stop the ADC */
 DMA_TaskStructEntry(1, UDMA_SIZE_32,
                     UDMA_SRC_INC_NONE, (void *)&ADCStopCommand,
                     UDMA_DST_INC_NONE, &ADC14->CTL0,
                     UDMA_ARB_1, UDMA_MODE_MEM_SCATTER_GATHER
                    ),
 /* 5. Reconfigure the ADC for Audio Band Sampling */
 DMA_TaskStructEntry(1, UDMA_SIZE_32,
                     UDMA_SRC_INC_NONE, (void *)&ADCConfigABFSampling,
                     UDMA_DST_INC_NONE, &ADC14->CTL1,
                     UDMA_ARB_1, UDMA_MODE_MEM_SCATTER_GATHER
                    ),
 /* 6. Configure the ADC MEMCTL channel */
 DMA_TaskStructEntry(1, UDMA_SIZE_32,
                     UDMA_SRC_INC_NONE, (void *)&ADCConfigABFChannel,
                     UDMA_DST_INC_NONE, &ADC14->MCTL[1],
                     UDMA_ARB_1, UDMA_MODE_MEM_SCATTER_GATHER
                    ),
 /* 7. Start the ADC */
 DMA_TaskStructEntry(1, UDMA_SIZE_32,
                     UDMA_SRC_INC_NONE, (void *)&ADCStartCommand,
                     UDMA_DST_INC_NONE, &ADC14->CTL0,
                     UDMA_ARB_1, UDMA_MODE_MEM_SCATTER_GATHER
                    ),
 /* 8. Start the Timer-A0 */
 DMA_TaskStructEntry(1, UDMA_SIZE_16,
                     UDMA_SRC_INC_NONE, (void *)&TimerUpModeCommand,
                     UDMA_DST_INC_NONE, &TIMER_A0->CTL,
                     UDMA_ARB_1, UDMA_MODE_MEM_SCATTER_GATHER
                    ),
 /* 9. Transfer the 1024 samples of 40 KHz */
 DMA_TaskStructEntry(ABF_SAMPLES, UDMA_SIZE_32,
                     UDMA_SRC_INC_NONE, &ADC14->MEM[1],
                     UDMA_DST_INC_32, (void *)&ui32ABFArray[0],
                     UDMA_ARB_1, UDMA_MODE_PER_SCATTER_GATHER
                    ),
 /* 10. Stop the Timer-A0 */
 DMA_TaskStructEntry(1, UDMA_SIZE_16,
                     UDMA_SRC_INC_NONE, (void *)&TimerStopCommand,
                     UDMA_DST_INC_NONE, &TIMER_A0->CTL,
                     UDMA_ARB_1, UDMA_MODE_MEM_SCATTER_GATHER
                    ),
 /* 11. Re configure the Timer-A0 CCR for 400 KHz */
 DMA_TaskStructEntry(1, UDMA_SIZE_16,
                     UDMA_SRC_INC_NONE, (void *)&TimerCCR1Update400K,
                     UDMA_DST_INC_NONE, &TIMER_A0->CCR[0],
                     UDMA_ARB_1, UDMA_MODE_MEM_SCATTER_GATHER
                    ),
 /* 12. Stop the ADC */
 DMA_TaskStructEntry(1, UDMA_SIZE_32,
                     UDMA_SRC_INC_NONE, (void *)&ADCStopCommand,
                     UDMA_DST_INC_NONE, &ADC14->CTL0,
                     UDMA_ARB_1, UDMA_MODE_MEM_SCATTER_GATHER
                     ),
 /* 13. Reconfigure the ADC for Sub Sonic range Sampling */
 DMA_TaskStructEntry(1, UDMA_SIZE_32,
                     UDMA_SRC_INC_NONE, (void *)&ADCConfigProcSampling,
                     UDMA_DST_INC_NONE, &ADC14->CTL0,
                     UDMA_ARB_1, UDMA_MODE_MEM_SCATTER_GATHER
                    ),
 /* 14. Reconfigure the ADC for Sub sonic channel */
 DMA_TaskStructEntry(1, UDMA_SIZE_32,
                     UDMA_SRC_INC_NONE, (void *)&ADCConfigSSFSampling,
                     UDMA_DST_INC_NONE, &ADC14->CTL1,
                     UDMA_ARB_1, UDMA_MODE_MEM_SCATTER_GATHER
                    ),
 /* 15. Reconfigure the ADC MEMCTL for sub-sonic sampling */
 DMA_TaskStructEntry(1, UDMA_SIZE_32,
                     UDMA_SRC_INC_NONE, (void *)&ADCConfigSSFChannel,
                     UDMA_DST_INC_NONE, &ADC14->MCTL[0],
                     UDMA_ARB_1, UDMA_MODE_BASIC
                    ),
};

/* -------------------------------------------------------------------
 * Timer_A Continuous Mode Configuration Parameter
 * ------------------------------------------------------------------- */
const Timer_A_UpModeConfig upModeConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,           // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,       // SMLK/120 = 400 Khz
        120,
        TIMER_A_TAIE_INTERRUPT_DISABLE,      // Disable Timer ISR
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE, // Disable CCR0
        TIMER_A_DO_CLEAR                     // Clear Counter
};

/* -------------------------------------------------------------------
 * Timer_A Compare Configuration Parameter
 * ------------------------------------------------------------------- */
const Timer_A_CompareModeConfig compareConfig =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_1,          // Use CCR1
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_SET_RESET,               // Keep the output reset
        60                                         // 120 Period
};

/* -------------------------------------------------------------------
 * Function to compute the next wake-up duration. When the alarm mode
 * parameter is set to 'true' then the device is woken up every minute
 * and samples are collected for FFT analysis. If the parameter is set
 * to 'false' then the device is woken up every 10 minutes and samples
 * are collected for FFT analysis.
 * ------------------------------------------------------------------- */
void predictive_maintenance_setWakeTime(bool alarmMode)
{
    RTC_C_Calendar newTime;

    /* Get the current time */
    newTime = MAP_RTC_C_getCalendarTime();

    /* If alarm mode has been set next sample acquisition will happen after
     * 2 minutes. If not then it will happen after 5 minutes. Also do make
     * sure to roll over the hour. */
    if(alarmMode)
    {
        set2minAlarm++;
        if(newTime.minutes >= 58)
        {
            if(newTime.hours >= 23)
            {
                MAP_RTC_C_setCalendarAlarm((newTime.minutes-58), 0, RTC_C_ALARMCONDITION_OFF,
                        RTC_C_ALARMCONDITION_OFF);
            }
            else
            {
                MAP_RTC_C_setCalendarAlarm((newTime.minutes-58), (newTime.hours+1), RTC_C_ALARMCONDITION_OFF,
                        RTC_C_ALARMCONDITION_OFF);
            }
        }
        else
        {
            MAP_RTC_C_setCalendarAlarm((newTime.minutes+2), (newTime.hours), RTC_C_ALARMCONDITION_OFF,
                    RTC_C_ALARMCONDITION_OFF);
        }
    }
    else
    {
        set5minAlarm++;
        if(newTime.minutes >= 58)
        {
            if(newTime.hours >= 23)
            {
                MAP_RTC_C_setCalendarAlarm((newTime.minutes-58), 0, RTC_C_ALARMCONDITION_OFF,
                        RTC_C_ALARMCONDITION_OFF);
            }
            else
            {
                MAP_RTC_C_setCalendarAlarm((newTime.minutes-58), (newTime.hours+1), RTC_C_ALARMCONDITION_OFF,
                        RTC_C_ALARMCONDITION_OFF);
            }
        }
        else
        {
            MAP_RTC_C_setCalendarAlarm((newTime.minutes+2), (newTime.hours), RTC_C_ALARMCONDITION_OFF,
                    RTC_C_ALARMCONDITION_OFF);
        }
    }
}

/* -------------------------------------------------------------------
 * ADC14 Interrupt Handler for reading the data from the ADC channel 0
 * to the sub-sonic frequency buffer.
 * ------------------------------------------------------------------- */
void ADC14_IRQHandler(void)
{
    /* Get the ADC Data into the LF Buffer and increment the count */
    ui32SSFArray[ui16SSFArrayIndex++] = MAP_ADC14_getResult(ADC_MEM0);

    bSSFDataAcquired = true;
}

/* -------------------------------------------------------------------
 * RTC Interrupt Handler to wake-up the device from LPM3.5 state. If
 * wake-up is due to Alarm match then the timer interrupt from the RTC
 * is also enabled so that sub-sonic frequency sample can be collected.
 * ------------------------------------------------------------------- */
void RTC_C_IRQHandler(void)
{
    uint_fast8_t getRTCStatus;

    /* Read the RTC interrupt status */
    getRTCStatus = MAP_RTC_C_getEnabledInterruptStatus();

    /* Clear the interrupt status for alarm and timer-0 interrupt */
    MAP_RTC_C_clearInterruptFlag(RTC_C_CLOCK_ALARM_INTERRUPT | RTC_C_PRESCALE_TIMER0_INTERRUPT);

    /* If only the alarm interrupt is set then the set the timer-0 interrupt for
     * sub sonic sampling. */
    if((getRTCStatus & RTC_C_CLOCK_ALARM_INTERRUPT) == RTC_C_CLOCK_ALARM_INTERRUPT)
    {
        MAP_RTC_C_enableInterrupt(RTC_C_PRESCALE_TIMER0_INTERRUPT);
    }

    /* If the number of sub-sonic samples are acquired then disable the timer-0
     * interrupt, clear the array index and set the FFT flag as true. */
    if(ui16SSFArrayIndex == SSF_SAMPLES)
    {
        MAP_RTC_C_disableInterrupt(RTC_C_PRESCALE_TIMER0_INTERRUPT);
        ui16SSFArrayIndex = 0;
        bProcessFFT    = true;
    }
    else
    {
        /* Start the Conversion for sub-sonic samples */
        MAP_ADC14_enableConversion();
        MAP_ADC14_toggleConversionTrigger();
    }

}

/* -------------------------------------------------------------------
 * DMA interrupt handler which indicates to the CPU that the ultra-
 * sonic and the audio-band sampling has been completed and the buffer
 * are ready for FFT processing.
 * ------------------------------------------------------------------- */
void DMA_INT0_IRQHandler(void)
{
    if(MAP_DMA_getChannelMode(UDMA_PRI_SELECT | DMA_CH7_ADC14) == UDMA_MODE_STOP)
    {
        MAP_DMA_setChannelScatterGather(DMA_CH7_ADC14, 15, &TaskList[0], true);
    }
}

/* -------------------------------------------------------------------
 * Main Code .
 * ------------------------------------------------------------------- */
int main(void)
{
    uint16_t ui16Loop;
    //uint16_t ui16T;

    /* Initialize the Energy Bin for ultrasonic range with for 20KHz to 60KHz
     * monitor mode is set for 3000 and alarm mode for the range is set to
     * 4000 */
    USFEnergyBin.iteration    = 0;
    USFEnergyBin.lowBin       = 51;
    USFEnergyBin.hiBin        = 154;
    USFEnergyBin.monitorValue = 3000;
    USFEnergyBin.alarmValue   = 4000;

    /* Initialize the Energy Bin for audio range with for 1KHz to 4KHz
     * monitor mode is set for 2500 and alarm mode for the range is set to
     * 4500 */
    ABFEnergyBin.iteration    = 0;
    ABFEnergyBin.lowBin       = 25;
    ABFEnergyBin.hiBin        = 102;
    ABFEnergyBin.monitorValue = 2500;
    ABFEnergyBin.alarmValue   = 4500;

    /* Initialize the Energy Bin for sub-sonic range with for 30Hz to 180Hz
     * monitor mode is set for 3200 and alarm mode for the range is set to
     * 4800 */
    SSFEnergyBin.iteration    = 0;
    SSFEnergyBin.lowBin       = 30;
    SSFEnergyBin.hiBin        = 180;
    SSFEnergyBin.monitorValue = 3000;
    SSFEnergyBin.alarmValue   = 4000;

    /* Halting WDT  */
    MAP_WDT_A_holdTimer();

    /* Enable Rude mode LPM3.5 Transition */
    MAP_PCM_enableRudeMode();

    /* Setting reference voltage to 2.5  and enabling reference */
    MAP_REF_A_setReferenceVoltage(REF_A_VREF2_5V);
    MAP_REF_A_enableReferenceVoltage();

    /* Configuring GPIO for Analog In */
#if !(POWER_MODE_OPT)
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2);
#endif

    /*
     * Initialize all outputs (except JTAG) for power consumption
     */
#if POWER_MODE_OPT
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
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Turn off PSS high-side supervisors */
    MAP_PSS_disableHighSide();
#endif

    /* Set up the GPIO Port P5.5 (A0), P5.4 (A1) and P4.7 (A6) for analog
     * sampling */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
            GPIO_PIN5, GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
            GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
            GPIO_PIN7, GPIO_TERTIARY_MODULE_FUNCTION);

    /* Initializing ADC (ADC_CLOCKSOURCE_HSMCLK/1/2) */
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_HSMCLK, ADC_PREDIVIDER_1,
                         ADC_DIVIDER_2, 0);
    MAP_ADC14_setSampleHoldTime(ADC_PULSE_WIDTH_16, ADC_PULSE_WIDTH_16);

    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM2) with internal 2.5v
     * reference. ADC_MEM0 (A0) is for Low Frequency, ADC_MEM1 (A1) is
     * for audio-band and ADC_MEM2 (A2) is for ultra-sonic band. */
    MAP_ADC14_setResolution(ADC_14BIT);
    MAP_ADC14_setResultFormat(ADC_UNSIGNED_BINARY);
    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);

    MAP_ADC14_configureConversionMemory(ADC_MEM0,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A0, false);
    MAP_ADC14_configureConversionMemory(ADC_MEM1,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A1, false);
    MAP_ADC14_configureConversionMemory(ADC_MEM2,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A6, false);

    /* Enabling the interrupt when a conversion on channel is complete. */
    MAP_ADC14_enableInterrupt(ADC_INT0);
    MAP_Interrupt_enableInterrupt(INT_ADC14);

    /* Set up the conversion for manual conversion */
    MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

    /* Configuring DMA module */
    MAP_DMA_enableModule();
    MAP_DMA_setControlBase(controlTable);

    /* Assigning Channel 7 to ADC14 and enabling channel 7*/
    MAP_DMA_assignChannel(DMA_CH7_ADC14);

    /* Disabling channel attributes */
    MAP_DMA_disableChannelAttribute(DMA_CH7_ADC14,
                                     UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST |
                                     UDMA_ATTR_HIGH_PRIORITY |
                                     UDMA_ATTR_REQMASK);
    MAP_DMA_enableChannelAttribute(DMA_CH7_ADC14, UDMA_ATTR_USEBURST);

    /* Set the DMAs primary channel for peripheral scatter gather mode */
    MAP_DMA_setChannelScatterGather(DMA_CH7_ADC14, 15, &TaskList[0], true);

    /* Assigning/Enabling Interrupts */
    MAP_DMA_assignInterrupt(DMA_INT0, 7);
    MAP_Interrupt_enableInterrupt(INT_DMA_INT0);

    /* Initializing RTC with current time as described in time in
     * definitions section */
    MAP_RTC_C_initCalendar(&currentTime, RTC_C_FORMAT_BINARY);

    /* Configure the RTC timer interval for 512 Hz sampling. In the beginning
     * only the alarm match will be used */
    MAP_RTC_C_setPrescaleValue(RTC_C_PRESCALE_0, 0);
    MAP_RTC_C_definePrescaleEvent(RTC_C_PRESCALE_0, RTC_C_PSEVENTDIVIDER_64);
    MAP_RTC_C_clearInterruptFlag(RTC_C_CLOCK_ALARM_INTERRUPT | RTC_C_PRESCALE_TIMER0_INTERRUPT);
    MAP_RTC_C_enableInterrupt(RTC_C_CLOCK_ALARM_INTERRUPT);
    MAP_RTC_C_startClock();

    /* Setup Calendar Alarm for every 10 minutes */
    predictive_maintenance_setWakeTime(false);

    /* Enable interrupts from RTC Module. */
    MAP_Interrupt_enableInterrupt(INT_RTC_C);

    /* Enable interrupt processing in the NVIC and disable auto-sleep on
     * exit from sleep mode */
    MAP_Interrupt_enableMaster();
    MAP_Interrupt_disableSleepOnIsrExit();

    /* Configuring Timer_A in continuous mode and sourced from ACLK */
    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upModeConfig);
    /* Configuring Timer_A0 in CCR1 to trigger at 120 (400 KHz) */
    MAP_Timer_A_initCompare(TIMER_A0_BASE, &compareConfig);

    /* Application Loop */
    while (1)
    {
        /* CPU goes to LPM3.5 after the SSF Conversion */
        MAP_PCM_gotoLPM3();

        /* If the sub-sonic conversion is completed or if the flag for
         * processing the FFT is set then break the while loop */
        while(!(bSSFDataAcquired) && !(bProcessFFT));
        bSSFDataAcquired = false;

        if(bProcessFFT)
        {
            /* Increment the averaging counter */
            avgWindowCounter++;

            /* Clear the flag for processing the FFT */
            bProcessFFT = false;

            /* Disable the interrupt from ADC14 in the NVIC as
             * the DMA conversion will trigger the CPU */
            MAP_Interrupt_disableInterrupt(INT_ADC14);

            /* Reconfigure the ADC to read data from the ADC channel 1 for
             * the ultrasonic and the audio band */
            MAP_ADC14_disableConversion();
            MAP_ADC14_configureSingleSampleMode(ADC_MEM2, true);

            /* Enable the DMA channel for the ADC14 */
            MAP_DMA_enableChannel(7);

            /* Set the trigger source as Timer-A0 and enable the ADC14 */
            MAP_ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false);
            MAP_ADC14_enableConversion();

#if SSF_HANN_WINDOW
            for(ui16T = 0; ui16T < SSF_SAMPLES; ui16T++)
            {
                float32_t f32Val = arm_cos_f32((float32_t)(ui16T*44)/(7*SSF_SAMPLES));
                f32Val = 0.5-(0.5*f32Val);
                ui32SSFArray[ui16T] = ui32SSFArray[ui16T] * f32Val;
            }
#endif

            /* Start the timer for the utlrasonic and the audio band data
             * acquisition */
            MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

#if !(POWER_MODE_OPT)
            MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0);
#endif
            /* CPU now performs the SSF-FFT calculation */
            arm_cfft_q15(&arm_cfft_sR_q15_len512, (q15_t *)ui32SSFArray, ifftFlag, doBitReverse);

            /* Process the data through the Complex Magnitude Module for
             * calculating the magnitude at each bin */
            arm_cmplx_mag_q15((q15_t *)ui32SSFArray, (q15_t *)fftOutput, SSF_SAMPLES);

#if !(POWER_MODE_OPT)
            MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN2);
#endif
            /* Increment the Energy Bin Iteration Counter */
            SSFEnergyBin.iteration++;

            /* Store the DC Value */
            SSFEnergyBin.energyValue[0] += fftOutput[0];
            for(ui16Loop=1; ui16Loop<(SSF_SAMPLES/2); ui16Loop++)
            {
                /* Perform the FFT Bin conversion for +ve and -ve frequency
                 * wrapping to +ve frequency */
                SSFEnergyBin.energyValue[ui16Loop] += 2*fftOutput[ui16Loop];

                /* If the energy index in the range of bin exceeds the monitor
                 * mode value then set the monitor mode flag */
                if((ui16Loop >= SSFEnergyBin.lowBin) && (ui16Loop <= SSFEnergyBin.hiBin))
                {
                    if((SSFEnergyBin.energyValue[ui16Loop]/SSFEnergyBin.iteration) >
                        SSFEnergyBin.monitorValue)
                    {
                        setMonitorMode = true;
                    }
                }

                /* If the SSF alarm value is crossed then light up the RED LED */
                if(2*fftOutput[ui16Loop] > SSFEnergyBin.alarmValue)
                {
                    setAlarmLED |= GPIO_PIN0;
                }
            }

#if !(POWER_MODE_OPT)
            MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2);
#endif

            /* CPU goes to LPM0 state waiting for ADC-DMA to complete */
            MAP_PCM_gotoLPM0();

            /* Clear any pending interrupt from the ADC14 that may have
             * been generated due to the DMA operation of the ADC before
             * re-enabling the ADC14 interrupt */
            MAP_Interrupt_unpendInterrupt(INT_ADC14);
            MAP_Interrupt_enableInterrupt(INT_ADC14);

#if USF_HANN_WINDOW
            for(ui16T = 0; ui16T < USF_SAMPLES; ui16T++)
            {
                float32_t f32Val = arm_cos_f32((float32_t)(ui16T*44)/(7*USF_SAMPLES));
                f32Val = 0.5-(0.5*f32Val);
                ui32USFArray[ui16T] = ui32USFArray[ui16T] * f32Val;
            }
#endif

            #if !(POWER_MODE_OPT)
            MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0);
#endif
            /* CPU now performs the USF-FFT calculation */
            arm_cfft_q15(&arm_cfft_sR_q15_len1024, (q15_t *)ui32USFArray, ifftFlag, doBitReverse);

            /* Process the data through the Complex Magnitude Module for
             * calculating the magnitude at each bin */
            arm_cmplx_mag_q15((q15_t *)ui32USFArray, (q15_t *)fftOutput, USF_SAMPLES);

#if !(POWER_MODE_OPT)
            MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN2);
#endif
            /* Increment the Energy Bin Iteration Counter */
            USFEnergyBin.iteration++;

            /* Store the DC Value */
            USFEnergyBin.energyValue[0] += fftOutput[0];
            for(ui16Loop=1; ui16Loop<(USF_SAMPLES/2); ui16Loop++)
            {
                /* Perform the FFT Bin conversion for +ve and -ve frequency
                 * wrapping to +ve frequency */
                USFEnergyBin.energyValue[ui16Loop] += 2*fftOutput[ui16Loop];

                /* If the energy index in the range of bin exceeds the monitor
                 * mode value then set the monitor mode flag */
                if((ui16Loop >= USFEnergyBin.lowBin) && (ui16Loop <= USFEnergyBin.hiBin))
                {
                    if((USFEnergyBin.energyValue[ui16Loop]/USFEnergyBin.iteration) >
                    USFEnergyBin.monitorValue)
                    {
                        setMonitorMode = true;
                    }
                }

                /* If the USF alarm value is crossed then light up the BLUE LED */
                if(2*fftOutput[ui16Loop] > USFEnergyBin.alarmValue)
                {
                    setAlarmLED |= GPIO_PIN2;
                }
            }
#if !(POWER_MODE_OPT)
            MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2);
#endif

#if !(POWER_MODE_OPT)
            MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0);
#endif
#if ABF_HANN_WINDOW
            for(ui16T = 0; ui16T < ABF_SAMPLES; ui16T++)
            {
                float32_t f32Val = arm_cos_f32((float32_t)(ui16T*44)/(7*ABF_SAMPLES));
                f32Val = 0.5-(0.5*f32Val);
                ui32ABFArray[ui16T] = ui32ABFArray[ui16T] * f32Val;
            }
#endif
            /* CPU now performs the ABF-FFT calculation */
            arm_cfft_q15(&arm_cfft_sR_q15_len1024, (q15_t *)ui32ABFArray, ifftFlag, doBitReverse);

            /* Process the data through the Complex Magnitude Module for
             * calculating the magnitude at each bin */
            arm_cmplx_mag_q15((q15_t *)ui32ABFArray, (q15_t *)fftOutput, ABF_SAMPLES);

#if !(POWER_MODE_OPT)
            MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN2);
#endif
            /* Increment the Energy Bin Iteration Counter */
            ABFEnergyBin.iteration++;

            /* Store the DC Value */
            ABFEnergyBin.energyValue[0] += fftOutput[0];
            for(ui16Loop=1; ui16Loop<(ABF_SAMPLES/2); ui16Loop++)
            {
                /* Perform the FFT Bin conversion for +ve and -ve frequency
                 * wrapping to +ve frequency */
                ABFEnergyBin.energyValue[ui16Loop] += 2*fftOutput[ui16Loop];

                /* If the energy index in the range of bin exceeds the monitor
                 * mode value then set the monitor mode flag */
                if((ui16Loop >= ABFEnergyBin.lowBin) && (ui16Loop <= ABFEnergyBin.hiBin))
                {
                    if((ABFEnergyBin.energyValue[ui16Loop]/ABFEnergyBin.iteration) >
                        ABFEnergyBin.monitorValue)
                    {
                        setMonitorMode = true;
                    }
                }

                /* If the ABF alarm value is crossed then light up the GREEN LED */
                if(2*fftOutput[ui16Loop] > ABFEnergyBin.alarmValue)
                {
                    setAlarmLED |= GPIO_PIN1;
                }
            }
#if !(POWER_MODE_OPT)
            MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2);
#endif

            /* If the averaging window is completed then start processing of
             * the FFT data */
            if(avgWindowCounter == AVG_WINDOW)
            {
                /* Clear the averaging window counter */
                avgWindowCounter = 0;

                /* If the monitor mode is set or if the alarm mode is set
                 * then set the monitor count to 5. The monitor mode or the
                 * alarm must not be set in the next 5 sampling windows to
                 * exit the quick sampling */
                if((setMonitorMode == true) || (setAlarmLED != 0x0))
                {
                    setMonitorMode  = false;
                    incMonitorCount = 5;
                }
                else
                {
                    /* Decrement the monitor count till it does not reach
                     * zero.*/
                    if(incMonitorCount != 0)
                    {
                        incMonitorCount--;
                    }
                }

                /* If the alarm is set then lit up the LED for the appropriate
                 * FFT analysis. if no alarm is set then clear the LED's */
                MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2,
                                           (SSF_ALARM_PIN | ABF_ALARM_PIN | USF_ALARM_PIN));
                if(setAlarmLED != 0)
                {
                    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, setAlarmLED);
                }

                /* Clear the energy bin structures by clearing the iteration
                 * count and the energy values being stored */
                USFEnergyBin.iteration = 0;
                for(ui16Loop=0; ui16Loop<(USF_SAMPLES/2); ui16Loop++)
                {
                    USFEnergyBin.energyValue[ui16Loop] = 0;
                }

                ABFEnergyBin.iteration = 0;
                for(ui16Loop=0; ui16Loop<(ABF_SAMPLES/2); ui16Loop++)
                {
                    ABFEnergyBin.energyValue[ui16Loop] = 0;
                }

                SSFEnergyBin.iteration = 0;
                for(ui16Loop=0; ui16Loop<(SSF_SAMPLES/2); ui16Loop++)
                {
                    SSFEnergyBin.energyValue[ui16Loop] = 0;
                }

                /* clear the alarm LED */
                setAlarmLED = 0;

                /* If the monitor count is not 0 then set the next wake-up
                 * period for 1 minute else set it up for 10 minutes */
                if(incMonitorCount != 0)
                {
                    predictive_maintenance_setWakeTime(true);
                }
                else
                {
                    predictive_maintenance_setWakeTime(false);
                }

            }
            else
            {
                /* If the averaging window conversion is not complete then
                 * clear the interrupt flag from the RTC periodic timer and
                 * enable the RTC periodic timer for the next set of samples */
                MAP_RTC_C_clearInterruptFlag(RTC_C_PRESCALE_TIMER0_INTERRUPT);
                MAP_RTC_C_enableInterrupt(RTC_C_PRESCALE_TIMER0_INTERRUPT);
            }
        }
    }
}
