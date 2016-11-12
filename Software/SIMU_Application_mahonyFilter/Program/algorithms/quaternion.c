/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    quaternion.c
  * @author  KitSprout
  * @date    12-Oct-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "algorithms\quaternion.h"

/** @addtogroup STM32_Algorithms
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
//#define USE_ARM_MATH_LIB

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  Quaternion_Clear
  * @param  pNumQ: 
  * @retval None
  */
void Quaternion_Clear( Quaternion_t *pNumQ )
{
  pNumQ->q0 = 1.0f;
  pNumQ->q1 = 0.0f;
  pNumQ->q2 = 0.0f;
  pNumQ->q3 = 0.0f;

  for (uint8_t i = 0; i < 3; i++) {
    for (uint8_t j = 0; j < 3; j++) {
      pNumQ->rMat[i][j] = 0.0f;
    }
  }
}

/**
  * @brief  Quaternion_ToNumQ
  * @param  pNumQ: 
  * @param  pAngE: 
  * @retval None
  */
__INLINE void Quaternion_ToNumQ( Quaternion_t *pNumQ, const EulerAngle_t *pAngE )
{
  const float32_t halfP = toRad(pAngE->pitch * 0.5f);
  const float32_t halfR = toRad(pAngE->roll  * 0.5f);
  const float32_t halfY = toRad(pAngE->yaw   * 0.5f);

#ifdef USE_ARM_MATH_LIB
  const float32_t sinP = arm_sin_f32(halfP);
  const float32_t cosP = arm_cos_f32(halfP);
  const float32_t sinR = arm_sin_f32(halfR);
  const float32_t cosR = arm_cos_f32(halfR);
  const float32_t sinY = arm_sin_f32(halfY);
  const float32_t cosY = arm_cos_f32(halfY);

#else
  const float32_t sinP = sinf(halfP);
  const float32_t cosP = cosf(halfP);
  const float32_t sinR = sinf(halfR);
  const float32_t cosR = cosf(halfR);
  const float32_t sinY = sinf(halfY);
  const float32_t cosY = cosf(halfY);

#endif

  pNumQ->q0 = cosP * cosR * cosY + sinP * sinR * sinY;
  pNumQ->q1 = cosP * sinR * cosY - sinP * cosR * sinY;
  pNumQ->q2 = cosP * sinR * sinY + sinP * cosR * cosY;
  pNumQ->q3 = cosP * cosR * sinY - sinP * sinR * cosY;
}

/**
  * @brief  Quaternion_ToAngE
  * @param  pNumQ: 
  * @param  pAngE: 
  * @retval None
  */
__INLINE void Quaternion_ToAngE( EulerAngle_t *pAngE, const Quaternion_t *pNumQ )
{
  pAngE->pitch = toDeg(-asinf(pNumQ->rMat[0][2]));
  pAngE->roll  = toDeg(atan2f(pNumQ->rMat[1][2], pNumQ->rMat[2][2]));
  pAngE->yaw   = toDeg(atan2f(pNumQ->rMat[0][1], pNumQ->rMat[0][0]));
}

/**
  * @brief  Quaternion_UpdateRotMatrix
  * @param  pNumQ: 
  * @retval None
  */
__INLINE void Quaternion_UpdateRotMatrix( Quaternion_t *pNumQ )
{
  const float32_t Mq0q0 = pNumQ->q0 * pNumQ->q0;
  const float32_t Mq1q1 = pNumQ->q1 * pNumQ->q1;
  const float32_t Mq2q2 = pNumQ->q2 * pNumQ->q2;
  const float32_t Mq3q3 = pNumQ->q3 * pNumQ->q3;
  const float32_t Mq0q1 = pNumQ->q0 * pNumQ->q1;
  const float32_t Mq0q2 = pNumQ->q0 * pNumQ->q2;
  const float32_t Mq0q3 = pNumQ->q0 * pNumQ->q3;
  const float32_t Mq1q2 = pNumQ->q1 * pNumQ->q2;
  const float32_t Mq1q3 = pNumQ->q1 * pNumQ->q3;
  const float32_t Mq2q3 = pNumQ->q2 * pNumQ->q3;

  pNumQ->rMat[0][0] = 2.0f * (Mq0q0 + Mq1q1 - 0.5f);
  pNumQ->rMat[0][1] = 2.0f * (Mq1q2 + Mq0q3);
  pNumQ->rMat[0][2] = 2.0f * (Mq1q3 - Mq0q2);
  pNumQ->rMat[1][0] = 2.0f * (Mq1q2 - Mq0q3);
  pNumQ->rMat[1][1] = 2.0f * (Mq0q0 + Mq2q2 - 0.5f);
  pNumQ->rMat[1][2] = 2.0f * (Mq2q3 + Mq0q1);
  pNumQ->rMat[2][0] = 2.0f * (Mq1q3 + Mq0q2);
  pNumQ->rMat[2][1] = 2.0f * (Mq2q3 - Mq0q1);
  pNumQ->rMat[2][2] = 2.0f * (Mq0q0 + Mq3q3 - 0.5f);
}

