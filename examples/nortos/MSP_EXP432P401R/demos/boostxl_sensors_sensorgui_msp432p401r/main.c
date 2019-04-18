//*****************************************************************************
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  Redistributions of source code must retain the above copyright
//  notice, this list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright
//  notice, this list of conditions and the following disclaimer in the
//  documentation and/or other materials provided with the
//  distribution.
//
//  Neither the name of Texas Instruments Incorporated nor the names of
//  its contributors may be used to endorse or promote products derived
//  from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// MSP432 Low Power main.c
//
// 		Description: Source code for the implementation of the Bosch BoosterPack with
// 		an MSP432P401R launchpad for low power consumption. BoosterPack includes:
// 		- Inertial Measurement Unit (IMU) sensor with an accelerometer and gyroscope
// 		- Magnetometer
// 		- Environmental sensor with pressure, ambient temperature and humidity
// 		- Ambient light sensor
// 		- Infrared temperature sensor
//
// Adapted by Michael Arriete
//
//****************************************************************************
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ti/devices/msp432p4xx/inc/msp432.h>
#include "stdio.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "i2c_driver.h"
#include "demo_sysctl.h"
#include "bmi160_support.h"
#include "bme280_support.h"
#include "tmp007.h"
#include "opt3001.h"
#include "uart_driver.h"

//***** Definitions *****
#define CPU_FREQ                    (48000000)
#define USING_BOSCH_BP
#define SAMPLE_TIME_1				(53)
#define SAMPLE_TIME_2				(26)
#define SAMPLE_TIME_4				(13)
#define SAMPLE_TIME_6				(8)
#define SAMPLE_TIME_8				(6)
#define SAMPLE_TIME_10				(5)
#define NUM_AVGR_SUMS				2 //x^2 frames

//***** Function Prototypes *****
void startCrystalOscillator(void);
void setSystemClock(uint32_t CPU_Frequency);
void configureGPIO(void);
void startWakeUpTimerA(uint16_t ulClockMS);
void stopWakeUpTimerA(void);
int32_t movingAvg(int prevAvg, int16_t newValue);


//***** Global Data *****
const uint8_t wdtWakeUpPeriod [8] = {
		WDT_A_CLOCKDIVIDER_2G,
		WDT_A_CLOCKDIVIDER_128M,
		WDT_A_CLOCKDIVIDER_8192K,
		WDT_A_CLOCKDIVIDER_512K,
		WDT_A_CLOCKDIVIDER_32K,
		WDT_A_CLOCKDIVIDER_8192,
		WDT_A_CLOCKDIVIDER_512,
		WDT_A_CLOCKDIVIDER_64,
};

const uint8_t timeSamplesBMI [6] = {
		SAMPLE_TIME_1, 		//Sample at 1 time per second
		SAMPLE_TIME_2, 		//Sample at 2 times per second
		SAMPLE_TIME_4, 		//Sample at 4 times per second
		SAMPLE_TIME_6, 		//Sample at 6 times per second
		SAMPLE_TIME_8, 		//Sample at 8 times per second
		SAMPLE_TIME_10, 	//Sample at 10 times per second
};

const uint8_t timeSamplesBMM [6] = {
		SAMPLE_TIME_1, 		//Sample at 1 time per second
		SAMPLE_TIME_2, 		//Sample at 2 times per second
		SAMPLE_TIME_4, 		//Sample at 4 times per second
		SAMPLE_TIME_6, 		//Sample at 6 times per second
		SAMPLE_TIME_8, 		//Sample at 8 times per second
		SAMPLE_TIME_10, 	//Sample at 10 times per second
};

const uint8_t timeSamplesBME [6] = {
		SAMPLE_TIME_1, 		//Sample at 1 time per second
		SAMPLE_TIME_2, 		//Sample at 2 times per second
		SAMPLE_TIME_4, 		//Sample at 4 times per second
		SAMPLE_TIME_6, 		//Sample at 6 times per second
		SAMPLE_TIME_8, 		//Sample at 8 times per second
		SAMPLE_TIME_10, 	//Sample at 10 times per second
};

