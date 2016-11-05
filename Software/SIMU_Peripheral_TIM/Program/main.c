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
#define TIMx_PRES       ((uint32_t)(SystemCoreClock / 10000)) // TIMCLK = 100MHz, 10kHz
#define TIM2_PERIOD     ((uint32_t)(5000))    // 2 Hz
#define TIM3_PERIOD     ((uint32_t)(10000))   // 1 Hz

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

int main( void )
{
  HAL_Init();

  SIMU_GPIO_Config();
  SIMU_TIM2_Config(LED_B_BLINK, TIMx_PRES, TIM2_PERIOD);
  SIMU_TIM3_Config(LED_G_BLINK, TIMx_PRES, TIM3_PERIOD);

  while (1) {

  }
}

/*************************************** END OF FILE ****************************************/
