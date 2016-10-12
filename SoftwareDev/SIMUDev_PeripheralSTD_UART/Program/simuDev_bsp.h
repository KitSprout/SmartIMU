/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    simuDev_bsp.h
  * @author  KitSprout
  * @date    11-Oct-2016
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __SIMUDEV_BSP_H
#define __SIMUDEV_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"

/* Exported types --------------------------------------------------------------------------*/
/* Exported constants ----------------------------------------------------------------------*/
#define LED_R_PIN         GPIO_Pin_15
#define LED_R_GPIO_PORT   GPIOC
#define LED_R_Set()       __GPIO_SET(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Reset()     __GPIO_RST(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Toggle()    __GPIO_TOG(LED_R_GPIO_PORT, LED_R_PIN)

#define LED_G_PIN         GPIO_Pin_14
#define LED_G_GPIO_PORT   GPIOC
#define LED_G_Set()       __GPIO_SET(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Reset()     __GPIO_RST(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Toggle()    __GPIO_TOG(LED_G_GPIO_PORT, LED_G_PIN)

#define LED_B_PIN         GPIO_Pin_13
#define LED_B_GPIO_PORT   GPIOC
#define LED_B_Set()       __GPIO_SET(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Reset()     __GPIO_RST(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Toggle()    __GPIO_TOG(LED_B_GPIO_PORT, LED_B_PIN)

#define KEY_PIN           GPIO_Pin_2
#define KEY_GPIO_PORT     GPIOB
#define KEY_Read()        (__GPIO_READ(KEY_GPIO_PORT, KEY_PIN) == KEY_PIN)

/* Exported functions ----------------------------------------------------------------------*/  
void SIMU_GPIO_Config( void );
void SIMU_UART_Config( pFunc pUARTx );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
