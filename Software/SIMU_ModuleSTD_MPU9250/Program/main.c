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
  * @date    11-Oct-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
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
void Serial_RecvIRQ( void );

/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  HAL_InitTick();

  SIMU_GPIO_Config();
  SIMU_UART_Config(NULL);
  SIMU_IMU_Config();

  while (1) {
    LED_G_Toggle();
    delay_ms(100);

    IMU_GetRawData(&IMU);
    printf("GX:%5.0f\tGY:%5.0f\tGZ:%5.0f\tAX:%5.0f\tAY:%5.0f\tAZ:%5.0f\tMX:%5.0f\tMY:%5.0f\tMZ:%5.0f\r\n",
      IMU.gyrRaw[0], IMU.gyrRaw[1], IMU.gyrRaw[2],
      IMU.accRaw[0], IMU.accRaw[1], IMU.accRaw[2],
      IMU.magRaw[0], IMU.magRaw[1], IMU.magRaw[2]
    );
  }
}

/*************************************** END OF FILE ****************************************/
