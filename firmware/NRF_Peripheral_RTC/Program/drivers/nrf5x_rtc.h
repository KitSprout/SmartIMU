/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_rtc.h
 *  @author  KitSprout
 *  @date    16-Jun-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __NRF5x_RTC_H
#define __NRF5x_RTC_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"

/* Define ----------------------------------------------------------------------------------*/
#define NRF_RTC_COMPARE0    (0UL)
#define NRF_RTC_COMPARE1    (1UL)
#define NRF_RTC_COMPARE2    (2UL)
#define NRF_RTC_COMPARE3    (3UL)
#define NRF_RTC_TICK        (4UL)
#define NRF_RTC_OVRFLW      (5UL)

/* Macro -----------------------------------------------------------------------------------*/
#define RTC_TASKS_START(__INS)              __INS->TASKS_START            // Start RTC COUNTER
#define RTC_TASKS_STOP(__INS)               __INS->TASKS_STOP             // Stop RTC COUNTER
#define RTC_TASKS_CLEAR(__INS)              __INS->TASKS_CLEAR            // Clear RTC COUNTER
#define RTC_TASKS_TRIGOVRFLW(__INS)         __INS->TASKS_TRIGOVRFLW       // Set COUNTER to 0xFFFFF0

#define RTC_EVENTS_TICK(__INS)              __INS->EVENTS_TICK            // Event on COUNTER increment
#define RTC_EVENTS_OVRFLW(__INS)            __INS->EVENTS_OVRFLW          // Event on COUNTER overflow
#define RTC_EVENTS_COMPARE(__INS, __CH)     __INS->EVENTS_COMPARE[__CH]   // Compare event on CC[x] match

/* Typedef ---------------------------------------------------------------------------------*/

typedef struct {
  NRF_RTC_Type *Instance;

  uint32_t Prescaler;
  // freq = 32.768kHz / (Prescaler + 1)
  pFunc TickEventCallback;
  // RTC TICK EVENT CALLBACK
  pFunc OverflowEventCallback;
  // RTC OVERFLOW EVENT CALLBACK
} RTC_BaseInitTypeDef;

typedef struct {
  uint32_t Period;
  // 24-bit value
  uint32_t Channel;
  // NRF_RTC_COMPARE0 - RTC compare channel 0.
  // NRF_RTC_COMPARE1 - RTC compare channel 1.
  // NRF_RTC_COMPARE2 - RTC compare channel 2.
  // NRF_RTC_COMPARE3 - RTC compare channel 3.
  pFunc EventCallback;
  // RTC COMPARE EVENT CALLBACK
} RTC_ChannelInitTypeDef;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
void      RTC_BaseInit( RTC_BaseInitTypeDef *hrtc );
void      RTC_ChannelInit( RTC_BaseInitTypeDef *hrtc, RTC_ChannelInitTypeDef *hcompare );
void      RTC_Cmd( RTC_BaseInitTypeDef *hrtc, uint8_t state );
void      RTC_InterruptCmd( RTC_BaseInitTypeDef *hrtc, uint32_t channel, uint8_t state );
void      RTC_RoutingCmd( RTC_BaseInitTypeDef *hrtc, uint32_t channel, uint8_t state );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
