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
  * @date    14-Jan-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "stm32f4xx_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
#define EXAMPLE_UART_IRQ

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
void Serial_RecvIRQ( void );

/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  HAL_InitTick();
  BSP_GPIO_Config();

#if defined(EXAMPLE_UART_IRQ)
  BSP_UART_Config(Serial_RecvIRQ);
  while (1) {
    while (KEY_Read()) {
      printf("/f");
    }
  }

#else
  static uint8_t i = 0;

  BSP_UART_Config(NULL);
  while (1) {
    int8_t state;
    uint8_t recvByte;
    state = Serial_RecvDataWTO(&recvByte, 1, 500);

    if (state == HAL_TIMEOUT) {
      LED_R_Toggle();
      printf("Timeout ... %d\r\n", i);
      i = (i == 255) ? 0 : i + 1;
    }
    else if (recvByte == 0x0D) { // if press enter
      LED_G_Toggle();
      Serial_SendStr("\r\n");
    }
    else {
      LED_B_Toggle();
      Serial_SendByte(recvByte);
    }
    if(KEY_Read()) {
      Serial_SendStr("\f");
    }
  }
#endif

}

void Serial_RecvIRQ( void )
{
  uint8_t recvData = Serial_RecvByte();
  if (recvData == 0x0D)
    Serial_SendStr("\r\n");
  else
    Serial_SendByte(recvData);
}

/*************************************** END OF FILE ****************************************/
