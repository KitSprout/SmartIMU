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
  * @date    6-Oct-2016
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
#define UARTx                 USART1
#define UARTx_CLK_ENABLE()    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE)
#define UARTx_IRQn            USART1_IRQn

#define UARTx_TX_PIN          GPIO_Pin_6
#define UARTx_TX_GPIO_PORT    GPIOB
#define UARTx_TX_AF           GPIO_AF_USART1
#define UARTx_TX_SOURCE       GPIO_PinSource6

#define UARTx_RX_PIN          GPIO_Pin_7
#define UARTx_RX_GPIO_PORT    GPIOB
#define UARTx_RX_AF           GPIO_AF_USART1
#define UARTx_RX_SOURCE       GPIO_PinSource7

#define UARTx_BAUDRATE        115200
#define UARTx_BYTESIZE        USART_WordLength_8b
#define UARTx_STOPBITS        USART_StopBits_1
#define UARTx_PARITY          USART_Parity_No
#define UARTx_HARDWARECTRL    USART_HardwareFlowControl_None
#define UARTx_MODE            (USART_Mode_Rx | USART_Mode_Tx)

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void Serial_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  USART_InitTypeDef UART_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  /* UART Clk ******************************************************************/
  UARTx_CLK_ENABLE();

  /* UART AF *******************************************************************/
  GPIO_PinAFConfig(UARTx_TX_GPIO_PORT, UARTx_TX_SOURCE, UARTx_TX_AF);
  GPIO_PinAFConfig(UARTx_RX_GPIO_PORT, UARTx_RX_SOURCE, UARTx_RX_AF);

  /* UART Pin ******************************************************************/
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = UARTx_TX_PIN;
  GPIO_Init(UARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = UARTx_RX_PIN;
  GPIO_Init(UARTx_RX_GPIO_PORT, &GPIO_InitStruct);

  /* UART IT *******************************************************************/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  NVIC_InitStruct.NVIC_IRQChannel                   = UARTx_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x000F;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

  /* UART Init *****************************************************************/
  UART_InitStruct.USART_BaudRate            = UARTx_BAUDRATE;
  UART_InitStruct.USART_WordLength          = UARTx_BYTESIZE;
  UART_InitStruct.USART_StopBits            = UARTx_STOPBITS;
  UART_InitStruct.USART_Parity              = UARTx_PARITY;
  UART_InitStruct.USART_HardwareFlowControl = UARTx_HARDWARECTRL;
  UART_InitStruct.USART_Mode                = UARTx_MODE;
  USART_Init(UARTx, &UART_InitStruct);

  /* UART Enable ***************************************************************/
//  USART_ITConfig(UARTx, USART_IT_RXNE, ENABLE);
  USART_Cmd(UARTx, ENABLE);
  USART_ClearFlag(UARTx, USART_FLAG_TC);
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
  while (!(UARTx->SR & USART_FLAG_TXE));
  return (ch);
}

int fgetc( FILE *f )
{
  while (!(UARTx->SR & USART_FLAG_RXNE));
  return (uint16_t)(UARTx->DR & (uint16_t)0x01FF);
}

/*************************************** END OF FILE ****************************************/
