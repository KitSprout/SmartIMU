/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    imu.h
  * @author  KitSprout
  * @date    19-Jan-2017
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __IMU_H
#define __IMU_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "algorithms\mathUnit.h"
#include "modules\mpu9250.h"
//#include "modules\lps22hb.h"

/* Exported types --------------------------------------------------------------------------*/

typedef struct {
  float32_t gyrRaw[3];      /* x = raw[0], y = raw[1], z = raw[2] */
  float32_t accRaw[3];
  float32_t magRaw[3];
  float32_t ictempRaw;
  float32_t baroRaw[2];     /* p = raw[0], t = raw[1] */

  float32_t gyrData[3];
  float32_t accData[3];
  float32_t magData[3];
  float32_t ictempData;
  float32_t baroData[2];

  float32_t accMotion[3];

  float32_t gyrScale[3];
  float32_t accScale[3];
  float32_t magScale[3];
  float32_t ictempScale;
  float32_t baroScale[2];

  float32_t gyrCalib[3];
  float32_t accCalib[9];
  float32_t magCalib[9];    /* c11 = calib[0], c12 = calib[1], c13 = calib[2],
                               c21 = calib[3], c22 = calib[4], c23 = calib[5],
                               c31 = calib[6], c32 = calib[7], c33 = calib[8] */

  float32_t gyrOffset[3];
  float32_t accOffset[3];
  float32_t magOffset[3];
  float32_t ictempOffset;

  float32_t accStrength;
  float32_t magStrength;

} __attribute__((aligned(4))) IMU_DataTypeDef;

typedef struct {
  IMU_DataTypeDef *Data;

#if defined(__MPU92)
  MPU_ConfigTypeDef InitMPU;
#endif

#if defined(__LPS22)
  LPS_ConfigTypeDef InitLPS;
#endif

} IMU_InitTypeDef;

/* Exported constants ----------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------*/  
void    IMU_Config( void );
int8_t  IMU_Init( IMU_InitTypeDef *IMUx );

int8_t  IMU_GetRawData( IMU_DataTypeDef *IMUx );
void    IMU_GetCalibData( IMU_DataTypeDef *IMUx );
void    IMU_GetScaleData( IMU_DataTypeDef *IMUx );
void    IMU_PrintData( IMU_DataTypeDef *IMUx );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
