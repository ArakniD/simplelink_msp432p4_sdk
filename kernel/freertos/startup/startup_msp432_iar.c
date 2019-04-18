//*****************************************************************************
//
// Copyright (C) 2012-2017 Texas Instruments Incorporated - http://www.ti.com/
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
// MSP432 Family Interrupt Vector Table for GCC
//
//****************************************************************************

#include <stdint.h>

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "FreeRTOSConfig.h"


//*****************************************************************************
//
// External declaration for the reset handler that is to be called when the
// processor is started
//
//*****************************************************************************
static void nmiISR(void);
static void faultISR(void);
static void defaultHandler(void);
static void busFaultHandler(void);
extern void vPortSVCHandler(void);
extern void xPortPendSVHandler(void);
extern void xPortSysTickHandler(void);

//*****************************************************************************
//
// The entry point for the application startup code.
//
//*****************************************************************************
extern void __iar_program_start(void);


//*****************************************************************************
//
//! Get stack start (highest address) symbol from linker file.
//
//*****************************************************************************
extern const void* STACK_TOP;

// It is required to place something in the CSTACK segment to get the stack
// check feature in IAR to work as expected
__root static void* dummy_stack @ ".stack";

//*****************************************************************************
//
// A union that describes the entries of the vector table.  The union is needed
// since the first entry is the stack pointer and the remainder are function
// pointers.
//
//*****************************************************************************
typedef union
{
    void (*pfnHandler)(void);
    unsigned long ulPtr;
}
uVectorEntry;

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
#pragma location = ".intvec"
const uVectorEntry __vector_table[] =
{
    (void (*)(void))&STACK_TOP,          // The initial stack pointer
    __iar_program_start,                 // The reset handler
    nmiISR,                              // The NMI handler
    faultISR,                            // The hard fault handler
    defaultHandler,                      // The MPU fault handler
    busFaultHandler,                     // The bus fault handler
    defaultHandler,                      // The usage fault handler
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    vPortSVCHandler,                     /* SVCall handler            */
    defaultHandler,                      // Debug monitor handler
    0,                                   /* Reserved                  */
    xPortPendSVHandler,                  /* The PendSV handler        */
    xPortSysTickHandler,                 /* The SysTick handler       */
    defaultHandler,                      /* PSS ISR                   */
    defaultHandler,                      /* CS ISR                    */
    defaultHandler,                      /* PCM ISR                   */
    defaultHandler,                      /* WDT ISR                   */
    defaultHandler,                      /* FPU ISR                   */
    defaultHandler,                      /* FLCTL ISR                 */
    defaultHandler,                      /* COMP0 ISR                 */
    defaultHandler,                      /* COMP1 ISR                 */
    defaultHandler,                      /* TA0_0 ISR                 */
    defaultHandler,                      /* TA0_N ISR                 */
    defaultHandler,                      /* TA1_0 ISR                 */
    defaultHandler,                      /* TA1_N ISR                 */
    defaultHandler,                      /* TA2_0 ISR                 */
    defaultHandler,                      /* TA2_N ISR                 */
    defaultHandler,                      /* TA3_0 ISR                 */
    defaultHandler,                      /* TA3_N ISR                 */
    defaultHandler,                      /* EUSCIA0 ISR               */
    defaultHandler,                      /* EUSCIA1 ISR               */
    defaultHandler,                      /* EUSCIA2 ISR               */
    defaultHandler,                      /* EUSCIA3 ISR               */
    defaultHandler,                      /* EUSCIB0 ISR               */
    defaultHandler,                      /* EUSCIB1 ISR               */
    defaultHandler,                      /* EUSCIB2 ISR               */
    defaultHandler,                      /* EUSCIB3 ISR               */
    defaultHandler,                      /* ADC14 ISR                 */
    defaultHandler,                      /* T32_INT1 ISR              */
    defaultHandler,                      /* T32_INT2 ISR              */
    defaultHandler,                      /* T32_INTC ISR              */
    defaultHandler,                      /* AES ISR                   */
    defaultHandler,                      /* RTC ISR                   */
    defaultHandler,                      /* DMA_ERR ISR               */
    defaultHandler,                      /* DMA_INT3 ISR              */
    defaultHandler,                      /* DMA_INT2 ISR              */
    defaultHandler,                      /* DMA_INT1 ISR              */
    defaultHandler,                      /* DMA_INT0 ISR              */
    defaultHandler,                      /* PORT1 ISR                 */
    defaultHandler,                      /* PORT2 ISR                 */
    defaultHandler,                      /* PORT3 ISR                 */
    defaultHandler,                      /* PORT4 ISR                 */
    defaultHandler,                      /* PORT5 ISR                 */
    defaultHandler,                      /* PORT6 ISR                 */
    defaultHandler,                      /* Reserved 41               */
    defaultHandler,                      /* Reserved 42               */
    defaultHandler,                      /* Reserved 43               */
    defaultHandler,                      /* Reserved 44               */
    defaultHandler,                      /* Reserved 45               */
    defaultHandler,                      /* Reserved 46               */
    defaultHandler,                      /* Reserved 47               */
    defaultHandler,                      /* Reserved 48               */
    defaultHandler,                      /* Reserved 49               */
    defaultHandler,                      /* Reserved 50               */
    defaultHandler,                      /* Reserved 51               */
    defaultHandler,                      /* Reserved 52               */
    defaultHandler,                      /* Reserved 53               */
    defaultHandler,                      /* Reserved 54               */
    defaultHandler,                      /* Reserved 55               */
    defaultHandler,                      /* Reserved 56               */
    defaultHandler,                      /* Reserved 57               */
    defaultHandler,                      /* Reserved 58               */
    defaultHandler,                      /* Reserved 59               */
    defaultHandler,                      /* Reserved 60               */
    defaultHandler,                      /* Reserved 61               */
    defaultHandler,                      /* Reserved 62               */
    defaultHandler,                      /* Reserved 63               */
};

/*
 * This function is called by __iar_program_start() early in the boot sequence.
 */
int __low_level_init(void)
{
    /* disable the watchdog timer */
    MAP_WDT_A_holdTimer();

    /*==================================*/
    /* Choose if segment initialization */
    /* should be done or not.           */
    /* Return: 0 to omit seg_init       */
    /*         1 to run seg_init        */
    /*==================================*/
    return 1;
}


/* This is the code that gets called when the processor receives a NMI.  This  */
/* simply enters an infinite loop, preserving the system state for examination */
static void
nmiISR(void)
{
    while(1)
    {
    }
}

/* by a debugger.                                                              */
static void
faultISR(void)
{
    /* Enter an infinite loop. */
    while(1)
    {
    }
}


/* This is the code that gets called when the processor receives a fault        */
/* interrupt.  This simply enters an infinite loop, preserving the system state */
/* for examination by a debugger.                                               */

static void
busFaultHandler(void)
{
    /* Enter an infinite loop. */
    while(1)
    {
    }
}


/* This is the code that gets called when the processor receives an unexpected  */
/* interrupt.  This simply enters an infinite loop, preserving the system state */
/* for examination by a debugger.                                               */
static void defaultHandler(void)
{
    /* Enter an infinite loop. */
    while(1)
    {
    }
}
