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
static TIM_HandleTypeDef htim_timer2;

TimHandle_st hTimer2 = {
  .handle            = &htim_timer2,
  .tickEventCallback = NULL,
};

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void Timer2_Config( uint32_t prescaler, uint32_t period )
{
  /* TIMX Clk ******************************************************************/
  TIMER2_CLK_ENABLE();

  /* NVIC Config ***************************************************************/
  HAL_NVIC_SetPriority(TIMER2_IRQn, TIMER2_TIMx_IRQn_PREEMPT, TIMER2_TIMx_IRQn_SUB);
  HAL_NVIC_EnableIRQ(TIMER2_IRQn);

  /* TIM Base Config ************************************************************/
  hTimer2.handle->Instance               = TIMER2;
  hTimer2.handle->Init.Prescaler         = prescaler - 1;
  hTimer2.handle->Init.Period            = period - 1;
  hTimer2.handle->Init.ClockDivision     = 0;
  hTimer2.handle->Init.CounterMode       = TIM_COUNTERMODE_UP;
  hTimer2.handle->Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(hTimer2.handle);

  /* TIM Enable *****************************************************************/
  HAL_TIM_Base_Start_IT(hTimer2.handle);
}

void Timer2_Cmd( uint8_t cmd )
{
  if (cmd == ENABLE) {
    HAL_TIM_Base_Start_IT(hTimer2.handle);
  }
  else {
    HAL_TIM_Base_Stop_IT(hTimer2.handle);
  }
}

/*************************************** END OF FILE ****************************************/