const uint8_t timeSamplesTMP [6] = {
		SAMPLE_TIME_1, 		//Sample at 1 time per second
		SAMPLE_TIME_2, 		//Sample at 2 times per second
		SAMPLE_TIME_4, 		//Sample at 4 times per second
		SAMPLE_TIME_6, 		//Sample at 6 times per second
		SAMPLE_TIME_8, 		//Sample at 8 times per second
		SAMPLE_TIME_10, 	//Sample at 10 times per second
};

const uint8_t timeSamplesOPT [6] = {
		SAMPLE_TIME_1, 		//Sample at 1 time per second
		SAMPLE_TIME_2, 		//Sample at 2 times per second
		SAMPLE_TIME_4, 		//Sample at 4 times per second
		SAMPLE_TIME_6, 		//Sample at 6 times per second
		SAMPLE_TIME_8, 		//Sample at 8 times per second
		SAMPLE_TIME_10, 	//Sample at 10 times per second
};

//Default time sample values for each sensor
volatile uint8_t sampleTimePeriodBMI = 2;
volatile uint8_t sampleTimePeriodBMM = 5;
volatile uint8_t sampleTimePeriodBME = 0;
volatile uint8_t sampleTimePeriodTMP = 0;
volatile uint8_t sampleTimePeriodOPT = 0;

// Changed by the GUI -  default ~ 0.0156 seconds  1/32KHz * WDT_A_CLOCKDIVIDER_512
volatile uint8_t wdtWakeUpPeriodIndex = 6;

// BMI160/BMM150
BMI160_RETURN_FUNCTION_TYPE returnValue;
struct bmi160_gyro_t        s_gyroXYZ;
struct bmi160_accel_t       s_accelXYZ;
struct bmi160_mag_xyz_s32_t s_magcompXYZ;

// BME280 
s32 returnRslt;
s32 g_s32ActualTemp   = 0;
u32 g_u32ActualPress  = 0;
u32 g_u32ActualHumity = 0;

// OPT3001
uint16_t rawData;
float    convertedLux;

// TMP007
uint16_t rawTemp;
uint16_t rawObjTemp;
float    tObjTemp;
float    tObjAmb;

//Timer Counter
uint16_t WDTcount = 0;

//Receive UART Variables
#define NUM_RX_CHARS 64
char rxMsgData[NUM_RX_CHARS] = "";
int numMsgsRx = 0;
int tempIndex = 5;
int numChars = 0;

//Calibration off-sets
int8_t accel_off_x;
int8_t accel_off_y;
int8_t accel_off_z;
s16 gyro_off_x;
s16 gyro_off_y;
s16 gyro_off_z;

//gesture recognition
int getGestures = 1;
int dominant = 0;
uint16_t gyroAbsX, gyroAbsY, gyroAbsZ;
uint16_t deltaAccelX, deltaAccelY, deltaAccelZ;
int16_t prevAccelX = 0;
int16_t prevAccelY = 0;
int16_t prevAccelZ = 0;
int16_t prevGyroX = 0;
int16_t prevGyroY = 0;
int16_t prevGyroZ = 0;
int16_t stillCount = 0;
int32_t gyroAvgX = 0.0;
int32_t gyroAvgY = 0.0;
int32_t gyroAvgZ = 0.0;
int32_t accelAvgX = 0.0;
int32_t accelAvgY = 0.0;
int32_t accelAvgZ = 0.0;

//Sensor Status Variables
bool BME_on = true;
bool BMI_on = true;
bool TMP_on = false;
bool OPT_on = true;


/***********************************************************
  Function:
 */
