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
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
// IMPORTANT NOTE: The following driver file supports using either RAM or
// non-volatile memory(Flash or FRAM) to store the LCD DisplayBuffer.
//
//*****************************************************************************

//*****************************************************************************
//
// OledGrLib.c
//
//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

#include <ti/grlib/grlib.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/GPIO.h>

#include <ti/drivers/dpl/ClockP.h>

#include <ti/display/oled/OledGrLib.h>
#include <ti/display/DisplayOled.h>

#include <string.h>

#define DELAY_US(i)    ClockP_usleep(i)

static void OLEDGrLib_initializeDisplayBuffer(const Graphics_Display *pDisplay, uint8_t ucValue);

static SPI_Handle      spiHandle;
static uint32_t        oledChipSelect;
static uint32_t        oledDataCommand;
static uint32_t        oledReset;
static uint8_t 		   oledRemap;
static uint8_t         oledOrientation;
static uint8_t         oledWidth;
static uint8_t         oledHeight;
static SPI_Transaction spiTrans;

__inline void ChipSelect()
{
    GPIO_write(oledChipSelect, 0x00);

    DELAY_US(10);
}

__inline void DeSelect()
{
    DELAY_US(10);

    GPIO_write(oledChipSelect, 0x01);
}

/*
 * Invalidate an area of the buffer
 */
static void OLEDGrLib_Invalidate(const Graphics_Display *pDisplay, Graphics_Rectangle *pArea)
{
    DisplayOled_DisplayBuffer * pData = (DisplayOled_DisplayBuffer*)pDisplay->displayData;

    if (pData->invalidated)
        Graphics_getRectangleUnion(pArea, &pData->invalidatedRegion, &pData->invalidatedRegion);
    else
        pData->invalidatedRegion = *pArea;

    pData->invalidated = true;
}

static void OLEDGrLib_WriteCommandCont(uint8_t command, uint8_t *data, uint16_t length)
{
    // Command being written
    GPIO_write(oledDataCommand, 0x00);

    spiTrans.txBuf = (void*)&command;
    spiTrans.rxBuf = NULL;
    spiTrans.count = 1;
    SPI_transfer(spiHandle, &spiTrans);

    DELAY_US(5);

    if (data != NULL)
    {
        // Data being written
        GPIO_write(oledDataCommand, 0x01);

        DELAY_US(1);

        spiTrans.txBuf = data;
        spiTrans.count = length;
        SPI_transfer(spiHandle, &spiTrans);

        DELAY_US(5);
    }

}

