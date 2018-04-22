/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    main.c
 *  @author  KitSprout
 *  @date    22-Apr-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "stm32f4xx_bsp.h"

/** @addtogroup STM32_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
__IO uint32_t sec = 0, msc = 0;
uint32_t counter_10ms = 0;
uint32_t counter_100ms = 0;
uint32_t counter_1000ms = 0;

/* Prototypes ------------------------------------------------------------------------------*/
void IRQEvent_TimerTick( void );

/* Functions -------------------------------------------------------------------------------*/

int main( void )
{
  HAL_Init();
  BSP_GPIO_Config();
  BSP_TIMER2_Config(IRQEvent_TimerTick, 1000);

  while (1) {
    if (counter_10ms == 10) {
      counter_10ms = 0;
      LED_B_Toggle();
    }
    if (counter_100ms == 100) {
      counter_100ms = 0;
      LED_G_Toggle();
    }
    if (counter_1000ms == 1000) {
      counter_1000ms = 0;
      LED_R_Toggle();
    }
  }
}

void IRQEvent_TimerTick( void )
{
  counter_10ms++;
  counter_100ms++;
  counter_1000ms++;
  if (++msc == 60) {
    msc = 0;
    sec++;
  }
}

/*************************************** END OF FILE ****************************************/
