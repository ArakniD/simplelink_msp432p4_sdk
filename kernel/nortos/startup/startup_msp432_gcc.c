/*
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
 */


//*****************************************************************************
//
// Check if compiler is GNU Compiler
//
//*****************************************************************************
#if !(defined(__GNUC__))
#error "startup_msp432_gcc.c: Unsupported compiler!"
#endif

#include <stddef.h>
#include <stdint.h>

#include <ti/devices/DeviceFamily.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/driverlib/interrupt.h>

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
void resetISR(void);
static void nmiISR(void);
static void faultISR(void);
static void defaultHandler(void);
static void busFaultHandler(void);

//*****************************************************************************
//
// External declaration for the reset handler that is to be called when the
// processor is started
//
//*****************************************************************************
extern void _c_int00(void);

//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int main(void);

//*****************************************************************************
//
// linker variable that marks the top of stack.
//
//*****************************************************************************
extern unsigned long _stack_end;

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
void (* const interruptVectors[])(void) __attribute__ ((section (".intvecs"))) =
{
    (void (*)(void))((uint32_t)&_stack_end),
                                            /* The initial stack pointer */
    resetISR,                               /* The reset handler         */
    nmiISR,                                 /* The NMI handler           */
    faultISR,                               /* The hard fault handler    */
    defaultHandler,                         /* The MPU fault handler     */
    busFaultHandler,                        /* The bus fault handler     */
    defaultHandler,                         /* The usage fault handler   */
    0,                                      /* Reserved                  */
    0,                                      /* Reserved                  */
    0,                                      /* Reserved                  */
    0,                                      /* Reserved                  */
    defaultHandler,                         /* SVCall handler            */
    defaultHandler,                         /* Debug monitor handler     */
    0,                                      /* Reserved                  */
    defaultHandler,                         /* The PendSV handler        */
    defaultHandler,                         /* The SysTick handler       */
    defaultHandler,                         /* PSS ISR                   */
    defaultHandler,                         /* CS ISR                    */
    defaultHandler,                         /* PCM ISR                   */
    defaultHandler,                         /* WDT ISR                   */
    defaultHandler,                         /* FPU ISR                   */
    defaultHandler,                         /* FLCTL ISR                 */
    defaultHandler,                         /* COMP0 ISR                 */
    defaultHandler,                         /* COMP1 ISR                 */
    defaultHandler,                         /* TA0_0 ISR                 */
    defaultHandler,                         /* TA0_N ISR                 */
    defaultHandler,                         /* TA1_0 ISR                 */
    defaultHandler,                         /* TA1_N ISR                 */
    defaultHandler,                         /* TA2_0 ISR                 */
    defaultHandler,                         /* TA2_N ISR                 */
    defaultHandler,                         /* TA3_0 ISR                 */
    defaultHandler,                         /* TA3_N ISR                 */
    defaultHandler,                         /* EUSCIA0 ISR               */
    defaultHandler,                         /* EUSCIA1 ISR               */
    defaultHandler,                         /* EUSCIA2 ISR               */
    defaultHandler,                         /* EUSCIA3 ISR               */
    defaultHandler,                         /* EUSCIB0 ISR               */
    defaultHandler,                         /* EUSCIB1 ISR               */
    defaultHandler,                         /* EUSCIB2 ISR               */
    defaultHandler,                         /* EUSCIB3 ISR               */
    defaultHandler,                         /* ADC14 ISR                 */
    defaultHandler,                         /* T32_INT1 ISR              */
    defaultHandler,                         /* T32_INT2 ISR              */
    defaultHandler,                         /* T32_INTC ISR              */
    defaultHandler,                         /* AES ISR                   */
    defaultHandler,                         /* RTC ISR                   */
    defaultHandler,                         /* DMA_ERR ISR               */
    defaultHandler,                         /* DMA_INT3 ISR              */
    defaultHandler,                         /* DMA_INT2 ISR              */
    defaultHandler,                         /* DMA_INT1 ISR              */
    defaultHandler,                         /* DMA_INT0 ISR              */
    defaultHandler,                         /* PORT1 ISR                 */
    defaultHandler,                         /* PORT2 ISR                 */
    defaultHandler,                         /* PORT3 ISR                 */
    defaultHandler,                         /* PORT4 ISR                 */
    defaultHandler,                         /* PORT5 ISR                 */
    defaultHandler,                         /* PORT6 ISR                 */
    defaultHandler,                         /* Reserved 41               */
    defaultHandler,                         /* Reserved 42               */
    defaultHandler,                         /* Reserved 43               */
    defaultHandler,                         /* Reserved 44               */
    defaultHandler,                         /* Reserved 45               */
    defaultHandler,                         /* Reserved 46               */
    defaultHandler,                         /* Reserved 47               */
    defaultHandler,                         /* Reserved 48               */
    defaultHandler,                         /* Reserved 49               */
    defaultHandler,                         /* Reserved 50               */
    defaultHandler,                         /* Reserved 51               */
    defaultHandler,                         /* Reserved 52               */
    defaultHandler,                         /* Reserved 53               */
    defaultHandler,                         /* Reserved 54               */
    defaultHandler,                         /* Reserved 55               */
    defaultHandler,                         /* Reserved 56               */
    defaultHandler,                         /* Reserved 57               */
    defaultHandler,                         /* Reserved 58               */
    defaultHandler,                         /* Reserved 59               */
    defaultHandler,                         /* Reserved 60               */
    defaultHandler,                         /* Reserved 61               */
    defaultHandler,                         /* Reserved 62               */
    defaultHandler,                         /* Reserved 63               */
};

//*****************************************************************************
//
// The following are arrays of pointers to constructor functions that need to
// be called during startup to initialize global objects.
//
//*****************************************************************************
extern void (*__init_array_start []) (void);
extern void (*__init_array_end []) (void);

//*****************************************************************************
//
// The following global variable is required for C++ support.
//
//*****************************************************************************
void * __dso_handle = (void *) &__dso_handle;

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __data_load__;
extern uint32_t __data_start__;
extern uint32_t __data_end__;

void resetISR(void)
{
    uint32_t * bs;
    uint32_t * be;
    uint32_t * dl;
    uint32_t * ds;
    uint32_t * de;
    uint32_t count;
    uint32_t i;

    Interrupt_disableMaster();

    /* Stop Watch Dog Timer */
    MAP_WDT_A_holdTimer();

    /* Enable FPU */
    MAP_FPU_enableModule();

    /* initiailize .bss to zero */
    bs = & __bss_start__;
    be = & __bss_end__;
    while (bs < be) {
        *bs = 0;
        bs++;
    }

    /* relocate the .data section */
    dl = & __data_load__;
    ds = & __data_start__;
    de = & __data_end__;
    if (dl != ds) {
        while (ds < de) {
            *ds = *dl;
            dl++;
            ds++;
        }
    }

    /* Run any constructors */
    count = (uint32_t)(__init_array_end - __init_array_start);
    for (i = 0; i < count; i++) {
        __init_array_start[i]();
    }

    /* Call the application's entry point. */
    main();
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
static void nmiISR(void)
{
    /* Enter an infinite loop. */
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void faultISR(void)
{
    /* Enter an infinite loop. */
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void busFaultHandler(void)
{
    /* Enter an infinite loop. */
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void defaultHandler(void)
{
    /* Enter an infinite loop. */
    while(1)
    {
    }
}

//*****************************************************************************
//
// This function is called by __libc_fini_array which gets called when exit()
// is called. In order to support exit(), an empty _fini() stub function is
// required.
//
//*****************************************************************************
void _fini(void)
{
    /* Function body left empty intentionally */
}
