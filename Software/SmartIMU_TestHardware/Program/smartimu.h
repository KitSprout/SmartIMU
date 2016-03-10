/* #include "smartimu.h" */

#ifndef __SMARTIMU_H
#define __SMARTIMU_H

#include "stm32f4xx_hal.h"
#include "smartimu_bsp.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define _SampleRateFreg_100Hz_

#ifdef _SampleRateFreg_100Hz_
  #define SampleRateFreg  ((uint16_t)100)   // 100Hz
  #define SampleRate      ((float)0.010f)   // 10.0ms
  #define SampleRateHelf  ((float)0.005f)   // 5.0ms
#endif

#ifdef _SampleRateFreg_200Hz_
  #define SampleRateFreg  ((uint16_t)200)   // 200Hz
  #define SampleRate      ((float)0.005f)   // 5.0ms
  #define SampleRateHelf  ((float)0.0025f)  // 2.5ms
#endif

#ifdef _SampleRateFreg_400Hz_
  #define SampleRateFreg  ((uint16_t)400)   // 400Hz
  #define SampleRate      ((float)0.0025f)  // 2.5ms
  #define SampleRateHelf  ((float)0.00125f) // 1.25ms
#endif

#ifdef _SampleRateFreg_500Hz_
  #define SampleRateFreg  ((uint16_t)500)   // 500Hz
  #define SampleRate      ((float)0.002f)   // 2.0ms
  #define SampleRateHelf  ((float)0.001f)   // 1.0ms
#endif

typedef enum {
  SENSOR_START = 0,
  SENSOR_CORR_SEL,
  SENSOR_GYRO,
  SENSOR_ACCEL,
  SENSOR_MAG,
  SENSOR_CORR_END,
  SENSOR_ALG,
  SENSOR_DEBUG,
  SENSOR_END
} SensorState_TypeDef;
/*====================================================================================================*/
/*====================================================================================================*/
extern __IO SensorState_TypeDef SensorState;
/*====================================================================================================*/
/*====================================================================================================*/
void SIMU_Init( void );
void SIMU_UpdateEven( TIM_HandleTypeDef *htim );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
