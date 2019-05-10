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

#ifndef ti_display_DisplayOled__include
#define ti_display_DisplayOled__include

#include <ti/drivers/dpl/SemaphoreP.h>

#include <ti/drivers/SPI.h>

#include <ti/display/Display.h>
#include <ti/grlib/grlib.h>

#include <stdint.h>

/**
 *  @addtogroup DISPLAY_CMD
 *  DisplayOled_CMD_* macros are command codes only defined in the
 *  DisplayOled.h driver implementation and need to:
 *  @code
 *  #include <ti/display/DisplayOled.h>
 *  @endcode
 *  @{
 */

/*!
 * @brief Command used by Display_control to invert the display
 *
 * This command allows changing the background and foreground colors from
 * the default, depending on the value (1 - invert, 0 - don't invert) of @b arg
 *
 * With this command @b arg is of type @c DisplayOledColor_t *.
 */
#define DisplayOled_CMD_SET_COLORS  DISPLAY_CMD_RESERVED + 0
/** @}*/

/*!
 *  @internal
 *  @brief
 *  Display function table pointer
 */
extern const Display_FxnTable DisplayOled_fxnTable;

/*!
 *  @brief Foreground and background color struct, compatible with GrLib.
 *
 *  Default values on DisplayOled_open() are - from grlib.h:
 *    bg - ClrBlack
 *    fg - ClrWhite
 */
typedef struct
{
    uint32_t fg;
    uint32_t bg;
} DisplayOledColor_t;

typedef enum  {
    ePORTRAIT,
    eLANDSCAPE,
    ePORTRAIT_FLIP,
    eLANDSCAPE_FLIP,
} DisplayOled_Orientation;

typedef struct DisplayOled_DisplayBuffer {
    uint16_t *          buffer;            /*!< pointer to buffer for display */
    bool                invalidated;
    Graphics_Rectangle  invalidatedRegion; /*!< invalidated Region that needs to be flushed */
    Graphics_Rectangle  useableRegion;     /*!< region of the display to write to */
} DisplayOled_DisplayBuffer;

typedef struct DisplayOled_HWAttrsV1
{
    uint32_t            spiIndex;
    uint32_t            csPin;
    uint32_t            dataPin;
    uint32_t            resetPin;
    uint16_t            pixelHeight;
    uint16_t            pixelWidth;
    DisplayOled_Orientation orientation;
    uint16_t             *displayBuffer;
} DisplayOled_HWAttrsV1;

// 5-6-5 colour mapping buffer
typedef uint16_t   DisplayOled_Buf_128x96[128 * 96];

typedef struct DisplayOled_Object
{
    Graphics_Context      g_sContext;
    SPI_Handle            hSpi;
    Display_LineClearMode lineClearMode;
    Graphics_Display      g_sDisplay;
    DisplayOledColor_t    displayColor;
    SemaphoreP_Handle     semLCD;
    DisplayOled_DisplayBuffer displayBuf;
} DisplayOled_Object, *DisplayOled_Handle;

void DisplayOled_init(Display_Handle handle);
Display_Handle DisplayOled_open(Display_Handle,
                                Display_Params * params);
void DisplayOled_clear(Display_Handle handle);
void DisplayOled_clearLines(Display_Handle handle,
                            uint8_t fromLine,
                            uint8_t toLine);
void DisplayOled_vprintf(Display_Handle handle, uint8_t line,
                         uint8_t column,
                         const char *fmt, va_list va);
void DisplayOled_close(Display_Handle);
int DisplayOled_control(Display_Handle handle, unsigned int cmd, void *arg);
unsigned int DisplayOled_getType(void);
#endif // ti_display_DisplayOled__include
