/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_spi.c
  * @author  KitSprout
  * @date    11-Oct-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4_system.h"
#include "stm32f4_spi.h"

/** @addtogroup STM32_Driver
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  SPI_RW
  * @param  SPIx: 
  * @param  writeByte: 
  * @retval read data
  */
uint16_t SPI_RW( SPI_TypeDef *SPIx, uint16_t writeByte )
{
  while ((SPIx->SR & SPI_FLAG_TXE) == (uint16_t)RESET);
  SPIx->DR = writeByte;
  while ((SPIx->SR & SPI_FLAG_RXNE) == (uint16_t)RESET);

  return SPIx->DR;
}

/*************************************** END OF FILE ****************************************/
