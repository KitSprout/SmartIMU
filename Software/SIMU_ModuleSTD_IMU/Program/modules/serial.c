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
  * @date    14-Jan-2017
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
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void Serial_Config( uint8_t interrupt )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  USART_InitTypeDef UART_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  /* UART Clk ******************************************************************/
  SERIAL_UARTx_CLK_ENABLE();

  /* UART AF *******************************************************************/
  GPIO_PinAFConfig(SERIAL_TX_GPIO_PORT, SERIAL_TX_SOURCE, SERIAL_TX_AF);
  GPIO_PinAFConfig(SERIAL_RX_GPIO_PORT, SERIAL_RX_SOURCE, SERIAL_RX_AF);

  /* UART Pin ******************************************************************/
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = SERIAL_TX_PIN;
  GPIO_Init(SERIAL_TX_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = SERIAL_RX_PIN;
  GPIO_Init(SERIAL_RX_GPIO_PORT, &GPIO_InitStruct);

  /* UART IT *******************************************************************/
  NVIC_InitStruct.NVIC_IRQChannel                   = SERIAL_UARTx_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = SERIAL_UARTx_IRQn_PREEMPT;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority        = SERIAL_UARTx_IRQn_SUB;
  NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

  /* UART Init *****************************************************************/
  UART_InitStruct.USART_BaudRate            = SERIAL_BAUDRATE;
  UART_InitStruct.USART_WordLength          = SERIAL_BYTESIZE;
  UART_InitStruct.USART_StopBits            = SERIAL_STOPBITS;
  UART_InitStruct.USART_Parity              = SERIAL_PARITY;
  UART_InitStruct.USART_HardwareFlowControl = SERIAL_HARDWARECTRL;
  UART_InitStruct.USART_Mode                = SERIAL_MODE;
  USART_Init(SERIAL_UARTx, &UART_InitStruct);

  /* UART Enable ***************************************************************/
  if (interrupt == ENABLE) {
    USART_ITConfig(SERIAL_UARTx, USART_IT_RXNE, ENABLE);
  }
  else {
    USART_ITConfig(SERIAL_UARTx, USART_IT_RXNE, DISABLE);
  }
  USART_Cmd(SERIAL_UARTx, ENABLE);
  USART_ClearFlag(SERIAL_UARTx, USART_FLAG_TC);
}

/**
  * @brief  Serial_SendByte
  */
void Serial_SendByte( uint8_t sendByte )
{
  UART_SendByte(SERIAL_UARTx, &sendByte);
}

/**
  * @brief  Serial_SendData
  */
void Serial_SendData( uint8_t *sendData, uint16_t lens )
{
  UART_SendData(SERIAL_UARTx, sendData, lens);
}

/**
  * @brief  Serial_SendStr
  */
void Serial_SendStr( char *pWord )
{
  while (*pWord != '\0') {
    UART_SendByte(SERIAL_UARTx, (uint8_t*)pWord++);
  }
}

/**
  * @brief  Serial_SendNum
  */
void Serial_SendNum( StringType_t type, uint8_t lens, int32_t number )
{
  char tmpStr[32] = {0};
  char *pWord = tmpStr;

  num2Str(type, lens, tmpStr, number);

  while (*pWord != '\0') {
    UART_SendByte(SERIAL_UARTx, (uint8_t*)pWord++);
  }
}

/**
  * @brief  Serial_RecvByte
  */
uint8_t Serial_RecvByte( void )
{
  uint8_t recvByte = 0;
  UART_RecvByte(SERIAL_UARTx, &recvByte);
  return recvByte;
}

/**
  * @brief  Serial_RecvData
  */
void Serial_RecvData( uint8_t *recvData, uint16_t lens )
{
  UART_RecvData(SERIAL_UARTx, recvData, lens);
}

/**
  * @brief  Serial_RecvDataWTO
  */
int8_t Serial_RecvDataWTO( uint8_t *recvData, uint16_t lens, int32_t timeoutMs )
{
  return UART_RecvDataWTO(SERIAL_UARTx, recvData, lens, timeoutMs);
}

/**
  * @brief  Serial_RecvStr
  */
void Serial_RecvStr( char *pWord )
{
  do {
    UART_RecvByte(SERIAL_UARTx, (uint8_t*)pWord++);
  } while (*(pWord-1) != '\0');
  *pWord = '\0';
}

int fputc( int ch, FILE *f )
{
  SERIAL_UARTx->DR = ((uint8_t)ch & (uint16_t)0x01FF);
  while (!(SERIAL_UARTx->SR & USART_FLAG_TC));
  return (ch);
}

int fgetc( FILE *f )
{
  while (!(SERIAL_UARTx->SR & USART_FLAG_RXNE));
  return (uint16_t)(SERIAL_UARTx->DR & (uint16_t)0x01FF);
}

/*************************************** END OF FILE ****************************************/
