/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_timer.c
 *  @author  KitSprout
 *  @date    16-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_timer.h"

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
 *  @brief  TIMER_BaseInit
 */
void TIMER_BaseInit( TIMER_BaseInitTypeDef *htimer )
{
  htimer->Instance->MODE      = htimer->Mode;
  htimer->Instance->PRESCALER = htimer->Prescaler;
  htimer->Instance->BITMODE   = htimer->BitMode;
}

/**
 *  @brief  TIMER_ChannelInit
 */
void TIMER_ChannelInit( TIMER_BaseInitTypeDef *htimer, TIMER_ChannelInitTypeDef *htimerch )
{
  htimer->Instance->CC[htimerch->Channel] = htimerch->Period;
}

/**
 *  @brief  TIMER_Cmd
 */
void TIMER_Cmd( TIMER_BaseInitTypeDef *htimer, uint8_t state )
{
  if (state != ENABLE) {
    TIMER_TASKS_STOP(htimer->Instance) = SET;
  }
  else {
    TIMER_TASKS_CLEAR(htimer->Instance) = SET;
    TIMER_TASKS_START(htimer->Instance) = SET;
  }
}

/**
 *  @brief  TIMER_InterruptCmd
 */
void TIMER_InterruptCmd( TIMER_BaseInitTypeDef *htimer, uint32_t channel, uint8_t state )
{
  uint32_t setting;

  switch (channel) {
    case NRF_TIMER_CC_CHANNEL0:
      setting = NRF_TIMER_INT_COMPARE0_MASK;
      break;
    case NRF_TIMER_CC_CHANNEL1:
      setting = NRF_TIMER_INT_COMPARE1_MASK;
      break;
    case NRF_TIMER_CC_CHANNEL2:
      setting = NRF_TIMER_INT_COMPARE2_MASK;
      break;
    case NRF_TIMER_CC_CHANNEL3:
      setting = NRF_TIMER_INT_COMPARE3_MASK;
      break;
#if (TIMER_COUNT > 3) || defined(__SDK_DOXYGEN__)
    case NRF_TIMER_CC_CHANNEL4:
      setting = NRF_TIMER_INT_COMPARE4_MASK;
      break;
    case NRF_TIMER_CC_CHANNEL5:
      setting = NRF_TIMER_INT_COMPARE5_MASK;
      break;
#endif
  }

  if (state != ENABLE) {
    htimer->Instance->INTENCLR |= setting;
  }
  else {
    htimer->Instance->INTENSET |= setting;
  }
}

/*************************************** END OF FILE ****************************************/
