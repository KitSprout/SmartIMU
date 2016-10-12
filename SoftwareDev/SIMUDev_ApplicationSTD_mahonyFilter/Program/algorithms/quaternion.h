/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    quaternion.h
  * @author  KitSprout
  * @date    12-Oct-2016
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __QUATERNION_H
#define __QUATERNION_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "algorithms\mathUnit.h"

/* Exported types --------------------------------------------------------------------------*/
typedef struct {
  float32_t q0;
  float32_t q1;
  float32_t q2;
  float32_t q3;
  float32_t rMat[3][3];
} __attribute__((aligned(4))) quaternion_t;

/* Exported constants ----------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------*/  
void Quaternion_Clear( quaternion_t *pNumQ );
void Quaternion_ToNumQ( quaternion_t *pNumQ, eulerAngle_t *pAngE );
void Quaternion_ToAngE( quaternion_t *pNumQ, eulerAngle_t *pAngE );
void Quaternion_UpdateRolMatrix( quaternion_t *pNumQ );
void Quaternion_Multiply( quaternion_t *newQ, quaternion_t *nowQ, quaternion_t *oldQ );
void Quaternion_Normalize( quaternion_t *pNumQ );
void Quaternion_RungeKutta( quaternion_t *pNumQ, float32_t *gyro, float32_t helfTime );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
