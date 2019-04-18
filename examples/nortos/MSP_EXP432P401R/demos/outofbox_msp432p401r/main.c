//*****************************************************************************
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  Redistributions of source code must retain the above copyright
//  notice, this list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright
//  notice, this list of conditions and the following disclaimer in the
//  documentation and/or other materials provided with the
//  distribution.
//
//  Neither the name of Texas Instruments Incorporated nor the names of
//  its contributors may be used to endorse or promote products derived
//  from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// main.c - MSP-EXP432P401R Out Of Box Demo
//
//          Blinks RGB LED at 1Hz. Button S1 allows taps-to-beat that will
//          match the RGB blink frequency to the tap frequency. Button S2
//          cycles through 4 different color states, R/G/B/RandomColor, each
//          with its saved blink frequency.
//
//          The demo also accepts UART data from the Out Of Box GUI that
//          allows changing the RGB LED's color with a color wheel and
//          frequency by entering desired beats-per-minute
//
//****************************************************************************

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <time.h>
#include <stdlib.h>

#define MCLK_FREQUENCY 3000000
#define PWM_PERIOD (MCLK_FREQUENCY/5000)

unsigned int S1buttonDebounce = 0; // Deboounce state for button S1
unsigned int S2buttonDebounce = 0; // Deboounce state for button S1

int32_t interval = 0;       // Elapsed interval between taps
int32_t periods[4];         // LED blink periods for each color state
int oldTick = 0;            // SysTick value of previous tap
int newTick = 0;            // SysTick value of current tap

int counting = 0;           // Whether currently counting taps and interval between taps
int count = 0;              // Counts # of seconds elapsed since last tap
int sysTickCount = 0;       // Counts # of SysTick interrupts since last tap
int taps = 0;               // Counts # of taps from last reset

/* Current color of the blinking RGB LED
 * 4 possible states: R, G, B, random color */
unsigned int currentColor = 0;

#define RED     255
#define GREEN   255
#define BLUE    255

int bytesReceived = 7;      // Keeps track of # UART bytes received
int32_t receiveFreq = 0;    // Receives LED blink frequency from GUI
char start[5];              // Receives 'START' keyword
int ack = 0;                // Track acknowledgement sent to GUI

/* TimerA UpMode Configuration Parameter */
const Timer_A_UpModeConfig upConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,          // SMCLK/1 = 3MHz
        45000,                                  // 15ms debounce period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};

const Timer_A_UpModeConfig TA2upConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_64,         // SMCLK/64 ~ 46.9 kMHz
        46875,                                  // 1s timer period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};

/* Port mapper configuration register */
const uint8_t port_mapping[] =
{
    //Port P2:
    PM_TA0CCR1A, PM_TA0CCR2A, PM_TA0CCR3A, PM_NONE, PM_TA1CCR1A, PM_NONE, PM_NONE, PM_NONE
};

/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 115200 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 *http://processors.wiki.ti.com/index.php/USCI_UART_Baud_Rate_Gen_Mode_Selection
 */
const eUSCI_UART_Config uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        26,                                      // BRDIV = 26
        0,                                       // UCxBRF = 0
        0,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // MSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION  // Low Frequency Mode
};


/*
 * Main function
 */
int main(void)
{
    /* Halting WDT and disabling master interrupts */
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_disableMaster();

    /* Seed the pseudo random num generator */
    srand(TLV->RANDOM_NUM_1);

    /* Set the core voltage level to VCORE1 */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Default SysTick period for all 4 color states = 0.5s */
    periods[0] = 1500000;
    periods[1] = 1500000;
    periods[2] = 1500000;
    periods[3] = 1500000;

    /* Initialize main clock to 3MHz */
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_3);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );

    /* Selecting P1.2 and P1.3 in UART mode and P1.0 as output (LED) */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
        GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* GPIO Setup for Pins 2.0-2.2 */
    MAP_PMAP_configurePorts((const uint8_t *) port_mapping, PMAP_P2MAP, 1,
        PMAP_DISABLE_RECONFIGURATION);

    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2,
        GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Confinguring P1.1 & P1.4 as an input and enabling interrupts */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4, GPIO_HIGH_TO_LOW_TRANSITION);

    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);

    /* Configure TimerA0 without Driverlib (CMSIS style register access) */
    TIMER_A0->CCR[0] = PWM_PERIOD;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;                      // CCR1 reset/set
    TIMER_A0->CCR[1] = PWM_PERIOD * (RED/255);         // CCR1 PWM duty cycle
    TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7;                      // CCR2 reset/set
    TIMER_A0->CCR[2] = PWM_PERIOD * (0/255);           // CCR2 PWM duty cycle
    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;                      // CCR3 reset/set
    TIMER_A0->CCR[3] = PWM_PERIOD * (0/255);           // CCR3 PWM duty cycle
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;  // SMCLK, up mode, clear TAR

    /* Configuring TimerA1 and TimerA2 for Up Mode  using Driverlib*/
    MAP_Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);
    MAP_Timer_A_configureUpMode(TIMER_A2_BASE, &TA2upConfig);

    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Configure and enable SysTick */
    MAP_SysTick_setPeriod(1500000);
    MAP_SysTick_enableModule();
    MAP_SysTick_enableInterrupt();

    /* Main while loop */
    while(1)
    {
        MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);
        MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
        MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
        MAP_Interrupt_enableInterrupt(INT_PORT1);
        MAP_Interrupt_enableInterrupt(INT_TA1_0);
        MAP_Interrupt_enableInterrupt(INT_TA2_0);
        MAP_Interrupt_enableMaster();
        MAP_PCM_gotoLPM0();
    }
}

