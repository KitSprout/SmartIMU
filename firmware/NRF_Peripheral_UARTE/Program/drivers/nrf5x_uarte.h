/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_uarte.h
 *  @author  KitSprout
 *  @date    16-Jun-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __NRF5x_UARTE_H
#define __NRF5x_UARTE_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
#define UARTE_TASKS_STARTRX(__INS)      __INS->TASKS_STARTRX      // Start UART receiver
#define UARTE_TASKS_STOPRX(__INS)       __INS->TASKS_STOPRX       // Stop UART receiver
#define UARTE_TASKS_STARTTX(__INS)      __INS->TASKS_STARTTX      // Start UART transmitter
#define UARTE_TASKS_STOPTX(__INS)       __INS->TASKS_STOPTX       // Stop UART transmitter
#define UARTE_TASKS_SUSPEND(__INS)      __INS->TASKS_FLUSHRX      // Flush RX FIFO into RX buffer

#define UARTE_EVENTS_CTS(__INS)         __INS->EVENTS_CTS         // CTS is activated (set low). Clear To Send
#define UARTE_EVENTS_NCTS(__INS)        __INS->EVENTS_NCTS        // CTS is deactivated (set high). Not Clear To Send
#define UARTE_EVENTS_RXDRDY(__INS)      __INS->EVENTS_RXDRDY      // Data received in RXD.
#define UARTE_EVENTS_ENDRX(__INS)       __INS->EVENTS_ENDRX       // Receive buffer is filled up
#define UARTE_EVENTS_TXDRDY(__INS)      __INS->EVENTS_TXDRDY      // Data sent from TXD
#define UARTE_EVENTS_ENDTX(__INS)       __INS->EVENTS_ENDTX       // Last TX byte transmitted
#define UARTE_EVENTS_ERROR(__INS)       __INS->EVENTS_ERROR       // Error detected
#define UARTE_EVENTS_RXTO(__INS)        __INS->EVENTS_RXTO        // Receiver timeout
#define UARTE_EVENTS_RXSTARTED(__INS)   __INS->EVENTS_RXSTARTED   // UART receiver has started
#define UARTE_EVENTS_TXSTARTED(__INS)   __INS->EVENTS_TXSTARTED   // UART transmitter has started
#define UARTE_EVENTS_TXSTOPPED(__INS)   __INS->EVENTS_TXSTOPPED   // Transmitter stopped

/* Typedef ---------------------------------------------------------------------------------*/

typedef struct {
  NRF_UARTE_Type *Instance;

  uint8_t  PinTXD;
  uint8_t  PinRXD;
  uint8_t  PinRTS;
  uint8_t  PinCTS;
  uint32_t Parity;
  // NRF_UARTE_PARITY_EXCLUDED   - Parity bit excluded.
  // NRF_UARTE_PARITY_INCLUDED   - Parity bit included.
  uint32_t HardwareFlowControl;
  // NRF_UARTE_HWFC_DISABLED     - Hardware flow control disabled.
  // NRF_UARTE_HWFC_ENABLED      - Hardware flow control enabled.
  uint32_t BaudRate;
  // NRF_UARTE_BAUDRATE_1200     -   1200 baud.
  // NRF_UARTE_BAUDRATE_2400     -   2400 baud.
  // NRF_UARTE_BAUDRATE_4800     -   4800 baud.
  // NRF_UARTE_BAUDRATE_9600     -   9600 baud.
  // NRF_UARTE_BAUDRATE_14400    -  14400 baud.
  // NRF_UARTE_BAUDRATE_19200    -  19200 baud.
  // NRF_UARTE_BAUDRATE_28800    -  28800 baud.
  // NRF_UARTE_BAUDRATE_38400    -  38400 baud.
  // NRF_UARTE_BAUDRATE_57600    -  57600 baud.
  // NRF_UARTE_BAUDRATE_76800    -  76800 baud.
  // NRF_UARTE_BAUDRATE_115200   - 115200 baud.
  // NRF_UARTE_BAUDRATE_230400   - 230400 baud.
  // NRF_UARTE_BAUDRATE_250000   - 250000 baud.
  // NRF_UARTE_BAUDRATE_460800   - 460800 baud.
  // NRF_UARTE_BAUDRATE_921600   - 921600 baud.
  // NRF_UARTE_BAUDRATE_1000000  -     1M baud.
  const uint16_t txBufLens;
  const uint16_t rxBufLens;
  uint8_t *pTxBuf;
  uint8_t *pRxBuf;
  // UARTE BUFFER
  pFunc TxEventCallback;
  pFunc RxEventCallback;
  // UARTE EVENT CALLBACK
} UARTE_InitTypeDef;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
void      UARTE_Init( UARTE_InitTypeDef *huarte );
void      UARTE_Cmd( UARTE_InitTypeDef *huarte, uint32_t state );
void      UARTE_InterruptCmd( UARTE_InitTypeDef *huarte, uint32_t mode, uint32_t state );
void      UARTE_Start( UARTE_InitTypeDef *huarte );
void      UARTE_Stop( UARTE_InitTypeDef *huarte );
void      UARTE_Set_TxBuffer( UARTE_InitTypeDef *huarte, uint8_t *buffer, uint32_t lens );
void      UARTE_Set_RxBuffer( UARTE_InitTypeDef *huarte, uint8_t *buffer, uint32_t lens );

void      UARTE_SendByte( UARTE_InitTypeDef *huarte, uint8_t *sendByte );
void      UARTE_RecvByte( UARTE_InitTypeDef *huarte, uint8_t *recvData );
uint32_t  UARTE_SendData( UARTE_InitTypeDef *huarte, uint8_t *sendData, uint32_t lens );
uint32_t  UARTE_RecvData( UARTE_InitTypeDef *huarte, uint8_t *recvData, uint32_t lens );
uint32_t  UARTE_SendDataWaitTimeout( UARTE_InitTypeDef *huarte, uint8_t *sendData, uint32_t lens, uint32_t timeout );
uint32_t  UARTE_RecvDataWaitTimeout( UARTE_InitTypeDef *huarte, uint8_t *recvData, uint32_t lens, uint32_t timeout );


#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
