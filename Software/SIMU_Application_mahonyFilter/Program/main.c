/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    main.c
  * @author  KitSprout
  * @date    5-Nov-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\kSerial.h"
#include "modules\imu.h"
#include "algorithms\mahonyFilter.h"
#include "smartimu_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/

#define __SAMPLE_RATE_FREQ1KHz_

#ifdef __SAMPLE_RATE_FREQ100Hz_
  #define SAMPLE_RATE_FREQ  ((uint16_t)100)       // 100Hz
  #define SAMPLE_RATE       ((float32_t)0.01f)    // 10ms
#endif

#ifdef __SAMPLE_RATE_FREQ200Hz_
  #define SAMPLE_RATE_FREQ  ((uint16_t)200)       // 200Hz
  #define SAMPLE_RATE       ((float32_t)0.005f)   // 5ms
#endif

#ifdef __SAMPLE_RATE_FREQ400Hz_
  #define SAMPLE_RATE_FREQ  ((uint16_t)400)       // 400Hz
  #define SAMPLE_RATE       ((float32_t)0.0025f)  // 2.5ms
#endif

#ifdef __SAMPLE_RATE_FREQ500Hz_
  #define SAMPLE_RATE_FREQ  ((uint16_t)500)       // 500Hz
  #define SAMPLE_RATE       ((float32_t)0.002f)   // 2.0ms
#endif

#ifdef __SAMPLE_RATE_FREQ1KHz_
  #define SAMPLE_RATE_FREQ  ((uint16_t)1000)      // 1KHz
  #define SAMPLE_RATE       ((float32_t)0.001f)   // 1.0ms
#endif

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
extern IMU_DataTypeDef IMU;

MahonyFilter_t mf;

struct {
  float32_t msec;
  float32_t sec;
  float32_t min;
} time;

/* Private function prototypes -------------------------------------------------------------*/
void IMU_EvenCallBack( void );

/* Private functions -----------------------------------------------------------------------*/

#define PACKET_LENS 19
int main( void )
{
  HAL_Init();

  SIMU_GPIO_Config();
  SIMU_UART_Config(NULL);
  SIMU_IMU_Config(IMU_EvenCallBack, SAMPLE_RATE_FREQ);

  MahonyFilter_Init(&mf, &IMU, SAMPLE_RATE);
  time.msec = 0.0f;
  time.sec  = 0.0f;
  time.min  = 0.0f;

  while (1) {
    LED_G_Toggle();

#if 0
    printf("GX:%.4f\tGY:%.4f\tGZ:%.4f\tAX:%.4f\tAY:%.4f\tAZ:%.4f\tMX:%.3f\tMY:%.3f\tMZ:%.3f\r\n",
      mf.angE.pitch,  mf.angE.roll,   mf.angE.yaw,
//      IMU.gyrData[0], IMU.gyrData[1], IMU.gyrData[2],
      IMU.accData[0], IMU.accData[1], IMU.accData[2],
      IMU.magData[0], IMU.magData[1], IMU.magData[2]
    );
    delay_ms(100);

#else
    float32_t sendBuf[PACKET_LENS] = {0};

    sendBuf[0]  = IMU.gyrData[0];
    sendBuf[1]  = IMU.gyrData[1];
    sendBuf[2]  = IMU.gyrData[2];
    sendBuf[3]  = IMU.accData[0];
    sendBuf[4]  = IMU.accData[1];
    sendBuf[5]  = IMU.accData[2];
    sendBuf[6]  = IMU.magData[0];
    sendBuf[7]  = IMU.magData[1];
    sendBuf[8]  = IMU.magData[2];
    sendBuf[9]  = mf.angE.pitch;
    sendBuf[10] = mf.angE.roll;
    sendBuf[11] = mf.angE.yaw;
    sendBuf[12] = mf.numQ.q0;
    sendBuf[13] = mf.numQ.q1;
    sendBuf[14] = mf.numQ.q2;
    sendBuf[15] = mf.numQ.q3;
    sendBuf[16] = time.min;
    sendBuf[17] = time.sec;
    sendBuf[18] = time.msec;
    kSerial_sendData(sendBuf, PACKET_LENS, KS_FLOAT32);

#endif

  }
}

#define CUTOFF_FREQ     (0.02f)
#define TIME_CONST      (1.0f / (2.0f * PI * CUTOFF_FREQ))
#define THRESHOLD       (2.0f)
#define STIRLESS_TIME   (2.0f)
void GyroBiasCorrection( float32_t *gyro )
{
  static const float32_t alpha   = TIME_CONST / (TIME_CONST + SAMPLE_RATE);
  static const float32_t alpha_n = 1.0f - alpha;

  static float32_t stirlessTime = 0;
  static float32_t gyroBias[3] = {0};

  if ((gyro[0] > THRESHOLD) || (gyro[0] < -THRESHOLD) ||
      (gyro[1] > THRESHOLD) || (gyro[1] < -THRESHOLD) ||
      (gyro[2] > THRESHOLD) || (gyro[2] < -THRESHOLD)) {
    stirlessTime = 0.0f;
  }
  else {
    if (stirlessTime >= STIRLESS_TIME) {
      gyroBias[0] = alpha * gyroBias[0] + alpha_n * gyro[0];
      gyroBias[1] = alpha * gyroBias[1] + alpha_n * gyro[1];
      gyroBias[2] = alpha * gyroBias[2] + alpha_n * gyro[2];
    }
    else {
      stirlessTime += SAMPLE_RATE;
    }
  }

  gyro[0] -= gyroBias[0];
  gyro[1] -= gyroBias[1];
  gyro[2] -= gyroBias[2];
}

void IMU_EvenCallBack( void )
{
  LED_R_Set();

  time.msec += SAMPLE_RATE;
  if (time.msec >= 1.0f) {
    time.msec = 0.0f;
    time.sec += 1.0f;
    if (time.sec >= 60.0f) {
      time.sec = 0.0f;
      time.min += 1.0f;
    }
  }

  IMU_GetScaleData(mf.imu);
  GyroBiasCorrection(IMU.gyrData);
  MahonyFilter_Update(&mf);

  LED_R_Reset();
}

/*************************************** END OF FILE ****************************************/
