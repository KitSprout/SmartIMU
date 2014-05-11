/* #include "algorithm_quaternion.h" */

#ifndef __ALGORITHM_QUATERNION_H
#define __ALGORITHM_QUATERNION_H

#include "stm32f4xx.h"
/*====================================================================================================*/
/*====================================================================================================*/
typedef __IO struct {
  fp32 Pitch;
  fp32 Roll;
  fp32 Yaw;
} EulerAngle;

typedef __IO struct {
  fp32 q0;
  fp32 q1;
  fp32 q2;
  fp32 q3;
} Quaternion;
/*====================================================================================================*/
/*====================================================================================================*/
void Quaternion_ToNumQ( Quaternion *pNumQ, EulerAngle *pAngE );
void Quaternion_ToAngE( Quaternion *pNumQ, EulerAngle *pAngE );
Quaternion Quaternion_Multiply( Quaternion NowQ, Quaternion OldQ );
void Quaternion_Normalize( Quaternion *pNumQ );
void Quaternion_RungeKutta( Quaternion *pNumQ, fp32 GyrX, fp32 GyrY, fp32 GyrZ, fp32 helfTimes );
/*====================================================================================================*/
/*====================================================================================================*/
extern Quaternion NumQ;
extern EulerAngle AngE;
/*====================================================================================================*/
/*====================================================================================================*/
#endif
