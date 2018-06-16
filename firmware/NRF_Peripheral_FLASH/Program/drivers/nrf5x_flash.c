/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_flash.c
 *  @author  KitSprout
 *  @date    16-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_flash.h"

/** @addtogroup NRF5x_Driver
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  FLASH_WriteDataU8
 */
void FLASH_WriteDataU8( const uint32_t writeAddr, const uint8_t *writeData, uint32_t lens )
{
  uint32_t count = 0;

  // write enable
  NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos);
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}

  // write data
  while (count < lens) {
    *(__IO uint32_t*)(writeAddr + count) = BYTE32(writeData[count+3], writeData[count+2], writeData[count+1], writeData[count]);
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}
    count += 4;
  }

  // read only enable
  NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos);
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}
}

/**
 *  @brief  FLASH_WriteDataU16
 */
void FLASH_WriteDataU16( const uint32_t writeAddr, const uint16_t *writeData, uint32_t lens )
{
  uint32_t count = 0;

  // write enable
  NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos);
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}

  // write data
  while (count < lens) {
    *(__IO uint32_t*)(writeAddr + (count << 1)) = ((uint32_t)writeData[count+1] << 16) | writeData[count];
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}
    count += 2;
  }

  // read only enable
  NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos);
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}
}

/**
 *  @brief  FLASH_WriteDataU32
 */
void FLASH_WriteDataU32( const uint32_t writeAddr, const uint32_t *writeData, uint32_t lens )
{
  uint32_t count = 0;

  // write enable
  NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos);
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}

  // write data
  do {
    *(__IO uint32_t*)(writeAddr + (count << 2)) = writeData[count];
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}
  } while (++count < lens);

  // read only enable
  NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos);
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}
}

/**
 *  @brief  FLASH_ReadDataU8
 */
void FLASH_ReadDataU8( const uint32_t readAddr, uint8_t *readData, const uint32_t lens )
{
  uint32_t count = 0;
  uint32_t data = 0;

  while (count < lens) {
    data = (uint32_t)(*(__IO uint32_t*)(readAddr + count));
    readData[count]   = BYTE32U8_1(data);
    readData[count+1] = BYTE32U8_2(data);
    readData[count+2] = BYTE32U8_3(data);
    readData[count+3] = BYTE32U8_4(data);
    count += 4;
  }
}

/**
 *  @brief  FLASH_ReadDataU16
 */
void FLASH_ReadDataU16( const uint32_t readAddr, uint16_t *readData, const uint32_t lens )
{
  uint32_t count = 0;
  uint32_t data = 0;

  while (count < lens) {
    data = (uint32_t)(*(__IO uint32_t*)(readAddr + (count << 1)));
    readData[count]   = BYTE32U16L(data);
    readData[count+1] = BYTE32U16H(data);
    count += 2;
  }
}

/**
 *  @brief  FLASH_ReadDataU32
 */
void FLASH_ReadDataU32( const uint32_t readAddr, uint32_t *readData, const uint32_t lens )
{
  uint32_t count = 0;

  do {
    readData[count] = (uint32_t)(*(__IO uint32_t*)(readAddr + (count << 2)));
  } while (++count < lens);
}

/**
 *  @brief  FLASH_ErasePage
 */
void FLASH_ErasePage( uint32_t erasePageAddr )
{
  // erase enable
  NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Een << NVMC_CONFIG_WEN_Pos);
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}

  // erase page
  NRF_NVMC->ERASEPAGE = erasePageAddr;
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}

  // read only enable
  NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos);
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}
}

/*************************************** END OF FILE ****************************************/
