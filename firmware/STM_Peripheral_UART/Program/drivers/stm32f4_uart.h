/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32f4_uart.h
 *  @author  KitSprout
 *  @date    22-Apr-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STM32F4_UART_H
#define __STM32F4_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
typedef struct {
  UART_HandleTypeDef *handle;
  const uint16_t txBufLens;
  const uint16_t rxBufLens;
  uint8_t *pTxBuf;
  uint8_t *pRxBuf;
  pFunc txEventCallback;
  pFunc rxEventCallback;
} __attribute__((aligned)) UartHandle_st;

/* Variables -------------------------------------------------------------------------------*/
extern UartHandle_st hSerial;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

#if KS_FW_UART_HAL_LIBRARY

#define UART_SendByte( __H, __DAT )                 HAL_UART_Transmit(__H, __DAT, 1, HAL_MAX_DELAY)
#define UART_RecvByte( __H, __DAT )                 HAL_UART_Receive(__H, __DAT, 1, HAL_MAX_DELAY)
#define UART_SendData( __H, __DAT, __LEN, __TIME )  HAL_UART_Transmit(__H, __DAT, __LEN, __TIME)
#define UART_RecvData( __H, __DAT, __LEN, __TIME )  HAL_UART_Receive(__H, __DAT, __LEN, __TIME)
#define UART_SendDataIT( __H, __DAT, __LEN )        HAL_UART_Transmit_IT(__H, __DAT, __LEN)
#define UART_RecvDataIT( __H, __DAT, __LEN )        HAL_UART_Receive_IT(__H, __DAT, __LEN)
#define UART_SendDataDMA( __H, __DAT, __LEN )       HAL_UART_Transmit_DMA(__H, __DAT, __LEN)
#define UART_RecvDataDMA( __H, __DAT, __LEN )       HAL_UART_Receive_DMA(__H, __DAT, __LEN)

#else
void      UART_SendByte( UART_HandleTypeDef *huart, uint8_t *sendByte );
void      UART_RecvByte( UART_HandleTypeDef *huart, uint8_t *recvByte );
uint32_t  UART_RecvWait( UART_HandleTypeDef *huart, uint8_t *recvData );
uint32_t  UART_SendData( UART_HandleTypeDef *huart, uint8_t *sendData, uint16_t lens, uint32_t timeout );
uint32_t  UART_RecvData( UART_HandleTypeDef *huart, uint8_t *recvData, uint16_t lens, uint32_t timeout );
//uint32_t  UART_SendDataIT( UART_HandleTypeDef *huart, uint8_t *sendData, uint16_t lens );
//uint32_t  UART_RecvDataIT( UART_HandleTypeDef *huart, uint8_t *recvData, uint16_t lens );
//uint32_t  UART_SendDataDMA( UART_HandleTypeDef *huart, uint8_t *sendData, uint16_t lens );
//uint32_t  UART_RecvDataDMA( UART_HandleTypeDef *huart, uint8_t *recvData, uint16_t lens );

#endif

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
