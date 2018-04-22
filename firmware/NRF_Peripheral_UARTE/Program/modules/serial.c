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
#include "drivers\nrf5x_system.h"
#include "modules\serial.h"

/** @addtogroup NRF5x_Module
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static uint8_t tx_buffer[SERIAL_MAX_TXBUF] = {0};
static uint8_t rx_buffer[SERIAL_MAX_RXBUF] = {0};

UARTE_InitTypeDef hSerial = {
  .Instance            = SERIAL_UARTEx,
  .PinTXD              = SERIAL_TXD_PIN,
  .PinRXD              = SERIAL_RXD_PIN,
  .PinRTS              = SERIAL_RTS_PIN,
  .PinCTS              = SERIAL_CTS_PIN,
  .Parity              = SERIAL_PARITY,
  .HardwareFlowControl = SERIAL_HARDWARECTRL,
  .BaudRate            = SERIAL_BAUDRATE,

  .txBufLens           = SERIAL_MAX_TXBUF,
  .rxBufLens           = SERIAL_MAX_RXBUF,
  .pTxBuf              = tx_buffer,
  .pRxBuf              = rx_buffer,

  .TxEventCallback     = NULL,
  .RxEventCallback     = NULL
};

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void Serial_Config( void )
{
  /* UART Init *****************************************************************/
  UARTE_Init(&hSerial);

  /* UART IT *******************************************************************/
  if (hSerial.RxEventCallback != NULL) {
    UARTE_Set_TxBuffer(&hSerial, hSerial.pTxBuf, hSerial.txBufLens);
    UARTE_Set_RxBuffer(&hSerial, hSerial.pRxBuf, hSerial.rxBufLens);

    UARTE_InterruptCmd(&hSerial, SERIAL_INTERRUPT_MODE, ENABLE);
    NVIC_SetPriority(SERIAL_UARTEx_IRQn, SERIAL_UARTEx_IRQn_PRIORITY);
    NVIC_EnableIRQ(SERIAL_UARTEx_IRQn);
  }

  /* UART Enable ***************************************************************/
  UARTE_Cmd(&hSerial, ENABLE);
}

/**
 *  @brief  Serial_SendByte
 */
__INLINE void Serial_SendByte( uint8_t sendByte )
{
  UARTE_SendByte(&hSerial, &sendByte);
}

/**
 *  @brief  Serial_RecvByte
 */
__INLINE uint8_t Serial_RecvByte( void )
{
  uint8_t recvByte;
  UARTE_RecvByte(&hSerial, &recvByte);
  return recvByte;
}

/**
 *  @brief  Serial_SendData
 */
__INLINE uint32_t Serial_SendData( uint8_t *sendData, uint32_t lens )
{
  return UARTE_SendData(&hSerial, sendData, lens);
}

/**
 *  @brief  Serial_RecvData
 */
__INLINE uint32_t Serial_RecvData( uint8_t *recvData, uint32_t lens )
{
  return UARTE_RecvData(&hSerial, recvData, lens);
}

/**
 *  @brief  Serial_SendDataWaitTimeout
 */
__INLINE uint32_t Serial_SendDataWaitTimeout( uint8_t *sendData, uint32_t lens, uint32_t timuout )
{
  return UARTE_SendDataWaitTimeout(&hSerial, sendData, lens, timuout);
}

/**
 *  @brief  Serial_RecvDataWaitTimeout
 */
__INLINE uint32_t Serial_RecvDataWaitTimeout( uint8_t *recvData, uint32_t lens, uint32_t timeout )
{
  return UARTE_RecvDataWaitTimeout(&hSerial, recvData, lens, timeout);
}

/**
 *  @brief  fputc
 */
int fputc( int ch, FILE *f )
{
  Serial_SendByte(ch);
  return (ch);
}

/**
 *  @brief  fgetc
 */
int fgetc( FILE *f )
{
  return Serial_RecvByte();
}

/*************************************** END OF FILE ****************************************/