/*
 * Port 1 interrupt handler. This handler is called whenever switches attached
 * to P1.1 (S1) and P1.4 (S2) are pressed.
 */
void PORT1_IRQHandler(void)
{
    newTick = MAP_SysTick_getValue();
    uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    /* Handles S1 button press */
    if (status & GPIO_PIN1)
    {
        if (S1buttonDebounce == 0)
        {
            S1buttonDebounce = 1;

            MAP_Interrupt_disableInterrupt(INT_PORT1);

            MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);

            MAP_Timer_A_stopTimer(TIMER_A2_BASE);
            MAP_Timer_A_configureUpMode(TIMER_A2_BASE, &TA2upConfig);
            MAP_Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);

            count = 0;
            taps++;

            if (counting == 0)
            {
                oldTick = newTick;
                counting = 1;
            }
            else
            {
                if (sysTickCount == 0)
                {
                    interval = oldTick - newTick;
                    periods[currentColor] = interval/2;
                    oldTick = newTick;
                }
                else
                {
                    interval = (MAP_SysTick_getPeriod() - newTick) + ((sysTickCount-1) * MAP_SysTick_getPeriod()) + oldTick;
                    oldTick = newTick;
                    sysTickCount = 0;
                }
                periods[currentColor] = ((periods[currentColor] * (taps-2)) + interval/2) / (taps-1);
                MAP_SysTick_setPeriod(periods[currentColor]);
            }

            /* Start button debounce timer */
            MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
        }
    }
    /* Handles S2 button press */
    if (status & GPIO_PIN4)
    {
        if (S2buttonDebounce == 0)
        {
            S2buttonDebounce = 1;

            MAP_Interrupt_disableInterrupt(INT_PORT1);

            /* Cycle through R, G, B, random color */
            if (currentColor < 3)
                currentColor++;
            else
                currentColor = 0;

            switch(currentColor)
            {
                case 0:  // Red
                    TIMER_A0->CCR[1] = PWM_PERIOD * (RED/255);              // CCR1 PWM duty cycle
                    TIMER_A0->CCR[2] = PWM_PERIOD * (0/255);                // CCR2 PWM duty cycle
                    TIMER_A0->CCR[3] = PWM_PERIOD * (0/255);                // CCR3 PWM duty cycle
                    break;
                case 1:  // Green
                    TIMER_A0->CCR[1] = PWM_PERIOD * (0/255);                // CCR1 PWM duty cycle
                    TIMER_A0->CCR[2] = PWM_PERIOD * (GREEN/255);            // CCR2 PWM duty cycle
                    TIMER_A0->CCR[3] = PWM_PERIOD * (0/255);                // CCR3 PWM duty cycle
                    break;
                case 2:  // Blue
                    TIMER_A0->CCR[1] = PWM_PERIOD * (0/255);                // CCR1 PWM duty cycle
                    TIMER_A0->CCR[2] = PWM_PERIOD * (0/255);                // CCR2 PWM duty cycle
                    TIMER_A0->CCR[3] = PWM_PERIOD * (BLUE/255);             // CCR3 PWM duty cycle
                    break;
                case 3:  // Random Color
                    TIMER_A0->CCR[1] = (int)((rand() % 255) / 255.0f * PWM_PERIOD);      // CCR1 PWM duty cycle
                    TIMER_A0->CCR[2] = (int)((rand() % 255) / 255.0f * PWM_PERIOD);      // CCR2 PWM duty cycle
                    TIMER_A0->CCR[3] = (int)((rand() % 255) / 255.0f * PWM_PERIOD);      // CCR3 PWM duty cycle
                    break;
                default:
                    break;
            }

            /* Set current color's LED blink period */
            MAP_SysTick_setPeriod(periods[currentColor]);

            /* Start button debounce timer */
            MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
        }
    }
}

