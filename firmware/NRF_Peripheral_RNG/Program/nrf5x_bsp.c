/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_bsp.c
 *  @author  KitSprout
 *  @date    16-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_clock.h"
#include "drivers\nrf5x_rng.h"
#include "drivers\nrf5x_timer.h"
#include "modules\serial.h"
#include "modules\kSerial.h"
#include "nrf5x_bsp.h"

/** @addtogroup NRF5x_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
RNG_InitTypeDef hrng;
TIMER_BaseInitTypeDef htimer;
TIMER_ChannelInitTypeDef htimerCC;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void bsp_gpio_init( void )
{
  CLOCK_Config();

  nrf_gpio_cfg_output(LED_PIN);
  nrf_gpio_cfg_input(KEY_PIN, NRF_GPIO_PIN_PULLUP);

  LED_Off();
}

void bsp_rng_init( pFunc event )
{
  hrng.Instance       = RNGx;
  hrng.Shortcut       = DISABLE;
  hrng.BiasCorrection = ENABLE;
  RNG_Init(&hrng);

  if (event != NULL) {
    hrng.EventCallback  = event;
    RNG_InterruptCmd(ENABLE);
    NVIC_SetPriority(RNGx_IRQn, RNGx_IRQn_PRIORITY);
    NVIC_EnableIRQ(RNGx_IRQn);
  }

  RNG_Cmd(ENABLE);
}

void bsp_timer_init( pFunc event, uint32_t freq )
{
  htimer.Instance  = TIMERx;
  htimer.Mode      = TIMERx_MODE;
  htimer.BitMode   = TIMERx_BIT_MODE;
  htimer.Prescaler = TIMERx_PRESCALER;
  TIMER_BaseInit(&htimer);

  htimerCC.Channel = TIMERx_CHANNEL;
  htimerCC.Period  = freq;
  htimerCC.EventCallback = event;
  TIMER_ChannelInit(&htimer, &htimerCC);
  TIMER_InterruptCmd(&htimer, htimerCC.Channel, ENABLE);

  NVIC_SetPriority(TIMERx_IRQn, TIMERx_IRQn_PRIORITY);
  NVIC_EnableIRQ(TIMERx_IRQn);
}

void bsp_timer_enable( uint32_t state )
{
  TIMER_Cmd(&htimer, state);
}

void bsp_serial_init( pFunc event )
{
  hSerial.RxEventCallback = event;

  Serial_Config();
  printf("\f\fHello World!\r\n\r\n");
}

/*************************************** END OF FILE ****************************************/
