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
  * @date    5-Nov-2016
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
#define UARTx                 USART6
#define UARTx_CLK_ENABLE()    __HAL_RCC_USART6_CLK_ENABLE()
#define UARTx_IRQn            USART6_IRQn

#define UARTx_TX_PIN          GPIO_PIN_6
#define UARTx_TX_GPIO_PORT    GPIOC
#define UARTx_TX_AF           GPIO_AF8_USART6

#define UARTx_RX_PIN          GPIO_PIN_7
#define UARTx_RX_GPIO_PORT    GPIOC
#define UARTx_RX_AF           GPIO_AF8_USART6

#define UARTx_BAUDRATE        115200
#define UARTx_BYTESIZE        UART_WORDLENGTH_8B
#define UARTx_STOPBITS        UART_STOPBITS_1
#define UARTx_PARITY          UART_PARITY_NONE
#define UARTx_HARDWARECTRL    UART_HWCONTROL_NONE
#define UARTx_MODE            UART_MODE_TX_RX
#define UARTx_OVERSAMPLE      UART_OVERSAMPLING_16

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
UART_HandleTypeDef SerialHandle;

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void Serial_Config( uint8_t interrupt )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* UART Clk ******************************************************************/
  UARTx_CLK_ENABLE();

  /* UART Pin ******************************************************************/
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin       = UARTx_TX_PIN;
  GPIO_InitStruct.Alternate = UARTx_TX_AF;
  HAL_GPIO_Init(UARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = UARTx_RX_PIN;
  GPIO_InitStruct.Alternate = UARTx_RX_AF;
  HAL_GPIO_Init(UARTx_RX_GPIO_PORT, &GPIO_InitStruct);

  /* UART IT *******************************************************************/
  HAL_NVIC_SetPriority(UARTx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(UARTx_IRQn);

  /* UART Init *****************************************************************/
  SerialHandle.Instance          = UARTx;
  SerialHandle.Init.BaudRate     = UARTx_BAUDRATE;
  SerialHandle.Init.WordLength   = UARTx_BYTESIZE;
  SerialHandle.Init.StopBits     = UARTx_STOPBITS;
  SerialHandle.Init.Parity       = UARTx_PARITY;
  SerialHandle.Init.HwFlowCtl    = UARTx_HARDWARECTRL;
  SerialHandle.Init.Mode         = UARTx_MODE;
  SerialHandle.Init.OverSampling = UARTx_OVERSAMPLE;
  HAL_UART_Init(&SerialHandle);

  /* UART Enable ***************************************************************/
  if (interrupt == ENABLE) {
    __HAL_UART_ENABLE_IT(&SerialHandle, UART_IT_RXNE);
  }
  else {
    __HAL_UART_DISABLE_IT(&SerialHandle, UART_IT_RXNE);
  }
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
  UART_SendByte(UARTx, &sendByte);
}

/**
  * @brief  Serial_SendData
  * @param  sendData: 
  * @param  lens: 
  * @retval None
  */
void Serial_SendData( uint8_t *sendData, uint16_t lens )
{
  UART_SendData(UARTx, sendData, lens);
}

/**
  * @brief  Serial_SendStr
  * @param  pWord: 
  * @retval None
  */
void Serial_SendStr( char *pWord )
{
  while (*pWord != '\0') {
    UART_SendByte(UARTx, (uint8_t*)pWord++);
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
    UART_SendByte(UARTx, (uint8_t*)pWord++);
  }
}

/**
  * @brief  Serial_RecvByte
  * @param  None
  * @retval receive byte
  */
uint8_t Serial_RecvByte( void )
{
  uint8_t recvByte = 0;
  UART_RecvByte(UARTx, &recvByte);
  return recvByte;
}

/**
  * @brief  Serial_RecvData
  * @param  recvData: 
  * @param  lens: 
  * @retval None
  */
void Serial_RecvData( uint8_t *recvData, uint16_t lens )
{
  UART_RecvData(UARTx, recvData, lens);
}

/**
  * @brief  Serial_RecvDataWTO
  * @param  recvData: 
  * @param  lens: 
  * @param  timeoutMs: 
  * @retval state of receive
  */
int8_t Serial_RecvDataWTO( uint8_t *recvData, uint16_t lens, int32_t timeoutMs )
{
  return UART_RecvDataWTO(UARTx, recvData, lens, timeoutMs);
}

/**
  * @brief  Serial_RecvStr
  * @param  pWord: 
  * @retval None
  */
void Serial_RecvStr( char *pWord )
{
  do {
    UART_RecvByte(UARTx, (uint8_t*)pWord++);
  } while (*(pWord-1) != '\0');
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
    state = UART_RecvByteWTO(UARTx, (uint8_t*)pWord++, timeoutMs);
    if (state == ERROR)
      return ERROR;
  } while (*(pWord-1) != '\0');
  *pWord = '\0';

  return SUCCESS;
}

int fputc( int ch, FILE *f )
{
  UARTx->DR = ((uint8_t)ch & (uint16_t)0x01FF);
  while (!(UARTx->SR & UART_FLAG_TC));
  return (ch);
}

int fgetc( FILE *f )
{
  while (!(UARTx->SR & UART_FLAG_RXNE));
  return (uint16_t)(UARTx->DR & (uint16_t)0x01FF);
}

/*************************************** END OF FILE ****************************************/
