/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    boardConfig.h
 *  @author  KitSprout
 *  @date    03-Jun-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __BOARDCONFIG_H
#define __BOARDCONFIG_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
/* Define ----------------------------------------------------------------------------------*/

#define KS_HW_BOARD_NAME                "SmartIMU"
#define KS_HW_MCU_NAME                  "STM32F411CE"

#define KS_HW_USE_CLOCK_SOUCE_INT       (0U)
#define KS_HW_USE_CLOCK_SOUCE_EXT       (1U)
#define KS_HW_HCLOCK_SOUCE              KS_HW_USE_CLOCK_SOUCE_EXT
#define KS_HW_LCLOCK_SOUCE              KS_HW_USE_CLOCK_SOUCE_INT

#define KS_FW_UART_HAL_LIBRARY          (0U)
#define KS_FW_SPI_HAL_LIBRARY           (0U)
#define KS_FW_I2C_HAL_LIBRARY           (0U)
#define KS_FW_USB_ENABLE                (1U)

#define KS_SYSCLK                       SystemCoreClock


/* -------- LED and KEY */

#define LED_R_PIN                       GPIO_PIN_13
#define LED_R_GPIO_PORT                 GPIOC
#define LED_R_Set()                     __GPIO_SET(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Reset()                   __GPIO_RST(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Toggle()                  __GPIO_TOG(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_On()                      LED_R_Reset()
#define LED_R_Off()                     LED_R_Set()

#define LED_G_PIN                       GPIO_PIN_14
#define LED_G_GPIO_PORT                 GPIOC
#define LED_G_Set()                     __GPIO_SET(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Reset()                   __GPIO_RST(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Toggle()                  __GPIO_TOG(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_On()                      LED_G_Reset()
#define LED_G_Off()                     LED_G_Set()

#define LED_B_PIN                       GPIO_PIN_15
#define LED_B_GPIO_PORT                 GPIOC
#define LED_B_Set()                     __GPIO_SET(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Reset()                   __GPIO_RST(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Toggle()                  __GPIO_TOG(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_On()                      LED_B_Reset()
#define LED_B_Off()                     LED_B_Set()


/* -------- TIM Timer */

#define PWM_PRESCALER                   (uint32_t)(SystemCoreClock / 1000000)
#define PWM_PERIOD                      (uint32_t)(1000)
#define PWM_MAX                         PWM_PERIOD
#define PWM_MIN                         0

#define PWM_R                           TIM3
#define PWM_R_CLK_ENABLE()              __HAL_RCC_TIM3_CLK_ENABLE()
#define PWM_R_PERI                      PWM_R->PSC
#define PWM_R_PULSE                     PWM_R->ARR

#define PWM1_GPIO_PIN                   GPIO_PIN_1
#define PWM1_GPIO_PORT                  GPIOB
#define PWM1_GPIO_AF                    GPIO_AF2_TIM3
#define PWM1_CHANNEL                    TIM_CHANNEL_4
#define PWM1_DUTY                       PWM_R->CCR4

#define PWM4_GPIO_PIN                   GPIO_PIN_0
#define PWM4_GPIO_PORT                  GPIOB
#define PWM4_GPIO_AF                    GPIO_AF2_TIM3
#define PWM4_CHANNEL                    TIM_CHANNEL_3
#define PWM4_DUTY                       PWM_R->CCR3

#define PWM_L                           TIM4
#define PWM_L_CLK_ENABLE()              __HAL_RCC_TIM4_CLK_ENABLE()
#define PWM_L_PERI                      PWM_L->PSC
#define PWM_L_PULSE                     PWM_L->ARR

#define PWM2_GPIO_PIN                   GPIO_PIN_9
#define PWM2_GPIO_PORT                  GPIOB
#define PWM2_GPIO_AF                    GPIO_AF2_TIM3
#define PWM2_CHANNEL                    TIM_CHANNEL_4
#define PWM2_DUTY                       PWM_L->CCR4

#define PWM3_GPIO_PIN                   GPIO_PIN_8
#define PWM3_GPIO_PORT                  GPIOB
#define PWM3_GPIO_AF                    GPIO_AF2_TIM3
#define PWM3_CHANNEL                    TIM_CHANNEL_3
#define PWM3_DUTY                       PWM_L->CCR3


/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