int main(void)
{
	volatile uint32_t index;

	// Stop WDT and disabling master interrupts
	MAP_WDT_A_holdTimer();
	MAP_Interrupt_disableMaster();

	// Enabling SRAM Bank Retention
	SYSCTL->SRAM_BANKRET |= SYSCTL_SRAM_BANKEN_BNK7_EN;
	for (index = 0;index < 100;index++);

#ifdef USE_LPM	
	// Configure I/O to minimize power consumption
	configureGPIO();

	// Turn off PSS high-side & low-side supervisors to minimize power consumption
	MAP_PSS_disableLowSide();
	MAP_PSS_disableHighSide();
#endif

	// Configure clocks
	startCrystalOscillator();
	setSystemClock(CPU_FREQ);

#ifdef USING_BOSCH_BP
	//Initialize uart
	uartInit();

	// Initialize i2c
	initI2C();

#endif	

	MAP_Interrupt_enableMaster();

#ifdef USING_BOSCH_BP	
	// Initialize bmi160 sensor
	bmi160_initialize_sensor();
	returnRslt = bmi160_config_running_mode(APPLICATION_NAVIGATION);
	bmi160_accel_foc_trigger_xyz(0x03, 0x03, 0x01, &accel_off_x, &accel_off_y, &accel_off_z);
	bmi160_set_foc_gyro_enable(0x01, &gyro_off_x, &gyro_off_y, &gyro_off_z);

	// Initialize bme280 sensor
	bme280_data_readout_template();
	returnRslt = bme280_set_power_mode(BME280_SLEEP_MODE);

	//Initialize opt3001 sensor
	sensorOpt3001Init();

	//Initialize tmp007 sensor
	sensorTmp007Init();
#endif

	//Keep TMP sensors disable by default (TMP007 is end-of-lifed and is not populated on newer BoosterPacks)
	sensorTmp007Enable(false);

	//Enable OPT, and BME sensors
	sensorOpt3001Enable(true);
	returnRslt = bme280_set_power_mode(BME280_NORMAL_MODE);

	// Using the wdt as interval timer to wake up from LPM3
	while(1)
	{
		// Stop WDT
		MAP_WDT_A_holdTimer();
		MAP_WDT_A_clearTimer();
		MAP_Interrupt_disableInterrupt(INT_WDT_A);

		//Set frequency back to full speed if reading/converting data
		if((WDTcount%timeSamplesBME[sampleTimePeriodBME] == 0) ||
				(WDTcount%timeSamplesBMM[sampleTimePeriodBMM] == 0) ||
				(WDTcount%timeSamplesBMI[sampleTimePeriodBMI] == 0) ||
				(WDTcount%timeSamplesTMP[sampleTimePeriodTMP] == 0) ||
				(WDTcount%timeSamplesOPT[sampleTimePeriodOPT] == 0) ||
				(WDTcount == 53))
		{
			if (getGestures)
			{

				gyroAbsX = abs(s_gyroXYZ.x);
				gyroAbsY = abs(s_gyroXYZ.y);
				gyroAbsZ = abs(s_gyroXYZ.z);

				gyroAvgX = movingAvg(gyroAvgX, gyroAbsX );
				gyroAvgY = movingAvg(gyroAvgY, gyroAbsY );
				gyroAvgZ = movingAvg(gyroAvgZ, gyroAbsZ );



				if( gyroAvgX > 1000 || gyroAvgY > 1000 || gyroAvgZ > 1000 )
				{
					stillCount = 0;
					if ( gyroAvgY > gyroAvgX )
					{
						if ( gyroAvgY > gyroAvgZ )
						{
							dominant = 2; //gyro Y
						}
						else
						{
							dominant = 3; //gyro Z
						}

					}
					else if ( gyroAvgZ > gyroAvgX )
					{
						dominant = 3; //gyro Z
					}
					else
					{
						dominant = 1; //gyro X
					}
				}
				else{

					deltaAccelX = abs(s_accelXYZ.x - prevAccelX);
					deltaAccelY = abs(s_accelXYZ.y - prevAccelY);
					deltaAccelZ = abs(s_accelXYZ.z - prevAccelZ);

					accelAvgX = movingAvg(accelAvgX, deltaAccelX);
					accelAvgY = movingAvg(accelAvgY, deltaAccelY);
					accelAvgZ = movingAvg(accelAvgZ, deltaAccelZ);


					if ( accelAvgX > 100 || accelAvgY > 100 || accelAvgZ > 100 )
					{
						stillCount = 0;

						if ( accelAvgY > accelAvgX )
						{
							if ( accelAvgY > accelAvgZ )
							{
								dominant = 5; //accel Y
							}
							else
							{
								dominant = 6; //accel Z
							}

						}
						else if ( accelAvgZ > accelAvgX )
						{
							dominant = 6; //accel Z
						}
						else
						{
							dominant = 4; //accel X
						}

					}
					else{
						stillCount++;
						if (stillCount>3)
						{
							dominant = 0; //still condition
						}

					}

				}
//				//contrsuct JSON string for GUI
//				snprintf(test.txString, 30,
//						"{\"dominant\":{\"code\":%d}}\n",
//						dominant);
//
//				//Send UART data JSON string
//				sendText();

				prevAccelX = s_accelXYZ.x;
				prevAccelY = s_accelXYZ.y;
				prevAccelZ = s_accelXYZ.z;
			}
		}

		//Mag update frequency
		if (WDTcount%timeSamplesBMM[sampleTimePeriodBMM] == 0)
		{
			if(BMI_on)
			{
				//Read Mag value (BMM) through BMI
				returnValue = bmi160_bmm150_mag_compensate_xyz(&s_magcompXYZ);

				//contrsuct JSON string for BMM
				snprintf(test.txString, 60,
						"{\"mag\":{\"x\":%d,\"y\":%d,\"z\":%d}}\n",
						s_magcompXYZ.x, s_magcompXYZ.y, s_magcompXYZ.z);

				//Send UART data JSON string
				sendText();
			}
		}
		//Accel and Gyro update frequency
		if (WDTcount%timeSamplesBMI[sampleTimePeriodBMI] == 0)
		{
			if(BMI_on)
			{
				//Read Accel and Gyro values
				returnValue = bmi160_read_accel_xyz(&s_accelXYZ);
				returnValue = bmi160_read_gyro_xyz(&s_gyroXYZ);

				//contrsuct JSON string for BMI
				snprintf(test.txString, 120,
						"{\"dominant\":{\"code\":%d},"
						"\"gyro\":{\"x\":%d,\"y\":%d,\"z\":%d},"
						"\"accel\":{\"x\":%d,\"y\":%d,\"z\":%d}}\n",
						dominant,
						s_gyroXYZ.x, s_gyroXYZ.y, s_gyroXYZ.z,
						s_accelXYZ.x, s_accelXYZ.y, s_accelXYZ.z);

				//Send UART data JSON string
				sendText();
			}
		}
		//**Will need to separate each sensor into own if statements to allow GUI to change each sensor's sampling rates
		//TMP update frequency
		if (WDTcount%timeSamplesTMP[sampleTimePeriodTMP] == 0)
		{
			if(TMP_on)
			{
				// Read/convert tmp007 and opt3001 data
				sensorTmp007Read(&rawTemp, &rawObjTemp);
				sensorTmp007Convert(rawTemp, rawObjTemp, &tObjTemp, &tObjAmb);

				//contrsuct JSON string for TMP
				snprintf(test.txString, 60,
						"{\"ir_temp\":{\"obj_temp\":%5.2f}}\n",
						tObjTemp);

				//Send UART data JSON string
				sendText();
			}
		}
		//OPT update frequency
		if (WDTcount%timeSamplesOPT[sampleTimePeriodOPT] == 0) {
			if(OPT_on)
			{
				//Read and convert OPT values
				sensorOpt3001Read(&rawData);
				sensorOpt3001Convert(rawData, &convertedLux);

				//contrsuct JSON string for OPT
				snprintf(test.txString, 60,
						"{\"light\":{\"lux\":%5.2f}}\n",
						convertedLux);

				//Send UART data JSON string
				sendText();
			}
		}
		//BME update frequency
		if (WDTcount%timeSamplesBME[sampleTimePeriodBME] == 0) {
			if(BME_on)
			{
				//Read BME environmental data
				returnRslt = bme280_read_pressure_temperature_humidity(
						&g_u32ActualPress, &g_s32ActualTemp, &g_u32ActualHumity);

				//contrsuct JSON string for BME
				snprintf(test.txString, 60,
						"{\"enviro\":{\"humid\":%d,\"press\":%d,\"amb_temp\":%d}}\n",
						g_u32ActualHumity, g_u32ActualPress, g_s32ActualTemp);

				//Send UART data JSON string
				sendText();
			}
		}
		//Reset WDTcount when max count has been reached
		if (WDTcount == 53) {
			//Reset WDT counter after reaching roughly one second
			WDTcount = 0;
		}
		WDTcount++;

		// Check to see if there is any message from the host
		if (receiveText(rxMsgData,NUM_RX_CHARS)){
			numMsgsRx++;
			numChars = strlen(rxMsgData);

			//Parses through UART received data and changes sampling rates and on/off statuses accordingly
			if (numChars > 6) {
				switch(rxMsgData[2]){
				case 's':  // e.g. {"s":10} or {"s":8} or {"s":4) to set sampling rates of 10Hz, 8Hz or 4Hz
					switch(rxMsgData[3]){
					case 'i':
						tempIndex = 6;
						if (rxMsgData[tempIndex] == '"'){
							tempIndex = 7;  // this accommodates values that have been converted to strings
						}
						switch (rxMsgData[tempIndex]){
						case '0':
							sampleTimePeriodBMI = 0;
							break;
						case '1':
							sampleTimePeriodBMI = 1;
							break;
						case '2':
							sampleTimePeriodBMI = 2;
							break;
						case '3':
							sampleTimePeriodBMI = 3;
							break;
						case '4':
							sampleTimePeriodBMI = 4;
							break;
						case '5':
							sampleTimePeriodBMI = 5;
							break;
						default:
							break;
						}
						break;
						case 'm':
							tempIndex = 6;
							if (rxMsgData[tempIndex] == '"'){
								tempIndex = 7;
							}
							switch (rxMsgData[tempIndex]){
							case '0':
								sampleTimePeriodBMM = 0;
								break;
							case '1':
								sampleTimePeriodBMM = 1;
								break;
							case '2':
								sampleTimePeriodBMM = 2;
								break;
							case '3':
								sampleTimePeriodBMM = 3;
								break;
							case '4':
								sampleTimePeriodBMM = 4;
								break;
							case '5':
								sampleTimePeriodBMM = 5;
								break;
							default:
								break;
							}
							break;
							case 't':
								tempIndex = 6;
								if (rxMsgData[tempIndex] == '"'){
									tempIndex = 7;
								}
								switch (rxMsgData[tempIndex]){
								case '0':
									sampleTimePeriodTMP = 0;
									break;
								case '1':
									sampleTimePeriodTMP = 1;
									break;
								case '2':
									sampleTimePeriodTMP = 2;
									break;
								case '3':
									sampleTimePeriodTMP = 3;
									break;
								case '4':
									sampleTimePeriodTMP = 4;
									break;
								case '5':
									sampleTimePeriodTMP = 5;
									break;
								default:
									break;
								}
								break;
								case 'o':
									tempIndex = 6;
									if (rxMsgData[tempIndex] == '"'){
										tempIndex = 7;
									}
									switch (rxMsgData[tempIndex]){
									case '0':
										sampleTimePeriodOPT = 0;
										break;
									case '1':
										sampleTimePeriodOPT = 1;
										break;
									case '2':
										sampleTimePeriodOPT = 2;
										break;
									case '3':
										sampleTimePeriodOPT = 3;
										break;
									case '4':
										sampleTimePeriodOPT = 4;
										break;
									case '5':
										sampleTimePeriodOPT = 5;
										break;
									default:
										break;
									}
									break;
									case 'e':
										tempIndex = 6;
										if (rxMsgData[tempIndex] == '"'){
											tempIndex = 7;
										}
										switch (rxMsgData[tempIndex]){
										case '0':
											sampleTimePeriodBME = 0;
											break;
										case '1':
											sampleTimePeriodBME = 1;
											break;
										case '2':
											sampleTimePeriodBME = 2;
											break;
										case '3':
											sampleTimePeriodBME = 3;
											break;
										case '4':
											sampleTimePeriodBME = 4;
											break;
										case '5':
											sampleTimePeriodBME = 5;
											break;
										default:
											break;
										}
										break;

					}
					break;
					case 'i': //e.g.: {"i":0} or {"i":1}
						if (rxMsgData[5] == '1'){
							returnRslt = bmi160_config_running_mode(APPLICATION_NAVIGATION);
							BMI_on = true;
						} else {
							returnRslt = bmi160_config_running_mode(ACCEL_PEDOMETER);
							BMI_on = false;
						}
						break;
					case 't': //e.g.: {"t":0} or {"t":1}
						if (rxMsgData[5] == '1'){
							sensorTmp007Enable(true);
							TMP_on = true;
						} else {
							sensorTmp007Enable(false);
							TMP_on = false;
						}
						break;
					case 'o': //e.g.: {"o":0} or {"o":1}
						if (rxMsgData[5] == '1'){
							sensorOpt3001Enable(true);
							OPT_on = true;
						} else {
							sensorOpt3001Enable(false);
							OPT_on = false;
						}
						break;
					case 'e': //e.g.: {"e":0} or {"e":1}
						if (rxMsgData[5] == '1'){
							returnRslt = bme280_set_power_mode(BME280_NORMAL_MODE);
							BME_on = true;
						} else {
							returnRslt = bme280_set_power_mode(BME280_SLEEP_MODE);
							BME_on = false;
						}
						break;
					case 'r':
						if (rxMsgData[5] == '1'){
							getGestures = 1;
						} else {
							getGestures = 0;
							dominant = 0;
						}
						break;
				}
			}
		}
		// Configure WDT
		// For LPM3 Clock Source should be BCLK or VLOCLK
		MAP_WDT_A_initIntervalTimer(WDT_A_CLOCKSOURCE_BCLK/*WDT_A_CLOCKSOURCE_ACLK*/,
				wdtWakeUpPeriod[wdtWakeUpPeriodIndex]);
		MAP_Interrupt_enableInterrupt(INT_WDT_A);

		// Start WDT
		MAP_WDT_A_startTimer();

		//Go to LPM0 (Cannot use LPM3 because we won't accurately receive UART data)
		MAP_PCM_gotoLPM0();
	}
}

