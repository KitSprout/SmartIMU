/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32f4_exti.c
 *  @author  KitSprout
 *  @date    10-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_exti.h"

/** @addtogroup STM32_Driver
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
pFunc lk_state_event = NULL;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  LK_STATE_EXTI_Config
 */
void LK_STATE_EXTI_Config( pFunc event )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  lk_state_event = event;

  /* GPIO Pin ******************************************************************/
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Pin  = LK_STATE_PIN;
  HAL_GPIO_Init(LK_STATE_GPIO_PORT, &GPIO_InitStruct);

  /* EXTI IT *******************************************************************/
  HAL_NVIC_SetPriority(LK_STATE_EXTIx_IRQ, LK_STATE_EXTIx_IRQn_PREEMPT, LK_STATE_EXTIx_IRQn_SUB);
  HAL_NVIC_EnableIRQ(LK_STATE_EXTIx_IRQ);
}

/*************************************** END OF FILE ****************************************/
