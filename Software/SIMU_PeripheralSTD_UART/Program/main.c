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
#include "simuDev_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
void Serial_RecvIRQ( void );

/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  static uint8_t i = 0;

  HAL_InitTick();

  SIMU_GPIO_Config();
  SIMU_UART_Config(NULL);

  while (1) {
    uint8_t recvData = 0;
    int8_t state = Serial_RecvDataWTO(&recvData, 1, 500);

    if (state == ERROR) {
      LED_R_Toggle();
      printf("timeout ... %d\r\n", i);
      i = (i == 255) ? 0 : i + 1;
    }
    else if (recvData == 0x0D) { // if press enter
      LED_G_Toggle();
      Serial_SendStr("\r\n");
    }
    else {
      LED_B_Toggle();
      Serial_SendData(&recvData, 1);
    }
    if (KEY_Read()) {
      Serial_SendStr("\f");
    }
  }
}

/*************************************** END OF FILE ****************************************/
