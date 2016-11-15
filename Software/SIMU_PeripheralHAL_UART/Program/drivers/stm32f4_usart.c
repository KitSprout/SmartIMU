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
  * @date    14-Nov-2016
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
pFunc Serial_TxCpltCallback = NULL;
pFunc Serial_RxCpltCallback = NULL;

/* Private function prototypes -------------------------------------------------------------*/

/* MSP functions ---------------------------------------------------------------------------*/

/**
  * @brief UART MSP Initialization
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
    HAL_NVIC_SetPriority(SERIAL_UARTx_IRQn, 0x0F, 1);
    HAL_NVIC_EnableIRQ(SERIAL_UARTx_IRQn);
  }
}

/**
  * @brief UART MSP De-Initialization
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
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
  * @brief  Tx Transfer completed callbacks.
  */
void HAL_UART_TxCpltCallback( UART_HandleTypeDef *huart )
{
  if (huart->Instance == SERIAL_UARTx) {
    Serial_TxCpltCallback();
  }
}

/**
  * @brief  Tx Half Transfer completed callbacks.
  */
void HAL_UART_TxHalfCpltCallback( UART_HandleTypeDef *huart )
{
  if (huart->Instance == SERIAL_UARTx) {
    // ...
  }
}

/**
  * @brief  Rx Transfer completed callbacks.
  */
void HAL_UART_RxCpltCallback( UART_HandleTypeDef *huart )
{
  if (huart->Instance == SERIAL_UARTx) {
    Serial_RxCpltCallback();
  }
}

/**
  * @brief  Rx Half Transfer completed callbacks.
  */
void HAL_UART_RxHalfCpltCallback( UART_HandleTypeDef *huart )
{
  if (huart->Instance == SERIAL_UARTx) {
    // ...
  }
}

/**
  * @brief  UART error callbacks.
  */
void HAL_UART_ErrorCallback( UART_HandleTypeDef *huart )
{
  if (huart->Instance == SERIAL_UARTx) {
    // ...
  }
}
 
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  UART_SendData
  * @param  huart: 
  * @param  sendData: 
  * @param  lens: 
  * @param  timeout: 
  * @retval None
  */
int8_t UART_SendData( UART_HandleTypeDef *huart, uint8_t *sendData, uint16_t lens, uint32_t timeout )
{
  return HAL_UART_Transmit(huart, sendData, lens, timeout);
}

/**
  * @brief  UART_RecvData
  * @param  huart: 
  * @param  recvData: 
  * @param  lens: 
  * @param  timeout: 
  * @retval None
  */
int8_t UART_RecvData( UART_HandleTypeDef *huart, uint8_t *recvData, uint16_t lens, uint32_t timeout )
{
  return HAL_UART_Receive(huart, recvData, lens, timeout);
}

/**
  * @brief  UART_SendDataIT
  * @param  huart: 
  * @param  sendData: 
  * @param  lens: 
  * @retval None
  */
int8_t UART_SendDataIT( UART_HandleTypeDef *huart, uint8_t *sendData, uint16_t lens )
{
  return HAL_UART_Transmit_IT(huart, sendData, lens);
}

/**
  * @brief  UART_RecvDataIT
  * @param  huart: 
  * @param  recvData: 
  * @param  lens: 
  * @retval None
  */
int8_t UART_RecvDataIT( UART_HandleTypeDef *huart, uint8_t *recvData, uint16_t lens )
{
  return HAL_UART_Receive_IT(huart, recvData, lens);
}

/**
  * @brief  UART_SendDataDMA
  * @param  huart: 
  * @param  sendData: 
  * @param  lens: 
  * @retval None
  */
int8_t UART_SendDataDMA( UART_HandleTypeDef *huart, uint8_t *sendData, uint16_t lens )
{
  return HAL_UART_Transmit_DMA(huart, sendData, lens);
}

/**
  * @brief  UART_RecvDataDMA
  * @param  huart: 
  * @param  recvData: 
  * @param  lens: 
  * @retval None
  */
int8_t UART_RecvDataDMA( UART_HandleTypeDef *huart, uint8_t *recvData, uint16_t lens )
{
  return HAL_UART_Receive_DMA(huart, recvData, lens);
}

/*************************************** END OF FILE ****************************************/
