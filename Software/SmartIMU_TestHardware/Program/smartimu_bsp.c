/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"

#include "smartimu.h"
#include "smartimu_bsp.h"
/*====================================================================================================*/
/*====================================================================================================*/
TIM_HandleTypeDef SIMU_TIM_HandleStruct;
/*====================================================================================================*/
/*====================================================================================================*/
void SIMU_GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Clk ******************************************************************/
  LED_R_GPIO_CLK_ENABLE();
  LED_G_GPIO_CLK_ENABLE();
  LED_B_GPIO_CLK_ENABLE();

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

  LED_R_Set();
  LED_G_Set();
  LED_B_Set();
}
/*====================================================================================================*/
/*====================================================================================================*/
void SIMU_TIM_Config( void )
{
  /* TIM Clk *******************************************************************/
  TIMx_CLK_ENABLE();

  HAL_NVIC_SetPriority(TIMx_IRQ, 3, 0);
  HAL_NVIC_EnableIRQ(TIMx_IRQ);

  /* TIM TimeBase **************************************************************/
  SIMU_TIM_HandleStruct.Instance               = TIMx;
  SIMU_TIM_HandleStruct.Init.Prescaler         = TIMx_PRES - 1;
  SIMU_TIM_HandleStruct.Init.Period            = TIMx_PERIOD - 1;
  SIMU_TIM_HandleStruct.Init.ClockDivision     = 0;
  SIMU_TIM_HandleStruct.Init.CounterMode       = TIM_COUNTERMODE_UP;
  SIMU_TIM_HandleStruct.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&SIMU_TIM_HandleStruct);
  HAL_TIM_Base_Start_IT(&SIMU_TIM_HandleStruct);
}
/*====================================================================================================*/
/*====================================================================================================*/
