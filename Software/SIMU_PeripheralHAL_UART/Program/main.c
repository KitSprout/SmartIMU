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
  * @date    11-Nov-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "smartimu_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
uint8_t recvByte = 0;

/* Private function prototypes -------------------------------------------------------------*/
void Serial_RecvIRQ( void );

/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  static uint8_t i = 0;

  HAL_Init();

  SIMU_GPIO_Config();

  if(!KEY_Read()) {
    SIMU_UART_Config(NULL, NULL);

    while (1) {
      int8_t status = Serial_RecvData(&recvByte, 1, 500);

      if(status == HAL_TIMEOUT) {
        LED_R_Toggle();
        printf("Timeout ... %d\r\n", i);
        i = (i == 255) ? 0 : i + 1;
      }
      else if(recvByte == 0x0D) { // if press enter
        LED_G_Toggle();
        printf("\r\n");
      }
      else {
        LED_B_Toggle();
        Serial_SendData(&recvByte, 1, 5000);
      }
      if(KEY_Read()) {
        printf("\f");
      }
    }
  }
  else {
    SIMU_UART_Config(NULL, Serial_RecvIRQ);
    Serial_RecvDataIT(&recvByte, 1);

    while (1) {
      LED_B_Toggle();
      delay_ms(100);
      while (KEY_Read()) {
        printf("\f");
      }
    }
  }
}

void Serial_RecvIRQ( void )
{
  LED_G_Toggle();
  if (recvByte == 0x0D) {
    printf("\r\n");
  }
  else {
    Serial_SendData(&recvByte, 1, 1000);
  }

  Serial_RecvDataIT(&recvByte, 1);
}
/*************************************** END OF FILE ****************************************/
