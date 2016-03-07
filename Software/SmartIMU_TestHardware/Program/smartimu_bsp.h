/* #include "smartimu_bsp.h" */

#ifndef __SMARTIMU_BSP_H
#define __SMARTIMU_BSP_H

#include "stm32f4xx_hal.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define LED_R_PIN                 GPIO_PIN_14
#define LED_R_GPIO_PORT           GPIOB
#define LED_R_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED_R_Set()               __GPIO_SET(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Reset()             __GPIO_RST(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Toggle()            __GPIO_TOG(LED_R_GPIO_PORT, LED_R_PIN)

#define LED_G_PIN                 GPIO_PIN_13
#define LED_G_GPIO_PORT           GPIOB
#define LED_G_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED_G_Set()               __GPIO_SET(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Reset()             __GPIO_RST(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Toggle()            __GPIO_TOG(LED_G_GPIO_PORT, LED_G_PIN)

#define LED_B_PIN                 GPIO_PIN_15
#define LED_B_GPIO_PORT           GPIOB
#define LED_B_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED_B_Set()               __GPIO_SET(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Reset()             __GPIO_RST(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Toggle()            __GPIO_TOG(LED_B_GPIO_PORT, LED_B_PIN)

#define TIMx                      TIM3      // 100MHz
#define TIMx_IRQ                  TIM3_IRQn // 
#define TIMx_PRES                 100       // 100MHz / 100 = 1MHz
#define TIMx_PERIOD               (uint32_t)((SystemCoreClock / TIMx_PRES) * SampleRate)  // SampleRateFreg
#define TIMx_CLK_ENABLE()         __HAL_RCC_TIM3_CLK_ENABLE()
/*====================================================================================================*/
/*====================================================================================================*/
void SIMU_GPIO_Config( void );
void SIMU_TIM_Config( void );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
