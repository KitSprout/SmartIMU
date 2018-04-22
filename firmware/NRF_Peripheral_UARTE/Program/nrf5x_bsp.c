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
 *  @date    22-Apr-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_clock.h"
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
TIMER_BaseInitTypeDef htimer;
TIMER_ChannelInitTypeDef htimerCC;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void BSP_CLOCK_Config( void )
{
  CLOCK_Config();
}

void BSP_GPIO_Config( void )
{
  nrf_gpio_cfg_output(LED_PIN);
  nrf_gpio_cfg_input(KEY_PIN, NRF_GPIO_PIN_PULLUP);

  LED_Off();
}

void BSP_TIMER_Config( pFunc event, uint32_t freq )
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

  TIMER_Cmd(&htimer, ENABLE);
}

void BSP_UART_SERIAL_Config( pFunc event )
{
  hSerial.RxEventCallback = event;

  Serial_Config();
  printf("\f\fHello World!\r\n\r\n");

  kSerial_SetSendFunction(Serial_SendData);
//  kSerial_SetRecvByteFunction(Serial_RecvByte);
}

/*************************************** END OF FILE ****************************************/