/***********************************************************
  Funtion:

   Works as a simple moving averager. Used for gesture recognition.

 */
int32_t movingAvg(int prevAvg, int16_t newValue)
{
	return (((prevAvg << NUM_AVGR_SUMS) + newValue - prevAvg) >> NUM_AVGR_SUMS);



}

/***********************************************************
  Function:

   The following function is responsible for starting XT1 in the
   MSP432 that is used to source the internal FLL that drives the
   MCLK and SMCLK.
 */
void startCrystalOscillator(void)
{
	/* Configuring pins for peripheral/crystal HFXT*/
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
			GPIO_PIN3 | GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);

	/* Configuring pins for peripheral/crystal LFXT*/
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
			GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);
}

/***********************************************************
  Function:

   The following function is responsible for setting up the system
   clock at a specified frequency.
 */
void setSystemClock(uint32_t CPU_Frequency)
{
    /* Setting the external clock frequency. This API is optional, but will
    * come in handy if the user ever wants to use the getMCLK/getACLK/etc
    * functions
    */
    MAP_CS_setExternalClockSourceFrequency(32768, CPU_Frequency);
	MAP_CS_setReferenceOscillatorFrequency(CS_REFO_32KHZ);

    /* Before we start we have to change VCORE to 1 to support the 24MHz frequency */
    MAP_PCM_setCoreVoltageLevel(PCM_AM_LDO_VCORE0);
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 1);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 1);

    /* Starting HFXT and LFXT in non-bypass mode without a timeout. */
    MAP_CS_startHFXT(false);
    MAP_CS_startLFXT(false);

    /* Initializing the clock source as follows:
    *      MCLK = HFXT/2 = 24MHz
    *      ACLK = LFXT = 32KHz
    *      HSMCLK = HFXT/4 = 6MHz
    *      SMCLK = HFXT/2 = 12MHz
    *      BCLK  = REFO = 32kHz
    */
    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_2);
    MAP_CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_HSMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_8);
    MAP_CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_4);
    MAP_CS_initClockSignal(CS_BCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
}

