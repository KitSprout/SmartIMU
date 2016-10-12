/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_usart.c
  * @author  KitSprout
  * @date    6-Oct-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"

/** @addtogroup STM32_Driver
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  UART_SendByte
  * @param  USARTx: 
  * @param  sendByte: 
  * @retval None
  */
void UART_SendByte( USART_TypeDef *USARTx, uint8_t *sendByte )
{
  USARTx->DR = (*sendByte & (uint16_t)0x01FF);
  while (!(USARTx->SR & USART_FLAG_TXE));
}

/**
  * @brief  UART_RecvByte
  * @param  USARTx: 
  * @param  recvByte: 
  * @retval None
  */
void UART_RecvByte( USART_TypeDef *USARTx, uint8_t *recvByte )
{
  while(!(USARTx->SR & USART_FLAG_RXNE));
  *recvByte = (uint16_t)(USARTx->DR & (uint16_t)0x01FF);
}

/**
  * @brief  UART_RecvByteWTO
  * @param  USARTx: 
  * @param  recvByte: 
  * @param  timeoutMs: 
  * @retval state of receive
  */
int8_t UART_RecvByteWTO( USART_TypeDef *USARTx, uint8_t *recvByte, uint32_t timeoutMs )
{
  while ((USARTx->SR & USART_FLAG_RXNE) == RESET) {
    if (timeoutMs-- > 0)
      delay_ms(1);
    else
      return ERROR; // timeout
  }
  *recvByte = (uint16_t)(USARTx->DR & (uint16_t)0x01FF);

  return SUCCESS;
}

/**
  * @brief  UART_SendData
  * @param  USARTx: 
  * @param  sendData: 
  * @param  lens: 
  * @retval None
  */
void UART_SendData( USART_TypeDef *USARTx, uint8_t *sendData, uint16_t lens )
{
  do {
    UART_SendByte(USARTx, sendData++);
  } while (--lens);
}

/**
  * @brief  UART_RecvData
  * @param  USARTx: 
  * @param  recvData: 
  * @param  lens: 
  * @retval None
  */
void UART_RecvData( USART_TypeDef *USARTx, uint8_t *recvData, uint16_t lens )
{
  do {
    UART_RecvByte(USARTx, recvData++);
  } while (--lens);
}

/**
  * @brief  UART_RecvDataWTO
  * @param  USARTx: 
  * @param  recvData: 
  * @param  lens: 
  * @param  timeoutMs: 
  * @retval state of receive
  */
int8_t UART_RecvDataWTO( USART_TypeDef *USARTx, uint8_t *recvData, uint16_t lens, uint32_t timeoutMs )
{
  int8_t state = ERROR;

  do {
    state = UART_RecvByteWTO(USARTx, recvData++, timeoutMs);
  } while ((--lens) && (state != ERROR));

  return state;
}

/*************************************** END OF FILE ****************************************/
