/*******************************************************************************
 * MSP432 Comparator - Comparator Toggle from SLEEP; input channel C6;
 *      Vcompare is compared against the internal reference 2.0V
 *
 * Description: Use the comparator (input channel C6) and internal reference to
 * determine if input'Vcompare'is high or low.  When Vcompare exceeds 2.0V
 * COUT goes low and when Vcompare is less than 2.0V then COUT goes high.
 *
 *                 MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST        P5.7/C6|<--Vcompare
 *            |                  |
 *            |         P7.2/COUT|----> 'high'(Vcompare<2.0V); 'low'(Vcompare>2.0V)
 *            |                  |  
 *            |                  |  
 *            |                  |
 ******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Comparator configuration structure */
const COMP_E_Config compConfig =
{
        COMP_E_VREF,                  // Positive Input Terminal
        COMP_E_INPUT6,                // Negative Input Terminal
        COMP_E_FILTEROUTPUT_DLYLVL4,  // Filter level of 4
        COMP_E_NORMALOUTPUTPOLARITY   // Normal Output Polarity
};

int main(void)
{
    /* Stop WDT  */
    MAP_WDT_A_holdTimer();

    /*
     * Select Port 7
     * Set Pin 1 to output Primary Module Function, (COUT).
     */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN2,
            GPIO_PRIMARY_MODULE_FUNCTION);

    /* Set P5.7 to be comparator in (C1.6) */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P7, GPIO_PIN7,
            GPIO_TERTIARY_MODULE_FUNCTION);

    /* Initialize the Comparator E module
     *  Comparator Instance 1,
     *  Pin CE6 to Positive(+) Terminal,
     *  Reference Voltage to Negative(-) Terminal,
     *  Normal Power Mode,
     *  Output Filter On with max delay,
     *  Non-Inverted Output Polarity
     */
    MAP_COMP_E_initModule(COMP_E1_BASE, &compConfig);

    /*Set the reference voltage that is being supplied to the (-) terminal
     *  Comparator Instance 1,
     *  Reference Voltage of 2.0 V,
     *  Lower Limit of 2.0*(32/32) = 2.0V,
     *  Upper Limit of 2.0*(32/32) = 2.0V
     */
    MAP_COMP_E_setReferenceVoltage(COMP_E1_BASE, COMP_E_VREFBASE2_0V, 32, 32);

    /* Disable Input Buffer on CE1.6
     *  Base Address of Comparator E,
     *  Input Buffer port
     *  Selecting the CEx input pin to the comparator
     *  multiplexer with the CEx bits automatically
     *  disables output driver and input buffer for
     *  that pin, regardless of the state of the
     *  associated CEPD.x bit
     */
    MAP_COMP_E_disableInputBuffer(COMP_E1_BASE, COMP_E_INPUT6);

    /* Allow power to Comparator module */
    MAP_COMP_E_enableModule(COMP_E1_BASE);

    MAP_PCM_gotoLPM0();
    __no_operation();
}
