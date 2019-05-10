/*
 * LED.h
 *
 *  Created on: 7 Nov. 2018
 *      Author: Lucas.Oldfield
 */

#ifndef DRIVERS_LED_H_
#define DRIVERS_LED_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/*!
 * Set the base colour via PWM control
 */
#define LED_CMD_SET_COLOUR              (1)
/*!
 * Set the control engine to a hardware based engine if it exists
 */
#define LED_CMD_SET_CONTROL             (2)
/*!
 * Set the control engine to run mode
 */
#define LED_CMD_SET_RUN                 (3)

/*!
 * Common LED_control command code reservation offset.
 * Timer driver implementations should offset command codes with LED_CMD_RESERVED
 * growing positively
 *
 * Example implementation specific command codes:
 * @code
 * #define TimerXYZ_CMD_COMMAND0      LED_CMD_RESERVED + 0
 * #define TimerXYZ_CMD_COMMAND1      LED_CMD_RESERVED + 1
 * @endcode
 */
#define LED_CMD_RESERVED                (32)

/*!
 * Common LED_control status code reservation offset.
 * Timer driver implementations should offset status codes with
 * LED_STATUS_RESERVED growing negatively.
 *
 * Example implementation specific status codes:
 * @code
 * #define TimerXYZ_STATUS_ERROR0     LED_STATUS_RESERVED - 0
 * #define TimerXYZ_STATUS_ERROR1     LED_STATUS_RESERVED - 1
 * @endcode
 */
#define LED_STATUS_RESERVED            (-32)

/*!
 * @brief   Successful status code.
 */
#define LED_STATUS_SUCCESS               (0)

/*!
 * @brief   Generic error status code.
 */
#define LED_STATUS_ERROR                (-1)

/*!
 * @brief   An error status code returned by LED_control() for undefined
 *          command codes.
 *
 * LED_control() returns LED_STATUS_UNDEFINEDCMD if the control code is not
 * recognized by the driver implementation.
 */
#define LED_STATUS_UNDEFINEDCMD         (-2)

/*!
 *  @brief      A handle that is returned from a LED_open() call.
 */
typedef struct LED_Config_ *LED_Handle;

/*!
 *  @brief Timer Parameters
 *
 *  Timer parameters are used to with the LED_open() call. Default values for
 *  these parameters are set using LED_Params_init().
 *
 */
typedef struct LED_Params {
    void *custom;   /*!< Custom argument used by driver implementation */
} LED_Params;

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              LED_control().
 */
typedef int_fast16_t (*LED_ControlFxn)(LED_Handle handle,
    uint_fast16_t cmd, void *arg);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              LED_close().
 */
typedef void (*LED_CloseFxn)(LED_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              LED_init().
 */
typedef void (*LED_InitFxn)(void);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              LED_open().
 */
typedef LED_Handle (*LED_OpenFxn)(uint_least8_t index, LED_Params *params);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              LED_start().
 */
typedef int32_t (*LED_StartFxn)(LED_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              LED_stop().
 */
typedef void (*LED_StopFxn)(LED_Handle handle);

/*!
 *  @brief      The definition of a led function table that contains the
 *              required set of functions to control a specific led driver
 *              implementation.
 */
typedef struct LED_FxnTable_ {
    /*! Function to close the specified peripheral. */
    LED_CloseFxn closeFxn;

    /*! Function to implementation specific control function. */
    LED_ControlFxn controlFxn;

    /*! Function to initialize the given data object. */
    LED_InitFxn initFxn;

    /*! Function to open the specified peripheral. */
    LED_OpenFxn openFxn;

    /*! Function to start the specified peripheral. */
    LED_StartFxn startFxn;

    /*! Function to stop the specified peripheral. */
    LED_StopFxn stopFxn;
} LED_FxnTable;

/*!
 *  @brief  Timer Global configuration
 *
 *  The LED_Config structure contains a set of pointers used to characterize
 *  the led driver implementation.
 *
 *  This structure needs to be defined before calling LED_init() and it must
 *  not be changed thereafter.
 *
 *  @sa     LED_init()
 */
typedef struct LED_Config_ {
    /*! Pointer to a table of driver-specific implementations of led APIs. */
    LED_FxnTable const *fxnTablePtr;

    /*! Pointer to a driver specific data object. */
    void                 *object;

    /*! Pointer to a driver specific hardware attributes structure. */
    void           const *hwAttrs;
} LED_Config;

/*!
 *  @brief  Function to close a led. The corresponding led to the
 *          LED_Handle becomes an available led resource.
 *
 *  @pre    LED_open() has been called.
 *
 *  @param  handle  A LED_Handle returned from LED_open().
 *
 *  @sa     LED_open()
 */
extern void LED_close(LED_Handle handle);

/*!
 *  @brief  Function performs device specific features on a given
 *          LED_Handle.
 *
 *  @pre    LED_open() has been called.
 *
 *  @param  handle      A LED_Handle returned from LED_open().
 *
 *  @param  cmd         A command value defined by the driver specific
 *                      implementation.
 *
 *  @param  arg         A pointer to an optional R/W (read/write) argument that
 *                      is accompanied with cmd.
 *
 *  @return A LED_Status describing an error or success state. Negative values
 *          indicate an error occurred.
 *
 *  @sa     LED_open()
 */
extern int_fast16_t LED_control(LED_Handle handle, uint_fast16_t cmd,
    void *arg);

/*!
 *  @brief  Function to initialize a led module. This function will go through
 *          all available hardware resources and mark them as "available".
 *
 *  @pre    The LED_config structure must exist and be persistent before this
 *          function can be called. This function must also be called before
 *          any other led driver APIs.
 *
 *  @sa     LED_open()
 */
extern void LED_init(void);

/*!
 *  @brief
 *
 *  @pre    LED_init() has been called.
 *
 *  @param  index         Logical peripheral number for the led indexed into
 *                        the LED_config table.
 *
 *  @param  params        Pointer to an parameter block, if NULL it will use
 *                        default values.
 *
 *  @return A LED_Handle upon success or NULL. If the desired period results
 *          in overflow, or saturation, of the led, NULL is returned. If the
 *          led resource is already in use, NULL is returned.
 *
 *  @sa     LED_init()
 *  @sa     LED_close()
 */
extern LED_Handle LED_open(uint_least8_t index, LED_Params *params);

/*!
 *  @brief  Function to initialize the LED_Params struct to its defaults.
 *
 *  @param  params      A pointer to LED_Params structure for
 *                      initialization.
 *
 *  Defaults values are:
 */
extern void LED_Params_init(LED_Params *params);

/*!
 *  @brief  Function to start the led.
 *
 *  @pre    LED_open() has been called.
 *
 *  @param  handle  A LED_Handle returned from LED_open().
 *
 *  @return LED_STATUS_SUCCESS or LED_STATUS_ERROR.
 *
 *  @sa     LED_stop()
 */
extern int32_t LED_start(LED_Handle handle);

/*!
 *  @brief  Function to stop led. If the led is already stopped, this
 *          function has no effect.
 *
 *  @pre    LED_open() has been called.
 *
 *  @param  handle  A LED_Handle returned from LED_open().
 *
 *  @sa     LED_start()
 */
extern void LED_stop(LED_Handle handle);

#ifdef __cplusplus
}
#endif


#endif /* DRIVERS_LED_H_ */
