/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    mahonyFilter.h
  * @author  KitSprout
  * @date    12-Oct-2016
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __MAHONYFILTER_H
#define __MAHONYFILTER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "modules\imu.h"
#include "algorithms\quaternion.h"

/* Exported types --------------------------------------------------------------------------*/
typedef struct {
  float32_t sampleRate;
  float32_t sampleRateHelf;

  float32_t ki;
  float32_t kp;

  quaternion_t numQ;
  eulerAngle_t angE;

  IMU_DataTypeDef *imu;
} __attribute__((aligned)) mahonyFilter_t;

/* Exported constants ----------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------*/  
void MahonyFilter_Init( mahonyFilter_t *mf );
void MahonyFilter_Update( mahonyFilter_t *mf );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
