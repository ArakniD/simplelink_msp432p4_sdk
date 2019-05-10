/*
 * LEDLP5562.h
 *
 *  Created on: 7 Nov. 2018
 *      Author: Lucas.Oldfield
 */

#ifndef DRIVERS_LED_LEDLP5562_H_
#define DRIVERS_LED_LEDLP5562_H_

#include <stdint.h>
#include <stdbool.h>

#include <ti/drivers/LED.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>

#if defined (__cplusplus)
extern "C" {
#endif

#define LEDLP5662_CMD_SET_MODE      (LED_CMD_RESERVED + 0)
#define LEDLP5662_CMD_SET_OFF       (LED_CMD_RESERVED + 1)

// Default device ID
#define LEDLP5662_DEV_ID            (0x60 >> 1)

/*!
 *  @internal @brief NVS function pointer table
 *
 *  'LEDLP5562_fxnTable' is a fully populated function pointer table
 *  that can be referenced in the LED_config[] array entries.
 *
 *  Users can minimize their application code size by providing their
 *  own custom LED function pointer table that contains only those APIs
 *  used by the application.
 */
extern const LED_FxnTable LEDLP5562_fxnTable;

enum lp55xx_engine_index {
    LP55XX_ENGINE_PWM,
    LP55XX_ENGINE_1,
    LP55XX_ENGINE_2,
    LP55XX_ENGINE_3,
    LP55XX_ENGINE_MAX = LP55XX_ENGINE_3,
} lp55xx_engine_index;

enum lp55xx_engine_mode {
    /* Disabled */
    LP55XX_ENGINE_DISABLED,
    /* Load program to SRAM, reset engine 1 PC  */
    LP55XX_ENGINE_LOAD,
    /* Run program defined by ENG1_EXEC */
    LP55XX_ENGINE_RUN,
    /* Direct control */
    LP55XX_ENGINE_DIRECT,
} lp55xx_engine_mode;

enum lp5562_exec_mode {
    /* Hold: Wait until current command is finished then stop
             while EXEC mode is hold. PC can be read or written only in this
             mode. */
    LP55XX_EXEC_HOLD,
    /* Step: Execute instruction defined by current engine 1 PC
             value, increment PC and change ENG1_EXEC to 00b (Hold) */
    LP55XX_EXEC_STEP,
    /* Run:  Start at program counter value defined by current
             engine 1 PC value */
    LP55XX_EXEC_RUN,
    /* Exec: Execute instruction defined by current engine 1 PC value
             and change ENG1_EXEC to 00b (Hold) */
    LP55XX_EXEC_EXEC,
} lp5562_run_mode;

typedef struct lp55xx_predef_pattern {
    const uint8_t *r;
    const uint8_t *g;
    const uint8_t *b;
    uint8_t size_r;
    uint8_t size_g;
    uint8_t size_b;
} lp55xx_predef_pattern;

typedef struct lp5562_pattern {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
} lp5562_pattern;

typedef struct lp5562_engine_map {
    enum lp55xx_engine_index r;
    enum lp55xx_engine_index g;
    enum lp55xx_engine_index b;
    enum lp55xx_engine_index w;
} lp5562_engine_map;

typedef struct lp55xx_run_pattern {
    enum lp55xx_engine_mode e1;
    enum lp55xx_engine_mode e2;
    enum lp55xx_engine_mode e3;
} lp55xx_run_pattern;

typedef struct lp55xx_led_config {
    const char *name;
    uint8_t chan_nr;
    enum lp55xx_engine_index engine_index;
    uint8_t led_current; /* mA x10, 0 if led is not connected */
    uint8_t max_current;
} lp55xx_led_config;

/*
 *  @brief      LEDLP5562_HWAttrs Hardware Attributes
 *
 *  The application must not access any member variables of this structure!
 *
 *
 *  clkPort = GPIO_PORT_P4
 *  clkPin = GPIO_PIN2
 *  clkPinMode = GPIO_SECONDARY_MODULE_FUNCTION
 */
typedef struct LEDLP5562_HWAttrs {
    I2C_Handle             *I2CHandle;      /*!< ptr to I2C handle if provided by user. */
    uint16_t                i2cIndex;       /*!< I2C instance index from Board file */
    uint8_t                 slaveAddress;   /*!< I2C slave address */
    uint_fast8_t            clkPort;        /*!< Port used for 32khz clock */
    uint_fast16_t           clkPin;         /*!< Pin used for 32khz clock */
    uint_fast8_t            clkPinMode;     /*!< Mode used for 32khz clock */
    uint8_t                 gpioEnable;     /*!< Enable (Active Hi)  */
    lp55xx_led_config      *led_config;     /*!< RGBW Current settings for White level  */
    uint8_t                 num_channels;   /*!< number of channels  */
    lp55xx_predef_pattern  *patterns;       /*!< RGBW patterns */
} LEDLP5562_HWAttrs;

/*
 *  @brief      LEDLP5562_Object Object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct LEDLP5562_Object {
    bool                opened;             /*!< LED Interface Opened */
    I2C_Handle          I2CHandle;          /*!< I2C Handle */
    Power_NotifyObj     perfChangeNotify;   /*!< Power notification object */
    lp5562_pattern      currentColour;      /*!< Current colour */
    lp5562_engine_map   currentMap;         /*!< Current engine mapping */
    lp55xx_run_pattern  currentRun;         /*!< Current engine run mode */
    uint8_t             currentOpMode;      /*!< Current operational mode register */
} LEDLP5562_Object;

/*!
 *  @brief Timer Parameters
 *
 *  Timer parameters are used to with the Led_open() call. Default values for
 *  these parameters are set using Led_Params_init().
 *
 */
typedef struct LEDLP5562_Params {
    uint16_t         periodMs;              /*!< Strobe period used */
} LEDLP5562_Params;

/*
 *  @cond NODOC
 *  LEDLP5562 driver public APIs
 */
extern int_fast16_t LEDLP5562_Control    (LED_Handle handle, uint_fast16_t cmd, void *arg);
extern void         LEDLP5562_Close      (LED_Handle handle);
extern void         LEDLP5562_Init       (void);
extern LED_Handle   LEDLP5562_Open       (uint_least8_t index, LED_Params *params);
extern int32_t      LEDLP5562_Start      (LED_Handle handle);
extern void         LEDLP5562_Stop       (LED_Handle handle);
/*! @endcond */

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */

/** @}*/
#endif /* DRIVERS_LED_LEDLP5562_H_ */
