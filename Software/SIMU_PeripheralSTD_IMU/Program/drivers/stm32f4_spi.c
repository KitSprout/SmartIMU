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
  * @date    19-Jan-2016
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
  * @brief  SPI_SetSpeed
  */
void SPI_SetSpeed( SPI_TypeDef *SPIx, SPI_InitTypeDef *pSPI_InitStruct, uint8_t speedSel )
{
  SPI_I2S_DeInit(SPIx);
  pSPI_InitStruct->SPI_BaudRatePrescaler = speedSel;
  SPI_Init(SPIx, pSPI_InitStruct);
  SPI_Cmd(SPIx, ENABLE);
}

/**
  * @brief  SPI_RW
  */
uint16_t SPI_RW( SPI_TypeDef *SPIx, uint16_t sendByte )
{
  while ((SPIx->SR & SPI_FLAG_TXE) == (uint16_t)RESET);
  SPIx->DR = sendByte;
  while ((SPIx->SR & SPI_FLAG_RXNE) == (uint16_t)RESET);
  return SPIx->DR;
}

/**
  * @brief  SPI_SendDataWTO
  */
int8_t SPI_SendDataWTO( SPI_TypeDef *SPIx, uint8_t *sendData, uint16_t lens, uint32_t timeout )
{
  uint32_t tickStart = 0;

  if ((sendData == NULL) || (lens == 0)) {
    return HAL_ERROR;
  }

  while (lens--) {
    tickStart = HAL_GetTick();
    while ((SPIx->SR & SPI_FLAG_TXE) == RESET) {
      if ((HAL_GetTick() - tickStart) > timeout) {
        return HAL_TIMEOUT;
      }
    }
    SPIx->DR = *sendData++;
  }

  return HAL_OK;
}

/**
  * @brief  SPI_RecvDataWTO
  */
int8_t SPI_RecvDataWTO( SPI_TypeDef *SPIx, uint8_t *recvData, uint16_t lens, uint32_t timeout )
{
  uint32_t tickStart = 0;

  if ((recvData == NULL) || (lens == 0)) {
    return HAL_ERROR;
  }

  while (lens--) {
    tickStart = HAL_GetTick();
    while ((SPIx->SR & SPI_FLAG_RXNE) == RESET) {
      if ((HAL_GetTick() - tickStart) > timeout) {
        return HAL_TIMEOUT;
      }
    }
    *recvData++ = (SPIx->DR);
  }

  return HAL_OK;
}

/*************************************** END OF FILE ****************************************/
