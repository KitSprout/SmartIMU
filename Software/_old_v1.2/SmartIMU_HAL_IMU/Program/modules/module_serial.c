/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_usart.h"
#include "algorithms\algorithm_string.h"

#include "module_serial.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define UARTx                       USART1
#define UARTx_CLK                   RCC_APB2Periph_USART1
#define UARTx_CLK_ENABLE()          __HAL_RCC_USART1_CLK_ENABLE()

#define UARTx_TX_PIN                GPIO_PIN_6
#define UARTx_TX_GPIO_PORT          GPIOB
#define UARTx_TX_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define UARTx_TX_AF                 GPIO_AF7_USART1

#define UARTx_RX_PIN                GPIO_PIN_7
#define UARTx_RX_GPIO_PORT          GPIOB
#define UARTx_RX_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define UARTx_RX_AF                 GPIO_AF7_USART1

#define UARTx_BAUDRATE              115200
#define UARTx_BYTESIZE              UART_WORDLENGTH_8B
#define UARTx_STOPBITS              UART_STOPBITS_1
#define UARTx_PARITY                UART_PARITY_NONE
#define UARTx_HARDWARECTRL          UART_HWCONTROL_NONE
#define UARTx_MODE                  UART_MODE_TX_RX
#define UARTx_OVERSAMPLE            UART_OVERSAMPLING_16
/*====================================================================================================*/
/*====================================================================================================*/
static UART_HandleTypeDef Serial_HandleStruct;
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Serial_Config
**功能 : Serial Config
**輸入 : None
**輸出 : None
**使用 : Serial_Config();
**====================================================================================================*/
/*====================================================================================================*/
void Serial_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* UART Clk ******************************************************************/
  UARTx_TX_GPIO_CLK_ENABLE();
  UARTx_RX_GPIO_CLK_ENABLE();
  UARTx_CLK_ENABLE();

  /* UART Pin ******************************************************************/
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin       = UARTx_TX_PIN;
  GPIO_InitStruct.Alternate = UARTx_TX_AF;
  HAL_GPIO_Init(UARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin       = UARTx_RX_PIN;
  GPIO_InitStruct.Alternate = UARTx_RX_AF;
  HAL_GPIO_Init(UARTx_RX_GPIO_PORT, &GPIO_InitStruct);

  /* UART Init *****************************************************************/
  Serial_HandleStruct.Instance          = UARTx;
  Serial_HandleStruct.Init.BaudRate     = UARTx_BAUDRATE;
  Serial_HandleStruct.Init.WordLength   = UARTx_BYTESIZE;
  Serial_HandleStruct.Init.StopBits     = UARTx_STOPBITS;
  Serial_HandleStruct.Init.Parity       = UARTx_PARITY;
  Serial_HandleStruct.Init.HwFlowCtl    = UARTx_HARDWARECTRL;
  Serial_HandleStruct.Init.Mode         = UARTx_MODE;
  Serial_HandleStruct.Init.OverSampling = UARTx_OVERSAMPLE;
  HAL_UART_Init(&Serial_HandleStruct);

  /* UART Enable ***************************************************************/
  __HAL_UART_ENABLE(&Serial_HandleStruct);
  __HAL_UART_CLEAR_FLAG(&Serial_HandleStruct, UART_FLAG_TC);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Serial_SendByte
**功能 : Send Byte
**輸入 : SendByte
**輸出 : None
**使用 : Serial_SendByte('A');
**====================================================================================================*/
/*====================================================================================================*/
void Serial_SendByte( uint8_t sendByte )
{
  UART_SendByte(UARTx, &sendByte);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Serial_RecvByte
**功能 : Recv Byte
**輸入 : None
**輸出 : RecvByte
**使用 : RecvByte = Serial_RecvByte();
**====================================================================================================*/
/*====================================================================================================*/
uint8_t Serial_RecvByte( void )
{
  uint8_t recvByte = 0;
  UART_RecvByte(UARTx, &recvByte);
  return recvByte;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Serial_SendData
**功能 : Send Bytes
**輸入 : *SendData, DataLen
**輸出 : None
**使用 : Serial_SendData(sendData, dataLen);
**====================================================================================================*/
/*====================================================================================================*/
void Serial_SendData( uint8_t *sendData, uint16_t lens )
{
  UART_SendData(UARTx, sendData, lens);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Serial_RecvData
**功能 : Recv Bytes
**輸入 : *RecvData, DataLen
**輸出 : None
**使用 : Serial_RecvData(recvData, dataLen);
**====================================================================================================*/
/*====================================================================================================*/
void Serial_RecvData( uint8_t *recvData, uint16_t lens )
{
  UART_RecvData(UARTx, recvData, lens);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Serial_RecvDataWTO
**功能 : Recv Bytes with Timeout
**輸入 : *RecvData, DataLen, TimeoutMs
**輸出 : State
**使用 : Serial_RecvDataWTO(recvData, dataLen, 200);
**====================================================================================================*/
/*====================================================================================================*/
int8_t Serial_RecvDataWTO( uint8_t *recvData, uint16_t lens, uint32_t timeoutMs )
{
  return UART_RecvDataWTO(UARTx, recvData, lens, timeoutMs);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Serial_SendStr
**功能 : Send String
**輸入 : *pWord
**輸出 : None
**使用 : Serial_SendStr("Hello World!\r\n");
**====================================================================================================*/
/*====================================================================================================*/
void Serial_SendStr( char *pWord )
{
  do {
    UART_SendByte(UARTx, (uint8_t*)pWord++);
  } while(*pWord != '\0');
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Serial_RecvStr
**功能 : Recv String
**輸入 : *pWord
**輸出 : None
**使用 : Serial_RecvStr(recvStirng);
**====================================================================================================*/
/*====================================================================================================*/
void Serial_RecvStr( char *pWord )
{
  do {
    UART_RecvByte(UARTx, (uint8_t*)pWord++);
  } while(*(pWord-1) != '\0');
  *pWord = '\0';
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Serial_RecvStrWTO
**功能 : Recv String with Timeout
**輸入 : *pWord, TimeoutMs
**輸出 : State
**使用 : Serial_RecvStrWTO(recvStirng, 200);
**====================================================================================================*/
/*====================================================================================================*/
int8_t Serial_RecvStrWTO( char *pWord, uint32_t timeoutMs )
{
  int8_t State = ERROR;

  do {
    State = UART_RecvByteWTO(UARTx, (uint8_t*)pWord++, timeoutMs);
    if(State == ERROR)
      return ERROR;
  } while(*(pWord-1) != '\0');
  *pWord = '\0';

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Serial_SendNum
**功能 : 將數值轉字串發送
**輸入 : Type, NumLen, SendData
**輸出 : None
**使用 : Serial_SendNum(Type_O, 6, 1024);
**====================================================================================================*/
/*====================================================================================================*/
void Serial_SendNum( StrType type, uint8_t lens, int32_t sendData )
{
  char covString[32] = {0};
  char *pWord = covString;

  Str_NumToChar(type, lens, covString, sendData);

  do {
    UART_SendByte(UARTx, (uint8_t*)pWord++);
  } while(*pWord != '\0');
}
/*====================================================================================================*/
/*====================================================================================================*/
int fputc( int ch, FILE *f )
{
  UARTx->DR = ((uint8_t)ch & (uint16_t)0x00FF);
  while(!(USART1->SR & UART_FLAG_TXE));
  return (ch);
}
/*====================================================================================================*/
/*====================================================================================================*/
