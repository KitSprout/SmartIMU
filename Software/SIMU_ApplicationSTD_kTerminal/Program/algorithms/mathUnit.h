/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    mathUnit.h
  * @author  KitSprout
  * @date    16-Nov-2016
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __MATHUNIT_H
#define __MATHUNIT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "core_cm4.h"
#include "arm_math.h"

/* Exported types --------------------------------------------------------------------------*/

/** 
  * @brief  euler angle structure definition
  */ 
typedef struct {
  float32_t pitch;
  float32_t roll;
  float32_t yaw;
} __attribute__((aligned(4))) eulerAngle_t;

/* Exported constants ----------------------------------------------------------------------*/
/* Exported macro --------------------------------------------------------------------------*/
#define invSqrtf( _iSq )  (1.0f / sqrtf((float32_t)(_iSq)))
#define squa( _Sq )       (((float32_t)_Sq) * ((float32_t)(_Sq)))
#define toRad( _mathD )   ((_mathD) * 0.0174532925f)
#define toDeg( _mathR )   ((_mathR) * 57.2957795f)

__STATIC_INLINE float32_t invSqrtf_fast( const float32_t _iSq )
{
  float32_t x2 = 0.5f * _iSq;
  float32_t y = _iSq;
  int32_t i = *(int32_t*)&y;
  i = 0x5F3759DF - (i >> 1);
  y = *(float32_t*)&i;
  y = y * (1.5f - (x2 * y * y));
  return y;
}

/* Exported functions ----------------------------------------------------------------------*/  

#ifdef __cplusplus
}
#endif

#endif
