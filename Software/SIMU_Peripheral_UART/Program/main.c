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
/* Private function prototypes -------------------------------------------------------------*/
void Serial_RecvIRQ( void );

/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  static uint8_t i = 0;

  HAL_Init();

  SIMU_GPIO_Config();

  if(!KEY_Read()) {
    SIMU_UART_Config(NULL);

    while (1) {
      uint8_t recvData = 0;
      int8_t status = Serial_RecvByteWTO(&recvData, 500);

      if(status == HAL_TIMEOUT) {
        LED_R_Toggle();
        printf("Timeout ... %d\r\n", i);
        i = (i == 255) ? 0 : i + 1;
      }
      else if(recvData == 0x0D) { // if press enter
        LED_G_Toggle();
        Serial_SendStr("\r\n");
      }
      else {
        LED_B_Toggle();
        Serial_SendData(&recvData, 1);
      }
      if(KEY_Read()) {
        Serial_SendStr("\f");
      }
    }
  }
  else {
    SIMU_UART_Config(Serial_RecvIRQ);

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
  uint8_t recvData = Serial_RecvByte();

  LED_G_Toggle();
  if (recvData == 0x0D)
    Serial_SendStr("\r\n");
  else
    Serial_SendByte(recvData);
}

/*************************************** END OF FILE ****************************************/
