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
 *  @date    03-Jun-2018
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
#define TIMER_TICK_FREQ   1000

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static uint32_t sec = 0;
static uint32_t msc = 0;

//uint32_t tick_10ms = 0;
//uint32_t tick_100ms = 0;
uint32_t tick_1000ms = 0;

/* Prototypes ------------------------------------------------------------------------------*/
void timer_tick_event( void );

/* Functions -------------------------------------------------------------------------------*/

int main( void )
{
  bsp_gpio_init();
  bsp_timer_init(timer_tick_event, 1000);
  bsp_timer_enable(ENABLE);

  while (1) {
//    if (tick_10ms == 10) {
//      tick_10ms = 0;
//      LED_B_Toggle();
//    }
//    if (tick_100ms == 100) {
//      tick_100ms = 0;
//      LED_G_Toggle();
//    }
    if (tick_1000ms == 1000) {
      tick_1000ms = 0;
      LED_R_Toggle();
    }
  }
}

void timer_tick_event( void )
{
//  tick_10ms++;
//  tick_100ms++;
  tick_1000ms++;
  if (++msc >= TIMER_TICK_FREQ) {
    msc = 0;
    sec++;
  }
}

/*************************************** END OF FILE ****************************************/
