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
  * @date    14-Jan-2017
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
  */
void UART_SendByte( USART_TypeDef *USARTx, uint8_t *sendByte )
{
  USARTx->DR = (*sendByte & (uint16_t)0x01FF);
  while (!(USARTx->SR & USART_FLAG_TC));
}

/**
  * @brief  UART_RecvByte
  */
void UART_RecvByte( USART_TypeDef *USARTx, uint8_t *recvByte )
{
  while (!(USARTx->SR & USART_FLAG_RXNE));
  *recvByte = (uint16_t)(USARTx->DR & (uint16_t)0x01FF);
}

/**
  * @brief  UART_SendData
  */
void UART_SendData( USART_TypeDef *USARTx, uint8_t *sendData, uint16_t lens )
{
  do {
    USARTx->DR = (*sendData++ & (uint16_t)0x01FF);
    while (!(USARTx->SR & USART_FLAG_TC));
  } while (--lens);
}

/**
  * @brief  UART_RecvData
  */
void UART_RecvData( USART_TypeDef *USARTx, uint8_t *recvData, uint16_t lens )
{
  do {
    while (!(USARTx->SR & USART_FLAG_RXNE));
    *recvData++ = (uint16_t)(USARTx->DR & (uint16_t)0x01FF);
  } while (--lens);
}

/**
  * @brief  UART_SendDataWTO
  */
int8_t UART_SendDataWTO( USART_TypeDef *USARTx, uint8_t *sendData, uint16_t lens, uint32_t timeout )
{
  uint32_t tickStart = 0;

  if((sendData == NULL) || (lens == 0)) {
    return HAL_ERROR;
  }

  while (lens--) {
    USARTx->DR = (*sendData++ & (uint16_t)0x01FF);
    tickStart = HAL_GetTick();
    while (!(USARTx->SR & USART_FLAG_TC)) {
      if ((HAL_GetTick() - tickStart) > timeout) {
        return HAL_TIMEOUT;
      }
    }
  }

  return HAL_OK;
}

/**
  * @brief  UART_RecvDataWTO
  */
int8_t UART_RecvDataWTO( USART_TypeDef *USARTx, uint8_t *recvData, uint16_t lens, uint32_t timeout )
{
  uint32_t tickStart = 0;

  if((recvData == NULL) || (lens == 0)) {
    return HAL_ERROR;
  }

  while (lens--) {
    tickStart = HAL_GetTick();
    while ((USARTx->SR & USART_FLAG_RXNE) == RESET) {
      if ((HAL_GetTick() - tickStart) > timeout) {
        return HAL_TIMEOUT;
      }
    }
    *recvData++ = (USARTx->DR & (uint16_t)0x01FF);
  }

  return HAL_OK;
}

/*************************************** END OF FILE ****************************************/
