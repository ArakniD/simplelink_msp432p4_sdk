/* --COPYRIGHT--,BSD,BSD
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
/*******************************************************************************
 * MSP432 DMA - Scatter-Gather between eUSCIB2 memory and eUSCIB1
 *
 * Description: In this code example, two software initiated DMA transfers are
 * setup and executed.  The scatter-gather transfers initiated from software
 * perform two tasks.  The first task is to read three bytes from the eUSCIB2
 * SPI and the second task is to transmit two bytes from the eUSCIB1 SPI.
 * In addition to the SPI communication the tasks also manage the chip select
 * via the port registers.
 * During the first interrupt a simple computation is performed to transfer
 * the information read to the information transmitted.
 *
 *              MSP432P4111
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST           P3.0|--> CS
 *            |     P3.5(eUSCIB2)|--> CLK
 *            |              P3.6|--> SIMO
 *            |              P3.7|<-- SOMI
 *            |                  |
 *            |              P6.1|--> CS
 *            |     P6.3(eUSCIB1)|--> CLK
 *            |              P6.4|--> SIMO
 *            |              P6.5|<-- SOMI
 *
 ******************************************************************************/
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/inc/msp.h>

#define RX_TASKS    9
#define TX_TASKS    7

/* SPI Configuration Parameter */
const eUSCI_SPI_MasterConfig spiMasterConfig_B2 =
{
    EUSCI_B_SPI_CLOCKSOURCE_SMCLK, 3000000, 1000000,
    EUSCI_B_SPI_MSB_FIRST,
    EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT,
    EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH,
    EUSCI_B_SPI_3PIN
};

/* SPI Configuration Parameter */
const eUSCI_SPI_MasterConfig spiMasterConfig_B1 =
{
    EUSCI_B_SPI_CLOCKSOURCE_SMCLK, 3000000, 1000000,
    EUSCI_B_SPI_MSB_FIRST,
    EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT,
    EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, EUSCI_B_SPI_3PIN
};

/* DMA Control Table */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(MSP_EXP432P401RLP_DMAControlTable, 1024)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=1024
#elif defined(__GNUC__)
__attribute__ ((aligned (1024)))
#elif defined(__CC_ARM)
__align(1024)
#endif
static DMA_ControlTable MSP_EXP432P401RLP_DMAControlTable[16];  // 8 primary and 8 alternate

uint8_t outputLow = 0;
uint8_t ADC_CS = BIT0;  /* P3.0 is Chip Select */
uint8_t DAC_CS = BIT1;  /* P6.1 is Chip Select */
uint8_t dummyTX = 0xa5;
uint8_t dummyRX;
uint8_t rxBuffer[3];
uint8_t txBuffer[2];

/*
 * ADC_DMA Sequence: This sequence sets CS, reads data, and clears CS
 * The 'MEM' instructions are automatic while the 'PER' requires a
 * peripheral trigger, which is the EUSCI_B2_RX.  The trigger is configured
 * before the sequence is started.
 */
