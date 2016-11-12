/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    mahonyFilter.c
  * @author  KitSprout
  * @date    12-Oct-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "algorithms\mahonyFilter.h"

/** @addtogroup STM32_Algorithms
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
#define MF_KP 0.4f
//#define MF_KI 0.001f

//#define MAHONY_FILTER_9DOF

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  MahonyFilter_Init
  * @param  mf: point to struct of mahonyFilter
  * @param  imu: 
  * @retval None
  */
void MahonyFilter_Init( MahonyFilter_t *mf, IMU_DataTypeDef *imu, float32_t sampleRate )
{
  mf->angE.pitch = 0.0f;
  mf->angE.roll  = 0.0f;
  mf->angE.yaw   = 0.0f;

  Quaternion_Clear(&mf->numQ);

  mf->imu = imu;

  mf->sampleRate = sampleRate;
}

/**
  * @brief  MahonyFilter_Update
  * @param  mf: point to struct of mahonyFilter
  * @retval None
  */
void MahonyFilter_Update( MahonyFilter_t *mf )
{
  float32_t gVect[3];
  float32_t gyr[3], acc[3];

#if defined(MAHONY_FILTER_9DOF)
  float32_t hVect[3], wVect[3];
  float32_t mag[3];
#endif

  float32_t err[3];
#if defined(MF_KI)
  static float32_t errInt[3] = {0};
#endif

  gyr[0] = toRad(mf->imu->gyrData[0]);
  gyr[1] = toRad(mf->imu->gyrData[1]);
  gyr[2] = toRad(mf->imu->gyrData[2]);
  acc[0] = mf->imu->accData[0];
  acc[1] = mf->imu->accData[1];
  acc[2] = mf->imu->accData[2];

  /* Normalise accelerometer measurement */
  const float32_t normAcc = invSqrtf(acc[0] * acc[0] + acc[1] * acc[1] + acc[2] * acc[2]);
  acc[0] *= normAcc;
  acc[1] *= normAcc;
  acc[2] *= normAcc;

  /* Estimated direction of gravity */
  gVect[0] = mf->numQ.rMat[0][2];
  gVect[1] = mf->numQ.rMat[1][2];
  gVect[2] = mf->numQ.rMat[2][2];

  err[0] = acc[1] * gVect[2] - acc[2] * gVect[1];
  err[1] = acc[2] * gVect[0] - acc[0] * gVect[2];
  err[2] = acc[0] * gVect[1] - acc[1] * gVect[0];

#if defined(MAHONY_FILTER_9DOF)
  mag[0] = mf->imu->magData[0];
  mag[1] = mf->imu->magData[1];
  mag[2] = mf->imu->magData[2];

//  const float32_t normMag = invSqrtf(mag[0] * mag[0] + mag[1] * mag[1] + mag[2] * mag[2]);
//  if ((normMag < fusionAhrs->minMagneticFieldSquared) ||
//      (normMag > fusionAhrs->maxMagneticFieldSquared)) {
//    break;
//  }

  /* Reference direction of Earth's magnetic field */
  wVect[0] = acc[1] * mag[2] - acc[2] * mag[1];
  wVect[1] = acc[2] * mag[0] - acc[0] * mag[2];
  wVect[2] = acc[0] * mag[1] - acc[1] * mag[0];

  /* Normalise magnetometer measurement */
  const float32_t normVect = invSqrtf(wVect[0] * wVect[0] + wVect[1] * wVect[1] + wVect[2] * wVect[2]);
  wVect[2] *= normVect;
  wVect[2] *= normVect;
  wVect[2] *= normVect;

  /* Estimated direction of Earth's magnetic field  */
  hVect[0] = mf->numQ.rMat[0][1];
  hVect[1] = mf->numQ.rMat[1][1];
  hVect[2] = mf->numQ.rMat[2][1];

  /* Error is sum of cross product between estimated direction and measured direction of field vectors */
  err[0] += wVect[1] * hVect[2] - wVect[2] * hVect[1];
  err[1] += wVect[2] * hVect[0] - wVect[0] * hVect[2];
  err[2] += wVect[0] * hVect[1] - wVect[1] * hVect[0];

#endif

#if defined(MF_KI)
  /* Compute and apply integral feedback */
  errInt[0] += (SAMPLE_RATE * MF_KI) * err[0];
  errInt[1] += (SAMPLE_RATE * MF_KI) * err[1];
  errInt[2] += (SAMPLE_RATE * MF_KI) * err[2];

  /* Apply proportional feedback */
  gyr[0] += MF_KP * err[0] + errInt[0];
  gyr[1] += MF_KP * err[1] + errInt[1];
  gyr[2] += MF_KP * err[2] + errInt[2];

#else
  gyr[0] += MF_KP * err[0];
  gyr[1] += MF_KP * err[1];
  gyr[2] += MF_KP * err[2];

#endif

  /* Integrate rate of change of quaternion */
//  Quaternion_Conjugate(&mf->numQ, &mf->numQ);
  Quaternion_RungeKutta(&mf->numQ, gyr, mf->sampleRate * 0.5f);
  Quaternion_Normalize(&mf->numQ, &mf->numQ);
  Quaternion_UpdateRotMatrix(&mf->numQ);
  Quaternion_ToAngE(&mf->angE, &mf->numQ);

//  // Get Euler Angle */
//  mf->angE.pitch = -toDeg(mf->angE.pitch);
//  mf->angE.roll  = toDeg(mf->angE.roll);
//  mf->angE.yaw   = -toDeg(mf->angE.yaw);
}

/*************************************** END OF FILE ****************************************/
