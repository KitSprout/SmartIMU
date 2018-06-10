/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32f4_tim_pwm.c
 *  @author  KitSprout
 *  @date    10-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_tim_pwm.h"

/** @addtogroup STM32_Driver
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static TIM_HandleTypeDef htim_pwm_r;
static TIM_HandleTypeDef htim_pwm_l;

/* Prototypes ------------------------------------------------------------------------------*/
/* MSP Functions ---------------------------------------------------------------------------*/

void HAL_TIM_PWM_MspInit( TIM_HandleTypeDef *htim )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if (htim->Instance == PWM_R) {

    /* TIM Clk ***********************************************************/
    PWM_R_CLK_ENABLE();

    /* TIM PWM Pin *******************************************************/
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

    GPIO_InitStruct.Pin       = PWM1_GPIO_PIN;
    GPIO_InitStruct.Alternate = PWM1_GPIO_AF;
    HAL_GPIO_Init(PWM1_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = PWM4_GPIO_PIN;
    GPIO_InitStruct.Alternate = PWM4_GPIO_AF;
    HAL_GPIO_Init(PWM4_GPIO_PORT, &GPIO_InitStruct);
  }
  else if (htim->Instance == PWM_L) {

    /* TIM Clk ***********************************************************/
    PWM_L_CLK_ENABLE();

    /* TIM PWM Pin *******************************************************/
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

    GPIO_InitStruct.Pin       = PWM2_GPIO_PIN;
    GPIO_InitStruct.Alternate = PWM2_GPIO_AF;
    HAL_GPIO_Init(PWM2_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = PWM3_GPIO_PIN;
    GPIO_InitStruct.Alternate = PWM3_GPIO_AF;
    HAL_GPIO_Init(PWM3_GPIO_PORT, &GPIO_InitStruct);
  }
}

/* Functions -------------------------------------------------------------------------------*/

void TIM_PWM_Config( void )
{
  TIM_OC_InitTypeDef TIM_OC_InitStruct;

  /* TIM Base Config ************************************************************/
  htim_pwm_r.Instance               = PWM_R;
  htim_pwm_r.Init.Prescaler         = PWM_PRESCALER - 1;
  htim_pwm_r.Init.Period            = PWM_PERIOD - 1;
  htim_pwm_r.Init.ClockDivision     = 0;
  htim_pwm_r.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim_pwm_r.Init.RepetitionCounter = 0;
  HAL_TIM_PWM_Init(&htim_pwm_r);

  htim_pwm_l.Instance               = PWM_L;
  htim_pwm_l.Init.Prescaler         = PWM_PRESCALER - 1;
  htim_pwm_l.Init.Period            = PWM_PERIOD - 1;
  htim_pwm_l.Init.ClockDivision     = 0;
  htim_pwm_l.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim_pwm_l.Init.RepetitionCounter = 0;
  HAL_TIM_PWM_Init(&htim_pwm_l);

  /* PWM Config *****************************************************************/
  TIM_OC_InitStruct.OCMode       = TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCFastMode   = TIM_OCFAST_DISABLE;
  TIM_OC_InitStruct.OCPolarity   = TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  TIM_OC_InitStruct.OCIdleState  = TIM_OCIDLESTATE_RESET;
  TIM_OC_InitStruct.OCNIdleState = TIM_OCNIDLESTATE_RESET;

  TIM_OC_InitStruct.Pulse        = PWM_MIN;
  HAL_TIM_PWM_ConfigChannel(&htim_pwm_r, &TIM_OC_InitStruct, PWM1_CHANNEL);
  HAL_TIM_PWM_ConfigChannel(&htim_pwm_l, &TIM_OC_InitStruct, PWM2_CHANNEL);
  HAL_TIM_PWM_ConfigChannel(&htim_pwm_l, &TIM_OC_InitStruct, PWM3_CHANNEL);
  HAL_TIM_PWM_ConfigChannel(&htim_pwm_r, &TIM_OC_InitStruct, PWM4_CHANNEL);

  /* PWM Start ******************************************************************/
  HAL_TIM_PWM_Start(&htim_pwm_r, PWM1_CHANNEL);
  HAL_TIM_PWM_Start(&htim_pwm_l, PWM2_CHANNEL);
  HAL_TIM_PWM_Start(&htim_pwm_l, PWM3_CHANNEL);
  HAL_TIM_PWM_Start(&htim_pwm_r, PWM4_CHANNEL);
}

void TIM_PWM_SetDuty( uint8_t channel, uint16_t duty )
{
  switch(channel) {
    case 1:
      PWM1_DUTY = (duty > PWM_MAX) ? (PWM_MIN) : (duty);
      break;
    case 2:
      PWM2_DUTY = (duty > PWM_MAX) ? (PWM_MIN) : (duty);
      break;
    case 3:
      PWM3_DUTY = (duty > PWM_MAX) ? (PWM_MIN) : (duty);
      break;
    case 4:
      PWM4_DUTY = (duty > PWM_MAX) ? (PWM_MIN) : (duty);
      break;
  }
  
}

/*************************************** END OF FILE ****************************************/
