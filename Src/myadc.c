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
/*
void AdcSendPointRes(uint8_t i2cAddress, uint8_t resist){
	HAL_I2C_Mem_Write(&hi2c1, i2cAddress, &resist, 1, 100);
}

uint16_t AdcGetConversionRes(uint8_t i2cAddress){
	uint8_t getData[2];
	HAL_I2C_Master_Recieve(&hi2c1, i2cAddress, getData, 2, 100);
	retuen ((getData[0] << 8) | (getData[1]));
}

void AdcSendConfigRes(uint8_t i2cAddress, uint16_t resist){
	uint8_t sendData[2];
	sendData[0] = (uint8_t)(resist >> 8);
	sendData[1] = (uint8_t)(resist && 0xFF);
	HAL_I2C_Master_Transmit(&hi2c1, i2cAddress, sendData, 2, 100);
}
*/

uint16_t ADCReadReg(MYADC adcSet){
	uint8_t getData[2] = {0};
	HAL_I2C_Mem_Read(&hi2c1, adcSet.m_i2caddress, ADC_REG_P_CONVERT, 1, getData, 2, adcSet.m_conversionDelay);
	return (((getData[0] << 8) | (getData[1])) >> adcSet.m_bitShift);
}

void ADCWriteConfigReg(MYADC adcSet, uint16_t configReg){

	uint8_t sendData[2] = {0};
	HAL_StatusTypeDef exitValue = HAL_OK;
	sendData[0] = (configReg >> 8);
	sendData[1] = (configReg & 0xFF);
	exitValue = HAL_I2C_Mem_Write(&hi2c1, adcSet.m_i2caddress, ADC_REG_P_CONFIG, 1, sendData, 2, adcSet.m_conversionDelay);

	/*
	uint8_t sendData[3] = {0};
	HAL_StatusTypeDef exitValue = HAL_OK;
	sendData[0] = ADC_REG_P_CONFIG;
	sendData[1] = (configReg >> 8);
	sendData[2] = (configReg & 0xFF);
	exitValue = HAL_I2C_Master_Transmit(&hi2c1, adcSet.m_i2caddress, sendData, 3, adcSet.m_conversionDelay);
	return;*/
}

MYADC ADCInit(MYADC adcSet, uint8_t i2caddress, uint8_t ch, uint8_t conversionDelay, uint8_t bitShift, uint16_t gain){
	adcSet.m_i2caddress = i2caddress << 1;
	adcSet.m_ch = ch;
	adcSet.m_bitShift = bitShift;
	adcSet.m_conversionDelay = conversionDelay;
	adcSet.m_gain = gain;
	return adcSet;
}

uint16_t ADCSetConfig(uint16_t os, uint16_t mux, uint16_t pga, uint16_t mode, uint16_t dr, uint16_t comp_mode, uint16_t comp_pol, uint16_t comp_lat, uint16_t comp_que){
	uint16_t configReg = (os | mux | pga | mode | dr | comp_mode | comp_pol | comp_lat | comp_que);
	return configReg;
}

void ADCSetReadSingle(MYADC adcSet){
	uint16_t mux, sendReg;
	switch(adcSet.m_ch){
	case 0:
		mux = ADC_REG_CONFIG_MUX_SINGLE_0;
		break;

	case 1:
		mux = ADC_REG_CONFIG_MUX_SINGLE_1;
		break;

	case 2:
		mux = ADC_REG_CONFIG_MUX_SINGLE_2;
		break;

	default:
		mux = ADC_REG_CONFIG_MUX_SINGLE_3;
		break;
	}

	sendReg = ADCSetConfig(ADC_REG_CONFIG_OS_SINGLE, mux, adcSet.m_gain, ADC_REG_CONFIG_MODE_CONTIN, ADC_REG_CONFIG_DR_1600SPS, ADC_REG_CONFIG_CMODE_TRAD, ADC_REG_CONFIG_CPOL_ACTVLOW, ADC_REG_CONFIG_CLAT_NONLAT, ADC_REG_CONFIG_CQUE_NONE);
	ADCWriteConfigReg(adcSet, sendReg);
}

uint16_t ADCReadSingle(MYADC adcSet){
	uint16_t convert = ADCReadReg(adcSet);
	return (convert >> adcSet.m_bitShift);
}
