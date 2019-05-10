/*
 * Copyright (c) 2018, Texas Instruments Incorporated
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

/* -----------------------------------------------------------------------------
 *  Includes
 * -----------------------------------------------------------------------------
 */

#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/dpl/SystemP.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/GPIO.h>

#include <ti/grlib/grlib.h>
#include <ti/display/Display.h>

#include <ti/display/DisplayOled.h>
#include <ti/display/oled/OledGrLib.h>

/* -----------------------------------------------------------------------------
 *  Constants and macros
 * -----------------------------------------------------------------------------
 */
// Timeout of semaphore that controls exclusive to the OLED (infinite)
#define ACCESS_TIMEOUT    SemaphoreP_WAIT_FOREVER

/* -----------------------------------------------------------------------------
 *   Type definitions
 * -----------------------------------------------------------------------------
 */


/* -----------------------------------------------------------------------------
 *                           Local variables
 * -----------------------------------------------------------------------------
 */
/* Display function table for sharp implementation */
const Display_FxnTable DisplayOled_fxnTable = {
    DisplayOled_init,
    DisplayOled_open,
    DisplayOled_clear,
    DisplayOled_clearLines,
    DisplayOled_vprintf,
    DisplayOled_close,
    DisplayOled_control,
    DisplayOled_getType,
};

//*****************************************************************************
//
//! The display structure that describes the driver for the
//! sharpLCD panel
//
//*****************************************************************************
extern const Graphics_Display_Functions g_oledFxns;

/* -----------------------------------------------------------------------------
 *                                          Functions
 * -----------------------------------------------------------------------------
 */
/*!
 * @fn          DisplayOled_init
 *
 * @brief       Does nothing.
 *
 * @return      void
 */
void DisplayOled_init(Display_Handle Display_config)
{
}

/*!
 * @fn          DisplayOled_open
 *
 * @brief       Initialize the OLED
 *
 * @descr       Initializes the pins used by the OLED, creates resource access
 *              protection semaphore, turns on the OLED device, initializes the
 *              frame buffer, initializes to white background/dark foreground,
 *              and finally clears the object->displayColor.
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       params - display parameters
 *
 * @return      Pointer to Display_Config struct
 */
