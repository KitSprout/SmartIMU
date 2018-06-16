/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_ppi.h
 *  @author  KitSprout
 *  @date    01-Jun-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __NRF5x_PPI_H
#define __NRF5x_PPI_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "nrf.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
#define PPI_TASKS_CHG_EN(__CHG)   NRF_PPI->TASKS_CHG[__CHG].EN    // Enable channel group x
#define PPI_TASKS_CHG_DIS(__CHG)  NRF_PPI->TASKS_CHG[__CHG].DIS   // Disable channel group x

/* Typedef ---------------------------------------------------------------------------------*/

typedef struct {
  uint32_t Channel;
  // NRF_PPI_CHANNEL0  - Channel 0.
  // NRF_PPI_CHANNEL1  - Channel 1.
  // NRF_PPI_CHANNEL2  - Channel 2.
  // NRF_PPI_CHANNEL3  - Channel 3.
  // NRF_PPI_CHANNEL4  - Channel 4.
  // NRF_PPI_CHANNEL5  - Channel 5.
  // NRF_PPI_CHANNEL6  - Channel 6.
  // NRF_PPI_CHANNEL7  - Channel 7.
  // NRF_PPI_CHANNEL8  - Channel 8.
  // NRF_PPI_CHANNEL9  - Channel 9.
  // NRF_PPI_CHANNEL10 - Channel 10.
  // NRF_PPI_CHANNEL11 - Channel 11.
  // NRF_PPI_CHANNEL12 - Channel 12.
  // NRF_PPI_CHANNEL13 - Channel 13.
  // NRF_PPI_CHANNEL14 - Channel 14.
  // NRF_PPI_CHANNEL15 - Channel 15.
  // NRF_PPI_CHANNEL16 - Channel 16.
  // NRF_PPI_CHANNEL17 - Channel 17.
  // NRF_PPI_CHANNEL18 - Channel 18.
  // NRF_PPI_CHANNEL19 - Channel 19.
  // NRF_PPI_CHANNEL20 - Channel 20.
  // NRF_PPI_CHANNEL21 - Channel 21.
  // NRF_PPI_CHANNEL22 - Channel 22.
  // NRF_PPI_CHANNEL23 - Channel 23.
  // NRF_PPI_CHANNEL24 - Channel 24.
  // NRF_PPI_CHANNEL25 - Channel 25.
  // NRF_PPI_CHANNEL26 - Channel 26.
  // NRF_PPI_CHANNEL27 - Channel 27.
  // NRF_PPI_CHANNEL28 - Channel 28.
  // NRF_PPI_CHANNEL29 - Channel 29.
  // NRF_PPI_CHANNEL30 - Channel 30.
  // NRF_PPI_CHANNEL31 - Channel 31.
  uint32_t Group;
  // NRF_PPI_CHANNEL_GROUP0 - Channel group 0.
  // NRF_PPI_CHANNEL_GROUP1 - Channel group 1.
  // NRF_PPI_CHANNEL_GROUP2 - Channel group 2.
  // NRF_PPI_CHANNEL_GROUP3 - Channel group 3.
  // NRF_PPI_CHANNEL_GROUP4 - Channel group 4.
  // NRF_PPI_CHANNEL_GROUP5 - Channel group 5.
  uint32_t EventEndPoint;
  // NRF_EVENTS IN
  uint32_t TaskEndPoint;
  // NRF_TASKS OUT
  uint32_t ForkState;
  // ENABLE / DISABLE
  uint32_t ForkTaskEndPoint;
  // NRF_TASKS OUT
} PPI_InitTypeDef;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
void      PPI_Init( PPI_InitTypeDef *hclock );
void      PPI_SetChannelGroup( PPI_InitTypeDef *hclock, uint32_t state );
void      PPI_ChannelCmd( PPI_InitTypeDef *hclock, uint32_t state );
void      PPI_GroupCmd( PPI_InitTypeDef *hclock, uint32_t state );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
