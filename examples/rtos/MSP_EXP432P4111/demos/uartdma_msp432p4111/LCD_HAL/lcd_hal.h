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

/*
 *  ======== lcd_hal.h ========
 */

#ifndef LCD_HAL_H_
#define LCD_HAL_H_

#define LCD_POSITION_1    16    // LCD Digit A1 - Pin L16
#define LCD_POSITION_2    32    // LCD Digit A2 - Pin L32
#define LCD_POSITION_3    40    // LCD Digit A3 - Pin L40
#define LCD_POSITION_4    36    // LCD Digit A4 - Pin L36
#define LCD_POSITION_5    28    // LCD Digit A5 - Pin L28
#define LCD_POSITION_6    44    // LCD Digit A6 - Pin L44

typedef enum {SET, CLEAR, TOGGLE} lcd_operation_type;

//*****************************************************************************
//
//! \brief Initializes LCD.
//!
//! This function initializes the LCD_F module on MSP432P4111 for the
//! FH-1138P LCD on the MSP-EXP432P4111 LaunchPad.
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************
void LCD_init(void);

//*****************************************************************************
//
//! \brief Clears all LCD memories.
//!
//! This function clears the main, blink, and animation memory of the LCD
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************
void LCD_clear(void);

//*****************************************************************************
//
//! \brief Shows a character on LCD.
//!
//! This function displays a single alphanumeric character on the
//! specified position on the LCD.
//! Only upper-case A-Z and numbers 0-9 are allowed. Otherwise, all
//! LCD segments are cleared on the specified LCD position.
//!
//! \param c is the alphanumeric character to be displayed.
//!
//! \param position is the position/place on the LCD to display the character
//!        Valid values are:
//!        - \b LCD_POSITION_1
//!        - \b LCD_POSITION_2
//!        - \b LCD_POSITION_3
//!        - \b LCD_POSITION_4
//!        - \b LCD_POSITION_5
//!        - \b LCD_POSITION_6
//!
//! \return None
//
//*****************************************************************************
void LCD_showChar(char character, int position);

//*****************************************************************************
//
//! \brief Shows a string on LCD.
//!
//! This function displays a string on the LCD (Left-justified). Strings
//! longer than 6 characters will be truncated on the right.
//! Only upper-case A-Z and numbers 0-9 are allowed.
//!
//! \param string is the character array pointer containing string to be shown.
//!
//!
//! \return None
//
//*****************************************************************************
void LCD_showString(char *string);

//*****************************************************************************
//
//! \brief Scrolls a string on LCD.
//!
//! This function scrolls a string on the LCD from right to left (Marquee).
//! Only upper-case A-Z and numbers 0-9 are allowed.
//!
//! \param string is the character array pointer containing string to be scrolled.
//!
//! \param delay is the number of microsecond delay between scrolling frames.
//!
//! \return None
//
//*****************************************************************************
void LCD_scrollString(char *string, int delay);

//*****************************************************************************
//
//! \brief Shows temperature in degree Celsius on LCD.
//!
//! This function formats and displays a Celsius temperature on the LCD.
//! It formats the temperature values to 2 decimal precisions and handles
//! displaying numbers ranging from -999.99 to 999.99.
//!
//! \param temperature is a float number storing the Celsius temperature value.
//!
//!
//! \return None
//
//*****************************************************************************
void LCD_showTemperatureC(float temperature);

//*****************************************************************************
//
//! \brief Starts LCD animation.
//!
//! This function configures the LCD animation control register and enables LCD
//! animation. It also configures the animation memory to display a simple
//! battery charging animation.
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************
void LCD_startAnimation(void);

//*****************************************************************************
//
//! \brief Stops LCD animation.
//!
//! This function disables LCD animation.
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************
void LCD_stopAnimation(void);

//*****************************************************************************
//
//! \brief Updates LCD main memory.
//!
//! This function allows direct updates to the LCD module's main memory by
//! performing bitwise operations.
//!
//! \param lcdMemory is the desired LCD memory # to update
//!
//! \param byte is the bit mask for setting, clearing, or toggling the memory bits
//!
//! \param operation is the bit wise operation to be performed using the mask
//!        Valid values are:
//!        - \b SET
//!        - \b CLEAR
//!        - \b TOGGLE
//!
//! \return None
//
//*****************************************************************************
void LCD_updateMemory(int lcdMemory, char byte, char operation);

//*****************************************************************************
//
//! \brief Updates LCD animation memory.
//!
//! This function allows direct updates to the LCD module's animation memory by
//! performing bitwise operations.
//!
//! \param lcdMemory is the desired LCD animation memory # to update
//!
//! \param byte is the bit mask for setting, clearing, or toggling the memory bits
//!
//! \param operation is the bit wise operation to be performed using the mask
//!        Valid values are:
//!        - \b SET
//!        - \b CLEAR
//!        - \b TOGGLE
//!
//! \return None
//
//*****************************************************************************
void LCD_updateAnimationMemory(int lcdMemory, char byte, char operation);

#endif /* LCD_HAL_H_ */