void OLEDGrLib_init(SPI_Handle hSpi, uint32_t csPin, uint32_t rstPin, uint32_t dcPin, uint8_t orientation)
{
    spiHandle       = hSpi;
    oledChipSelect  = csPin;
	oledRemap		= SSD1351_DAT_SETREMAP_DEFAULT;
	oledDataCommand = dcPin;
    oledReset 		= rstPin;
    oledOrientation = orientation;

    spiTrans.rxBuf  = NULL;
    spiTrans.txBuf  = NULL;
    spiTrans.count  = 0;
	
	// We are initializing the display properly here
	if (hSpi != NULL)
	{
	    oledWidth=128;
	    oledHeight=96;

        ChipSelect();

        GPIO_write(oledReset, 1);

        DELAY_US(100000);

        GPIO_write(oledReset, 0);

        DELAY_US(100000);

        GPIO_write(oledReset, 1);

        DELAY_US(200000);

        oledRemap &= ~(0x1B);//clear bit 0,1,3,4

        switch(oledOrientation)
        {
        case ePORTRAIT:
            oledRemap |= SSD1351_DAT_SETREMAP_PORT;
            oledWidth=96;
            oledHeight=128;
            break;

        case eLANDSCAPE:
            oledRemap |= SSD1351_DAT_SETREMAP_LAND;
            break;

        case ePORTRAIT_FLIP:
            oledRemap |= SSD1351_DAT_SETREMAP_PORT_FLIP;
            oledWidth=96;
            oledHeight=128;
            break;

        case eLANDSCAPE_FLIP:
            oledRemap |= SSD1351_DAT_SETREMAP_LAND_FLIP;
            break;
        }

        {
            /* Physical display size
            */
            static const uint8_t    SSD_COMMANDLOCK1        =   0x12;
            static const uint8_t    SSD_COMMANDLOCK2        =   0xB1;
            static const uint8_t    SSD_MUXRATIO            =   0x7F;
            static const uint8_t    SSD_SETGPIO             =   0x0F;
            static const uint8_t    SSD_PRECHARGE2          =   0x01;
            static const uint8_t    SSD_FUNCTIONSELECT      =   0x01;
            static const uint8_t    SSD_DISPLAYOFFSET       =   0x00;
            static const uint8_t    SSD_PRECHARGE           =   0x32;
            static const uint8_t    SSD_CLOCKDIV            =   0xF1;
            static const uint8_t    SSD_SETVSL_A            =   0xA0;
            static const uint8_t    SSD_SETVSL_B            =   0xB5;
            static const uint8_t    SSD_SETVSL_C            =   0x55;
            static const uint8_t    SSD_VCOMH               =   0x05;
            static const uint8_t    SSD_MASTERCURRENT       =   0x0F;//0x06
            static const uint8_t    SSD_CONTRAST_A          =   0xC8;
            static const uint8_t    SSD_CONTRAST_B          =   0x80;
            static const uint8_t    SSD_CONTRAST_C          =   0xC8;
            uint8_t iCount=1;

            oledRemap |= ((1 << 5));

            // List of command/parameters to initialize the SSD1351 OLED display
            unsigned char ucOLEDInitList[] = {
                2, CMD_CMDLOCK, SSD_COMMANDLOCK1,
                2, CMD_CMDLOCK, SSD_COMMANDLOCK2,
                1, CMD_DISPLAYOFF,
                2, CMD_CLOCKDIV, SSD_CLOCKDIV,
                2, CMD_MUXRATIO, SSD_MUXRATIO,
                2, CMD_STARTLINE, 0,

                2, CMD_DISPLAYOFFSET, SSD_DISPLAYOFFSET,
                2, CMD_SETREMAP, oledRemap,  // set remap
                3, CMD_SETCOLUMN, 0x00, oledWidth,    // set column
                3, CMD_SETROW, 0x00, oledHeight,    // set row
                2, CMD_SETGPIO, SSD_SETGPIO,
                2, CMD_FUNCTIONSELECT, SSD_FUNCTIONSELECT,
                4, CMD_SETVSL, SSD_SETVSL_A, SSD_SETVSL_B, SSD_SETVSL_C,
                4, CMD_CONTRASTABC, SSD_CONTRAST_A, SSD_CONTRAST_B, SSD_CONTRAST_C,
                2, CMD_MASTERCURRENT, SSD_MASTERCURRENT,
                4, CMD_DISPLAYENHANCE, 0xA4,0x00,0x00,
                1, CMD_USELUT,
                // phase here
                2, CMD_PRECHARGE, SSD_PRECHARGE,
                2, CMD_PRECHARGE2, SSD_PRECHARGE2,
                2, CMD_VCOMH, SSD_VCOMH,

                1, CMD_NORMALDISPLAY,    // set normal display mode
                1, CMD_DISPLAYON,    // display ON
                0
            };
            uint8_t *s = ucOLEDInitList;

            while (iCount)
            {
              iCount = *s++;
              if (iCount != 0)
              {
                  OLEDGrLib_WriteCommandCont(*s++,s,iCount-1);
                  s+= iCount-1;
              }
            }
        }

        DeSelect();
	}
}

