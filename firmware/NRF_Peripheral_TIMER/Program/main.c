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
 *  @date    16-Jun-2018
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

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static uint32_t sec = 0;
static uint32_t msc = 0;

uint32_t tick_100ms = 0;

/* Prototypes ------------------------------------------------------------------------------*/
void timer_tick_event( void );

/* Functions -------------------------------------------------------------------------------*/

int main( void )
{
  bsp_gpio_init();
  bsp_timer_init(timer_tick_event, TIMER_TICK_FREQ);
  bsp_timer_enable(ENABLE);

  while (1) {
    if (tick_100ms >= 100) {
      tick_100ms = 0;
      LED_Toggle();
    }
  }
}

void timer_tick_event( void )
{
  tick_100ms++;
  if (++msc >= TIMER_TICK_FREQ) {
    msc = 0;
    sec++;
  }
}

/*************************************** END OF FILE ****************************************/