/*
 * Timer A1 interrupt handler. This handler determines whether to reset button
 * debounce after debounce timer expires.
 */
void TA1_0_IRQHandler(void)
{
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    if (P1IN & GPIO_PIN1)
    {
        S1buttonDebounce = 0;
    }
    if (P1IN & GPIO_PIN4)
    {
        S2buttonDebounce = 0;
    }

    if ((P1IN & GPIO_PIN1) && (P1IN & GPIO_PIN4))
    {
        MAP_Timer_A_stopTimer(TIMER_A1_BASE);
    }
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
                TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

/*
 * Timer A2 interrupt handler. This handler resets tapping state variables if button S1 is
 * not pressed for more than 4 seconds.
 */
void TA2_0_IRQHandler(void)
{
    if (counting == 1)
    {
        if (count < 4)
            count++;
        else
        {
            counting = 0;
            count = 0;
            sysTickCount = 0;
            taps = 0;
            MAP_Timer_A_stopTimer(TIMER_A2_BASE);
        }
    }
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE,
                TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

/*
 * SysTick interrupt handler. This handler toggles RGB LED on/off.
 */
void SysTick_Handler(void)
{
    if (counting == 1)
        sysTickCount++;

    /* Toggle RGB LED OFF */
    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_OUTMOD_7){
        TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_0;
        TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_0;
        TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_0;
    }
    /* Toggle RGB LED ON */
    else if (!(TIMER_A0->CCTL[1] & TIMER_A_CCTLN_OUTMOD_0)){
        if (currentColor == 3)
        {
            TIMER_A0->CCR[1] = (int)((rand() % 255) / 255.0f * PWM_PERIOD);                 // CCR1 PWM duty cycle
            TIMER_A0->CCR[2] = (int)((rand() % 255) / 255.0f * PWM_PERIOD);                // CCR2 PWM duty cycle
            TIMER_A0->CCR[3] = (int)((rand() % 255) / 255.0f * PWM_PERIOD);                  // CCR3 PWM duty cycle
        }
        TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
        TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7;
        TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;
    }
}

/*
 * EUSCI A0 UART interrupt handler. Receives data from GUI and sets LED color/blink frequency
 */
void EUSCIA0_IRQHandler(void)
{
    int receiveByte = UCA0RXBUF;

    /* Send acknowledgement to the GUI */
    if (ack == 0) {
        MAP_UART_transmitData(EUSCI_A0_BASE, 'A');
        ack = 1;
    }

    /* Wait for 'START' keyword to be received before changing color */
    if (bytesReceived > 6) {
        start[0] = start[1];
        start[1] = start[2];
        start[2] = start[3];
        start[3] = start[4];
        start[4] = receiveByte;

        if (start[0] == 'S' && start[1] == 'T' && start[2] == 'A' && start[3] == 'R' && start[4] == 'T')
            bytesReceived = -1;
    }

    /* Receive/Set color and frequency of LED */
    else {
        if (bytesReceived == 0)
            TIMER_A0->CCR[1] = (int)(PWM_PERIOD * (receiveByte/255.0f));                  // CCR3 PWM duty cycle
        else if (bytesReceived == 1)
            TIMER_A0->CCR[2] = (int)(PWM_PERIOD * (receiveByte/255.0f));                  // CCR3 PWM duty cycle
        else if (bytesReceived == 2)
            TIMER_A0->CCR[3] = (int)(PWM_PERIOD * (receiveByte/255.0f));                  // CCR3 PWM duty cycle
        else if (bytesReceived == 3)
        {
            receiveFreq = 0;
            receiveFreq |= receiveByte;
        }
        else if (bytesReceived == 4)
            receiveFreq |= (receiveByte << 8);
        else if (bytesReceived == 5)
            receiveFreq |= (receiveByte << 16);
        else if (bytesReceived == 6)
        {
            receiveFreq |= (receiveByte << 24);
            MAP_SysTick_setPeriod(receiveFreq);
            ack = 0;
        }
    }

    if (bytesReceived < 7)
        bytesReceived++;
}
