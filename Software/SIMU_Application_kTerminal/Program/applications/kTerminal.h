/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    kTerminal.h
  * @author  KitSprout
  * @date    24-Jan-2017
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __KTERMINAL_H
#define __KTERMINAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"

/* Exported types --------------------------------------------------------------------------*/
typedef enum {
  KST_OK = 0,
  KST_REST,
  KST_ORGL,
  KST_INFO,
  KST_DEVID,
  KST_ODR,
  KST_BAUD,
  KST_MODE,
  KST_CALIB,
  KST_EOUT,
  KST_DOUT,
  KST_EXIT,
  KST_LENS,
  KST_ERROR = 255
} KTerminal_t;

/* Exported constants ----------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------*/  
void KTerminal_Init( USART_TypeDef *USARTx );
void KTerminal_IRQ( void );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
