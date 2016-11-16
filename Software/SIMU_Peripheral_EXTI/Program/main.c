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
#include "smartimu_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
static __IO uint8_t flag = 0;

/* Private function prototypes -------------------------------------------------------------*/
void EXTI0_IRQCallback( void );

/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  HAL_Init();

  SIMU_GPIO_Config();
  SIMU_EXTI0_Config(EXTI0_IRQCallback);

  while (1) {
    LED_R_Toggle();
    delay_ms(80);
    LED_G_Toggle();
    delay_ms(80);
    LED_B_Toggle();
    delay_ms(80);
    while(flag) {
      delay_ms(2000);
      flag = 0;
    }
  }
}

void EXTI0_IRQCallback( void )
{
  flag = 1;
}
  
/*************************************** END OF FILE ****************************************/
