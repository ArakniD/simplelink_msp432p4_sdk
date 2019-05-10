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
// OledGrLib.h - Prototypes for the Sharp LCD graphicslib back-end
//
//*****************************************************************************


#ifndef __OLED_H__
#define __OLED_H__

#include <ti/drivers/SPI.h>

//*****************************************************************************
//
// User Configuration for the LCD Driver
//
//*****************************************************************************

//Maximum Colors in an image color palette
#define MAX_PALETTE_COLORS                65536

//*****************************************************************************
//
// Various internal SSD1351 registers name labels
//
//*****************************************************************************
#define SSD1351_CMD_SETCOLUMN       0x15
#define SSD1351_CMD_SETROW          0x75
#define SSD1351_CMD_WRITERAM        0x5C
#define SSD1351_CMD_READRAM         0x5D
#define SSD1351_CMD_SETREMAP        0xA0
#define SSD1351_DAT_SETREMAP_LAND		((1 << 4))
#define SSD1351_DAT_SETREMAP_PORT		((1 << 4) | (1 << 1) | (1 << 0))
#define SSD1351_DAT_SETREMAP_LAND_FLIP  ((1 << 1))
#define SSD1351_DAT_SETREMAP_PORT_FLIP  ((1 << 0))
#define SSD1351_DAT_SETREMAP_DEFAULT    ((0 << 7) | (1 << 6) | (1 << 4))
#define SSD1351_CMD_SETREMAP_ROW	0x00
#define SSD1351_CMD_SETREMAP_COL	0x01
#define SSD1351_CMD_STARTLINE       0xA1
#define SSD1351_CMD_DISPLAYOFFSET   0xA2
#define SSD1351_CMD_DISPLAYALLOFF   0xA4
#define SSD1351_CMD_DISPLAYALLON    0xA5
#define SSD1351_CMD_NORMALDISPLAY   0xA6
#define SSD1351_CMD_INVERTDISPLAY   0xA7
#define SSD1351_CMD_FUNCTIONSELECT  0xAB
#define SSD1351_CMD_DISPLAYOFF      0xAE
#define SSD1351_CMD_DISPLAYON       0xAF
#define SSD1351_CMD_PRECHARGE       0xB1
#define SSD1351_CMD_DISPLAYENHANCE  0xB2
#define SSD1351_CMD_CLOCKDIV        0xB3
#define SSD1351_CMD_SETVSL          0xB4
#define SSD1351_CMD_SETGPIO         0xB5
#define SSD1351_CMD_PRECHARGE2      0xB6
#define SSD1351_CMD_SETGRAY         0xB8
#define SSD1351_CMD_USELUT          0xB9
#define SSD1351_CMD_PRECHARGELEVEL  0xBB
#define SSD1351_CMD_VCOMH           0xBE
#define SSD1351_CMD_CONTRASTABC     0xC1
#define SSD1351_CMD_CONTRASTMASTER  0xC7
#define SSD1351_CMD_MUXRATIO        0xCA
#define SSD1351_CMD_COMMANDLOCK     0xFD
#define SSD1351_CMD_HORIZSCROLL     0x96
#define SSD1351_CMD_STOPSCROLL      0x9E
#define SSD1351_CMD_STARTSCROLL     0x9F

//registers-----------------------
static const uint8_t CMD_NOP                = 0xAD;//B0//D1//E3
static const uint8_t CMD_WRITERAM           = 0x5C;
static const uint8_t CMD_READRAM            = 0x5D;
static const uint8_t CMD_FUNCTIONSELECT     = 0xAB;
static const uint8_t CMD_DISPLAYENHANCE     = 0xB2;
static const uint8_t CMD_SETVSL             = 0xB4;
static const uint8_t CMD_SETGPIO            = 0xB5;
static const uint8_t CMD_PRECHARGE2         = 0xB6;
static const uint8_t CMD_USELUT             = 0xB9;
static const uint8_t CMD_CONTRASTABC        = 0xC1;
static const uint8_t CMD_MASTERCURRENT      = 0xC7;
static const uint8_t CMD_MUXRATIO           = 0xCA;
static const uint8_t CMD_CMDLOCK            = 0xFD;
static const uint8_t CMD_HORIZSCROLL        = 0x96;
static const uint8_t CMD_SCROLL_OFF         = 0x9E;//SSD1351
static const uint8_t CMD_SCROLL_ON          = 0x9F;//SSD1351
static const uint8_t CMD_STARTLINE          = 0xA1;//SSD1351
static const uint8_t CMD_DISPLAYALLON       = 0xA5;//SSD1351
static const uint8_t CMD_DISPLAYALLOFF      = 0xA6;//SSD1351
static const uint8_t CMD_SETREMAP           = 0xA0;//SSD1351
static const uint8_t CMD_DISPLAYOFFSET      = 0xA2;//SSD1351
static const uint8_t CMD_PRECHARGE          = 0xB1;//SSD1351
static const uint8_t CMD_CLOCKDIV           = 0xB3;//SSD1351
static const uint8_t CMD_GRAYSCALE          = 0xB8;
static const uint8_t CMD_PRECHARGELEVEL     = 0xBB;//SSD1351
static const uint8_t CMD_VCOMH              = 0xBE;//SSD1351
static const uint8_t CMD_NORMALDISPLAY      = 0xA6;//SSD1351
static const uint8_t CMD_INVERTDISPLAY      = 0xA7;//SSD1351
static const uint8_t CMD_DISPLAYOFF         = 0xAE;//SSD1351
static const uint8_t CMD_DISPLAYON          = 0xAF;//SSD1351
static const uint8_t CMD_SETCOLUMN          = 0x15;//SSD1351
static const uint8_t CMD_SETROW             = 0x75;//SSD1351
static const uint8_t CMD_SLEEPON            = 0xAE;//SSD1351
static const uint8_t CMD_SLEEPOFF           = 0xAF;//SSD1351

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern const Graphics_Display_Functions g_oledFxns;

extern void OLEDGrLib_init(SPI_Handle hSpi, uint32_t csPin, uint32_t rstPin, uint32_t dcPin, uint8_t orientation);

#endif // __OLED_H__
