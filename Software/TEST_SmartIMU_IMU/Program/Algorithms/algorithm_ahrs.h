/* #include "algorithm_ahrs.h" */

#ifndef __ALGORITHM_AHRS_H
#define __ALGORITHM_AHRS_H

#include "stm32f4xx.h"
#include "algorithm_quaternion.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define _SampleRateFreg_500Hz_

#ifdef _SampleRateFreg_400Hz_
  #define SampleRateFreg  ((u16)400)        // 400Hz
  #define SampleRate      ((fp32)0.0025f)   // 2.5ms
  #define SampleRateHelf  ((fp32)0.00125f)  // 1.25ms
#endif

#ifdef _SampleRateFreg_500Hz_
  #define SampleRateFreg  ((u16)500)        // 500Hz
  #define SampleRate      ((fp32)0.002f)    // 2.0ms
  #define SampleRateHelf  ((fp32)0.001f)    // 1.0ms
#endif
/*====================================================================================================*/
/*====================================================================================================*/
void AHRS_Init( Quaternion *pNumQ, EulerAngle *pAngE );
void AHRS_Update( void );
/*====================================================================================================*/
/*====================================================================================================*/
#endif	
