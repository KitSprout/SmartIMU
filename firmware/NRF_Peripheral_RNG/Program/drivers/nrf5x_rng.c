/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_rng.c
 *  @author  KitSprout
 *  @date    16-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_rng.h"

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
 *  @brief  RNG_Init
 */
void RNG_Init( RNG_InitTypeDef *hrng )
{
  if (hrng->Shortcut != ENABLE) {
    NRF_RNG->SHORTS &= ~NRF_RNG_SHORT_VALRDY_STOP_MASK;
  }
  else {
    NRF_RNG->SHORTS |= NRF_RNG_SHORT_VALRDY_STOP_MASK;
  }

  if (hrng->BiasCorrection != ENABLE) {
    NRF_RNG->CONFIG &= ~RNG_CONFIG_DERCEN_Msk;
  }
  else {
    NRF_RNG->CONFIG |= RNG_CONFIG_DERCEN_Msk;
  }
}

/**
 *  @brief  RNG_Cmd
 */
void RNG_Cmd( uint32_t state )
{
  if (state != ENABLE) {
    RNG_TASKS_STOP() = SET;
  }
  else {
    RNG_TASKS_START() = SET;
  }
}

/**
 *  @brief  RNG_InterruptCmd
 */
void RNG_InterruptCmd( uint32_t state )
{
  if (state != ENABLE) {
    NRF_RNG->INTENCLR = NRF_RNG_INT_VALRDY_MASK;
  }
  else {
    NRF_RNG->INTENSET = NRF_RNG_INT_VALRDY_MASK;
  }
}

/**
 *  @brief  RNG_GetRandomValue
 */
uint8_t RNG_GetRandomValue( RNG_InitTypeDef *hrng )
{
  if (hrng->EventCallback == NULL) {
    while (RNG_EVENTS_VALRDY != SET);
  }
  RNG_EVENTS_VALRDY = RESET;
  return (uint8_t)(NRF_RNG->VALUE & RNG_VALUE_VALUE_Msk);
}

/*************************************** END OF FILE ****************************************/
