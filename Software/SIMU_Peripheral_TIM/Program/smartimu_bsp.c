/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    smartimu_bsp.c
  * @author  KitSprout
  * @date    5-Nov-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "smartimu_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
TIM_HandleTypeDef Timer2Handle;
TIM_HandleTypeDef Timer3Handle;
pFunc IRQEven_TIM2 = NULL;
pFunc IRQEven_TIM3 = NULL;

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void SIMU_GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable all GPIO Clk *******************************************************/
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* GPIO all analog input *****************************************************/
  GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  GPIO_InitStruct.Pin   = GPIO_PIN_All & (~(GPIO_PIN_13 | GPIO_PIN_14));
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* GPIO Pin ******************************************************************/
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin   = LED_R_PIN;
  HAL_GPIO_Init(LED_R_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin   = LED_G_PIN;
  HAL_GPIO_Init(LED_G_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin   = LED_B_PIN;
  HAL_GPIO_Init(LED_B_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  GPIO_InitStruct.Pin   = KEY_PIN;
  HAL_GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStruct);

  LED_R_Set();
  LED_G_Set();
  LED_B_Set();
}

void SIMU_TIM2_Config( pFunc pTIMx, uint32_t prescaler, uint32_t period )
{
  IRQEven_TIM2 = pTIMx;

  /* TIMX Clk ******************************************************************/
  __HAL_RCC_TIM2_CLK_ENABLE();

  /* NVIC Config ***************************************************************/
  HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);

  /* TIM Base Config ************************************************************/
  Timer2Handle.Instance               = TIM2;
  Timer2Handle.Init.Prescaler         = prescaler - 1;
  Timer2Handle.Init.Period            = period - 1;
  Timer2Handle.Init.ClockDivision     = 0;
  Timer2Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  Timer2Handle.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&Timer2Handle);

  /* TIM Enable *****************************************************************/
  HAL_TIM_Base_Start_IT(&Timer2Handle);
}

void SIMU_TIM3_Config( pFunc pTIMx, uint32_t prescaler, uint32_t period )
{
  IRQEven_TIM3 = pTIMx;

  /* TIMX Clk ******************************************************************/
  __HAL_RCC_TIM3_CLK_ENABLE();

  /* NVIC Config ***************************************************************/
  HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);

  /* TIM Base Config ************************************************************/
  Timer3Handle.Instance               = TIM3;
  Timer3Handle.Init.Prescaler         = prescaler - 1;
  Timer3Handle.Init.Period            = period - 1;
  Timer3Handle.Init.ClockDivision     = 0;
  Timer3Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  Timer3Handle.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&Timer3Handle);

  /* TIM Enable *****************************************************************/
  HAL_TIM_Base_Start_IT(&Timer3Handle);
}

/*************************************** END OF FILE ****************************************/
