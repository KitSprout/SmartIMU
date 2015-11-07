/* #include "experiment_stm32f4.h" */

#ifndef __EXPERIMENT_STM32F4_H
#define __EXPERIMENT_STM32F4_H

#include "stm32f4xx_hal.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define LED_R_PIN                 GPIO_PIN_15
#define LED_R_GPIO_PORT           GPIOC
#define LED_R_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED_R_Set                 __GPIO_SET(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Reset               __GPIO_RST(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Toggle              __GPIO_TOG(LED_R_GPIO_PORT, LED_R_PIN)

#define LED_G_PIN                 GPIO_PIN_14
#define LED_G_GPIO_PORT           GPIOC
#define LED_G_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED_G_Set                 __GPIO_SET(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Reset               __GPIO_RST(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Toggle              __GPIO_TOG(LED_G_GPIO_PORT, LED_G_PIN)

#define LED_B_PIN                 GPIO_PIN_13
#define LED_B_GPIO_PORT           GPIOC
#define LED_B_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED_B_Set                 __GPIO_SET(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Reset               __GPIO_RST(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Toggle              __GPIO_TOG(LED_B_GPIO_PORT, LED_B_PIN)

#define KEY_WU_PIN                GPIO_PIN_0
#define KEY_WU_GPIO_PORT          GPIOA
#define KEY_WU_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY_WU_Read               (__GPIO_READ(KEY_WU_GPIO_PORT, KEY_WU_PIN) == KEY_WU_PIN)

#define KEY_BO_PIN                GPIO_PIN_2
#define KEY_BO_GPIO_PORT          GPIOB
#define KEY_BO_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define KEY_BO_Read               (__GPIO_READ(KEY_BO_GPIO_PORT, KEY_BO_PIN) == KEY_BO_PIN)

#define GPIO_A_PINs                GPIO_PIN_8 | GPIO_PIN_15
#define GPIO_A_GPIO_PORT           GPIOA
#define GPIO_A_Set                 HAL_GPIO_WritePin(GPIO_A_GPIO_PORT, GPIO_A_PINs, GPIO_PIN_SET)
#define GPIO_A_Reset               HAL_GPIO_WritePin(GPIO_A_GPIO_PORT, GPIO_A_PINs, GPIO_PIN_RESET)
#define GPIO_A_Toggle              HAL_GPIO_TogglePin(GPIO_A_GPIO_PORT, GPIO_A_PINs)

#define GPIO_B_PINs                GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_3  | GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_6  | GPIO_PIN_7  | GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15
#define GPIO_B_GPIO_PORT           GPIOB
#define GPIO_B_Set                 HAL_GPIO_WritePin(GPIO_B_GPIO_PORT, GPIO_B_PINs, GPIO_PIN_SET)
#define GPIO_B_Reset               HAL_GPIO_WritePin(GPIO_B_GPIO_PORT, GPIO_B_PINs, GPIO_PIN_RESET)
#define GPIO_B_Toggle              HAL_GPIO_TogglePin(GPIO_B_GPIO_PORT, GPIO_B_PINs)

#define GPIO_C_PINs                GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3  | GPIO_PIN_5  | GPIO_PIN_6  | GPIO_PIN_7
#define GPIO_C_GPIO_PORT           GPIOC
#define GPIO_C_Set                 HAL_GPIO_WritePin(GPIO_C_GPIO_PORT, GPIO_C_PINs, GPIO_PIN_SET)
#define GPIO_C_Reset               HAL_GPIO_WritePin(GPIO_C_GPIO_PORT, GPIO_C_PINs, GPIO_PIN_RESET)
#define GPIO_C_Toggle              HAL_GPIO_TogglePin(GPIO_C_GPIO_PORT, GPIO_C_PINs)
/*====================================================================================================*/
/*====================================================================================================*/
void GPIO_Config( void );
void GPIO_EX_Config( void );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
