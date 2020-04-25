/*
 * myadc.h
 *
 *  Created on: Apr 17, 2020
 *      Author: aegis
 */

#ifndef INC_MYADC_H_
#define INC_MYADC_H_

#include "main.h"
#include "i2c.h"

typedef struct{
	uint8_t m_i2caddress;
	uint8_t m_conversionDelay;
	uint8_t m_bitShift;
	uint16_t m_gain;
}MYADC;

#define ADC_ZER 0x48 // ADR should be GND
#define ADC_ONE 0x49 // ADR should be VDD
#define ADC_TWO 0x4a // ADC should be SDA
#define ADC_THR 0x4b // ADC should be SCL

//conversion delay
#define ADC_CONVERSIONDELAY 20

//pointer register
#define ADC_REG_P_MASK 0x03 //point mask
#define ADC_REG_P_CONVERT 0x00 //conversion
#define ADC_REG_P_CONFIG 0x01 //configuration
#define ADC_REG_P_LOWTHRESH 0x02 //low threshold
#define ADC_REG_P_HITHRESH 0x03 // high threshold

//config register
#define ADC_REG_CONFIG_OS_MASK (0x8000) ///< OS Mask
#define ADC_REG_CONFIG_OS_SINGLE (0x8000) ///< Write: Set to start a single-conversion
#define ADC_REG_CONFIG_OS_BUSY (0x0000) ///< Read: Bit = 0 when conversion is in progress
#define ADC_REG_CONFIG_OS_NOTBUSY (0x8000) ///< Read: Bit = 1 when device is not performing a conversion

#define ADC_REG_CONFIG_MUX_MASK (0x7000) ///< Mux Mask
#define ADC_REG_CONFIG_MUX_DIFF_0_1 (0x0000) ///< Differential P = AIN0, N = AIN1 (default)
#define ADC_REG_CONFIG_MUX_DIFF_0_3 (0x1000) ///< Differential P = AIN0, N = AIN3
#define ADC_REG_CONFIG_MUX_DIFF_1_3 (0x2000) ///< Differential P = AIN1, N = AIN3
#define ADC_REG_CONFIG_MUX_DIFF_2_3 (0x3000) ///< Differential P = AIN2, N = AIN3
#define ADC_REG_CONFIG_MUX_SINGLE_0 (0x4000) ///< Single-ended AIN0
#define ADC_REG_CONFIG_MUX_SINGLE_1 (0x5000) ///< Single-ended AIN1
#define ADC_REG_CONFIG_MUX_SINGLE_2 (0x6000) ///< Single-ended AIN2
#define ADC_REG_CONFIG_MUX_SINGLE_3 (0x7000) ///< Single-ended AIN3

#define ADC_REG_CONFIG_PGA_MASK (0x0E00)   ///< PGA Mask
#define ADC_REG_CONFIG_PGA_6_144V (0x0000) ///< +/-6.144V range = Gain 2/3
#define ADC_REG_CONFIG_PGA_4_096V (0x0200) ///< +/-4.096V range = Gain 1
#define ADC_REG_CONFIG_PGA_2_048V (0x0400) ///< +/-2.048V range = Gain 2 (default)
#define ADC_REG_CONFIG_PGA_1_024V (0x0600) ///< +/-1.024V range = Gain 4
#define ADC_REG_CONFIG_PGA_0_512V (0x0800) ///< +/-0.512V range = Gain 8
#define ADC_REG_CONFIG_PGA_0_256V (0x0A00) ///< +/-0.256V range = Gain 16

#define ADC_REG_CONFIG_MODE_MASK (0x0100)   ///< Mode Mask
#define ADC_REG_CONFIG_MODE_CONTIN (0x0000) ///< Continuous conversion mode
#define ADC_REG_CONFIG_MODE_SINGLE (0x0100) ///< Power-down single-shot mode (default)

#define ADC_REG_CONFIG_DR_MASK (0x00E0)   ///< Data Rate Mask
#define ADC_REG_CONFIG_DR_128SPS (0x0000) ///< 128 samples per second
#define ADC_REG_CONFIG_DR_250SPS (0x0020) ///< 250 samples per second
#define ADC_REG_CONFIG_DR_490SPS (0x0040) ///< 490 samples per second
#define ADC_REG_CONFIG_DR_920SPS (0x0060) ///< 920 samples per second
#define ADC_REG_CONFIG_DR_1600SPS (0x0080) ///< 1600 samples per second (default)
#define ADC_REG_CONFIG_DR_2400SPS (0x00A0) ///< 2400 samples per second
#define ADC_REG_CONFIG_DR_3300SPS (0x00C0) ///< 3300 samples per second

#define ADC_REG_CONFIG_CMODE_MASK (0x0010) ///< CMode Mask
#define ADC_REG_CONFIG_CMODE_TRAD (0x0000) ///< Traditional comparator with hysteresis (default)
#define ADC_REG_CONFIG_CMODE_WINDOW (0x0010) ///< Window comparator

#define ADC_REG_CONFIG_CPOL_MASK (0x0008) ///< CPol Mask
#define ADC_REG_CONFIG_CPOL_ACTVLOW (0x0000) ///< ALERT/RDY pin is low when active (default)
#define ADC_REG_CONFIG_CPOL_ACTVHI (0x0008) ///< ALERT/RDY pin is high when active

#define ADC_REG_CONFIG_CLAT_MASK (0x0004) ///< Determines if ALERT/RDY pin latches once asserted
#define ADC_REG_CONFIG_CLAT_NONLAT (0x0000) ///< Non-latching comparator (default)
#define ADC_REG_CONFIG_CLAT_LATCH (0x0004) ///< Latching comparator

#define ADC_REG_CONFIG_CQUE_MASK (0x0003) ///< CQue Mask
#define ADC_REG_CONFIG_CQUE_1CONV (0x0000) ///< Assert ALERT/RDY after one conversions
#define ADC_REG_CONFIG_CQUE_2CONV (0x0001) ///< Assert ALERT/RDY after two conversions
#define ADC_REG_CONFIG_CQUE_4CONV (0x0002) ///< Assert ALERT/RDY after four conversions
#define ADC_REG_CONFIG_CQUE_NONE (0x0003) ///< Disable the comparator and put ALERT/RDY in high state (default)

#define ADC_REG_BITSHIFT 4

MYADC AdcInit(uint8_t, uint8_t, uint8_t, uint16_t);
void AdcSetContinuous(MYADC, uint8_t);
void AdcSetSingleShot(MYADC, uint8_t);
uint16_t AdcReadSingle(MYADC adcSet);
#endif /* INC_MYADC_H_ */
