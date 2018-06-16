/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_clock.c
 *  @author  KitSprout
 *  @date    21-Apr-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_clock.h"

/** @addtogroup NRF5x_Driver
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
CLOCK_InitTypeDef hclk;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  CLOCK_Config
 */
void CLOCK_Config( void )
{
#if KS_HW_CLOCK_SOUCE
  /* Enable External Oscillator */
  hclk.HFCLKSource = NRF_CLOCK_HFCLK_HIGH_ACCURACY;
  hclk.HFCLKState  = ENABLE;

#else
  /* Enable Internal Oscillator */
  hclk.HFCLKSource = NRF_CLOCK_HFCLK_LOW_ACCURACY;
  hclk.HFCLKState  = ENABLE;

#endif

#if KS_HW_LCLOCK_SOUCE
  /* Enable External Oscillator */
  hclk.LFCLKSource = NRF_CLOCK_LFCLK_Xtal;
  hclk.LFCLKState  = ENABLE;

#else
  /* Enable Internal Oscillator */
  hclk.LFCLKSource = NRF_CLOCK_LFCLK_RC;
  hclk.LFCLKState  = ENABLE;

#endif

  CLOCK_Init(&hclk);
}

/**
 *  @brief  CLOCK_Init
 */
void CLOCK_Init( CLOCK_InitTypeDef *hclock )
{
  // Configure HFCLK
  if (hclock->HFCLKState == ENABLE) {
    if (hclock->HFCLKSource == NRF_CLOCK_HFCLK_HIGH_ACCURACY) {
      CLOCK_EVENTS_HFCLKSTARTED = RESET;
      CLOCK_TASKS_HFCLKSTART = SET;
      while (CLOCK_EVENTS_HFCLKSTARTED != SET);
    }
    else {
      CLOCK_TASKS_HFCLKSTOP = SET;
    }
    CLOCK_EVENTS_HFCLKSTARTED = RESET;
  }
  else {
    CLOCK_TASKS_HFCLKSTOP = SET;
  }

  // Configure LFCLK
  if (hclock->LFCLKState == ENABLE) {
    NRF_CLOCK->LFCLKSRC = (uint32_t)((hclock->LFCLKSource << CLOCK_LFCLKSRC_SRC_Pos) & CLOCK_LFCLKSRC_SRC_Msk);
    CLOCK_EVENTS_LFCLKSTARTED = RESET;
    CLOCK_TASKS_LFCLKSTART = SET;
    while (CLOCK_EVENTS_LFCLKSTARTED != SET);
    CLOCK_EVENTS_LFCLKSTARTED = RESET;
  }
  else {
    CLOCK_TASKS_LFCLKSTOP = SET;
  }

  // calibrated
  // interval   New calibration timer interval in 0.25 s resolution (range: 0.25 seconds to 31.75 seconds).
//  NRF_CLOCK->CTIV = ((interval << CLOCK_CTIV_CTIV_Pos) & CLOCK_CTIV_CTIV_Msk);
}

/*************************************** END OF FILE ****************************************/