/***********************************************************
  Function:
 */
void configureGPIO(void)
{
	/* Configure I/O to minimize power consumption before going to sleep */
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P3, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P6, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P7, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P8, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P9, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P10, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PJ, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, PIN_ALL8);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, PIN_ALL8);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, (PIN_ALL8 & ~GPIO_PIN6));
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, (PIN_ALL8 & ~(GPIO_PIN1 | GPIO_PIN6)));
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P5, (PIN_ALL8 & ~(GPIO_PIN0 | GPIO_PIN2)));
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P6, (PIN_ALL8 & ~GPIO_PIN7));
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P7, PIN_ALL8);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P8, PIN_ALL8);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P9, PIN_ALL8);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P10, PIN_ALL8);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_PJ, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3);
}

/***********************************************************
  Function:
 */
void startWakeUpTimerA(uint16_t ulClockMS)
{
	ulClockMS = (ulClockMS * 32768)/1000;

	/* TimerA UpMode Configuration Parameter */
	Timer_A_UpModeConfig upConfig =
	{
			TIMER_A_CLOCKSOURCE_ACLK,              // ACLK Clock Source
			TIMER_A_CLOCKSOURCE_DIVIDER_1,         // ACLK/1 = 32KHz
			ulClockMS,                             // tick period
			TIMER_A_TAIE_INTERRUPT_DISABLE,        // Disable Timer interrupt
			TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,   // Enable CCR0 interrupt
			TIMER_A_SKIP_CLEAR                     // Clear value
	};

	MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
	MAP_Timer_A_enableCaptureCompareInterrupt(TIMER_A0_BASE,
			TIMER_A_CAPTURECOMPARE_REGISTER_0);

	MAP_Interrupt_enableInterrupt(INT_TA0_0);
	MAP_Timer_A_startCounter(TIMER_A0_BASE,TIMER_A_UP_MODE);
}

