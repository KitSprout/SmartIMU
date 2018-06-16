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
#include "drivers\nrf5x_rtc.h"
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
TIMER_BaseInitTypeDef htimer;
TIMER_ChannelInitTypeDef htimerCC;
RTC_BaseInitTypeDef hrtc;
RTC_ChannelInitTypeDef hrtc_ch;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void bsp_gpio_init( void )
{
  CLOCK_Config();

  nrf_gpio_cfg_output(LED_PIN);
  nrf_gpio_cfg_input(KEY_PIN, NRF_GPIO_PIN_PULLUP);

  LED_Off();
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

void bsp_rtc_init( pFunc tick_event, pFunc compare_event, uint32_t freq )
{
  hrtc.Instance              = RTCx;
  hrtc.Prescaler             = RTC_FREQ_TO_PRESCALER(freq);
  hrtc.TickEventCallback     = tick_event;
  hrtc.OverflowEventCallback = NULL;
  RTC_BaseInit(&hrtc);

  hrtc_ch.Channel       = RTCx_CHANNEL;
  hrtc_ch.Period        = RTCx_CHANNEL_PERIOD;
  hrtc_ch.EventCallback = compare_event;
  RTC_ChannelInit(&hrtc, &hrtc_ch);

  if (tick_event != NULL) {
    RTC_InterruptCmd(&hrtc, NRF_RTC_TICK, ENABLE);
//    RTC_RoutingCmd(&hrtc, NRF_RTC_TICK, ENABLE);
  }
  if (compare_event != NULL) {
    RTC_InterruptCmd(&hrtc, NRF_RTC_COMPARE0, ENABLE);
//    RTC_RoutingCmd(&hrtc, NRF_RTC_COMPARE0, ENABLE);
  }
  if ((tick_event != NULL) || (compare_event != NULL)) {
    NVIC_SetPriority(RTCx_IRQn, RTCx_IRQn_PRIORITY);
    NVIC_EnableIRQ(RTCx_IRQn);
  }
}

void bsp_rtc_enable( uint32_t state )
{
  RTC_Cmd(&hrtc, state);
}

void bsp_serial_init( pFunc event )
{
  hSerial.RxEventCallback = event;

  Serial_Config();
  printf("\f\fHello World!\r\n\r\n");
}

/*************************************** END OF FILE ****************************************/
