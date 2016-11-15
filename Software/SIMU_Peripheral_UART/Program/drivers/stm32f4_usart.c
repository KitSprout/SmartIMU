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
#define UART_Delay(__TIME)    delay_ms(__TIME);

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
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
  }
}

/**
  * @brief  Tx Transfer completed callbacks.
  */
void HAL_UART_TxCpltCallback( UART_HandleTypeDef *huart )
{

}

/**
  * @brief  Tx Half Transfer completed callbacks.
  */
void HAL_UART_TxHalfCpltCallback( UART_HandleTypeDef *huart )
{

}
extern pFunc IRQEven_UART6;
/**
  * @brief  Rx Transfer completed callbacks.
  */
void HAL_UART_RxCpltCallback( UART_HandleTypeDef *huart )
{

}

/**
  * @brief  Rx Half Transfer completed callbacks.
  */
void HAL_UART_RxHalfCpltCallback( UART_HandleTypeDef *huart )
{

}

/**
  * @brief  UART error callbacks.
  */
void HAL_UART_ErrorCallback( UART_HandleTypeDef *huart )
{

}
 
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  UART_SendByte
  * @param  huart: 
  * @param  sendByte: 
  * @retval None
  */
void UART_SendByte( UART_HandleTypeDef *huart, uint8_t sendByte )
{
  huart->Instance->DR = (sendByte & (uint16_t)0x01FF);
  while (!(huart->Instance->SR & UART_FLAG_TC));
}

/**
  * @brief  UART_SendData
  * @param  huart: 
  * @param  sendData: 
  * @param  lens: 
  * @retval None
  */
void UART_SendData( UART_HandleTypeDef *huart, uint8_t *sendData, uint16_t lens )
{
  while (lens--) {
    huart->Instance->DR = (*sendData++ & (uint16_t)0x01FF);
    while (!(huart->Instance->SR & UART_FLAG_TC));
  };
}

/**
  * @brief  UART_RecvByte
  * @param  huart: 
  * @retval recvByte
  */
uint8_t UART_RecvByte( UART_HandleTypeDef *huart )
{
  while(!(huart->Instance->SR & UART_FLAG_RXNE));
  return (huart->Instance->DR & (uint16_t)0x01FF);
}

/**
  * @brief  UART_RecvData
  * @param  huart: 
  * @param  recvData: 
  * @param  lens: 
  * @retval None
  */
void UART_RecvData( UART_HandleTypeDef *huart, uint8_t *recvData, uint16_t lens )
{
  while (lens--) {
    while (!(huart->Instance->SR & UART_FLAG_RXNE));
    *recvData++ = (huart->Instance->DR & (uint16_t)0x01FF);
  };
}

/**
  * @brief  UART_RecvByteWTO
  * @param  huart: 
  * @param  recvByte: 
  * @param  timeout: millisecond
  * @retval status of receive
  */
int8_t UART_RecvByteWTO( UART_HandleTypeDef *huart, uint8_t *recvByte, uint32_t timeout )
{
  while ((huart->Instance->SR & UART_FLAG_RXNE) == RESET) {
    if (timeout-- > 0) {
      UART_Delay(1);
    }
    else {
      return HAL_TIMEOUT;
    }
  }
  *recvByte = (huart->Instance->DR & (uint16_t)0x01FF);
 
  return HAL_OK;
}

/**
  * @brief  UART_RecvDataWTO
  * @param  UARTx: 
  * @param  recvData: 
  * @param  lens: 
  * @param  timeout: 
  * @retval status of receive
  */
int8_t UART_RecvDataWTO( UART_HandleTypeDef *huart, uint8_t *recvData, uint16_t lens, uint32_t timeout )
{
  HAL_StatusTypeDef status = HAL_OK;

  while ((lens--) && (status != HAL_TIMEOUT)) {
    while ((huart->Instance->SR & UART_FLAG_RXNE) == RESET) {
      if (timeout-- > 0) {
        UART_Delay(1);
      }
      else {
        status = HAL_TIMEOUT;
        break;
      }
    }
    if (status != HAL_TIMEOUT) {
      *recvData++ = (huart->Instance->DR & (uint16_t)0x01FF);
      status = HAL_OK;
    }
  };

  return status;
}

/*************************************** END OF FILE ****************************************/