/***********************************************************
  Function:
 */
void stopWakeUpTimerA(void)
{
	MAP_Interrupt_disableInterrupt(INT_TA0_0);
	MAP_Timer_A_stopTimer(TIMER_A0_BASE);
}

/***********************************************************
  Function: TA0_0_IRQHandler
 */
void TA0_0_IRQHandler(void)
{
	MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
			TIMER_A_CAPTURECOMPARE_REGISTER_0);

#ifdef USE_LPM
	MAP_Interrupt_disableSleepOnIsrExit();
#endif
}

/***********************************************************
  Function: WDT_A_IRQHandler
 */
void WDT_A_IRQHandler(void)
{
	//MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
	// Waking up from LMP3 take us to PCM_AM_LDO_VCORE0 instead of PCM_AM_LF_VCORE0
//	MAP_PCM_setPowerState(PCM_AM_LDO_VCORE0);
//    MAP_PCM_setCoreVoltageLevel(PCM_AM_DCDC_VCORE0);

#ifdef USE_LPM    
	MAP_Interrupt_disableSleepOnIsrExit();
#endif    
}

/***********************************************************
  Function: PORT1_IRQHandler
 */
void PORT1_IRQHandler(void)
{
	uint32_t debounce;
	uint32_t status;

	MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);

	status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);

	if(status & GPIO_PIN1)
	{
	}

	/* Delay for switch debounce */
	for(debounce = 0; debounce < 10000; debounce++)
		MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

#ifdef USE_LPM    
	MAP_Interrupt_disableSleepOnIsrExit();
#endif
}

/***********************************************************
  Function: PORT5_IRQHandler
 */
void PORT5_IRQHandler(void)
{
	uint32_t status;

	status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);

	MAP_GPIO_disableInterrupt(GPIO_PORT_P5, GPIO_PIN2);
	MAP_Interrupt_disableInterrupt(INT_PORT5);

	if(status & GPIO_PIN2)
	{
	}

	/* Delay for switch debounce */
	MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

#ifdef USE_LPM
	MAP_Interrupt_disableSleepOnIsrExit();
#endif
}

/***********************************************************
  Function: _system_pre_init
 */
int _system_pre_init(void)
{
	// stop WDT
	MAP_WDT_A_holdTimer();                        // Hold watchdog timer

	// Perform C/C++ global data initialization
	return 1;
}
