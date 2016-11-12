/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_spi.h
  * @author  KitSprout
  * @date    7-Nov-2016
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STM32F4_SPI_H
#define __STM32F4_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types --------------------------------------------------------------------------*/
typedef struct {
  uint32_t      Pin;
  GPIO_TypeDef  *Port;
  SPI_TypeDef   *Spi;
} SPIx_MultSet_t;

/* Exported constants ----------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------*/  
uint16_t SPI_RW( SPI_TypeDef *SPIx, uint16_t writeByte );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
