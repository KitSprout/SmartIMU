/* #include "stm32f4_usart.h" */

#ifndef __STM32F4_USART_H
#define __STM32F4_USART_H

#include "stm32f4xx_hal.h"
/*====================================================================================================*/
/*====================================================================================================*/
void   UART_SendByte( USART_TypeDef *USARTx, uint8_t *SendData );
void   UART_RecvByte( USART_TypeDef *USARTx, uint8_t *RecvData );
int8_t UART_RecvByteWTO( USART_TypeDef *USARTx, uint8_t *RecvData, uint32_t TimeoutMs );
void   UART_SendData( USART_TypeDef *USARTx, uint8_t *SendData, uint16_t DataLen );
void   UART_RecvData( USART_TypeDef *USARTx, uint8_t *RecvData, uint16_t DataLen );
int8_t UART_RecvDataWTO( USART_TypeDef *USARTx, uint8_t *RecvData, uint16_t DataLen, uint32_t TimeoutMs );
/*====================================================================================================*/
/*====================================================================================================*/
#endif	 
