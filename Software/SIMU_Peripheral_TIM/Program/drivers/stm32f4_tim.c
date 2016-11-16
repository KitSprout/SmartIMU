/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_tim.c
  * @author  KitSprout
  * @date    16-Nov-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4_system.h"
#include "stm32f4_tim.h"

/** @addtogroup STM32_Driver
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
TIM_HandleTypeDef Timer2Handle;
TIM_HandleTypeDef Timer3Handle;
TIM_HandleTypeDef Timer4Handle;

TimHandle_st hTimer2 = {
  .handle       = &Timer2Handle,
  .EvenCallback = NULL,
};

TimHandle_st hTimer3 = {
  .handle       = &Timer3Handle,
  .EvenCallback = NULL,
};

TimHandle_st hTimer4 = {
  .handle       = &Timer4Handle,
  .EvenCallback = NULL,
};

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void Timer2_Config( uint32_t prescaler, uint32_t period )
{
  /* TIMX Clk ******************************************************************/
  TIMER2_CLK_ENABLE();

  /* NVIC Config ***************************************************************/
  HAL_NVIC_SetPriority(TIMER2_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(TIMER2_IRQn);

  /* TIM Base Config ************************************************************/
  Timer2Handle.Instance               = TIMER2;
  Timer2Handle.Init.Prescaler         = prescaler - 1;
  Timer2Handle.Init.Period            = period - 1;
  Timer2Handle.Init.ClockDivision     = 0;
  Timer2Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  Timer2Handle.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&Timer2Handle);

  /* TIM Enable *****************************************************************/
  HAL_TIM_Base_Start_IT(&Timer2Handle);
}

void Timer3_Config( uint32_t prescaler, uint32_t period )
{
  /* TIMX Clk ******************************************************************/
  TIMER3_CLK_ENABLE();

  /* NVIC Config ***************************************************************/
  HAL_NVIC_SetPriority(TIMER3_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(TIMER3_IRQn);

  /* TIM Base Config ************************************************************/
  Timer3Handle.Instance               = TIMER3;
  Timer3Handle.Init.Prescaler         = prescaler - 1;
  Timer3Handle.Init.Period            = period - 1;
  Timer3Handle.Init.ClockDivision     = 0;
  Timer3Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  Timer3Handle.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&Timer3Handle);

  /* TIM Enable *****************************************************************/
  HAL_TIM_Base_Start_IT(&Timer3Handle);
}

void Timer4_Config( uint32_t prescaler, uint32_t period )
{
  /* TIMX Clk ******************************************************************/
  TIMER4_CLK_ENABLE();

  /* NVIC Config ***************************************************************/
  HAL_NVIC_SetPriority(TIMER4_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(TIMER4_IRQn);

  /* TIM Base Config ************************************************************/
  Timer4Handle.Instance               = TIMER4;
  Timer4Handle.Init.Prescaler         = prescaler - 1;
  Timer4Handle.Init.Period            = period - 1;
  Timer4Handle.Init.ClockDivision     = 0;
  Timer4Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  Timer4Handle.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&Timer4Handle);

  /* TIM Enable *****************************************************************/
  HAL_TIM_Base_Start_IT(&Timer4Handle);
}

void Timer2_Cmd( uint8_t cmd )
{
  if (cmd == ENABLE) {
    HAL_TIM_Base_Start_IT(&Timer2Handle);
  }
  else {
    HAL_TIM_Base_Stop_IT(&Timer2Handle);
  }
}

void Timer3_Cmd( uint8_t cmd )
{
  if (cmd == ENABLE) {
    HAL_TIM_Base_Start_IT(&Timer3Handle);
  }
  else {
    HAL_TIM_Base_Stop_IT(&Timer3Handle);
  }
}
void Timer4_Cmd( uint8_t cmd )
{
  if (cmd == ENABLE) {
    HAL_TIM_Base_Start_IT(&Timer4Handle);
  }
  else {
    HAL_TIM_Base_Stop_IT(&Timer4Handle);
  }
}

/*************************************** END OF FILE ****************************************/
