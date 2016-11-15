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
pFunc SerialIRQEven = NULL;

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void Serial_Config( pFunc pUARTx )
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

  /* UART IT *******************************************************************/
  SerialIRQEven = pUARTx;
  if (SerialIRQEven != NULL) {
    HAL_NVIC_SetPriority(SERIAL_UARTx_IRQn, 0x0F, 1);
    HAL_NVIC_EnableIRQ(SERIAL_UARTx_IRQn);
    __HAL_UART_ENABLE_IT(&SerialHandle, UART_IT_RXNE);
  }
  else {
    HAL_NVIC_DisableIRQ(SERIAL_UARTx_IRQn);
    __HAL_UART_DISABLE_IT(&SerialHandle, UART_IT_RXNE);
  }

  /* UART Enable ***************************************************************/
  __HAL_UART_ENABLE(&SerialHandle);
  __HAL_UART_CLEAR_FLAG(&SerialHandle, UART_FLAG_TC);
}

/**
  * @brief  Serial_SendByte
  * @param  sendByte: 
  * @retval None
  */
void Serial_SendByte( uint8_t sendByte )
{
  UART_SendByte(&SerialHandle, sendByte);
}

/**
  * @brief  Serial_SendData
  * @param  sendData: 
  * @param  lens: 
  * @retval None
  */
void Serial_SendData( uint8_t *sendData, uint16_t lens )
{
  UART_SendData(&SerialHandle, sendData, lens);
}

/**
  * @brief  Serial_SendStr
  * @param  pWord: 
  * @retval None
  */
void Serial_SendStr( char *pWord )
{
  while (*pWord != '\0') {
    UART_SendByte(&SerialHandle, *pWord++);
  }
}

/**
  * @brief  Serial_SendNum
  * @param  type: 
  * @param  lens: 
  * @param  sendNum: 
  * @retval None
  */
void Serial_SendNum( StringType_t type, uint8_t lens, int32_t number )
{
  char tmpStr[32] = {0};
  char *pWord = tmpStr;

  num2Str(type, lens, tmpStr, number);

  while (*pWord != '\0') {
    UART_SendByte(&SerialHandle, *pWord++);
  }
}

/**
  * @brief  Serial_RecvByte
  * @param  None
  * @retval receive byte
  */
uint8_t Serial_RecvByte( void )
{
  return UART_RecvByte(&SerialHandle);
}

/**
  * @brief  Serial_RecvData
  * @param  recvData: 
  * @param  lens: 
  * @retval None
  */
void Serial_RecvData( uint8_t *recvData, uint16_t lens )
{
  UART_RecvData(&SerialHandle, recvData, lens);
}

/**
  * @brief  Serial_RecvByteWTO
  * @param  recvByte: 
  * @param  lens: 
  * @param  timeout: 
  * @retval state of receive
  */
int8_t Serial_RecvByteWTO( uint8_t *recvByte, int32_t timeout )
{
  return UART_RecvByteWTO(&SerialHandle, recvByte, timeout);
}

/**
  * @brief  Serial_RecvDataWTO
  * @param  recvData: 
  * @param  lens: 
  * @param  timeout: 
  * @retval state of receive
  */
int8_t Serial_RecvDataWTO( uint8_t *recvData, uint16_t lens, int32_t timeout )
{
  return UART_RecvDataWTO(&SerialHandle, recvData, lens, timeout);
}

/**
  * @brief  Serial_RecvStr
  * @param  pWord: 
  * @retval None
  */
void Serial_RecvStr( char *pWord )
{
  do {
    *pWord++ = UART_RecvByte(&SerialHandle);
  } while (*(pWord - 1) != '\0');
  *pWord = '\0';
}

/**
  * @brief  Serial_RecvStrWTO
  * @param  pWord: 
  * @param  timeoutMs: 
  * @retval state of receive
  */
int8_t Serial_RecvStrWTO( char *pWord, int32_t timeoutMs )
{
  int8_t state = ERROR;

  do {
    state = UART_RecvByteWTO(&SerialHandle, (uint8_t*)pWord++, timeoutMs);
    if (state == ERROR)
      return ERROR;
  } while (*(pWord-1) != '\0');
  *pWord = '\0';

  return SUCCESS;
}

int fputc( int ch, FILE *f )
{
  SerialHandle.Instance->DR = ((uint8_t)ch & (uint16_t)0x01FF);
  while (!(SerialHandle.Instance->SR & UART_FLAG_TC));
  return (ch);
}

int fgetc( FILE *f )
{
  while (!(SerialHandle.Instance->SR & UART_FLAG_RXNE));
  return (SerialHandle.Instance->DR & (uint16_t)0x01FF);
}

/*************************************** END OF FILE ****************************************/
