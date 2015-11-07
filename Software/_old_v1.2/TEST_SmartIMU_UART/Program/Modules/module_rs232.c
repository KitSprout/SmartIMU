/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
#include "module_rs232.h"
#include "algorithm_string.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define USARTx                USART2
#define USARTx_CLK            RCC_APB1Periph_USART2

#define USARTx_TX_PIN         GPIO_Pin_2
#define USARTx_TX_GPIO_PORT   GPIOA
#define USARTx_TX_GPIO_CLK    RCC_AHB1Periph_GPIOA
#define USARTx_TX_SOURCE      GPIO_PinSource2
#define USARTx_TX_AF          GPIO_AF_USART2

#define USARTx_RX_PIN         GPIO_Pin_3
#define USARTx_RX_GPIO_PORT   GPIOA
#define USARTx_RX_GPIO_CLK    RCC_AHB1Periph_GPIOA
#define USARTx_RX_SOURCE      GPIO_PinSource3
#define USARTx_RX_AF          GPIO_AF_USART2

#define USARTx_BAUDRATE       115200
#define USARTx_BYTESIZE       USART_WordLength_8b
#define USARTx_STOPBITS       USART_StopBits_1
#define USARTx_PARITY         USART_Parity_No
#define USARTx_HARDWARECTRL   USART_HardwareFlowControl_None
/*====================================================================================================*/
/*====================================================================================================*
**函數 : RS232_Config
**功能 : RS232 配置
**輸入 : None
**輸出 : None
**使用 : RS232_Config();
**====================================================================================================*/
/*====================================================================================================*/
void RS232_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  USART_InitTypeDef USART_InitStruct;

  /* UART Clk Init *************************************************************/
  RCC_AHB1PeriphClockCmd(USARTx_TX_GPIO_CLK | USARTx_RX_GPIO_CLK, ENABLE);
  RCC_APB1PeriphClockCmd(USARTx_CLK, ENABLE);

  GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);
  GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);

  /* USARTx Tx PA2 */
  GPIO_InitStruct.GPIO_Pin = USARTx_TX_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
  /* USARTx Rx PA3 */
  GPIO_InitStruct.GPIO_Pin = USARTx_RX_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

  /* UART Init *****************************************************************/
  USART_InitStruct.USART_BaudRate = USARTx_BAUDRATE;
  USART_InitStruct.USART_WordLength = USARTx_BYTESIZE;
  USART_InitStruct.USART_StopBits = USARTx_STOPBITS;
  USART_InitStruct.USART_Parity = USARTx_PARITY;
  USART_InitStruct.USART_HardwareFlowControl = USARTx_HARDWARECTRL;
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USARTx, &USART_InitStruct);
  USART_Cmd(USARTx, ENABLE);

  USART_ClearFlag(USARTx, USART_FLAG_TC);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : RS232_SendStr
**功能 : 發送字串
**輸入 : *pWord
**輸出 : None
**使用 : RS232_SendStr((u8*)"Hellow World!");
**====================================================================================================*/
/*====================================================================================================*/
void RS232_SendStr( u8 *pWord )
{
  while(*pWord != '\0') {
    UART_SendByte(USARTx, *pWord);
    pWord++;
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : RS232_SendNum
**功能 : 將數值轉字串發送
**輸入 : Type, NumLen, SendData
**輸出 : None
**使用 : RS232_SendNum(Type_O, 6, 1024);
**====================================================================================================*/
/*====================================================================================================*/
void RS232_SendNum( u8 Type, u8 NumLen, s32 SendData )
{
  u8 TrData[32] = {0};
  u8 *pWord = TrData;

  Str_NumToChar(Type, NumLen, TrData, SendData);

  while(*pWord != '\0') {
    UART_SendByte(USARTx, *pWord);
    pWord++;
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : RS232_SendData
**功能 : 發送資料
**輸入 : *SendData, DataLen
**輸出 : None
**使用 : RS232_SendData(SendData, DataLen);
**====================================================================================================*/
/*====================================================================================================*/
void RS232_SendData( u8 *SendData, u16 DataLen )
{
  UART_SendData(USARTx, SendData, DataLen);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : RS232_RecvStr
**功能 : 接收字串
**輸入 : *pWord
**輸出 : None
**使用 : RS232_RecvStr(RecvStirng);
**====================================================================================================*/
/*====================================================================================================*/
void RS232_RecvStr( u8 *pWord )
{
  do {
    *pWord = UART_RecvByte(USARTx);
    pWord++;
  } while(*(pWord-1) != '\0');
  *pWord = '\0';
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : RS232_RecvData
**功能 : 接收資料
**輸入 : *RecvData, DataLen
**輸出 : None
**使用 : RS232_RecvData(RecvData, DataLen);
**====================================================================================================*/
/*====================================================================================================*/
void RS232_RecvData( u8 *RecvData, u16 DataLen )
{
  UART_RecvData(USARTx, RecvData, DataLen);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : RS232_VisualScope_CRC16
**功能 : VisualScope CRC
**輸入 : *SendData, Len
**輸出 : UART_CRC
**使用 : UART_CRC = RS232_VisualScope_CRC16(SendData, DataLen);
**====================================================================================================*/
/*====================================================================================================*/
static u16 RS232_VisualScope_CRC16( u8 *SendData, u8 DataLen )
{
  u16 UART_IX, UART_IY, UART_CRC;

  UART_CRC = 0xffff;
  for(UART_IX=0; UART_IX<DataLen; UART_IX++) {
    UART_CRC = UART_CRC^(u16)(SendData[UART_IX]);
    for(UART_IY=0; UART_IY<=7; UART_IY++) {
      if((UART_CRC&0x01)!=0x00)
        UART_CRC = (UART_CRC>>1)^0xA001;
      else
        UART_CRC = UART_CRC>>1;
    }
  }
  return(UART_CRC);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : RS232_VisualScope
**功能 : VisualScope
**輸入 : *SendBuf
**輸出 : None
**使用 : RS232_VisualScope(SendBuf);
**====================================================================================================*/
/*====================================================================================================*/
void RS232_VisualScope( u8 *SendBuf )
{
  u8 SendCRC[2] = {0};
  u16 UART_CRC;

  UART_CRC = RS232_VisualScope_CRC16(SendBuf, 8);
  SendCRC[0] = UART_CRC&0x00ff;
  SendCRC[1] = (UART_CRC&0xff00)>>8;

  UART_SendData(USARTx, SendBuf, 8);
  UART_SendData(USARTx, SendCRC, 2);
}
/*====================================================================================================*/
/*====================================================================================================*/
