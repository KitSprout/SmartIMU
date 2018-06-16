/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_rtc.c
 *  @author  KitSprout
 *  @date    16-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_rtc.h"

/** @addtogroup NRF5x_Driver
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  RTC_BaseInit
 */
void RTC_BaseInit( RTC_BaseInitTypeDef *hrtc )
{
  hrtc->Instance->PRESCALER = hrtc->Prescaler;
}

void RTC_ChannelInit( RTC_BaseInitTypeDef *hrtc, RTC_ChannelInitTypeDef *hcompare )
{
  hrtc->Instance->CC[hcompare->Channel] = hcompare->Period;
}

/**
 *  @brief  RTC_Cmd
 */
void RTC_Cmd( RTC_BaseInitTypeDef *hrtc, uint8_t state )
{
  if (state != ENABLE) {
    RTC_TASKS_STOP(hrtc->Instance) = SET;
  }
  else {
    RTC_TASKS_CLEAR(hrtc->Instance) = SET;
    RTC_TASKS_START(hrtc->Instance) = SET;
  }
}

/**
 *  @brief  RTC_InterruptCmd
 */
void RTC_InterruptCmd( RTC_BaseInitTypeDef *hrtc, uint32_t channel, uint8_t state )
{
  uint32_t setting;

  switch (channel) {
    case NRF_RTC_TICK:
      setting = NRF_RTC_INT_TICK_MASK;
      break;
    case NRF_RTC_OVRFLW:
      setting = NRF_RTC_INT_OVERFLOW_MASK;
      break;
    case NRF_RTC_COMPARE0:
      setting = NRF_RTC_INT_COMPARE0_MASK;
      break;
    case NRF_RTC_COMPARE1:
      setting = NRF_RTC_INT_COMPARE1_MASK;
      break;
    case NRF_RTC_COMPARE2:
      setting = NRF_RTC_INT_COMPARE2_MASK;
      break;
    case NRF_RTC_COMPARE3:
      setting = NRF_RTC_INT_COMPARE3_MASK;
      break;
  }

  if (state != ENABLE) {
    hrtc->Instance->INTENCLR |= setting;
  }
  else {
    hrtc->Instance->INTENSET |= setting;
  }
}

/**
 *  @brief  RTC_RoutingCmd
 */
void RTC_RoutingCmd( RTC_BaseInitTypeDef *hrtc, uint32_t channel, uint8_t state )
{
  uint32_t setting;

  switch (channel) {
    case NRF_RTC_TICK:
      setting = NRF_RTC_INT_TICK_MASK;
      break;
    case NRF_RTC_OVRFLW:
      setting = NRF_RTC_INT_OVERFLOW_MASK;
      break;
    case NRF_RTC_COMPARE0:
      setting = NRF_RTC_INT_COMPARE0_MASK;
      break;
    case NRF_RTC_COMPARE1:
      setting = NRF_RTC_INT_COMPARE1_MASK;
      break;
    case NRF_RTC_COMPARE2:
      setting = NRF_RTC_INT_COMPARE2_MASK;
      break;
    case NRF_RTC_COMPARE3:
      setting = NRF_RTC_INT_COMPARE3_MASK;
      break;
  }

  if (state != ENABLE) {
    hrtc->Instance->EVTENCLR |= setting;
  }
  else {
    hrtc->Instance->EVTENSET |= setting;
  }
}

/*************************************** END OF FILE ****************************************/
