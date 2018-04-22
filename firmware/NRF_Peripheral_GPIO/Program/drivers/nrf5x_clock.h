/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_clock.h
 *  @author  KitSprout
 *  @date    11-Apr-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __NRF5x_CLOCK_H
#define __NRF5x_CLOCK_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "nrf.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
#define CLOCK_TASKS_HFCLKSTART      NRF_CLOCK->TASKS_HFCLKSTART     // Start HFCLK crystal oscillator
#define CLOCK_TASKS_HFCLKSTOP       NRF_CLOCK->TASKS_HFCLKSTOP      // Stop HFCLK crystal oscillator
#define CLOCK_TASKS_LFCLKSTART      NRF_CLOCK->TASKS_LFCLKSTART     // Start LFCLK source
#define CLOCK_TASKS_LFCLKSTOP       NRF_CLOCK->TASKS_LFCLKSTOP      // Stop LFCLK source
#define CLOCK_TASKS_CAL             NRF_CLOCK->TASKS_CAL            // Start calibration of LFCLK RC oscillator
#define CLOCK_TASKS_CTSTART         NRF_CLOCK->TASKS_CTSTART        // Start calibration timer
#define CLOCK_TASKS_CTSTOP          NRF_CLOCK->TASKS_CTSTOP         // Stop calibration timer

#define CLOCK_EVENTS_HFCLKSTARTED   NRF_CLOCK->EVENTS_HFCLKSTARTED  // HFCLK oscillator started
#define CLOCK_EVENTS_LFCLKSTARTED   NRF_CLOCK->EVENTS_LFCLKSTARTED  // LFCLK started
#define CLOCK_EVENTS_DONE           NRF_CLOCK->EVENTS_DONE          // Calibration of LFCLK RC oscillator complete event
#define CLOCK_EVENTS_CTTO           NRF_CLOCK->EVENTS_CTTO          // Calibration timer timeout

/* Typedef ---------------------------------------------------------------------------------*/

typedef struct {
  uint32_t HFCLKSource;
  // NRF_CLOCK_HFCLK_LOW_ACCURACY  - Internal 16 MHz RC oscillator.
  // NRF_CLOCK_HFCLK_HIGH_ACCURACY - External 16 MHz/32 MHz crystal oscillator.
  uint32_t HFCLKState;
  // ENABLE or DISABLE
  uint32_t LFCLKSource;
  // NRF_CLOCK_LFCLK_RC    - Internal 32KiHz RC oscillator.
  // NRF_CLOCK_LFCLK_Xtal  - External 32KiHz crystal.
  // NRF_CLOCK_LFCLK_Synth - Internal 32KiHz synthesizer from HFCLK system clock.
  uint32_t LFCLKState;
  // ENABLE or DISABLE
  uint32_t LFCLKCalibrate;
  //
} CLOCK_InitTypeDef;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
void CLOCK_Config( void );
void CLOCK_Init( CLOCK_InitTypeDef *hclock );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
