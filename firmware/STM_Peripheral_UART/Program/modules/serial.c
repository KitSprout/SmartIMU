 /**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    serial.c
 *  @author  KitSprout
 *  @date    22-Apr-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"

/** @addtogroup STM32_Module
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static uint8_t TX_BUFFER[SERIAL_MAX_TXBUF] = {0};
static uint8_t RX_BUFFER[SERIAL_MAX_RXBUF] = {0};

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  delay_nop
 */
void Serial_Config( void )
{
  hSerial.pTxBuf = TX_BUFFER;
  hSerial.pRxBuf = RX_BUFFER;

  /* UART Init *****************************************************************/
  hSerial.handle->Instance          = SERIAL_UARTx;
  hSerial.handle->Init.BaudRate     = SERIAL_BAUDRATE;
  hSerial.handle->Init.WordLength   = SERIAL_BYTESIZE;
  hSerial.handle->Init.StopBits     = SERIAL_STOPBITS;
  hSerial.handle->Init.Parity       = SERIAL_PARITY;
  hSerial.handle->Init.HwFlowCtl    = SERIAL_HARDWARECTRL;
  hSerial.handle->Init.Mode         = SERIAL_MODE;
  hSerial.handle->Init.OverSampling = SERIAL_OVERSAMPLE;
  HAL_UART_Init(hSerial.handle);

#if KS_FW_UART_HAL_LIBRARY

#else
  /* UART IT *******************************************************************/
  if (hSerial.txEventCallback != NULL) {
    __HAL_UART_ENABLE_IT(hSerial.handle, UART_IT_TXE);
  }
  else {
    __HAL_UART_DISABLE_IT(hSerial.handle, UART_IT_TXE);
  }

  if (hSerial.rxEventCallback != NULL) {
    __HAL_UART_ENABLE_IT(hSerial.handle, UART_IT_RXNE);
  }
  else {
    __HAL_UART_DISABLE_IT(hSerial.handle, UART_IT_RXNE);
  }

  /* UART Enable ***************************************************************/
  __HAL_UART_ENABLE(hSerial.handle);
  __HAL_UART_CLEAR_FLAG(hSerial.handle, UART_FLAG_TC);

#endif
}

/**
 *  @brief  Serial_SendByte
 */
__INLINE void Serial_SendByte( uint8_t sendByte )
{
  UART_SendByte(hSerial.handle, &sendByte);
}

/**
 *  @brief  Serial_RecvByte
 */
__INLINE uint8_t Serial_RecvByte( void )
{
  uint8_t recvByte = 0;
  UART_RecvByte(hSerial.handle, &recvByte);
  return recvByte;
}

#if !KS_FW_UART_HAL_LIBRARY
/**
 *  @brief  Serial Recv Wait
 */
__INLINE uint32_t Serial_RecvWait( uint8_t *recvData )
{
  return UART_RecvWait(hSerial.handle, recvData);
}
#endif

/**
 *  @brief  Serial Send Data
 */
__INLINE uint32_t Serial_SendData( uint8_t *sendData, uint16_t lens, uint32_t timuout )
{
  return UART_SendData(hSerial.handle, sendData, lens, timuout);
}

/**
 *  @brief  Serial Recv Data
 */
__INLINE uint32_t Serial_RecvData( uint8_t *recvData, uint16_t lens, uint32_t timeout )
{
  return UART_RecvData(hSerial.handle, recvData, lens, timeout);
}

#if KS_FW_UART_HAL_LIBRARY

/**
 *  @brief  Serial Send Data IT
 */
__INLINE uint32_t Serial_SendDataIT( uint8_t *sendData, uint16_t lens)
{
  return HAL_UART_Transmit_IT(hSerial.handle, sendData, lens);
}

/**
 *  @brief  Serial Recv Data IT
 */
__INLINE uint32_t Serial_RecvDataIT( uint8_t *recvData, uint16_t lens )
{
  return HAL_UART_Receive_IT(hSerial.handle, recvData, lens);
}

/**
 *  @brief  Serial Send Data DMA
 */
__INLINE uint32_t Serial_SendDataDMA( uint8_t *sendData, uint16_t lens )
{
  return HAL_UART_Transmit_DMA(hSerial.handle, sendData, lens);
}

/**
 *  @brief  Serial Recv Data DMA
 */
__INLINE uint32_t Serial_RecvDataDMA( uint8_t *recvData, uint16_t lens )
{
  return HAL_UART_Receive_DMA(hSerial.handle, recvData, lens);
}
#endif

/**
 *  @brief  fputc
 */
int fputc( int ch, FILE *f )
{
  hSerial.handle->Instance->DR = ((uint8_t)ch & (uint16_t)0x01FF);
  while (!(hSerial.handle->Instance->SR & UART_FLAG_TXE));
  return (ch);
}

/**
 *  @brief  fgetc
 */
int fgetc( FILE *f )
{
  while (!(hSerial.handle->Instance->SR & UART_FLAG_RXNE));
  return (hSerial.handle->Instance->DR & (uint16_t)0x01FF);
}

/*************************************** END OF FILE ****************************************/