DMA_ControlTable spiAdcDmaSeq[RX_TASKS] =
{
    /* Task1, Dummy read RX buffer to clear IFG */
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
                UDMA_SRC_INC_NONE, &EUSCI_B2_SPI->RXBUF,
                UDMA_DST_INC_NONE, &dummyRX,
                UDMA_ARB_4, (UDMA_MODE_MEM_SCATTER_GATHER+UDMA_MODE_ALT_SELECT)),
    /* Task2, Clear Chip select */
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
                UDMA_SRC_INC_NONE, &outputLow,
                UDMA_DST_INC_NONE, &P3->OUT,
                UDMA_ARB_4, (UDMA_MODE_MEM_SCATTER_GATHER+UDMA_MODE_ALT_SELECT)),
    /* Task3, load TX buffer to initiate SPI */
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
                UDMA_SRC_INC_NONE, &dummyTX,
                UDMA_DST_INC_NONE, &EUSCI_B2_SPI->TXBUF,
                UDMA_ARB_4, (UDMA_MODE_PER_SCATTER_GATHER+UDMA_MODE_ALT_SELECT)),
    /* Task4, read RX buffer to clear IFG */
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
                UDMA_SRC_INC_NONE, &EUSCI_B2_SPI->RXBUF,
                UDMA_DST_INC_NONE, &rxBuffer[0],
                UDMA_ARB_4, (UDMA_MODE_MEM_SCATTER_GATHER+UDMA_MODE_ALT_SELECT)),
    /* Task5, load TX buffer to initiate SPI */
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
                UDMA_SRC_INC_NONE, &dummyTX,
                UDMA_DST_INC_NONE, &EUSCI_B2_SPI->TXBUF,
                UDMA_ARB_4, (UDMA_MODE_PER_SCATTER_GATHER+UDMA_MODE_ALT_SELECT)),
    /* Task6, read RX buffer to initiate SPI */
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
                UDMA_SRC_INC_NONE, &EUSCI_B2_SPI->RXBUF,
                UDMA_DST_INC_NONE, &rxBuffer[1],
                UDMA_ARB_4, (UDMA_MODE_MEM_SCATTER_GATHER+UDMA_MODE_ALT_SELECT)),
    /* Task7, load TX buffer to initiate SPI */
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
                UDMA_SRC_INC_NONE, &dummyTX,
                UDMA_DST_INC_NONE, &EUSCI_B2_SPI->TXBUF,
                UDMA_ARB_4, (UDMA_MODE_PER_SCATTER_GATHER+UDMA_MODE_ALT_SELECT)),
    /* Task8, read RX buffer to initiate SPI */
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
                UDMA_SRC_INC_NONE, &EUSCI_B2_SPI->RXBUF,
                UDMA_DST_INC_NONE, &rxBuffer[2],
                UDMA_ARB_4, (UDMA_MODE_MEM_SCATTER_GATHER+UDMA_MODE_ALT_SELECT)),
    /* Task 9, set GPIO, chip select */
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
                UDMA_SRC_INC_NONE, &ADC_CS,
                UDMA_DST_INC_NONE, &P3->OUT,
                UDMA_ARB_4, UDMA_MODE_BASIC)
};

/*
 * DAC_DMA Sequence: This sequence sets CS, reads data, and clears CS
 * The 'MEM' instructions are automatic while the 'PER' requires a
 * peripheral trigger, which is the EUSCI_B1_TX. The trigger is configured
 * before the sequence is started.
 */
DMA_ControlTable spiDacDmaSeq[TX_TASKS] =
{
    /* Task1, Dummy read RX buffer to clear IFG */
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
                UDMA_SRC_INC_NONE, &EUSCI_B1_SPI->RXBUF,
                UDMA_DST_INC_NONE, &dummyRX,
                UDMA_ARB_4, (UDMA_MODE_MEM_SCATTER_GATHER+UDMA_MODE_ALT_SELECT)),
    /* Task2, Clear Chip select P6.2 */
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
                UDMA_SRC_INC_NONE, &outputLow,
                UDMA_DST_INC_NONE, &P6->OUT,
                UDMA_ARB_4, (UDMA_MODE_MEM_SCATTER_GATHER+UDMA_MODE_ALT_SELECT)),
    /* Task3, load TX buffer to initiate SPI */
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
                UDMA_SRC_INC_NONE, &txBuffer[0],
                UDMA_DST_INC_NONE, &EUSCI_B1_SPI->TXBUF,
                UDMA_ARB_4, (UDMA_MODE_PER_SCATTER_GATHER+UDMA_MODE_ALT_SELECT)),
    /* Task4, Dummy read RX buffer to clear IFG, wait for RXIFG trigger */
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
                UDMA_SRC_INC_NONE, &EUSCI_B1_SPI->RXBUF,
                UDMA_DST_INC_NONE, &dummyRX,
                UDMA_ARB_4, (UDMA_MODE_MEM_SCATTER_GATHER+UDMA_MODE_ALT_SELECT)),
    /* Task 5, load TX buffer to initiate SPI */
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
                UDMA_SRC_INC_NONE, &txBuffer[1],
                UDMA_DST_INC_NONE, &EUSCI_B1_SPI->TXBUF,
                UDMA_ARB_4, (UDMA_MODE_PER_SCATTER_GATHER+UDMA_MODE_ALT_SELECT)),
    /*
     * Task 6, Dummy read RX buffer to clear IFG
     * This delay ensures that the chip select happens after the last byte is transmitted
     */
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
                UDMA_SRC_INC_NONE, &EUSCI_B1_SPI->RXBUF,
                UDMA_DST_INC_NONE, &dummyRX,
                UDMA_ARB_4, (UDMA_MODE_MEM_SCATTER_GATHER+UDMA_MODE_ALT_SELECT)),
    /* Task 7, set GPIO, chip select */
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
                UDMA_SRC_INC_NONE, &DAC_CS,
                UDMA_DST_INC_NONE, &P6->OUT,
                UDMA_ARB_4, UDMA_MODE_BASIC)
};

