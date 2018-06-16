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
#include "drivers\nrf5x_rtc.h"
#include "nrf5x_bsp.h"

/** @addtogroup NRF5x_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
void rtc_tick_event( void );
void rtc_compare_event( void );

/* Functions -------------------------------------------------------------------------------*/

int main( void )
{
  bsp_gpio_init();
  bsp_rtc_init(rtc_tick_event, rtc_compare_event, RTCx_CLOCK_FREQ);
  bsp_rtc_enable(ENABLE);

  while (1) {
    __WFI();
  }
}

void rtc_tick_event( void )
{
//  LED_Toggle();
}

void rtc_compare_event( void )
{
  LED_Toggle();
}

/*************************************** END OF FILE ****************************************/
