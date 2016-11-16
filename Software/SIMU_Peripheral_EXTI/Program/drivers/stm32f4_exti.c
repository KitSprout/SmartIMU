/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_exti.c
  * @author  KitSprout
  * @date    16-Nov-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4_system.h"
#include "stm32f4_exti.h"

/** @addtogroup STM32_Driver
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
static pFunc EXTI0_Callback = NULL;

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  EXTI0_Config
  * @param  None
  * @retval None
  */
void EXTI0_Config( pFunc extix )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  EXTI0_Callback = extix;

  /* GPIO Pin ******************************************************************/
  GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Pin   = EXTI0_PIN;
  HAL_GPIO_Init(EXTI0_GPIO_PORT, &GPIO_InitStruct);

  /* EXTI IT *******************************************************************/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/**
  * @brief  EXTI line detection callbacks.
  */
void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
  if (GPIO_Pin == EXTI0_PIN) {
    EXTI0_Callback();
  }
}

/*************************************** END OF FILE ****************************************/
