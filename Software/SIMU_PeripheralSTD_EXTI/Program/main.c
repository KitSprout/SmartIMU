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
  * @date    16-Nov-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "stm32f4xx_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
__IO int8_t state = 0;

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void EXTI_EvenCallback( void )
{
  state = !state;
}

int main( void )
{
  HAL_InitTick();
  BSP_GPIO_Config();
  BSP_EXTI_Config(EXTI_EvenCallback);

  while (1) {
    if (state == HAL_OK) {
      LED_R_Toggle();
      delay_ms(80);
      LED_G_Toggle();
      delay_ms(80);
      LED_B_Toggle();
      delay_ms(80);
    }
  }
}

/*************************************** END OF FILE ****************************************/