/**
  * @brief  Quaternion_Add
  * @param  pNumQ: 
  * @param  pNumA: 
  * @param  pNumB: 
  * @retval None
  */
__INLINE void Quaternion_Add( Quaternion_t *pNumQ, const Quaternion_t *pNumA, const Quaternion_t *pNumB )
{
  pNumQ->q0 = pNumA->q0 + pNumB->q0;
  pNumQ->q1 = pNumA->q1 + pNumB->q1;
  pNumQ->q2 = pNumA->q2 + pNumB->q2;
  pNumQ->q3 = pNumA->q3 + pNumB->q3;
}

/**
  * @brief  Quaternion_Sub
  * @param  pNumQ: 
  * @param  pNumA: 
  * @param  pNumB: 
  * @retval None
  */
__INLINE void Quaternion_Sub( Quaternion_t *pNumQ, const Quaternion_t *pNumA, const Quaternion_t *pNumB )
{
  pNumQ->q0 = pNumA->q0 - pNumB->q0;
  pNumQ->q1 = pNumA->q1 - pNumB->q1;
  pNumQ->q2 = pNumA->q2 - pNumB->q2;
  pNumQ->q3 = pNumA->q3 - pNumB->q3;
}

/**
  * @brief  Quaternion_Multiply
  * @param  pNumQ: 
  * @param  pNumA: 
  * @param  pNumB: 
  * @retval None
  */
__INLINE void Quaternion_Multiply( Quaternion_t *pNumQ, const Quaternion_t *pNumA, const Quaternion_t *pNumB )
{
  if (pNumQ == pNumA) {
    Quaternion_t tmpQ;
    tmpQ.q0 = pNumA->q0;
    tmpQ.q1 = pNumA->q1;
    tmpQ.q2 = pNumA->q2;
    tmpQ.q3 = pNumA->q3;
    pNumQ->q0 = tmpQ.q0 * pNumB->q0 - tmpQ.q1 * pNumB->q1 - tmpQ.q2 * pNumB->q2 - tmpQ.q3 * pNumB->q3;
    pNumQ->q1 = tmpQ.q1 * pNumB->q0 + tmpQ.q0 * pNumB->q1 - tmpQ.q3 * pNumB->q2 + tmpQ.q2 * pNumB->q3;
    pNumQ->q2 = tmpQ.q2 * pNumB->q0 + tmpQ.q3 * pNumB->q1 + tmpQ.q0 * pNumB->q2 - tmpQ.q1 * pNumB->q3;
    pNumQ->q3 = tmpQ.q3 * pNumB->q0 - tmpQ.q2 * pNumB->q1 + tmpQ.q1 * pNumB->q2 + tmpQ.q0 * pNumB->q3;
  }
  else {
    pNumQ->q0 = pNumA->q0 * pNumB->q0 - pNumA->q1 * pNumB->q1 - pNumA->q2 * pNumB->q2 - pNumA->q3 * pNumB->q3;
    pNumQ->q1 = pNumA->q1 * pNumB->q0 + pNumA->q0 * pNumB->q1 - pNumA->q3 * pNumB->q2 + pNumA->q2 * pNumB->q3;
    pNumQ->q2 = pNumA->q2 * pNumB->q0 + pNumA->q3 * pNumB->q1 + pNumA->q0 * pNumB->q2 - pNumA->q1 * pNumB->q3;
    pNumQ->q3 = pNumA->q3 * pNumB->q0 - pNumA->q2 * pNumB->q1 + pNumA->q1 * pNumB->q2 + pNumA->q0 * pNumB->q3;
  }
}

/**
  * @brief  Quaternion_MultiplyVector
  * @param  pNumQ: 
  * @param  pNumA: 
  * @param  pVect: 
  * @retval None
  */
