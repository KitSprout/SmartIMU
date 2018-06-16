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
#include "drivers\nrf5x_timer.h"
#include "drivers\nrf5x_ppi.h"
#include "nrf5x_bsp.h"

/** @addtogroup NRF5x_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
TIMER_BaseInitTypeDef htick;
TIMER_BaseInitTypeDef htick_start;
TIMER_BaseInitTypeDef htick_stop;
TIMER_ChannelInitTypeDef htick_cc[3];

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void bsp_gpio_init( void )
{
  CLOCK_Config();

  nrf_gpio_cfg_output(LED_PIN);
  nrf_gpio_cfg_input(KEY_PIN, NRF_GPIO_PIN_PULLUP);

  LED_Off();
}

void bsp_timer_init( pFunc event )
{
  // tick
  htick.Instance  = TIMERx;
  htick.Mode      = TIMERx_MODE;
  htick.BitMode   = TIMERx_BIT_MODE;
  htick.Prescaler = TIMERx_PRESCALER;
  TIMER_BaseInit(&htick);

  htick_cc[0].Channel       = TIMERx_CHANNEL;
  htick_cc[0].Period        = TIMERx_CHANNEL_PERIOD;
  htick_cc[0].EventCallback = event;
  TIMER_ChannelInit(&htick, &htick_cc[0]);
  TIMER_InterruptCmd(&htick, htick_cc[0].Channel, ENABLE);

  NVIC_SetPriority(TIMERx_IRQn, TIMERx_IRQn_PRIORITY);
  NVIC_EnableIRQ(TIMERx_IRQn);

  // tick start event
  htick_start.Instance  = NRF_TIMER1;
  htick_start.Mode      = NRF_TIMER_MODE_TIMER;
  htick_start.BitMode   = NRF_TIMER_BIT_WIDTH_16;
  htick_start.Prescaler = NRF_TIMER_FREQ_31250Hz;
  TIMER_BaseInit(&htick_start);

  htick_cc[1].Channel       = NRF_TIMER_CC_CHANNEL0;
  htick_cc[1].Period        = 2000 * 31.25f;
  htick_cc[1].EventCallback = NULL;
  TIMER_ChannelInit(&htick_start, &htick_cc[1]);

  // tick stop event
  htick_stop.Instance  = NRF_TIMER2;
  htick_stop.Mode      = NRF_TIMER_MODE_TIMER;
  htick_stop.BitMode   = NRF_TIMER_BIT_WIDTH_16;
  htick_stop.Prescaler = NRF_TIMER_FREQ_31250Hz;
  TIMER_BaseInit(&htick_stop);

  htick_cc[2].Channel       = NRF_TIMER_CC_CHANNEL0;
  htick_cc[2].Period        = 1000 * 31.25f;
  htick_cc[2].EventCallback = NULL;
  TIMER_ChannelInit(&htick_stop, &htick_cc[2]);

//  TIMER_Cmd(&htick, ENABLE);
  TIMER_Cmd(&htick_start, ENABLE);
  TIMER_Cmd(&htick_stop, ENABLE);
}

void bsp_ppi_init( void )
{
  PPI_InitTypeDef hppi_start;
  PPI_InitTypeDef hppi_stop;

  hppi_start.Channel          = NRF_PPI_CHANNEL0;
  hppi_start.Group            = NRF_PPI_CHANNEL_GROUP0;
  hppi_start.EventEndPoint    = (uint32_t)(&TIMER_EVENTS_COPPARE(htick_start.Instance, htick_cc[1].Channel));
  hppi_start.TaskEndPoint     = (uint32_t)(&htick.Instance->TASKS_START);
  hppi_start.ForkState        = DISABLE;
  hppi_start.ForkTaskEndPoint = NULL;
  PPI_Init(&hppi_start);

  hppi_stop.Channel           = NRF_PPI_CHANNEL1;
  hppi_stop.Group             = NRF_PPI_CHANNEL_GROUP0;
  hppi_stop.EventEndPoint     = (uint32_t)(&TIMER_EVENTS_COPPARE(htick_stop.Instance, htick_cc[2].Channel));
  hppi_stop.TaskEndPoint      = (uint32_t)(&htick.Instance->TASKS_STOP);
  hppi_start.ForkState        = DISABLE;
  hppi_start.ForkTaskEndPoint = NULL;
  PPI_Init(&hppi_stop);

  PPI_ChannelCmd(&hppi_start, ENABLE);
  PPI_ChannelCmd(&hppi_stop,  ENABLE);

//  PPI_SetChannelGroup(&hppi_start, ENABLE);
//  PPI_SetChannelGroup(&hppi_stop,  ENABLE);
//  PPI_GroupCmd(&hppi_start, ENABLE);
//  PPI_GroupCmd(&hppi_stop,  ENABLE);
}

/*************************************** END OF FILE ****************************************/
