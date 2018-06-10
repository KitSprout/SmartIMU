/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32f4_flash.c
 *  @author  KitSprout
 *  @date    24-Apr-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_flash.h"

/** @addtogroup STM32_Driver
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  FLASH_WritePageU8
 */
uint32_t FLASH_WritePageU8( uint32_t writeAddr, const uint8_t *writeData, uint32_t lens )
{
  uint32_t status;
  uint32_t count = 0;

  HAL_FLASH_Unlock();
  do {
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, writeAddr + count, writeData[count]);
  } while ((++count < lens) && (status == HAL_OK));
  HAL_FLASH_Lock();

  return status;
}

/**
 *  @brief  FLASH_WritePageU16
 */
uint32_t FLASH_WritePageU16( uint32_t writeAddr, const uint16_t *writeData, uint32_t lens )
{
  uint32_t status;
  uint32_t count = 0;

  HAL_FLASH_Unlock();
  do {
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, writeAddr + (count << 1), writeData[count]);
  } while ((++count < lens) && (status == HAL_OK));
  HAL_FLASH_Lock();

  return status;
}

/**
 *  @brief  FLASH_WritePageU32
 */
uint32_t FLASH_WritePageU32( uint32_t writeAddr, const uint32_t *writeData, uint32_t lens )
{
  uint32_t status;
  uint32_t count = 0;

  HAL_FLASH_Unlock();
  do {
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, writeAddr + (count << 2), writeData[count]);
  } while((++count < lens) && (status == HAL_OK));
  HAL_FLASH_Lock();

  return status;
}

/**
 *  @brief  FLASH_WritePageU64
 */
uint32_t FLASH_WritePageU64( uint32_t writeAddr, const uint64_t *writeData, uint32_t lens )
{
  uint32_t status;
  uint32_t count = 0;

  HAL_FLASH_Unlock();
  do {
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, writeAddr + (count << 3), writeData[count]);
  } while ((++count < lens) && (status == HAL_OK));
  HAL_FLASH_Lock();

  return status;
}

/**
 *  @brief  FLASH_ReadPageU8
 */
void FLASH_ReadPageU8( uint32_t readAddr, uint8_t *readData, uint32_t lens )
{
  uint32_t count = 0;

  do {
    readData[count] = (uint8_t)(*(__IO uint32_t*)(readAddr + count));
  } while (++count < lens);
}

/**
 *  @brief  FLASH_ReadPageU16
 */
void FLASH_ReadPageU16( uint32_t readAddr, uint16_t *readData, uint32_t lens )
{
  uint32_t count = 0;

  do {
    readData[count] = (uint16_t)(*(__IO uint32_t*)(readAddr + (count << 1)));
  } while (++count < lens);
}

/**
 *  @brief  FLASH_ReadPageU32
 */
void FLASH_ReadPageU32( uint32_t readAddr, uint32_t *readData, uint32_t lens )
{
  uint32_t count = 0;

  do {
    readData[count] = (uint32_t)(*(__IO uint32_t*)(readAddr + (count << 2)));
  } while (++count < lens);
}

/**
 *  @brief  FLASH_ReadPageU64
 */
void FLASH_ReadPageU64( uint32_t readAddr, uint64_t *readData, uint32_t lens )
{
  uint32_t count = 0;

  do {
    readData[count] = (uint64_t)(*(__IO uint32_t*)(readAddr + (count << 3)));
  } while (++count < lens);
}

/**
 *  @brief  FLASH_GetSector
 */
uint32_t FLASH_GetSector( uint32_t address )
{
  uint32_t sector = 0;

  if     ((address >= FLASH_ADDR_SECTOR_0) && (address < FLASH_ADDR_SECTOR_1))      sector = FLASH_SECTOR_0;
  else if((address >= FLASH_ADDR_SECTOR_1) && (address < FLASH_ADDR_SECTOR_2))      sector = FLASH_SECTOR_1;
  else if((address >= FLASH_ADDR_SECTOR_2) && (address < FLASH_ADDR_SECTOR_3))      sector = FLASH_SECTOR_2;
  else if((address >= FLASH_ADDR_SECTOR_3) && (address < FLASH_ADDR_SECTOR_4))      sector = FLASH_SECTOR_3;
  else if((address >= FLASH_ADDR_SECTOR_4) && (address < FLASH_ADDR_SECTOR_5))      sector = FLASH_SECTOR_4;
  else if((address >= FLASH_ADDR_SECTOR_5) && (address < FLASH_ADDR_SECTOR_6))      sector = FLASH_SECTOR_5;
  else if((address >= FLASH_ADDR_SECTOR_6) && (address < FLASH_ADDR_SECTOR_7))      sector = FLASH_SECTOR_6;
  else /*((address >= FLASH_ADDR_SECTOR_7) && (address < FLASH_ADDR_SECTOR_END)) */ sector = FLASH_SECTOR_7;

  return sector;
}

/**
 *  @brief  FLASH_EraseSectors
 */
uint32_t FLASH_EraseSectors( uint32_t eraseSector, uint32_t lens )
{
  FLASH_EraseInitTypeDef EraseInitStruct;

  uint32_t status;
  uint32_t sectorError = 0;

  HAL_FLASH_Unlock();
  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Sector        = eraseSector;
  EraseInitStruct.NbSectors     = lens;
  status = HAL_FLASHEx_Erase(&EraseInitStruct, &sectorError);

  __HAL_FLASH_DATA_CACHE_DISABLE();
  __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();
  __HAL_FLASH_DATA_CACHE_RESET();
  __HAL_FLASH_INSTRUCTION_CACHE_RESET();
  __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
  __HAL_FLASH_DATA_CACHE_ENABLE();

  HAL_FLASH_Lock();

  return status;
}

/*************************************** END OF FILE ****************************************/
