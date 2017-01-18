/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_tim.h
  * @author  KitSprout
  * @date    18-Jan-2016
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STM32F4_TIM_H
#define __STM32F4_TIM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4_system.h"

/* Exported types --------------------------------------------------------------------------*/
/* Exported constants ----------------------------------------------------------------------*/
#define TIMx_CLOCK( __PRES, __PERIOD )  (SystemCoreClock / (__PRES * __PERIOD))

/* Exported functions ----------------------------------------------------------------------*/
void Timer2_Config( uint32_t prescaler, uint32_t period );
void Timer3_Config( uint32_t prescaler, uint32_t period );
void Timer4_Config( uint32_t prescaler, uint32_t period );

void Timer_Cmd( TIM_TypeDef* TIMx, FunctionalState Cmd );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
