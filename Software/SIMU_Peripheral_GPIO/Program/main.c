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
  * @date    5-Nov-2016
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
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  HAL_Init();

  SIMU_GPIO_Config();

  while (1) {
    LED_R_Toggle();
    delay_ms(100);
    LED_G_Toggle();
    delay_ms(100);
    LED_B_Toggle();
    delay_ms(100);
    while(KEY_Read()) {
      LED_R_Set();
      LED_G_Set();
      LED_B_Set();
      delay_ms(200);
      LED_R_Reset();
      LED_G_Reset();
      LED_B_Reset();
      delay_ms(200);
    }
  }
}

/*************************************** END OF FILE ****************************************/