Display_Handle DisplayOled_open(Display_Handle hDisplay,
                                 Display_Params *params)
{
    DisplayOled_HWAttrsV1 *hwAttrs = (DisplayOled_HWAttrsV1 *)hDisplay->hwAttrs;
    DisplayOled_Object  *object  = (DisplayOled_Object  *)hDisplay->object;

    GPIO_init();    /* It is OK to call GPIO_init() and SPI_init() multiple times. */
    SPI_init();

    object->lineClearMode = params->lineClearMode;

    GPIO_setConfig(hwAttrs->csPin, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(hwAttrs->dataPin, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(hwAttrs->resetPin, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);

    SPI_Params spiParams;
    SPI_Params_init(&spiParams);
    spiParams.bitRate = 3e6;
    spiParams.transferMode = SPI_MODE_BLOCKING;
    spiParams.mode = SPI_MASTER;
    spiParams.dataSize = 8;
    spiParams.frameFormat = SPI_POL1_PHA1;
    spiParams.transferTimeout = 500;

    object->hSpi = SPI_open(hwAttrs->spiIndex, &spiParams);

    if (object->hSpi == NULL)
    {
        return NULL;
    }

    // Init colors
    object->displayColor.bg = ClrBlack;
    object->displayColor.fg = ClrWhite;

    object->semLCD = SemaphoreP_createBinary(1);
    if (object->semLCD == NULL)
    {
        SPI_close(object->hSpi);
        return NULL;
    }

    // Grab OLED
    SemaphoreP_pend(object->semLCD, ACCESS_TIMEOUT);

    // Initialize the GrLib back-end transport
    OLEDGrLib_init(object->hSpi, hwAttrs->csPin, hwAttrs->resetPin, hwAttrs->dataPin, hwAttrs->orientation);

    object->displayBuf.buffer             = hwAttrs->displayBuffer;
    object->displayBuf.useableRegion.xMin = 0;
    object->displayBuf.useableRegion.yMin = 0;
    object->displayBuf.useableRegion.xMax = hwAttrs->pixelWidth;
    object->displayBuf.useableRegion.yMax = hwAttrs->pixelHeight;

    // Initial invalid region is complete usable range
    object->displayBuf.invalidatedRegion = object->displayBuf.useableRegion;

    object->g_sDisplay.size          = sizeof(struct Graphics_Display);
    object->g_sDisplay.pFxns         = &g_oledFxns;
    object->g_sDisplay.displayData   = (void*)&object->displayBuf;
    object->g_sDisplay.height        = hwAttrs->pixelHeight;
    object->g_sDisplay.width         = hwAttrs->pixelWidth;

    // Graphics library init
    Graphics_initContext(&object->g_sContext, &object->g_sDisplay, &g_oledFxns);

    // Graphics properties
    Graphics_setForegroundColor(&object->g_sContext, object->displayColor.fg);
    Graphics_setBackgroundColor(&object->g_sContext, object->displayColor.bg);
    Graphics_setFont(&object->g_sContext, &g_sFontFixed6x8);

    // Clear display
    Graphics_clearDisplay(&object->g_sContext);
    Graphics_flushBuffer(&object->g_sContext);

    // Release OLED
    SemaphoreP_post(object->semLCD);

    return hDisplay;
}


/*!
 * @fn          DisplayOled_clear
 *
 * @brief       Clears the display
 *
 * @param       hDisplay - pointer to Display_Config struct
 *
 * @return      void
 */
void DisplayOled_clear(Display_Handle hDisplay)
{
    DisplayOled_Object *object = (DisplayOled_Object  *)hDisplay->object;

    // Grab OLED
    if (SemaphoreP_pend(object->semLCD, ACCESS_TIMEOUT) == SemaphoreP_OK)
    {
        Graphics_clearDisplay(&object->g_sContext);
        Graphics_flushBuffer(&object->g_sContext);

        // Release OLED
        SemaphoreP_post(object->semLCD);
    }
}


/*!
 * @fn          DisplayOled_clearLines
 *
 * @brief       Clears lines lineFrom-lineTo of the display, inclusive
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       lineFrom - line index (0 .. )
 * @param       lineTo - line index (0 .. )
 *
 * @return      void
 */
void DisplayOled_clearLines(Display_Handle hDisplay,
                             uint8_t lineFrom, uint8_t lineTo)
{
    DisplayOled_Object *object = (DisplayOled_Object  *)hDisplay->object;

    if (lineTo <= lineFrom)
    {
        lineTo = lineFrom;
    }

    Graphics_Rectangle rect = {
        .xMin = 0,
        .xMax = object->g_sContext.clipRegion.xMax,
        .yMin = lineFrom * object->g_sContext.font->height,
        .yMax = (lineTo + 1) * object->g_sContext.font->height - 1,
    };

    Graphics_setForegroundColor(&object->g_sContext, object->displayColor.bg);
    Graphics_fillRectangle(&object->g_sContext, &rect);
    Graphics_setForegroundColor(&object->g_sContext, object->displayColor.fg);
    Graphics_flushBuffer(&object->g_sContext);
}


/*!
 * @fn          DisplayOled_put5
 *
 * @brief       Write a text string to a specific line/column of the display
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       line - line index (0..)
 * @param       column - column index (0..)
 * @param       fmt - format string
 * @param       aN - optional format arguments
 *
 * @return      void
 */
void DisplayOled_vprintf(Display_Handle hDisplay, uint8_t line,
                          uint8_t column, const char *fmt, va_list va)
{
    DisplayOled_Object *object = (DisplayOled_Object  *)hDisplay->object;

    uint8_t xp, yp, clearStartX, clearEndX;

    char    dispStr[23];

    // Grab OLED
    if (SemaphoreP_pend(object->semLCD, ACCESS_TIMEOUT) == SemaphoreP_OK)
    {
        xp          = column * object->g_sContext.font->maxWidth + 1;
        yp          = line * object->g_sContext.font->height + 0;
        clearStartX = clearEndX = xp;

        switch (object->lineClearMode)
        {
        case DISPLAY_CLEAR_LEFT:
            clearStartX = 0;
            break;
        case DISPLAY_CLEAR_RIGHT:
            clearEndX = object->g_sContext.clipRegion.xMax;
            break;
        case DISPLAY_CLEAR_BOTH:
            clearStartX = 0;
            clearEndX   = object->g_sContext.clipRegion.xMax;
            break;
        case DISPLAY_CLEAR_NONE:
        default:
            break;
        }

        if (clearStartX != clearEndX)
        {
            Graphics_Rectangle rect = {
                .xMin = clearStartX,
                .xMax = clearEndX,
                .yMin = yp,
                .yMax = yp + object->g_sContext.font->height - 1,
            };

            Graphics_setForegroundColor(&object->g_sContext, object->displayColor.bg);
            Graphics_fillRectangle(&object->g_sContext, &rect);
            Graphics_setForegroundColor(&object->g_sContext, object->displayColor.fg);
        }

        SystemP_vsnprintf(dispStr, sizeof(dispStr), fmt, va);

        // Draw a text on the display
        Graphics_drawString(&object->g_sContext,
                           (int8_t *)dispStr,
                           AUTO_STRING_LENGTH,
                           xp,
                           yp,
                           OPAQUE_TEXT);

        Graphics_flushBuffer(&object->g_sContext);

        // Release OLED
        SemaphoreP_post(object->semLCD);
    }
}


/*!
 * @fn          DisplayOled_close
 *
 * @brief       Turns of the display and releases the OLED control pins
 *
 * @param       hDisplay - pointer to Display_Config struct
 *
 * @return      void
 */
void DisplayOled_close(Display_Handle hDisplay)
{
    DisplayOled_HWAttrsV1 *hwAttrs = (DisplayOled_HWAttrsV1 *)hDisplay->hwAttrs;
    DisplayOled_Object  *object  = (DisplayOled_Object  *)hDisplay->object;

    // Grab OLED
    if (SemaphoreP_pend(object->semLCD, ACCESS_TIMEOUT) == SemaphoreP_OK)
    {
        // Turn off the display
        GPIO_write(hwAttrs->resetPin, 0x00);

        SPI_close(object->hSpi);
        object->hSpi = NULL;

        // Deconfigure GrLib back-end
        OLEDGrLib_init(NULL,0,0,0,0);

        // Release OLED
        SemaphoreP_post(object->semLCD);
    }
}

/*!
 * @fn          DisplayOled_control
 *
 * @brief       Function for setting control parameters of the Display driver
 *              after it has been opened.
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       cmd - command to execute, supported commands are:
 *              | Command                        | Description             |
 *              |------------------------------- |-------------------------|
 *              | ::DISPLAY_CMD_TRANSPORT_CLOSE  | Close SPI but leave control pins |
 *              | ::DISPLAY_CMD_TRANSPORT_OPEN   | Re-open SPI driver      |
 * @param       arg - argument to the command
 *
 * @return      ::DISPLAY_STATUS_SUCCESS if success, or error code if error.
 */
int DisplayOled_control(Display_Handle hDisplay, unsigned int cmd, void *arg)
{
    DisplayOled_HWAttrsV1 *hwAttrs = (DisplayOled_HWAttrsV1 *)hDisplay->hwAttrs;
    DisplayOled_Object  *object  = (DisplayOled_Object  *)hDisplay->object;

    /* Initialize return value */
    int ret = DISPLAY_STATUS_ERROR;

    /* Perform command */
    switch(cmd)
    {
        case DISPLAY_CMD_TRANSPORT_CLOSE:
            // Grab OLED
            if (SemaphoreP_pend(object->semLCD, ACCESS_TIMEOUT) == SemaphoreP_OK)
            {
                if (object->hSpi)
                {
                    // Close SPI and tell back-end there is no SPI
                    SPI_close(object->hSpi);
                    OLEDGrLib_init(NULL,0,0,0,0);
                    object->hSpi = NULL;
                    ret = DISPLAY_STATUS_SUCCESS;
                }
                // Release OLED
                SemaphoreP_post(object->semLCD);
            }
            break;

        case DISPLAY_CMD_TRANSPORT_OPEN:
            // Grab OLED
            if (SemaphoreP_pend(object->semLCD, ACCESS_TIMEOUT) == SemaphoreP_OK)
            {
                if (NULL == object->hSpi)
                {
                    // Re-open SPI and re-init back-end
                    SPI_Params spiParams;
                    SPI_Params_init(&spiParams);
                    spiParams.bitRate = 3e6;
                    spiParams.transferMode = SPI_MODE_BLOCKING;
                    spiParams.mode = SPI_MASTER;
                    spiParams.dataSize = 8;
                    spiParams.frameFormat = SPI_POL1_PHA0;
                    spiParams.transferTimeout = 500;
                    object->hSpi = SPI_open(hwAttrs->spiIndex, &spiParams);
                    OLEDGrLib_init(object->hSpi, hwAttrs->csPin, hwAttrs->resetPin, hwAttrs->dataPin, hwAttrs->orientation);
                    ret = DISPLAY_STATUS_SUCCESS;
                }
                // Release OLED
                SemaphoreP_post(object->semLCD);
            }
            break;

        case DisplayOled_CMD_SET_COLORS:
            // Grab OLED
            if (SemaphoreP_pend(object->semLCD, ACCESS_TIMEOUT) == SemaphoreP_OK)
            {
                object->displayColor = *(DisplayOledColor_t *)arg;

                Graphics_setForegroundColor(&object->g_sContext, object->displayColor.fg);
                Graphics_setBackgroundColor(&object->g_sContext, object->displayColor.bg);

                // Release OLED
                SemaphoreP_post(object->semLCD);

                // Return success
                ret = DISPLAY_STATUS_SUCCESS;
            }
            break;

        default:
            /* The command is not defined */
            ret = SPI_STATUS_UNDEFINEDCMD;
            break;
    }

    return ret;
}

/*!
 * @fn          DisplayOled_getType
 *
 * @brief       Returns type of transport
 *
 * @return      Display type define OLED
 */
unsigned int DisplayOled_getType(void)
{
    return Display_Type_GRLIB | Display_Type_LCD;
}
