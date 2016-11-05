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
  * @date    12-Oct-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\kSerial.h"
#include "modules\imu.h"
#include "algorithms\mahonyFilter.h"
#include "simuDev_bsp.h"
#include "simuDev_sample.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
extern IMU_DataTypeDef IMU;

mahonyFilter_t mf = {
  .sampleRate = SAMPLE_RATE,
  .imu        = &IMU
};

/* Private function prototypes -------------------------------------------------------------*/
void IMU_EvenCallBack( void );

/* Private functions -----------------------------------------------------------------------*/

#define PACKET_LENS 12
int main( void )
{
  HAL_InitTick();

  SIMU_GPIO_Config();
  SIMU_UART_Config(NULL);
  SIMU_IMU_Config(IMU_EvenCallBack);

  MahonyFilter_Init(&mf);

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
    kSerial_sendData(sendBuf, PACKET_LENS, KS_FLOAT32);

#endif

  }
}

void IMU_EvenCallBack( void )
{
  IMU_GetScaleData(mf.imu);
  MahonyFilter_Update(&mf);
}

/*************************************** END OF FILE ****************************************/