int main(void)
{
    /* Halt watchdog timer */
    MAP_WDT_A_holdTimer();
    /*
     * Configuring P1.1 as an input and enabling interrupts
     */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    MAP_Interrupt_enableInterrupt(INT_PORT1);

    /*
     * DAC SPI
     * Configure CLK, MOSI & MISO for SPI0 (EUSCI_B1)
     * P6.3 UCB1CLK
     * P6.4 SIMO
     * P6.5 SOMI
     *
     */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6,
            GPIO_PIN3 | GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
            GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);
    /*
     * ADC SPI
     * Configure CLK, MOSI & MISO for SPI0 (EUSCI_B2)
     * 3.5 CLK
     * 3.6 SIMO
     * 3.7 SOMI
     */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3,
            GPIO_PIN5 | GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
            GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configure CS pin P3.0 */
    P3->OUT |= BIT0;
    P3->DIR |= BIT0;
    /* Configure CS pin P6.1 */
    P6->OUT |= BIT1;
    P6->DIR |= BIT1;

    /* Debug Port P1.0       */
    P1->DIR |= BIT0;

    /*
     * LDO, Vcore0 (default)
     * 24Mhz MCLK,
     * 3Mhz SMCLK
     */

    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);
    MAP_CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_8);

    /* Configuring SPI module */
    MAP_SPI_initMaster(EUSCI_B2_BASE, &spiMasterConfig_B2);
    MAP_SPI_initMaster(EUSCI_B1_BASE, &spiMasterConfig_B1);

    /* Enable the SPI module */
    MAP_SPI_enableModule(EUSCI_B2_BASE);
    MAP_SPI_enableModule(EUSCI_B1_BASE);

    MAP_SPI_clearInterruptFlag(EUSCI_B2_BASE,EUSCI_B_SPI_RECEIVE_INTERRUPT);
    MAP_SPI_clearInterruptFlag(EUSCI_B1_BASE,EUSCI_B_SPI_RECEIVE_INTERRUPT);

    /* Configuring DMA module */
    MAP_DMA_enableModule();
    MAP_DMA_setControlBase(MSP_EXP432P401RLP_DMAControlTable);

    /*
     * Assign DMA triggers
     * channel 2 to EUSCI_B1_TX0
     * channel 5 to EUSCI_B2_RX0
     */

    MAP_DMA_assignChannel(DMA_CH3_EUSCIB1RX0);

    MAP_DMA_assignChannel(DMA_CH5_EUSCIB2RX0);

    /* Enable DMA interrupt */
    /*
     * Debug
     * Change to Channel1 eUSCI_B2_RX2
     */
    MAP_DMA_assignInterrupt(INT_DMA_INT1, 5);  /* Channel 5 is the ADC (RX) */
    MAP_DMA_assignInterrupt(INT_DMA_INT2, 3);  /* Channel 3 is the DAC (TX) */

    while(1)
    {
        MAP_PCM_gotoLPM0();
    }
}

