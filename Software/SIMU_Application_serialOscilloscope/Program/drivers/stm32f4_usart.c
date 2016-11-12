/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_UART.c
  * @author  KitSprout
  * @date    5-Nov-2016
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
  * @param  UARTx: 
  * @param  sendByte: 
  * @retval None
  */
void UART_SendByte( USART_TypeDef *UARTx, uint8_t *sendByte )
{
  UARTx->DR = (*sendByte & (uint16_t)0x01FF);
  while (!(UARTx->SR & UART_FLAG_TC));
}

/**
  * @brief  UART_RecvByte
  * @param  UARTx: 
  * @param  recvByte: 
  * @retval None
  */
void UART_RecvByte( USART_TypeDef *UARTx, uint8_t *recvByte )
{
  while(!(UARTx->SR & UART_FLAG_RXNE));
  *recvByte = (uint16_t)(UARTx->DR & (uint16_t)0x01FF);
}

/**
  * @brief  UART_RecvByteWTO
  * @param  UARTx: 
  * @param  recvByte: 
  * @param  timeoutMs: 
  * @retval state of receive
  */
int8_t UART_RecvByteWTO( USART_TypeDef *UARTx, uint8_t *recvByte, uint32_t timeoutMs )
{
  while ((UARTx->SR & UART_FLAG_RXNE) == RESET) {
    if (timeoutMs-- > 0)
      delay_ms(1);
    else
      return ERROR; // timeout
  }
  *recvByte = (uint16_t)(UARTx->DR & (uint16_t)0x01FF);

  return SUCCESS;
}

/**
  * @brief  UART_SendData
  * @param  UARTx: 
  * @param  sendData: 
  * @param  lens: 
  * @retval None
  */
void UART_SendData( USART_TypeDef *UARTx, uint8_t *sendData, uint16_t lens )
{
  do {
    UART_SendByte(UARTx, sendData++);
  } while (--lens);
}

/**
  * @brief  UART_RecvData
  * @param  UARTx: 
  * @param  recvData: 
  * @param  lens: 
  * @retval None
  */
void UART_RecvData( USART_TypeDef *UARTx, uint8_t *recvData, uint16_t lens )
{
  do {
    UART_RecvByte(UARTx, recvData++);
  } while (--lens);
}

/**
  * @brief  UART_RecvDataWTO
  * @param  UARTx: 
  * @param  recvData: 
  * @param  lens: 
  * @param  timeoutMs: 
  * @retval state of receive
  */
int8_t UART_RecvDataWTO( USART_TypeDef *UARTx, uint8_t *recvData, uint16_t lens, uint32_t timeoutMs )
{
  int8_t state = ERROR;

  do {
    state = UART_RecvByteWTO(UARTx, recvData++, timeoutMs);
  } while ((--lens) && (state != ERROR));

  return state;
}

/*************************************** END OF FILE ****************************************/