//*****************************************************************************
//
//! Draws a pixel on the screen.
//!
//! \param displayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the pixel.
//! \param lY is the Y coordinate of the pixel.
//! \param ulValue is the color of the pixel.
//!
//! This function sets the given pixel to a particular color.  The coordinates
//! of the pixel are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void OLEDGrLib_pixelDraw(const Graphics_Display *pDisplay, int16_t lX, int16_t lY,
                                 uint16_t ulValue)
{
#ifdef NON_VOLATILE_MEMORY_BUFFER
    PrepareMemoryWrite();
#endif

    ((DisplayOled_DisplayBuffer*)pDisplay->displayData)->buffer[(lY * pDisplay->width) + (lX)] = ulValue;

    Graphics_Rectangle area = {
        .xMax = lX,
        .xMin = lX,
        .yMax = lY,
        .yMin = lY
    };

    OLEDGrLib_Invalidate(pDisplay, &area);

#ifdef NON_VOLATILE_MEMORY_BUFFER
    FinishMemoryWrite();
#endif
}

//*****************************************************************************
//
//! Draws a horizontal sequence of pixels on the screen.
//!
//! \param displayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the first pixel.
//! \param lY is the Y coordinate of the first pixel.
//! \param lX0 is sub-pixel offset within the pixel data, which is valid for 1
//! or 4 bit per pixel formats.
//! \param lCount is the number of pixels to draw.
//! \param lBPP is the number of bits per pixel; must be 1, 4, or 8.
//! \param pucData is a pointer to the pixel data.  For 1 and 4 bit per pixel
//! formats, the most significant bit(s) represent the left-most pixel.
//! \param pucPalette is a pointer to the palette used to draw the pixels.
//!
//! This function draws a horizontal sequence of pixels on the screen, using
//! the supplied palette.  For 1 bit per pixel format, the palette contains
//! pre-translated colors; for 4 and 8 bit per pixel formats, the palette
//! contains 24-bit RGB values that must be translated before being written to
//! the display.
//!
//! \return None.
//
//*****************************************************************************
static void OLEDGrLib_drawMultiple(const Graphics_Display *pDisplay, int16_t lX,
                                    int16_t lY, int16_t lX0, int16_t lCount,
                                    int16_t lBPP,
                                    const uint8_t *pucData,
                                    const uint32_t *pucPalette)
{
    uint16_t Data;
    uint16_t *pData  = &((DisplayOled_DisplayBuffer*)pDisplay->displayData)->buffer[(lY * pDisplay->width) + (lX)];

    Graphics_Rectangle area = {
            .xMax = min(lX + lCount, pDisplay->width),
            .xMin = lX,
            .yMax = lY,
            .yMin = lY
        };

    OLEDGrLib_Invalidate(pDisplay, &area);

#ifdef NON_VOLATILE_MEMORY_BUFFER
    PrepareMemoryWrite();
#endif

    //
        // Determine how to interpret the pixel data based on the number of bits
        // per pixel.
        //
        switch(lBPP)
        {
        // The pixel data is in 1 bit per pixel format
        case 1:
        {
            // Loop while there are more pixels to draw
            while(lCount > 0)
            {
                // Get the next byte of image data
                Data = *pucData++;

                // Loop through the pixels in this byte of image data
                for(; (lX0 < 8) && lCount; lX0++, lCount--)
                {
                    // Draw this pixel in the appropriate color
                    *pData++ = ((uint32_t *)pucPalette)[(Data >> (7 - lX0)) & 1];
                }

                // Start at the beginning of the next byte of image data
                lX0 = 0;
            }
            // The image data has been drawn

            break;
        }

        // The pixel data is in 4 bit per pixel format
        case 4:
        {
            // Loop while there are more pixels to draw.  "Duff's device" is
            // used to jump into the middle of the loop if the first nibble of
            // the pixel data should not be used.  Duff's device makes use of
            // the fact that a case statement is legal anywhere within a
            // sub-block of a switch statement.  See
            // http://en.wikipedia.org/wiki/Duff's_device for detailed
            // information about Duff's device.
            switch(lX0 & 1)
            {
            case 0:

                while(lCount)
                {
                    // Get the upper nibble of the next byte of pixel data
                    // and extract the corresponding entry from the palette
                    *pData++ = (*(uint16_t *)(pucPalette + (*pucData >> 4)));

                    // Decrement the count of pixels to draw
                    lCount--;

                    // See if there is another pixel to draw
                    if(lCount)
                    {
                    case 1:
                        // Get the lower nibble of the next byte of pixel
                        // data and extract the corresponding entry from
                        // the palette
                        *pData++ = (*(uint16_t *)(pucPalette + (*pucData++ & 15)));

                        // Decrement the count of pixels to draw
                        lCount--;
                    }
                }
            }
            // The image data has been drawn.

            break;
        }

        // The pixel data is in 8 bit per pixel format
        case 8:
        {
            // Loop while there are more pixels to draw
            while(lCount--)
            {
                // Get the next byte of pixel data and extract the
                // corresponding entry from the palette
                *pData++ = (*(uint16_t *)(pucPalette + *pucData++));
            }
            // The image data has been drawn
            break;
        }

        //
        // We are being passed data in the display's native format.  Merely
        // write it directly to the display.  This is a special case which is
        // not used by the graphics library but which is helpful to
        // applications which may want to handle, for example, JPEG images.
        //
        case 16:
        {
            uint16_t usData;

            // Loop while there are more pixels to draw.

            while(lCount--)
            {
                // Get the next byte of pixel data and extract the
                // corresponding entry from the palette
                usData = *((uint16_t *)pucData);
                pucData += 2;

                *pData++ = usData;
            }
        }
    }

#ifdef NON_VOLATILE_MEMORY_BUFFER
    FinishMemoryWrite();
#endif
}
//*****************************************************************************
//
//! Draws a horizontal line.
//!
//! \param displayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX1 is the X coordinate of the start of the line.
//! \param lX2 is the X coordinate of the end of the line.
//! \param lY is the Y coordinate of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a horizontal line on the display.  The coordinates of
//! the line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void OLEDGrLib_lineDrawH(const Graphics_Display *pDisplay, int16_t lX1, int16_t lX2,
                                 int16_t lY, uint16_t ulValue)
{
    uint16_t *pData  = &((DisplayOled_DisplayBuffer*)pDisplay->displayData)->buffer[(lY * pDisplay->width) + (lX1)];

    Graphics_Rectangle area = {
            .xMax = lX2,
            .xMin = lX1,
            .yMax = lY,
            .yMin = lY
        };

    OLEDGrLib_Invalidate(pDisplay, &area);

#ifdef NON_VOLATILE_MEMORY_BUFFER
    PrepareMemoryWrite();
#endif

    //
    // Loop through the pixels of this horizontal line.
    //
    while(lX1++ <= lX2)
    {
       //
       // Write the pixel value.
       //
       *pData++ = ulValue;
    }

#ifdef NON_VOLATILE_MEMORY_BUFFER
    FinishMemoryWrite();
#endif
}


