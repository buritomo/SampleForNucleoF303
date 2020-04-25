/*
 * myadc.c
 *
 *  Created on: Apr 17, 2020
 *      Author: aegis
 */

/*
 * Attention!
 *
 * This file is based on I2C library.
 * Please include this file after I2C.h
 *
 * ADC type : ADS1015
 *
 */


/*
 * Includes
 */
#include "myadc.h"

static uint8_t ADSwrite[6];
static uint16_t sendReg;

/*
 * Set ADC setting
 */
MYADC AdcInit(uint8_t address, uint8_t conversionDelay, uint8_t bitShift, uint16_t gain){
	MYADC adcSet;
	adcSet.m_i2caddress = address;
	adcSet.m_conversionDelay = conversionDelay;
	adcSet.m_bitShift = bitShift;
	adcSet.m_gain = gain;
	return adcSet;
}

/*
 * Config ADC by singlemode, and continuous reading
 */
void AdcSetContinuous(MYADC adcSet, uint8_t ch){
	sendReg = ADC_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
		      ADC_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
		      ADC_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
		      ADC_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
		      ADC_REG_CONFIG_DR_1600SPS |   // 1600 samples per second (default)
		      ADC_REG_CONFIG_MODE_CONTIN;   // Continuous mode

	sendReg |= adcSet.m_gain;
	switch(ch){
		case(0):
			sendReg |= ADC_REG_CONFIG_MUX_SINGLE_0;
		break;
		case(1):
			sendReg |= ADC_REG_CONFIG_MUX_SINGLE_1;
		break;
		case(2):
			sendReg |= ADC_REG_CONFIG_MUX_SINGLE_2;
		break;
		case(3):
			sendReg |= ADC_REG_CONFIG_MUX_SINGLE_3;
		break;
		}
	ADSwrite[0] = ADC_REG_P_CONFIG;
	ADSwrite[1] = (sendReg & 0xFF00) >> 8;
	ADSwrite[2] = (sendReg & 0x00FF);
	HAL_I2C_Master_Transmit(&hi2c1, adcSet.m_i2caddress << 1, ADSwrite, 3, 100);

	ADSwrite[0] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, adcSet.m_i2caddress << 1 , ADSwrite, 1 ,100);
	HAL_Delay(adcSet.m_conversionDelay);
}

/*
 * Config ADC by singlemode, and single shot reading
 */
void AdcSetSingleShot(MYADC adcSet, uint8_t ch){
	sendReg = ADC_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
		      ADC_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
		      ADC_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
		      ADC_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
		      ADC_REG_CONFIG_DR_1600SPS |   // 1600 samples per second (default)
		      ADC_REG_CONFIG_MODE_SINGLE;   // Single-shot mode(default)

	sendReg |= adcSet.m_gain;
	switch(ch){
		case(0):
			sendReg |= ADC_REG_CONFIG_MUX_SINGLE_0;
		break;
		case(1):
			sendReg |= ADC_REG_CONFIG_MUX_SINGLE_1;
		break;
		case(2):
			sendReg |= ADC_REG_CONFIG_MUX_SINGLE_2;
		break;
		case(3):
			sendReg |= ADC_REG_CONFIG_MUX_SINGLE_3;
		break;
		}
	ADSwrite[0] = ADC_REG_P_CONFIG;
	ADSwrite[1] = (sendReg & 0xFF00) >> 8;
	ADSwrite[2] = (sendReg & 0x00FF);
	HAL_I2C_Master_Transmit(&hi2c1, adcSet.m_i2caddress << 1, ADSwrite, 3, 100);

	ADSwrite[0] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, adcSet.m_i2caddress << 1 , ADSwrite, 1 ,100);
	HAL_Delay(adcSet.m_conversionDelay);
}

/*
 * AD convert to 12bit(int)
 */
uint16_t AdcReadSingle(MYADC adcSet){
	int16_t reading = 0;

	ADSwrite[0] = ADC_REG_P_CONVERT;
	HAL_I2C_Master_Transmit(&hi2c1, adcSet.m_i2caddress << 1 , ADSwrite, 1 ,100);
	HAL_Delay(adcSet.m_conversionDelay);

	HAL_I2C_Master_Receive(&hi2c1, adcSet.m_i2caddress <<1, ADSwrite, 2, 100);
	reading = (ADSwrite[0] << 8 | ADSwrite[1] ) >> adcSet.m_bitShift;

	return reading;
}
