/*
 * LED.c
 *
 *  Created on: 7 Nov. 2018
 *      Author: Lucas.Oldfield
 */


/*
 *  ======== LED.c ========
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/LED.h>

extern LED_Config LED_config[];
extern const uint8_t LED_count;

static bool isInitialized = false;

/* Default NVS parameters structure */
const LED_Params LED_defaultParams = {
    NULL    /* custom */
};

/*
 *  ======== LED_close =======
 */
void LED_close(LED_Handle handle)
{
    handle->fxnTablePtr->closeFxn(handle);
}

/*
 *  ======== LED_control ========
 */
int_fast16_t LED_control(LED_Handle handle, uint_fast16_t cmd, void *arg)
{
    return (handle->fxnTablePtr->controlFxn(handle, cmd, arg));
}

/*
 *  ======== LED_init =======
 */
void LED_init(void)
{
    uint_least8_t i;

    /* Call each driver's init function */
    for (i = 0; i < LED_count; i++) {
        LED_config[i].fxnTablePtr->initFxn();
    }

    isInitialized = true;
}

/*
 *  ======== LED_open =======
 */
LED_Handle LED_open(uint_least8_t index, LED_Params *params)
{
    LED_Handle handle = NULL;

    /* do init if not done yet */
    if (!isInitialized) {
        LED_init();
    }

    if (index < LED_count) {
        if (params == NULL) {
            /* No params passed in, so use the defaults */
            params = (LED_Params *)&LED_defaultParams;
        }
        handle = LED_config[index].fxnTablePtr->openFxn(index, params);
    }

    return (handle);
}

/*
 *  ======== LED_Params_init =======
 */
void LED_Params_init(LED_Params *params)
{
    *params = LED_defaultParams;
}

/*
 *  ======== LED_start =======
 */
int32_t LED_start(LED_Handle handle)
{
    return (handle->fxnTablePtr->startFxn(handle));
}

/*
 *  ======== LED_stop =======
 */
void LED_stop(LED_Handle handle)
{
    return (handle->fxnTablePtr->stopFxn(handle));
}
