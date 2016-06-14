/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"

#include "smartimu_bsp.h"
/*====================================================================================================*/
/*====================================================================================================*/
void SIMU_GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Clk ******************************************************************/
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* GPIO all analog input *****************************************************/
  GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
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
/*====================================================================================================*/
/*====================================================================================================*/
#define TIM3_PRES       ((uint32_t)(10000))   // 100MHz / 1000 = 10kHz
#define TIM3_PERIOD     ((uint32_t)(10000))   // 1 Hz

TIM_HandleTypeDef TIM3_HandleStruct;
pFunc TIM3_Update_irqEven;

void SIMU_TIM3_Config( pFunc pTIMx )
{
  __HAL_RCC_TIM3_CLK_ENABLE();

  TIM3_Update_irqEven = pTIMx;

  HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);

  /* TIM3 100MHz */
  TIM3_HandleStruct.Instance               = TIM3;
  TIM3_HandleStruct.Init.Prescaler         = 10000 - 1;  // 100MHz / 10000 = 10KHz
  TIM3_HandleStruct.Init.Period            = 10000 - 1;  // 1Hz
  TIM3_HandleStruct.Init.ClockDivision     = 0;
  TIM3_HandleStruct.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TIM3_HandleStruct.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&TIM3_HandleStruct);
  HAL_TIM_Base_Start_IT(&TIM3_HandleStruct);
}
/*====================================================================================================*/
/*====================================================================================================*/
