/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_system.h
  * @author  KitSprout
  * @date    16-Nov-2016
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STM32F4_SYSTEM_H
#define __STM32F4_SYSTEM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "boardConfig.h"
#include "stm32f4_delay.h"
#include "algorithms\mathUnit.h"

/* Exported types --------------------------------------------------------------------------*/
typedef void (*pFunc)( void );

/* Exported constants ----------------------------------------------------------------------*/
/* Exported macro --------------------------------------------------------------------------*/
#define __GPIO_SET(_PORT, _PIN)         ((_PORT)->BSRR = (_PIN))
#define __GPIO_RST(_PORT, _PIN)         ((_PORT)->BSRR = ((uint32_t)_PIN << 16U))
#define __GPIO_TOG(_PORT, _PIN)         ((_PORT)->ODR ^= (_PIN))
#define __GPIO_READ(_PORT, _PIN)        ((_PORT)->IDR & (_PIN))

#define Byte32(_BH2, _BH1, _BL2, _BL1)  (((uint8_t)(_BH2)<<24) | ((uint8_t)(_BH1)<<16) | \\
                                         ((uint8_t)(_BL2)<<8)  | ((uint8_t)(_BL1)))
#define Byte32U16H(_B32U16)             ((uint16_t)(((uint32_t)(_B32U16)&0xFFFF0000)>>16))
#define Byte32U16L(_B32U16)             ((uint16_t)( (uint32_t)(_B32U16)&0x0000FFFF))
#define Byte32U8_4(_B32U8)              ((uint8_t)(((uint32_t)(_B32U8)&0xFF000000)>>24))
#define Byte32U8_3(_B32U8)              ((uint8_t)(((uint32_t)(_B32U8)&0x00FF0000)>>16))
#define Byte32U8_2(_B32U8)              ((uint8_t)(((uint32_t)(_B32U8)&0x0000FF00)>>8))
#define Byte32U8_1(_B32U8)              ((uint8_t)( (uint32_t)(_B32U8)&0x000000FF))

#define Byte16(_BH, _BL)                ((((uint16_t)(_BH))<<8) | ((uint16_t)(_BL)))
#define Byte16U8H(_B16U8)               ((uint8_t)(((uint16_t)(_B16U8)&0xFF00)>>8))
#define Byte16U8L(_B16U8)               ((uint8_t)( (uint16_t)(_B16U8)&0x00FF))

/* Exported functions ----------------------------------------------------------------------*/  

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