//*****************************************************************************
//
//! Draws a vertical line.
//!
//! \param displayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the line.
//! \param lY1 is the Y coordinate of the start of the line.
//! \param lY2 is the Y coordinate of the end of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a vertical line on the display.  The coordinates of the
//! line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void OLEDGrLib_lineDrawV(const Graphics_Display *pDisplay, int16_t lX, int16_t lY1,
                                 int16_t lY2, uint16_t ulValue)
{
    uint16_t *pData  = &((DisplayOled_DisplayBuffer*)pDisplay->displayData)->buffer[(lY1 * pDisplay->width) + (lX)];

    Graphics_Rectangle area = {
            .xMax = lX,
            .xMin = lX,
            .yMax = lY2,
            .yMin = lY1
        };

    OLEDGrLib_Invalidate(pDisplay, &area);

#ifdef NON_VOLATILE_MEMORY_BUFFER
    PrepareMemoryWrite();
#endif

    //
    // Loop through the pixels of this horizontal line.
    //
    while (lY1++ <= lY2)
    {
        //
        // Write the pixel value.
        //
        *pData = ulValue;

        // Add the width as we're going down one line
        pData += pDisplay->width;
    }


#ifdef NON_VOLATILE_MEMORY_BUFFER
    FinishMemoryWrite();
#endif
}

