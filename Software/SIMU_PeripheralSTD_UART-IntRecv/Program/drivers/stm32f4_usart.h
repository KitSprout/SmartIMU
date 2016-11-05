/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_usart.h
  * @author  KitSprout
  * @date    6-Oct-2016
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STM32F4_USART_H
#define __STM32F4_USART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types --------------------------------------------------------------------------*/
/* Exported constants ----------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------*/  
void   UART_SendByte( USART_TypeDef *USARTx, uint8_t *sendByte );
void   UART_RecvByte( USART_TypeDef *USARTx, uint8_t *recvByte );
int8_t UART_RecvByteWTO( USART_TypeDef *USARTx, uint8_t *recvByte, uint32_t timeoutMs );
void   UART_SendData( USART_TypeDef *USARTx, uint8_t *sendData, uint16_t lens );
void   UART_RecvData( USART_TypeDef *USARTx, uint8_t *recvData, uint16_t lens );
int8_t UART_RecvDataWTO( USART_TypeDef *USARTx, uint8_t *recvData, uint16_t lens, uint32_t timeoutMs );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
