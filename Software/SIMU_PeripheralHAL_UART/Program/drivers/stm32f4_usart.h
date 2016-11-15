/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_usart.h
  * @author  KitSprout
  * @date    15-Nov-2016
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STM32F4_USART_H
#define __STM32F4_USART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types --------------------------------------------------------------------------*/
/* Exported constants ----------------------------------------------------------------------*/

/* Serial */
#define SERIAL_UARTx                  USART6
#define SERIAL_UARTx_CLK_ENABLE()     __HAL_RCC_USART6_CLK_ENABLE()
#define SERIAL_UARTx_IRQn             USART6_IRQn

#define SERIAL_UARTx_FORCE_RESET()    __HAL_RCC_USART6_FORCE_RESET()
#define SERIAL_UARTx_RELEASE_RESET()  __HAL_RCC_USART6_RELEASE_RESET()

#define SERIAL_TX_PIN                 GPIO_PIN_6
#define SERIAL_TX_GPIO_PORT           GPIOC
#define SERIAL_TX_AF                  GPIO_AF8_USART6

#define SERIAL_RX_PIN                 GPIO_PIN_7
#define SERIAL_RX_GPIO_PORT           GPIOC
#define SERIAL_RX_AF                  GPIO_AF8_USART6

#define SERIAL_BAUDRATE               115200
#define SERIAL_BYTESIZE               UART_WORDLENGTH_8B
#define SERIAL_STOPBITS               UART_STOPBITS_1
#define SERIAL_PARITY                 UART_PARITY_NONE
#define SERIAL_HARDWARECTRL           UART_HWCONTROL_NONE
#define SERIAL_MODE                   UART_MODE_TX_RX
#define SERIAL_OVERSAMPLE             UART_OVERSAMPLING_16

/* Exported functions ----------------------------------------------------------------------*/  
int8_t UART_SendData( UART_HandleTypeDef *huart, uint8_t *sendData, uint16_t lens, uint32_t timeout );
int8_t UART_RecvData( UART_HandleTypeDef *huart, uint8_t *recvData, uint16_t lens, uint32_t timeout );
int8_t UART_SendDataIT( UART_HandleTypeDef *huart, uint8_t *sendData, uint16_t lens );
int8_t UART_RecvDataIT( UART_HandleTypeDef *huart, uint8_t *recvData, uint16_t lens );
int8_t UART_SendDataDMA( UART_HandleTypeDef *huart, uint8_t *sendData, uint16_t lens );
int8_t UART_RecvDataDMA( UART_HandleTypeDef *huart, uint8_t *recvData, uint16_t lens );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