/* Completion interrupt for DMA */
void DMA_INT1_IRQHandler(void)
{
    MAP_DMA_disableChannel(5);
    MAP_DMA_disableInterrupt(INT_DMA_INT1);
    MAP_Interrupt_disableInterrupt(INT_DMA_INT1);
    MAP_SPI_clearInterruptFlag(EUSCI_B2_BASE,EUSCI_B_IFG_RXIFG0);
    MAP_DMA_clearInterruptFlag(5);
    /*
     * Translate ADC data to DAC data
     * 0 -> MSB (transmit first)
     * 1 -> LSB (transmit second)
     */
    txBuffer[0] = (rxBuffer[0] & 0x3) << 6;
    txBuffer[0] |= (rxBuffer[1] & 0xFC) >> 2;
    txBuffer[1] = (rxBuffer[1] & 0x3) << 6;
    txBuffer[1] |= (rxBuffer[2] & 0xFC) >> 2;

    MAP_SPI_clearInterruptFlag(EUSCI_B1_BASE,EUSCI_B_SPI_RECEIVE_INTERRUPT);
    MAP_DMA_setChannelScatterGather(DMA_CH3_EUSCIB1RX0,TX_TASKS,(void*)&spiDacDmaSeq[0],0);
    MAP_DMA_clearInterruptFlag(DMA_CH3_EUSCIB1RX0 & 0x0F);
    /* Assigning/Enabling Interrupts */
    MAP_DMA_enableInterrupt(INT_DMA_INT2);
    MAP_Interrupt_enableInterrupt(INT_DMA_INT2);
    MAP_DMA_enableChannel(3);
    EUSCI_B1_SPI->IFG |= EUSCI_B_IFG_RXIFG0;
}

void DMA_INT2_IRQHandler(void)
{
    MAP_DMA_disableChannel(3);
    MAP_DMA_disableInterrupt(INT_DMA_INT2);
    MAP_Interrupt_disableInterrupt(INT_DMA_INT2);
    MAP_SPI_clearInterruptFlag(EUSCI_B1_BASE,EUSCI_B_IFG_RXIFG0);
    MAP_DMA_clearInterruptFlag(3);
    P1->OUT ^= BIT0;
}

void PORT1_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    /* Toggling the output on the LED */
    if(status & GPIO_PIN1)
    {
        rxBuffer[0] = 0;
        rxBuffer[1] = 0;
        rxBuffer[2] = 0;

        txBuffer[0] = 0;
        txBuffer[1] = 0;

        /*
         * Start SPI read
         */
        while(status & GPIO_PIN1)
        {
            status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
            MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
        }
        /*
         * Peripheral trigger.  Memory Scatter Gather
         */
        MAP_DMA_setChannelScatterGather(DMA_CH5_EUSCIB2RX0,RX_TASKS,(void*)&spiAdcDmaSeq[0],0);
        MAP_DMA_clearInterruptFlag(DMA_CH5_EUSCIB2RX0 & 0x0F);

        /* Assigning/Enabling Interrupts */
        MAP_DMA_enableInterrupt(INT_DMA_INT1);
        MAP_Interrupt_enableInterrupt(INT_DMA_INT1);
        MAP_DMA_enableChannel(5);

        /*
         * The DMA channel5 is set to trigger off of theEUSCIB0RX IFG.  This
         * did not really happen but is forced manually to start the
         * scatter-gather sequence.  Because the setting is 'memory'
         * scatter-gather all triggers (transitions) from primary to
         * auxiliary actions will be automatic.  The auxiliary action triggers
         * are defined in the task list.
         */
        EUSCI_B2_SPI->IFG |= EUSCI_B_IFG_RXIFG0;
    }
}
