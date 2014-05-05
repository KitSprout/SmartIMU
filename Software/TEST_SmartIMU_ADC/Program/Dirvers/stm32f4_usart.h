/* #include "stm32f4_usart.h" */

#ifndef __STM32F4_USART_H
#define __STM32F4_USART_H

#include "stm32f4xx.h"
/*====================================================================================================*/
/*====================================================================================================*/
void UART_SendByte( USART_TypeDef *USARTx, u8 SendData );
u8   UART_RecvByte( USART_TypeDef *USARTx );
void UART_SendData( USART_TypeDef *USARTx, u8 *SendData, u16 DataLen );
void UART_RecvData( USART_TypeDef *USARTx, u8 *RecvData, u16 DataLen );
/*====================================================================================================*/
/*====================================================================================================*/
#endif	 
