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
} __attribute__((aligned(4))) Quaternion_t;

/* Exported constants ----------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------*/  
void Quaternion_Clear( Quaternion_t *pNumQ );
void Quaternion_ToNumQ( Quaternion_t *pNumQ, const EulerAngle_t *pAngE );
void Quaternion_ToAngE( EulerAngle_t *pAngE, const Quaternion_t *pNumQ );
void Quaternion_UpdateRotMatrix( Quaternion_t *pNumQ );
void Quaternion_Add( Quaternion_t *pNumQ, const Quaternion_t *pNumA, const Quaternion_t *pNumB );
void Quaternion_Sub( Quaternion_t *pNumQ, const Quaternion_t *pNumA, const Quaternion_t *pNumB );
void Quaternion_Multiply( Quaternion_t *pNumQ, const Quaternion_t *pNumA, const Quaternion_t *pNumB );
void Quaternion_MultiplyVector( Quaternion_t *pNumQ, const Quaternion_t *pNumA, const float32_t *pVect );
void Quaternion_Conjugate( Quaternion_t *pNumQ, const Quaternion_t *pNumC );
void Quaternion_Normalize( Quaternion_t *pNumQ, const Quaternion_t *pNumN );
void Quaternion_NormalizeFast( Quaternion_t *pNumQ, const Quaternion_t *pNumN );
void Quaternion_RungeKutta( Quaternion_t *pNumQ, const float32_t *gyro, const float32_t helfTime );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
