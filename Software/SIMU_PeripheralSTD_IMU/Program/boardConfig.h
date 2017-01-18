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
  * @date    19-Jan-2017
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

#define NVIC_IRQn_GROUP               NVIC_PriorityGroup_4
#define SYSCFG_CLK_ENABLE()           RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

/* -------- LED and KEY */
#define LED_R_PIN                     GPIO_Pin_15
#define LED_R_GPIO_PORT               GPIOC
#define LED_R_Set()                   __GPIO_SET(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Reset()                 __GPIO_RST(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Toggle()                __GPIO_TOG(LED_R_GPIO_PORT, LED_R_PIN)

#define LED_G_PIN                     GPIO_Pin_14
#define LED_G_GPIO_PORT               GPIOC
#define LED_G_Set()                   __GPIO_SET(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Reset()                 __GPIO_RST(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Toggle()                __GPIO_TOG(LED_G_GPIO_PORT, LED_G_PIN)

#define LED_B_PIN                     GPIO_Pin_13
#define LED_B_GPIO_PORT               GPIOC
#define LED_B_Set()                   __GPIO_SET(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Reset()                 __GPIO_RST(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Toggle()                __GPIO_TOG(LED_B_GPIO_PORT, LED_B_PIN)

#define KEY_PIN                       GPIO_Pin_0
#define KEY_GPIO_PORT                 GPIOB
#define KEY_Read()                    (__GPIO_READ(KEY_GPIO_PORT, KEY_PIN) == KEY_PIN)

/* -------- UART Serial */
#define SERIAL_UARTx                  USART6
#define SERIAL_UARTx_CLK_ENABLE()     RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE)
#define SERIAL_UARTx_IRQn             USART6_IRQn
#define SERIAL_UARTx_IRQn_PREEMPT     0x000F
#define SERIAL_UARTx_IRQn_SUB         0

#define SERIAL_TX_PIN                 GPIO_Pin_6
#define SERIAL_TX_GPIO_PORT           GPIOC
#define SERIAL_TX_AF                  GPIO_AF_USART6
#define SERIAL_TX_SOURCE              GPIO_PinSource6

#define SERIAL_RX_PIN                 GPIO_Pin_7
#define SERIAL_RX_GPIO_PORT           GPIOC
#define SERIAL_RX_AF                  GPIO_AF_USART6
#define SERIAL_RX_SOURCE              GPIO_PinSource7

#define SERIAL_BAUDRATE               115200
#define SERIAL_BYTESIZE               USART_WordLength_8b
#define SERIAL_STOPBITS               USART_StopBits_1
#define SERIAL_PARITY                 USART_Parity_No
#define SERIAL_HARDWARECTRL           USART_HardwareFlowControl_None
#define SERIAL_MODE                   (USART_Mode_Rx | USART_Mode_Tx)

/* -------- Inertial Sensor */

#define IMU_SPIx                      SPI1
#define IMU_SPIx_CLK_ENABLE()         RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE)

#define IMU_SPIx_SPEED_HIGH           SPI_BaudRatePrescaler_2
#define IMU_SPIx_SPEED_LOW            SPI_BaudRatePrescaler_256

#define IMU_SCK_PIN                   GPIO_Pin_5
#define IMU_SCK_GPIO_PORT             GPIOA
#define IMU_SCK_AF                    GPIO_AF_SPI1
#define IMU_SCK_SOURCE                GPIO_PinSource5

#define IMU_SDO_PIN                   GPIO_Pin_6
#define IMU_SDO_GPIO_PORT             GPIOA
#define IMU_SDO_AF                    GPIO_AF_SPI1
#define IMU_SDO_SOURCE                GPIO_PinSource6

#define IMU_SDI_PIN                   GPIO_Pin_7
#define IMU_SDI_GPIO_PORT             GPIOA
#define IMU_SDI_AF                    GPIO_AF_SPI1
#define IMU_SDI_SOURCE                GPIO_PinSource7

#define IMU_CSM_PIN                   GPIO_Pin_4
#define IMU_CSM_GPIO_PORT             GPIOA
#define IMU_CSM_H()                   __GPIO_SET(IMU_CSM_GPIO_PORT, IMU_CSM_PIN)
#define IMU_CSM_L()                   __GPIO_RST(IMU_CSM_GPIO_PORT, IMU_CSM_PIN)

#define IMU_CSB_PIN                   GPIO_Pin_4
#define IMU_CSB_GPIO_PORT             GPIOC
#define IMU_CSB_H()                   __GPIO_SET(IMU_CSB_GPIO_PORT, IMU_CSB_PIN)
#define IMU_CSB_L()                   __GPIO_RST(IMU_CSB_GPIO_PORT, IMU_CSB_PIN)

#define IMU_INTM_PIN                  GPIO_Pin_0
#define IMU_INTM_GPIO_PORT            GPIOA
#define IMU_INTM                      EXTI0_IRQn

#define IMU_INTB_PIN                  GPIO_Pin_5
#define IMU_INTB_GPIO_PORT            GPIOC
#define IMU_INTB                      EXTI9_5_IRQn

/* Exported functions ----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
