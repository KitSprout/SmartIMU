/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_temp.c
 *  @author  KitSprout
 *  @date    16-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_temp.h"

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
 *  @brief  TEMP_Init
 */
void TEMP_Init( void )
{
  nrf_temp_init();
}

/**
 *  @brief  TEMP_GetTemperature
 */
float32_t TEMP_GetTemperature( void )
{
  float32_t temperature;

  TEMP_TASKS_START() = SET;
  while (TEMP_EVENTS_DATARDY() != SET);
  TEMP_EVENTS_DATARDY() = RESET;
  temperature = nrf_temp_read() * 0.25f;

  return temperature;
}

/*************************************** END OF FILE ****************************************/
