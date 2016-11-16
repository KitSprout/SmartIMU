/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    boardConfig.h
  * @author  KitSprout
  * @date    16-Nov-2016
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __BOARDCONFIG_H
#define __BOARDCONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
/* Exported types --------------------------------------------------------------------------*/
/* Exported constants ----------------------------------------------------------------------*/

#define KS_HW_BOARD_NAME              "SmartIMU"
#define KS_HW_MCU_NAME                "STM32F412Rx"

#define KS_HW_CLOCK_SOUCE_HSE
//#define KS_HW_USE_CLOCK_SOUCE_HSI

//#define KS_HW_UART_HAL_LIBRARY
//#define KS_HW_SPI_HAL_LIBRARY
//#define KS_HW_I2C_HAL_LIBRARY

/* -------- LED and KEY */
#define LED_R_PIN                     GPIO_PIN_15
#define LED_R_GPIO_PORT               GPIOC
#define LED_R_Set()                   __GPIO_SET(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Reset()                 __GPIO_RST(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Toggle()                __GPIO_TOG(LED_R_GPIO_PORT, LED_R_PIN)

#define LED_G_PIN                     GPIO_PIN_14
#define LED_G_GPIO_PORT               GPIOC
#define LED_G_Set()                   __GPIO_SET(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Reset()                 __GPIO_RST(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Toggle()                __GPIO_TOG(LED_G_GPIO_PORT, LED_G_PIN)

#define LED_B_PIN                     GPIO_PIN_13
#define LED_B_GPIO_PORT               GPIOC
#define LED_B_Set()                   __GPIO_SET(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Reset()                 __GPIO_RST(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Toggle()                __GPIO_TOG(LED_B_GPIO_PORT, LED_B_PIN)

#define KEY_PIN                       GPIO_PIN_0
#define KEY_GPIO_PORT                 GPIOB
#define KEY_Read()                    (__GPIO_READ(KEY_GPIO_PORT, KEY_PIN) == KEY_PIN)

/* -------- ADC Battery */
#define BAT_MAX_CONVBUF               1

#define BAT_ADCx                      ADC1
#define BAT_ADCx_CLK_ENABLE()         __HAL_RCC_ADC1_CLK_ENABLE()
#define BAT_DMAx_CLK_ENABLE()         __HAL_RCC_DMA2_CLK_ENABLE()

#define BAT_ADCx_FORCE_RESET()        __HAL_RCC_ADC_FORCE_RESET()
#define BAT_ADCx_RELEASE_RESET()      __HAL_RCC_ADC_RELEASE_RESET()

#define BAT_ADCx_RANK                 1
#define BAT_ADCx_CHANNEL              ADC_CHANNEL_9
#define BAT_ADCx_CHANNEL_PIN          GPIO_PIN_1
#define BAT_ADCx_CHANNEL_GPIO_PORT    GPIOB

#define BAT_ADCx_DMA_CHANNEL          DMA_CHANNEL_0
#define BAT_ADCx_DMA_STREAM           DMA2_Stream0
#define BAT_ADCx_DMA_IRQn             DMA2_Stream0_IRQn

/* -------- UART Serial */
#define SERIAL_MAX_TXBUF              16
#define SERIAL_MAX_RXBUF              16

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

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
