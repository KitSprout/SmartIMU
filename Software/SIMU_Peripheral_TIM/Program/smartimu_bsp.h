/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    smartimu_bsp.h
  * @author  KitSprout
  * @date    4-Nov-2016
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __SMARTIMU_BSP_H
#define __SMARTIMU_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"

/* Exported types --------------------------------------------------------------------------*/
/* Exported constants ----------------------------------------------------------------------*/
#define LED_R_PIN         GPIO_PIN_15
#define LED_R_GPIO_PORT   GPIOC
#define LED_R_Set()       __GPIO_SET(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Reset()     __GPIO_RST(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Toggle()    __GPIO_TOG(LED_R_GPIO_PORT, LED_R_PIN)

#define LED_G_PIN         GPIO_PIN_14
#define LED_G_GPIO_PORT   GPIOC
#define LED_G_Set()       __GPIO_SET(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Reset()     __GPIO_RST(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Toggle()    __GPIO_TOG(LED_G_GPIO_PORT, LED_G_PIN)

#define LED_B_PIN         GPIO_PIN_13
#define LED_B_GPIO_PORT   GPIOC
#define LED_B_Set()       __GPIO_SET(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Reset()     __GPIO_RST(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Toggle()    __GPIO_TOG(LED_B_GPIO_PORT, LED_B_PIN)

#define KEY_PIN           GPIO_PIN_0
#define KEY_GPIO_PORT     GPIOB
#define KEY_Read()        (__GPIO_READ(KEY_GPIO_PORT, KEY_PIN) == KEY_PIN)

/* Exported functions ----------------------------------------------------------------------*/
void SIMU_GPIO_Config( void );
void SIMU_TIM2_Config( pFunc pTIMx, uint32_t prescaler, uint32_t period );
void SIMU_TIM3_Config( pFunc pTIMx, uint32_t prescaler, uint32_t period );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
