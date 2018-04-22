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
#include "drivers\nrf5x_system.h"
#include "nrf5x_bsp.h"

/** @addtogroup NRF5x_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
#define TIMER_TICK_FREQ   1000
#define LED_TOGGLE_FREQ   5

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static uint32_t sec = 0, msc = 0;
static uint16_t toggleCount = 0;

/* Prototypes ------------------------------------------------------------------------------*/
void IRQEvent_TimerTick( void );

/* Functions -------------------------------------------------------------------------------*/

int main( void )
{
  BSP_CLOCK_Config();
  BSP_GPIO_Config();
  BSP_TIMER_Config(IRQEvent_TimerTick, TIMER_TICK_FREQ);

  while (1) {
    if (toggleCount >= (TIMER_TICK_FREQ / LED_TOGGLE_FREQ)) {
      toggleCount = 0;
      LED_Toggle();
    }
  }
}

void IRQEvent_TimerTick( void )
{
  toggleCount++;
  if (++msc >= TIMER_TICK_FREQ) {
    msc = 0;
    sec++;
  }
}

/*************************************** END OF FILE ****************************************/
