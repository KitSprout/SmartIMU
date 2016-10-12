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
#include "simuDev_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
extern IMU_DataTypeDef IMU;

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

#define PACKET_LENS 9
int main( void )
{
  int16_t sendBuf[PACKET_LENS] = {0};

  HAL_InitTick();

  SIMU_GPIO_Config();
  SIMU_UART_Config(NULL);
  SIMU_IMU_Config();

  while (1) {
    LED_G_Toggle();
    IMU_GetRawData(&IMU);

#if 0
    printf("GX:%5.0f\tGY:%5.0f\tGZ:%5.0f\tAX:%5.0f\tAY:%5.0f\tAZ:%5.0f\tMX:%5.0f\tMY:%5.0f\tMZ:%5.0f\r\n",
      IMU.gyrRaw[0], IMU.gyrRaw[1], IMU.gyrRaw[2],
      IMU.accRaw[0], IMU.accRaw[1], IMU.accRaw[2],
      IMU.magRaw[0], IMU.magRaw[1], IMU.magRaw[2]
    );
    delay_ms(100);

#else
    sendBuf[0] = IMU.gyrRaw[0];
    sendBuf[1] = IMU.gyrRaw[1];
    sendBuf[2] = IMU.gyrRaw[2];
    sendBuf[3] = IMU.accRaw[0];
    sendBuf[4] = IMU.accRaw[1];
    sendBuf[5] = IMU.accRaw[2];
    sendBuf[6] = IMU.magRaw[0];
    sendBuf[7] = IMU.magRaw[1];
    sendBuf[8] = IMU.magRaw[2];
    kSerial_sendData(sendBuf, PACKET_LENS, KS_INT16);
#endif

  }
}

/*************************************** END OF FILE ****************************************/
