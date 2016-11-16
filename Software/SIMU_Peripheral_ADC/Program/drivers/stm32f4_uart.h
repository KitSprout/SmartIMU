/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_uart.h
  * @author  KitSprout
  * @date    16-Nov-2016
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STM32F4_UART_H
#define __STM32F4_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4_system.h"

/* Exported types --------------------------------------------------------------------------*/
typedef struct {
  UART_HandleTypeDef *handle;
  pFunc TxCallback;
  pFunc RxCallback;
  uint8_t *pTxBuf;
  uint8_t *pRxBuf;
} UartHandle_st;

/* Exported constants ----------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------*/  
int8_t  UART_SendData( UART_HandleTypeDef *huart, uint8_t *sendData, uint16_t lens, uint32_t timeout );
int8_t  UART_RecvData( UART_HandleTypeDef *huart, uint8_t *recvData, uint16_t lens, uint32_t timeout );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
