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
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void LED_B_BLINK( void )
{
  LED_B_Toggle();
}

void LED_G_BLINK( void )
{
  LED_G_Toggle();
}

void LED_R_BLINK( void )
{
  LED_R_Toggle();
}

int main( void )
{
  HAL_Init();

  BSP_GPIO_Config();
  BSP_TIMER2_Config(LED_B_BLINK, 1);
  BSP_TIMER3_Config(LED_G_BLINK, 2);
  BSP_TIMER4_Config(LED_R_BLINK, 4);

  while (1) {

  }
}

/*************************************** END OF FILE ****************************************/