//*****************************************************************************
//
//! Fills a rectangle.
//!
//! \param displayData is a pointer to the driver-specific data for this
//! display driver.
//! \param pRect is a pointer to the structure describing the rectangle.
//! \param ulValue is the color of the rectangle.
//!
//! This function fills a rectangle on the display.  The coordinates of the
//! rectangle are assumed to be within the extents of the display, and the
//! rectangle specification is fully inclusive (in other words, both sXMin and
//! sXMax are drawn, along with sYMin and sYMax).
//!
//! \return None.
//
//*****************************************************************************
static void OLEDGrLib_rectFill(const Graphics_Display *pDisplay, const tRectangle *pRect,
                                uint16_t ulValue)
{
    int x0 = pRect->sXMin;
    int x1 = pRect->sXMax;
    int y0 = pRect->sYMin;
    int y1 = pRect->sYMax;

    OLEDGrLib_Invalidate(pDisplay, (Graphics_Rectangle*)pRect);

    // Less horizontal lines
    if ((x1 - x0) > (y1 - y0))     // Less horizontal lines
    {
        for (; y0 <= y1; y0++)
        {
            OLEDGrLib_lineDrawH(pDisplay, x0, x1, y0,
                                         ulValue);
        }
    }
    else                          // Less vertical lines
    {
        for (; x0 <= x1; x0++)
        {
            OLEDGrLib_lineDrawV(pDisplay, x0, y0, y1,
                                         ulValue);
        }
    }
}

//*****************************************************************************
//
//! Translates a 24-bit RGB color to a display driver-specific color.
//!
//! \param displayData is a pointer to the driver-specific data for this
//! display driver.
//! \param ulValue is the 24-bit RGB color.  The least-significant byte is the
//! blue channel, the next byte is the green channel, and the third byte is the
//! red channel.
//!
//! This function translates a 24-bit RGB color into a value that can be
//! written into the display's frame buffer in order to reproduce that color,
//! or the closest possible approximation of that color.
//!
//! \return Returns the display-driver specific color.
//
//*****************************************************************************
static uint32_t OLEDGrLib_colorTranslate(const Graphics_Display *pDisplay,
                                          uint32_t ulValue)
{
    //
    // Translate from a 24-bit RGB color to a 5-6-5 RGB color.
    //
    return(((((ulValue) & 0x00F80000) >> 8) |
            (((ulValue) & 0x0000FC00) >> 5) |
            (((ulValue) & 0x000000F8) >> 3)));
}


