/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    serial.c
  * @author  KitSprout
  * @date    14-Nov-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_usart.h"
#include "modules\serial.h"

/** @addtogroup STM32_Module
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
UART_HandleTypeDef SerialHandle;

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void Serial_Config( void )
{
  /* UART Clk ******************************************************************/
  SERIAL_UARTx_CLK_ENABLE();

  /* UART Init *****************************************************************/
  SerialHandle.Instance          = SERIAL_UARTx;
  SerialHandle.Init.BaudRate     = SERIAL_BAUDRATE;
  SerialHandle.Init.WordLength   = SERIAL_BYTESIZE;
  SerialHandle.Init.StopBits     = SERIAL_STOPBITS;
  SerialHandle.Init.Parity       = SERIAL_PARITY;
  SerialHandle.Init.HwFlowCtl    = SERIAL_HARDWARECTRL;
  SerialHandle.Init.Mode         = SERIAL_MODE;
  SerialHandle.Init.OverSampling = SERIAL_OVERSAMPLE;
  HAL_UART_Init(&SerialHandle);
}

/**
  * @brief  Serial_SendData
  * @param  sendData: 
  * @param  lens: 
  * @retval None
  */
__INLINE int8_t Serial_SendData( uint8_t *sendData, uint16_t lens, uint32_t timuout )
{
  return HAL_UART_Transmit(&SerialHandle, sendData, lens, timuout);
}

/**
  * @brief  Serial_RecvData
  * @param  recvData: 
  * @param  lens: 
  * @retval None
  */
__INLINE int8_t Serial_RecvData( uint8_t *recvData, uint16_t lens, uint32_t timeout )
{
  return HAL_UART_Receive(&SerialHandle, recvData, lens, timeout);
}


/**
  * @brief  Serial_SendData
  * @param  sendData: 
  * @param  lens: 
  * @retval None
  */
__INLINE int8_t Serial_SendDataIT( uint8_t *sendData, uint16_t lens)
{
  return HAL_UART_Transmit_IT(&SerialHandle, sendData, lens);
}

/**
  * @brief  Serial_RecvData
  * @param  recvData: 
  * @param  lens: 
  * @retval None
  */
__INLINE int8_t Serial_RecvDataIT( uint8_t *recvData, uint16_t lens )
{
  return HAL_UART_Receive_IT(&SerialHandle, recvData, lens);
}


/**
  * @brief  Serial_SendData
  * @param  sendData: 
  * @param  lens: 
  * @retval None
  */
__INLINE int8_t Serial_SendDataDMA( uint8_t *sendData, uint16_t lens )
{
  return HAL_UART_Transmit_DMA(&SerialHandle, sendData, lens);
}

/**
  * @brief  Serial_RecvData
  * @param  recvData: 
  * @param  lens: 
  * @retval None
  */
__INLINE int8_t Serial_RecvDataDMA( uint8_t *recvData, uint16_t lens )
{
  return HAL_UART_Receive_DMA(&SerialHandle, recvData, lens);
}

/**
  * @brief  Serial_RecvData
  * @param  recvData: 
  * @param  lens: 
  * @retval None
  */
int fputc( int ch, FILE *f )
{
  SerialHandle.Instance->DR = ((uint8_t)ch & (uint16_t)0x01FF);
  while (!(SerialHandle.Instance->SR & UART_FLAG_TC));
  return (ch);
}

/**
  * @brief  Serial_RecvData
  * @param  recvData: 
  * @param  lens: 
  * @retval None
  */
int fgetc( FILE *f )
{
  while (!(SerialHandle.Instance->SR & UART_FLAG_RXNE));
  return (SerialHandle.Instance->DR & (uint16_t)0x01FF);
}

/*************************************** END OF FILE ****************************************/
