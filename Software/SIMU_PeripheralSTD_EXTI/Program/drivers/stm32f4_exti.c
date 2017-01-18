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
  * @date    18-Jan-2016
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
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void EXTI0_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Pin ******************************************************************/
  GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Pin   = EXTI0_PIN;
  HAL_GPIO_Init(EXTI0_GPIO_PORT, &GPIO_InitStruct);

  /* EXTI IT *******************************************************************/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/*************************************** END OF FILE ****************************************/
