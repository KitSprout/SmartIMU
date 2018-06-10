/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32f4_uart.c
 *  @author  KitSprout
 *  @date    22-Apr-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_uart.h"

/** @addtogroup STM32_Driver
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static UART_HandleTypeDef huart_serial;
UartHandle_st hSerial = {
  .handle          = &huart_serial,
  .txBufLens       = SERIAL_MAX_TXBUF,
  .rxBufLens       = SERIAL_MAX_RXBUF,
  .pTxBuf          = NULL,
  .pRxBuf          = NULL,
  .txEventCallback = NULL,
  .rxEventCallback = NULL
};

/* Prototypes ------------------------------------------------------------------------------*/
/* MSP Functions ---------------------------------------------------------------------------*/

/**
 *  @brief UART MSP Initialization
 */
void HAL_UART_MspInit( UART_HandleTypeDef *huart )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if (huart->Instance == SERIAL_UARTx) {
    /* UART Clk ******************************************************************/
    SERIAL_UARTx_CLK_ENABLE();

    /* UART Pin ******************************************************************/
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

    GPIO_InitStruct.Pin       = SERIAL_TX_PIN;
    GPIO_InitStruct.Alternate = SERIAL_TX_AF;
    HAL_GPIO_Init(SERIAL_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = SERIAL_RX_PIN;
    GPIO_InitStruct.Alternate = SERIAL_RX_AF;
    HAL_GPIO_Init(SERIAL_RX_GPIO_PORT, &GPIO_InitStruct);

    /* UART TI *******************************************************************/
    HAL_NVIC_SetPriority(SERIAL_UARTx_IRQn, SERIAL_UARTx_IRQn_PREEMPT, SERIAL_UARTx_IRQn_SUB);
    HAL_NVIC_EnableIRQ(SERIAL_UARTx_IRQn);
  }
}

/**
 *  @brief UART MSP De-Initialization
 */
void HAL_UART_MspDeInit( UART_HandleTypeDef *huart )
{
  if (huart->Instance == SERIAL_UARTx) {
    SERIAL_UARTx_FORCE_RESET();
    SERIAL_UARTx_RELEASE_RESET();
    HAL_GPIO_DeInit(SERIAL_TX_GPIO_PORT, SERIAL_TX_PIN);
    HAL_GPIO_DeInit(SERIAL_RX_GPIO_PORT, SERIAL_RX_PIN);
    HAL_NVIC_DisableIRQ(SERIAL_UARTx_IRQn);
  }
}

/**
 *  @brief  Tx Transfer completed callbacks.
 */
void HAL_UART_TxCpltCallback( UART_HandleTypeDef *huart )
{
  if (huart->Instance == SERIAL_UARTx) {
    hSerial.txEventCallback();
  }
}

/**
 *  @brief  Tx Half Transfer completed callbacks.
 */
void HAL_UART_TxHalfCpltCallback( UART_HandleTypeDef *huart )
{
  if (huart->Instance == SERIAL_UARTx) {
    
  }
}

/**
 *  @brief  Rx Transfer completed callbacks.
 */
void HAL_UART_RxCpltCallback( UART_HandleTypeDef *huart )
{
  if (huart->Instance == SERIAL_UARTx) {
    hSerial.rxEventCallback();
  }
}

/**
 *  @brief  Rx Half Transfer completed callbacks.
 */
void HAL_UART_RxHalfCpltCallback( UART_HandleTypeDef *huart )
{
  if (huart->Instance == SERIAL_UARTx) {
    
  }
}

/**
 *  @brief  UART error callbacks.
 */
void HAL_UART_ErrorCallback( UART_HandleTypeDef *huart )
{
  if (huart->Instance == SERIAL_UARTx) {
    
  }
}

/* Functions -------------------------------------------------------------------------------*/

#if !KS_FW_UART_HAL_LIBRARY

/**
 *  @brief  UART_SendByte
 */
void UART_SendByte( UART_HandleTypeDef *huart, uint8_t *sendByte )
{
  huart->Instance->DR = (*sendByte & (uint16_t)0x01FF);
  while (!(huart->Instance->SR & UART_FLAG_TXE));
}

/**
 *  @brief  UART_RecvByte
 */
void UART_RecvByte( UART_HandleTypeDef *huart, uint8_t *recvByte )
{
  while (!(huart->Instance->SR & UART_FLAG_RXNE));
  *recvByte = (uint16_t)(huart->Instance->DR & (uint16_t)0x01FF);
}

/**
 *  @brief  UART_RecvWait
 */
uint32_t UART_RecvWait( UART_HandleTypeDef *huart, uint8_t *recvData )
{
  uint32_t lens = 0;
  uint16_t timeout = 0x7FFF;

  while (timeout--) {
    while ((huart->Instance->SR & UART_FLAG_RXNE) != RESET) {
      *recvData++ = (huart->Instance->DR & (uint16_t)0x01FF);
      lens++;
      timeout = 0x7FFF;
    }
    if (lens == 128) {
      break;
    }
  }

  return lens;
}

/**
 *  @brief  UART_SendData
 */
uint32_t UART_SendData( UART_HandleTypeDef *huart, uint8_t *sendData, uint16_t lens, uint32_t timeout )
{
  uint32_t tickStart = 0;

  if ((sendData == NULL) || (lens == 0)) {
    return HAL_ERROR;
  }

  while (lens--) {
    huart->Instance->DR = (*sendData++ & (uint16_t)0x01FF);
    tickStart = HAL_GetTick();
    while (!(huart->Instance->SR & UART_FLAG_TC)) {
      if ((HAL_GetTick() - tickStart) > timeout) {
        return HAL_TIMEOUT;
      }
    }
  }

  return HAL_OK;
}

/**
 *  @brief  UART_RecvData
 */
uint32_t UART_RecvData( UART_HandleTypeDef *huart, uint8_t *recvData, uint16_t lens, uint32_t timeout )
{
  uint32_t tickStart = 0;

  if ((recvData == NULL) || (lens == 0)) {
    return HAL_ERROR;
  }

  while (lens--) {
    tickStart = HAL_GetTick();
    while ((huart->Instance->SR & UART_FLAG_RXNE) == RESET) {
      if ((HAL_GetTick() - tickStart) > timeout) {
        return HAL_TIMEOUT;
      }
    }
    *recvData++ = (huart->Instance->DR & (uint16_t)0x01FF);
  }

  return HAL_OK;
}

#endif

/*************************************** END OF FILE ****************************************/
