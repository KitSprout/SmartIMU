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
  * @date    16-Nov-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"

/** @addtogroup STM32_Module
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
static UART_HandleTypeDef SerialHandle;
static uint8_t serialTxBuf[SERIAL_MAX_TXBUF] = {0};
static uint8_t serialRxBuf[SERIAL_MAX_RXBUF] = {0};

UartHandle_st hSerial = {
  .handle     = &SerialHandle,
  .TxCallback = NULL,
  .RxCallback = NULL,
  .pTxBuf     = serialTxBuf,
  .pRxBuf     = serialRxBuf
};

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

#if defined(KS_HW_UART_HAL_LIBRARY)

#else
  /* UART IT *******************************************************************/
  if (hSerial.TxCallback != NULL)
    __HAL_UART_ENABLE_IT(&SerialHandle, UART_IT_TXE);
  else
    __HAL_UART_DISABLE_IT(&SerialHandle, UART_IT_TXE);

  if (hSerial.RxCallback != NULL)
    __HAL_UART_ENABLE_IT(&SerialHandle, UART_IT_RXNE);
  else
    __HAL_UART_DISABLE_IT(&SerialHandle, UART_IT_RXNE);

  /* UART Enable ***************************************************************/
  __HAL_UART_ENABLE(&SerialHandle);
  __HAL_UART_CLEAR_FLAG(&SerialHandle, UART_FLAG_TC);

#endif

}

/**
  * @brief  Serial Send Data
  */
__INLINE int8_t Serial_SendData( uint8_t *sendData, uint16_t lens, uint32_t timuout )
{
  return UART_SendData(&SerialHandle, sendData, lens, timuout);
}

/**
  * @brief  Serial Recv Data
  */
__INLINE int8_t Serial_RecvData( uint8_t *recvData, uint16_t lens, uint32_t timeout )
{
  return UART_RecvData(&SerialHandle, recvData, lens, timeout);
}

#if defined(KS_HW_UART_HAL_LIBRARY)
/**
  * @brief  Serial Send Data IT
  */
__INLINE int8_t Serial_SendDataIT( uint8_t *sendData, uint16_t lens)
{
  return HAL_UART_Transmit_IT(&SerialHandle, sendData, lens);
}

/**
  * @brief  Serial Recv Data IT
  */
__INLINE int8_t Serial_RecvDataIT( uint8_t *recvData, uint16_t lens )
{
  return HAL_UART_Receive_IT(&SerialHandle, recvData, lens);
}

/**
  * @brief  Serial Send Data DMA
  */
__INLINE int8_t Serial_SendDataDMA( uint8_t *sendData, uint16_t lens )
{
  return HAL_UART_Transmit_DMA(&SerialHandle, sendData, lens);
}

/**
  * @brief  Serial Recv Data DMA
  */
__INLINE int8_t Serial_RecvDataDMA( uint8_t *recvData, uint16_t lens )
{
  return HAL_UART_Receive_DMA(&SerialHandle, recvData, lens);
}
#endif

/**
  * @brief  fputc
  */
int fputc( int ch, FILE *f )
{
  SerialHandle.Instance->DR = ((uint8_t)ch & (uint16_t)0x01FF);
  while (!(SerialHandle.Instance->SR & UART_FLAG_TC));
  return (ch);
}

/**
  * @brief  fgetc
  */
int fgetc( FILE *f )
{
  while (!(SerialHandle.Instance->SR & UART_FLAG_RXNE));
  return (SerialHandle.Instance->DR & (uint16_t)0x01FF);
}

/*************************************** END OF FILE ****************************************/