__INLINE void Quaternion_MultiplyVector( Quaternion_t *pNumQ, const Quaternion_t *pNumA, const float32_t *pVect )
{
  if (pNumQ == pNumA) {
    Quaternion_t tmpQ;
    tmpQ.q0 = pNumA->q0;
    tmpQ.q1 = pNumA->q1;
    tmpQ.q2 = pNumA->q2;
    tmpQ.q3 = pNumA->q3;
    pNumQ->q0 = -tmpQ.q1 * pVect[0] - tmpQ.q2 * pVect[1] - tmpQ.q3 * pVect[2];
    pNumQ->q1 =  tmpQ.q0 * pVect[0] - tmpQ.q3 * pVect[1] + tmpQ.q2 * pVect[2];
    pNumQ->q2 =  tmpQ.q3 * pVect[0] + tmpQ.q0 * pVect[1] - tmpQ.q1 * pVect[2];
    pNumQ->q3 = -tmpQ.q2 * pVect[0] + tmpQ.q1 * pVect[1] + tmpQ.q0 * pVect[2];
  }
  else {
    pNumQ->q0 = -pNumA->q1 * pVect[0] - pNumA->q2 * pVect[1] - pNumA->q3 * pVect[2];
    pNumQ->q1 =  pNumA->q0 * pVect[0] - pNumA->q3 * pVect[1] + pNumA->q2 * pVect[2];
    pNumQ->q2 =  pNumA->q3 * pVect[0] + pNumA->q0 * pVect[1] - pNumA->q1 * pVect[2];
    pNumQ->q3 = -pNumA->q2 * pVect[0] + pNumA->q1 * pVect[1] + pNumA->q0 * pVect[2];
  }
}

/**
  * @brief  Quaternion_Conjugate
  * @param  pNumQ: 
  * @param  pNumC: 
  * @retval None
  */
__INLINE void Quaternion_Conjugate( Quaternion_t *pNumQ, const Quaternion_t *pNumC )
{
  pNumQ->q0 =  pNumC->q0;
  pNumQ->q1 = -pNumC->q1;
  pNumQ->q2 = -pNumC->q2;
  pNumQ->q3 = -pNumC->q3;
}

/**
  * @brief  Quaternion_Normalize
  * @param  pNumQ: 
  * @param  pNumN: 
  * @retval None
  */
__INLINE void Quaternion_Normalize( Quaternion_t *pNumQ, const Quaternion_t *pNumN )
{
  const float32_t norm = invSqrtf(squa(pNumQ->q0) + squa(pNumQ->q1) + squa(pNumQ->q2) + squa(pNumQ->q3));

  pNumQ->q0 = pNumN->q0 * norm;
  pNumQ->q1 = pNumN->q1 * norm;
  pNumQ->q2 = pNumN->q2 * norm;
  pNumQ->q3 = pNumN->q3 * norm;
}

/**
  * @brief  Quaternion_FastNormalize
  * @param  pNumQ: 
  * @param  pNumN: 
  * @retval None
  */
__INLINE void Quaternion_NormalizeFast( Quaternion_t *pNumQ, const Quaternion_t *pNumN )
{
  const float32_t sqrtNumber = squa(pNumQ->q0) + squa(pNumQ->q1) + squa(pNumQ->q2) + squa(pNumQ->q3);

  float32_t x2 = 0.5f * sqrtNumber;
  float32_t norm = sqrtNumber;
  int32_t i = *(int32_t*)&norm;

  i = 0x5F3759DF - (i >> 1);
  norm = *(float32_t*)&i;
  norm = norm * (1.5f - (x2 * norm * norm));

  pNumQ->q0 = pNumN->q0 * norm;
  pNumQ->q1 = pNumN->q1 * norm;
  pNumQ->q2 = pNumN->q2 * norm;
  pNumQ->q3 = pNumN->q3 * norm;
}

/**
  * @brief  Quaternion_RungeKutta
  * @param  pNumQ: 
  * @param  gyro: 
  * @param  helfTime: 
  * @retval None
  */
__INLINE void Quaternion_RungeKutta( Quaternion_t *pNumQ, const float32_t *gyro, const float32_t helfTime )
{
  float32_t q[4];
  float32_t gyr[3];

  gyr[0] = gyro[0] * helfTime;
  gyr[1] = gyro[1] * helfTime;
  gyr[2] = gyro[2] * helfTime;

  q[0] = -pNumQ->q1 * gyr[0] - pNumQ->q2 * gyr[1] - pNumQ->q3 * gyr[2];
  q[1] =  pNumQ->q0 * gyr[0] - pNumQ->q3 * gyr[1] + pNumQ->q2 * gyr[2];
  q[2] =  pNumQ->q3 * gyr[0] + pNumQ->q0 * gyr[1] - pNumQ->q1 * gyr[2];
  q[3] = -pNumQ->q2 * gyr[0] + pNumQ->q1 * gyr[1] + pNumQ->q0 * gyr[2];

  pNumQ->q0 += q[0];
  pNumQ->q1 += q[1];
  pNumQ->q2 += q[2];
  pNumQ->q3 += q[3];
}

/*************************************** END OF FILE ****************************************/
