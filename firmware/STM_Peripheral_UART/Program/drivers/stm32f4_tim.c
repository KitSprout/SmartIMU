 /**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32f4_tim.c
 *  @author  KitSprout
 *  @date    22-Apr-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_tim.h"

/** @addtogroup STM32_Driver
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static TIM_HandleTypeDef htim_tim10;

TimHandle_st hTim10 = {
  .handle            = &htim_tim10,
  .tickEventCallback = NULL,
};

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void Timer10_Config( uint32_t prescaler, uint32_t period )
{
  /* TIMX Clk ******************************************************************/
  TIMER10_CLK_ENABLE();

  /* NVIC Config ***************************************************************/
  HAL_NVIC_SetPriority(TIMER10_IRQn, TIMER10_TIMx_IRQn_PREEMPT, TIMER10_TIMx_IRQn_SUB);
  HAL_NVIC_EnableIRQ(TIMER10_IRQn);

  /* TIM Base Config ************************************************************/
  hTim10.handle->Instance               = TIMER10;
  hTim10.handle->Init.Prescaler         = prescaler - 1;
  hTim10.handle->Init.Period            = period - 1;
  hTim10.handle->Init.ClockDivision     = 0;
  hTim10.handle->Init.CounterMode       = TIM_COUNTERMODE_UP;
  hTim10.handle->Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(hTim10.handle);

  /* TIM Enable *****************************************************************/
  HAL_TIM_Base_Start_IT(hTim10.handle);
}

void Timer10_Cmd( uint32_t cmd )
{
  if (cmd == ENABLE) {
    HAL_TIM_Base_Start_IT(hTim10.handle);
  }
  else {
    HAL_TIM_Base_Stop_IT(hTim10.handle);
  }
}

/*************************************** END OF FILE ****************************************/
