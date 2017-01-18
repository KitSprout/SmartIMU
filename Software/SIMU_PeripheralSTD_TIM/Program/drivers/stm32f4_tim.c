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
  * @date    18-Jan-2016
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
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void Timer2_Config( uint32_t prescaler, uint32_t period )
{
  NVIC_InitTypeDef NVIC_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;

  /* TIMX Clk ******************************************************************/
  TIMER2_CLK_ENABLE();

  /* NVIC Config ***************************************************************/
  NVIC_InitStruct.NVIC_IRQChannel                   = TIMER2_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = TIMER2_IRQn_PREEMPT;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority        = TIMER2_IRQn_SUB;
  NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

  /* TIM Base Config ************************************************************/
  TIM_TimeBaseStruct.TIM_Prescaler     = prescaler - 1;
  TIM_TimeBaseStruct.TIM_Period        = period - 1;
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStruct.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIMER2, &TIM_TimeBaseStruct);

  /* TIM Enable *****************************************************************/
  TIM_ClearFlag(TIMER2, TIM_FLAG_Update);
}

void Timer3_Config( uint32_t prescaler, uint32_t period )
{
  NVIC_InitTypeDef NVIC_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;

  /* TIMX Clk ******************************************************************/
  TIMER3_CLK_ENABLE();

  /* NVIC Config ***************************************************************/
  NVIC_InitStruct.NVIC_IRQChannel                   = TIMER3_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = TIMER3_IRQn_PREEMPT;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority        = TIMER3_IRQn_SUB;
  NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

  /* TIM Base Config ************************************************************/
  TIM_TimeBaseStruct.TIM_Prescaler     = prescaler - 1;
  TIM_TimeBaseStruct.TIM_Period        = period - 1;
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStruct.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIMER3, &TIM_TimeBaseStruct);

  /* TIM Enable *****************************************************************/
  TIM_ClearFlag(TIMER3, TIM_FLAG_Update);
}

void Timer4_Config( uint32_t prescaler, uint32_t period )
{
  NVIC_InitTypeDef NVIC_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;

  /* TIMX Clk ******************************************************************/
  TIMER4_CLK_ENABLE();

  /* NVIC Config ***************************************************************/
  NVIC_InitStruct.NVIC_IRQChannel                   = TIMER4_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = TIMER4_IRQn_PREEMPT;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority        = TIMER4_IRQn_SUB;
  NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

  /* TIM Base Config ************************************************************/
  TIM_TimeBaseStruct.TIM_Prescaler     = prescaler - 1;
  TIM_TimeBaseStruct.TIM_Period        = period - 1;
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStruct.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIMER4, &TIM_TimeBaseStruct);

  /* TIM Enable *****************************************************************/
  TIM_ClearFlag(TIMER4, TIM_FLAG_Update);
}

void Timer_Cmd( TIM_TypeDef* TIMx, FunctionalState Cmd )
{
  TIM_ITConfig(TIMx, TIM_IT_Update, Cmd);
  TIM_Cmd(TIMx, Cmd);
}

/*************************************** END OF FILE ****************************************/
