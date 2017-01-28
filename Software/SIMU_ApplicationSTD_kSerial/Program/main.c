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
  * @date    28-Jan-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\kSerial.h"
#include "modules\imu.h"
#include "stm32f4xx_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
#define PACKET_LENS 9

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
extern IMU_DataTypeDef IMU;

int16_t sendBuf[PACKET_LENS] = {0};

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  HAL_InitTick();
  BSP_GPIO_Config();
  BSP_UART_Config(NULL);
  BSP_IMU_Config();

  while (1) {
#if 1
    IMU_GetCalibData(&IMU);
    sendBuf[0] = IMU.gyrData[0];
    sendBuf[1] = IMU.gyrData[1];
    sendBuf[2] = IMU.gyrData[2];
    sendBuf[3] = IMU.accData[0];
    sendBuf[4] = IMU.accData[1];
    sendBuf[5] = IMU.accData[2];
    sendBuf[6] = IMU.magData[0];
    sendBuf[7] = IMU.magData[1];
    sendBuf[8] = IMU.magData[2];
    kSerial_SendData(sendBuf, PACKET_LENS, KS_INT16);
    LED_G_Toggle();
    delay_ms(10);
#else
    IMU_GetScaleData(&IMU);
    printf("GX:%8.2f\tGY:%8.2f\tG8:%5.2f\tAX:%7.4f\tAY:%7.4f\tAZ:%7.4f\tMX:%6.1f\tMY:%6.1f\tMZ:%6.1f\r\n",
      IMU.gyrData[0], IMU.gyrData[1], IMU.gyrData[2],
      IMU.accData[0], IMU.accData[1], IMU.accData[2],
      IMU.magData[0], IMU.magData[1], IMU.magData[2]
    );
    LED_B_Toggle();
    delay_ms(100);
#endif

  }
}

/*************************************** END OF FILE ****************************************/