//*****************************************************************************
//
//! Flushes any cached drawing operations.
//!
//! \param displayData is a pointer to the driver-specific data for this
//! display driver.
//!
//!
//! This functions flushes any cached drawing operations to the display.  This
//! is useful when a local frame buffer is used for drawing operations, and the
//! flush would copy the local frame buffer to the display.
//!
//! \return None.
//
//*****************************************************************************
static void OLEDGrLib_flush(const Graphics_Display *pDisplay)
{
    DisplayOled_DisplayBuffer * pData = (DisplayOled_DisplayBuffer*)pDisplay->displayData;
    Graphics_Rectangle  drawArea;

    uint8_t   widthStr[2] = {0,0};
    uint8_t   heightStr[2]= {0,0};

    // Check if valid handles since this is a somewhat opaque API
    if (NULL == spiHandle)
    {
        return;
    }

    ChipSelect();

    if ( pData->invalidated == true && Graphics_getRectangleIntersection( &pData->invalidatedRegion, &pData->useableRegion, &drawArea))
    {
        uint_fast8_t width = drawArea.xMax - drawArea.xMin + 1;
        uint_fast8_t x,y;

        widthStr[0] = drawArea.xMin;
        widthStr[1] = drawArea.xMax;
        heightStr[0] = drawArea.yMin;
        heightStr[1] = drawArea.yMax;

        // Write the entire display
        OLEDGrLib_WriteCommandCont(SSD1351_CMD_SETCOLUMN,widthStr,2);
        OLEDGrLib_WriteCommandCont(SSD1351_CMD_SETROW,heightStr,2);

        // One complete line at a time
        for (x=drawArea.xMin,y=drawArea.yMin ; y <= drawArea.yMax; y++)
        {
            OLEDGrLib_WriteCommandCont(
                    SSD1351_CMD_WRITERAM, (uint8_t*)&((DisplayOled_DisplayBuffer*)pDisplay->displayData)->buffer[(y * pDisplay->width) + (x)],
                    width * sizeof(uint16_t));
        }

    }
    else
    {
        widthStr[1] = oledWidth -1;
        heightStr[1] = oledHeight -1;

        // Write the entire display
        OLEDGrLib_WriteCommandCont(SSD1351_CMD_SETCOLUMN,widthStr,2);
        OLEDGrLib_WriteCommandCont(SSD1351_CMD_SETROW,heightStr,2);

        OLEDGrLib_WriteCommandCont(SSD1351_CMD_WRITERAM, (uint8_t*)((DisplayOled_DisplayBuffer*)pDisplay->displayData)->buffer,
                                   pDisplay->width * pDisplay->height * sizeof(uint16_t));
    }

    pData->invalidated = false;

    DeSelect();

}

//*****************************************************************************
//
//! Send command to clear screen.
//!
//! \param displayData is a pointer to the driver-specific data for this
//! display driver.
//! \param ucValue is the background color of the buffered data.
//!
//! This function sets every pixel to the background color.
//!
//! \return None.
//
//*****************************************************************************
static void OLEDGrLib_clearScreen(const Graphics_Display *pDisplay, uint16_t ulValue)
{
    // Check if valid handles since this is a somewhat opaque API
    if (NULL == spiHandle)
    {
        return;
    }

    ChipSelect();

    OLEDGrLib_WriteCommandCont(SSD1351_CMD_DISPLAYALLOFF,NULL, 0);

    DeSelect();

    if (ClrBlack == ulValue)
        OLEDGrLib_initializeDisplayBuffer(pDisplay, 0x00);
    else
        OLEDGrLib_initializeDisplayBuffer(pDisplay, 0xFF);
}

//*****************************************************************************
//
//! Initialize DisplayBuffer.
//!
//! \param displayData is a pointer to the driver-specific data for this
//! display driver.
//!
//!    \param ucValue is the foreground color of the buffered data.
//!
//! This function initializes the display buffer and discards any cached data.
//!
//! \return None.
//
//*****************************************************************************
static void OLEDGrLib_initializeDisplayBuffer(const Graphics_Display *pDisplay, uint8_t ucValue)
{
#ifdef USE_FLASH_BUFFER
    // This is a callback function to HAL file since it implements device specific
    // functionality
    InitializeDisplayBuffer(pDisplay->displayData, ucValue);

#else
    memset(((DisplayOled_DisplayBuffer*)pDisplay->displayData)->buffer, (ucValue >> 8), pDisplay->width * pDisplay->height * sizeof(uint16_t));
#endif //USE_FLASH_BUFFER
}

//*****************************************************************************
//
//! The display structure that describes the driver for the
//! sharpLCD panel
//
//*****************************************************************************
const Graphics_Display_Functions g_oledFxns =
{
    OLEDGrLib_pixelDraw,     //PixelDraw,
    OLEDGrLib_drawMultiple,
    OLEDGrLib_lineDrawH,
    OLEDGrLib_lineDrawV,  //LineDrawV,
    OLEDGrLib_rectFill,   //RectFill,
    OLEDGrLib_colorTranslate,
    OLEDGrLib_flush,      //Flush
    OLEDGrLib_clearScreen //Clear screen. Contents of display buffer unmodified
};


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
