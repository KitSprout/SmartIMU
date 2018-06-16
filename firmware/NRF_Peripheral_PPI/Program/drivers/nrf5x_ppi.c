/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_ppi.c
 *  @author  KitSprout
 *  @date    16-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_ppi.h"

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
 *  @brief  PPI_Init
 */
void PPI_Init( PPI_InitTypeDef *hclock )
{
  NRF_PPI->CH[hclock->Channel].EEP = hclock->EventEndPoint;
  NRF_PPI->CH[hclock->Channel].TEP = hclock->TaskEndPoint;
  if (hclock->ForkState == ENABLE) {
    NRF_PPI->FORK[hclock->Channel].TEP = hclock->ForkTaskEndPoint;
  }
}

/**
 *  @brief  PPI_SetChannelGroup
 */
void PPI_SetChannelGroup( PPI_InitTypeDef *hclock, uint32_t state )
{
  if (state != ENABLE) {
    NRF_PPI->CHG[hclock->Group] = NRF_PPI->CHG[hclock->Group] & ~(1UL << hclock->Channel);
  }
  else {
    NRF_PPI->CHG[hclock->Group] = NRF_PPI->CHG[hclock->Group] |  (1UL << hclock->Channel);
  }
}

/**
 *  @brief  PPI_ChannelCmd
 */
void PPI_ChannelCmd( PPI_InitTypeDef *hclock, uint32_t state )
{
  if (state != ENABLE) {
    NRF_PPI->CHENCLR = 1UL << hclock->Channel;
  }
  else {
    NRF_PPI->CHENSET = 1UL << hclock->Channel;
  }
}

/**
 *  @brief  PPI_GroupCmd
 */
void PPI_GroupCmd( PPI_InitTypeDef *hclock, uint32_t state )
{
  if (state != ENABLE) {
    PPI_TASKS_CHG_DIS(hclock->Group) = SET;
  }
  else {
    PPI_TASKS_CHG_EN(hclock->Group) = SET;
  }
}

/*************************************** END OF FILE ****************************************/
