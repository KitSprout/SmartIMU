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
void Quaternion_Clear( quaternion_t *pNumQ )
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
void Quaternion_ToNumQ( quaternion_t *pNumQ, eulerAngle_t *pAngE )
{
  float32_t halfP = toRad(pAngE->pitch / 2.0f);
  float32_t halfR = toRad(pAngE->roll / 2.0f);
  float32_t halfY = toRad(pAngE->yaw / 2.0f);

#ifdef USE_ARM_MATH_LIB
  float32_t sinP = arm_sin_f32(halfP);
  float32_t cosP = arm_cos_f32(halfP);
  float32_t sinR = arm_sin_f32(halfR);
  float32_t cosR = arm_cos_f32(halfR);
  float32_t sinY = arm_sin_f32(halfY);
  float32_t cosY = arm_cos_f32(halfY);
#else
  float32_t sinP = sinf(halfP);
  float32_t cosP = cosf(halfP);
  float32_t sinR = sinf(halfR);
  float32_t cosR = cosf(halfR);
  float32_t sinY = sinf(halfY);
  float32_t cosY = cosf(halfY);
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
__inline void Quaternion_ToAngE( quaternion_t *pNumQ, eulerAngle_t *pAngE )
{
  pAngE->pitch = toDeg(asinf(-pNumQ->rMat[0][2]));
  pAngE->roll  = toDeg(atan2f(pNumQ->rMat[1][2], pNumQ->rMat[2][2]));
  pAngE->yaw   = toDeg(atan2f(pNumQ->rMat[0][1], pNumQ->rMat[0][0]));
}

/**
  * @brief  Quaternion_UpdateRolMatrix
  * @param  pNumQ: 
  * @retval None
  */
__inline void Quaternion_UpdateRolMatrix( quaternion_t *pNumQ )
{
  float32_t Mq0q0 = pNumQ->q0 * pNumQ->q0;
  float32_t Mq1q1 = pNumQ->q1 * pNumQ->q1;
  float32_t Mq2q2 = pNumQ->q2 * pNumQ->q2;
  float32_t Mq3q3 = pNumQ->q3 * pNumQ->q3;
  float32_t Mq0q1 = pNumQ->q0 * pNumQ->q1;
  float32_t Mq0q2 = pNumQ->q0 * pNumQ->q2;
  float32_t Mq0q3 = pNumQ->q0 * pNumQ->q3;
  float32_t Mq1q2 = pNumQ->q1 * pNumQ->q2;
  float32_t Mq1q3 = pNumQ->q1 * pNumQ->q3;
  float32_t Mq2q3 = pNumQ->q2 * pNumQ->q3;

//  pNumQ->rMat[0][0] = 1.0f - 2.0f * (Mq2q2 + Mq3q3);
//  pNumQ->rMat[0][1] = 2.0f * (Mq1q2 + Mq0q3);
//  pNumQ->rMat[0][2] = 2.0f * (Mq1q3 - Mq0q2);

//  pNumQ->rMat[1][0] = 2.0f * (Mq1q2 - Mq0q3);
//  pNumQ->rMat[1][1] = 1.0f - 2.0f * (Mq1q1 + Mq3q3);
//  pNumQ->rMat[1][2] = 2.0f * (Mq2q3 + Mq0q1);

//  pNumQ->rMat[2][0] = 2.0f * (Mq1q3 + Mq0q2);
//  pNumQ->rMat[2][1] = 2.0f * (Mq2q3 - Mq0q1);
//  pNumQ->rMat[2][2] = 1.0f - 2.0f * (Mq1q1 + Mq2q2);

  pNumQ->rMat[0][0] = Mq0q0 + Mq1q1 - Mq2q2 - Mq3q3;
  pNumQ->rMat[0][1] = 2.0f * (Mq1q2 + Mq0q3);
  pNumQ->rMat[0][2] = 2.0f * (Mq1q3 - Mq0q2);

  pNumQ->rMat[1][0] = 2.0f * (Mq1q2 - Mq0q3);
  pNumQ->rMat[1][1] = Mq0q0 - Mq1q1 + Mq2q2 - Mq3q3;
  pNumQ->rMat[1][2] = 2.0f * (Mq2q3 + Mq0q1);

  pNumQ->rMat[2][0] = 2.0f * (Mq1q3 + Mq0q2);
  pNumQ->rMat[2][1] = 2.0f * (Mq2q3 - Mq0q1);
  pNumQ->rMat[2][2] = Mq0q0 - Mq1q1 - Mq2q2 + Mq3q3;
}

/**
  * @brief  Quaternion_Multiply
  * @param  mulQ: 
  * @param  numQA: 
  * @param  numQB: 
  * @retval None
  */
void Quaternion_Multiply( quaternion_t *mulQ, quaternion_t *numQA, quaternion_t *numQB )
{
  if (mulQ == numQA) {
    quaternion_t tmpQ;
    tmpQ.q0 = numQA->q0;
    tmpQ.q1 = numQA->q1;
    tmpQ.q2 = numQA->q2;
    tmpQ.q3 = numQA->q3;
    mulQ->q0 = tmpQ.q0 * numQB->q0 - tmpQ.q1 * numQB->q1 - tmpQ.q2 * numQB->q2 - tmpQ.q3 * numQB->q3;
    mulQ->q1 = tmpQ.q1 * numQB->q0 + tmpQ.q0 * numQB->q1 - tmpQ.q3 * numQB->q2 + tmpQ.q2 * numQB->q3;
    mulQ->q2 = tmpQ.q2 * numQB->q0 + tmpQ.q3 * numQB->q1 + tmpQ.q0 * numQB->q2 - tmpQ.q1 * numQB->q3;
    mulQ->q3 = tmpQ.q3 * numQB->q0 - tmpQ.q2 * numQB->q1 + tmpQ.q1 * numQB->q2 + tmpQ.q0 * numQB->q3;
  }
  else {
    mulQ->q0 = numQA->q0 * numQB->q0 - numQA->q1 * numQB->q1 - numQA->q2 * numQB->q2 - numQA->q3 * numQB->q3;
    mulQ->q1 = numQA->q1 * numQB->q0 + numQA->q0 * numQB->q1 - numQA->q3 * numQB->q2 + numQA->q2 * numQB->q3;
    mulQ->q2 = numQA->q2 * numQB->q0 + numQA->q3 * numQB->q1 + numQA->q0 * numQB->q2 - numQA->q1 * numQB->q3;
    mulQ->q3 = numQA->q3 * numQB->q0 - numQA->q2 * numQB->q1 + numQA->q1 * numQB->q2 + numQA->q0 * numQB->q3;
  }

  Quaternion_Normalize(mulQ);
}

/**
  * @brief  Quaternion_Normalize
  * @param  pNumQ: 
  * @retval None
  */
__inline void Quaternion_Normalize( quaternion_t *pNumQ )
{
  float32_t norm = invSqrtf(squa(pNumQ->q0) + squa(pNumQ->q1) + squa(pNumQ->q2) + squa(pNumQ->q3));

  pNumQ->q0 = pNumQ->q0 * norm;
  pNumQ->q1 = pNumQ->q1 * norm;
  pNumQ->q2 = pNumQ->q2 * norm;
  pNumQ->q3 = pNumQ->q3 * norm;
}

/**
  * @brief  Quaternion_RungeKutta
  * @param  pNumQ: 
  * @param  gyro: 
  * @param  helfTime: 
  * @retval None
  */
__inline void Quaternion_RungeKutta( quaternion_t *pNumQ, float32_t *gyro, float32_t helfTime )
{
  float32_t q[4];

  q[0] = (-pNumQ->q1 * gyro[0] - pNumQ->q2 * gyro[1] - pNumQ->q3 * gyro[2]) * helfTime;
  q[1] = ( pNumQ->q0 * gyro[0] - pNumQ->q3 * gyro[1] + pNumQ->q2 * gyro[2]) * helfTime;
  q[2] = ( pNumQ->q3 * gyro[0] + pNumQ->q0 * gyro[1] - pNumQ->q1 * gyro[2]) * helfTime;
  q[3] = (-pNumQ->q2 * gyro[0] + pNumQ->q1 * gyro[1] + pNumQ->q0 * gyro[2]) * helfTime;

  pNumQ->q0 = pNumQ->q0 + q[0];
  pNumQ->q1 = pNumQ->q1 + q[1];
  pNumQ->q2 = pNumQ->q2 + q[2];
  pNumQ->q3 = pNumQ->q3 + q[3];
}

/*************************************** END OF FILE ****************************************/
