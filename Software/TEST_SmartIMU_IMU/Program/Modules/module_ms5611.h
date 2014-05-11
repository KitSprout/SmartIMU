/* #include "module_ms5611.h" */

#ifndef __MODULE_MS5611_H
#define __MODULE_MS5611_H

#include "stm32f4xx.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define MS5611_I2C_ADDR       ((u8)0xEE)

#define MS5611_ADC            ((u8)0x00)  // ADC Read
#define MS5611_RESET          ((u8)0x1E)  // RESET

#define MS5611_D1_OSR_256     ((u8)0x40)  // 3 bytes
#define MS5611_D1_OSR_512     ((u8)0x42)  // 3 bytes
#define MS5611_D1_OSR_1024    ((u8)0x44)  // 3 bytes
#define MS5611_D1_OSR_2048    ((u8)0x46)  // 3 bytes
#define MS5611_D1_OSR_4096    ((u8)0x48)  // 3 bytes
#define MS5611_D2_OSR_256     ((u8)0x50)  // 3 bytes
#define MS5611_D2_OSR_512     ((u8)0x52)  // 3 bytes
#define MS5611_D2_OSR_1024    ((u8)0x54)  // 3 bytes
#define MS5611_D2_OSR_2048    ((u8)0x56)  // 3 bytes
#define MS5611_D2_OSR_4096    ((u8)0x58)  // 3 bytes

#define MS5611_PROM_COEFF_1   ((u8)0xA2)  // 2 bytes
#define MS5611_PROM_COEFF_2   ((u8)0xA4)  // 2 bytes
#define MS5611_PROM_COEFF_3   ((u8)0xA6)  // 2 bytes
#define MS5611_PROM_COEFF_4   ((u8)0xA8)  // 2 bytes
#define MS5611_PROM_COEFF_5   ((u8)0xAA)  // 2 bytes
#define MS5611_PROM_COEFF_6   ((u8)0xAC)  // 2 bytes

#define MS5611_ADC_D1         ((u8)0x01)
#define MS5611_ADC_D2         ((u8)0x02)

#define MS5611_RespFreq_256   ((u16)1650) // 0.48 - 0.54 - 0.60 - 1650Hz
#define MS5611_RespFreq_512   ((u16)850)  // 0.95 - 1.06 - 1.17 - 850Hz
#define MS5611_RespFreq_1024  ((u16)430)  // 1.88 - 2.08 - 2.28 - 430Hz
#define MS5611_RespFreq_2048  ((u16)220)  // 3.72 - 4.13 - 4.54 - 220Hz
#define MS5611_RespFreq_4096  ((u16)110)  // 7.40 - 8.22 - 9.04 - 110Hz

// 1010~1000  上升100m 下降10.5mbar
// 1000~900   上升100m 下降11.2mbar
// 900~800    上升100m 下降12.2mbar
// 800~700    上升100m 下降13.3mbar
// 700~600    上升100m 下降15mbar
/*====================================================================================================*/
/*====================================================================================================*/
typedef __IO enum {
  MS5611_ConvInit,
  MS5611_ConvD1,
  MS5611_ConvD2
} MS5611_STATUS;
/*====================================================================================================*/
/*====================================================================================================*/
void MS5611_Init( void );
void MS5611_Config( void );

void MS5611_Read( s32 *Baro_Buf, u8 ADC_ConvMode );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
