/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_flash.c
  * @author  KitSprout
  * @date    26-Jan-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4_system.h"
#include "stm32f4_flash.h"

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  FLASH_WritePageU8
  */
int8_t FLASH_WritePageU8( uint32_t writeAddr, const uint8_t *writeData, uint32_t lens )
{
  int8_t status = ERROR;
  uint32_t count = 0;

  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR  | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

  do {
    status = FLASH_ProgramByte(writeAddr + count, writeData[count]);
  } while ((++count < lens) && (status == FLASH_COMPLETE));
  FLASH_Lock();

  return status;
}

/**
  * @brief  FLASH_WritePageU16
  */
int8_t FLASH_WritePageU16( uint32_t writeAddr, const uint16_t *writeData, uint32_t lens )
{
  int8_t status = ERROR;
  uint32_t count = 0;

  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR  | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
  do {
    status = FLASH_ProgramHalfWord(writeAddr + (count << 1), writeData[count]);
  } while ((++count < lens) && (status == FLASH_COMPLETE));
  FLASH_Lock();

  return status;
}

/**
  * @brief  FLASH_WritePageU32
  */
int8_t FLASH_WritePageU32( uint32_t writeAddr, const uint32_t *writeData, uint32_t lens )
{
  int8_t status = ERROR;
  uint32_t count = 0;

  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR  | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
  do {
    status = FLASH_ProgramWord(writeAddr + (count << 2), writeData[count]);
  } while ((++count < lens) && (status == FLASH_COMPLETE));
  FLASH_Lock();

  return status;
}

/**
  * @brief  FLASH_WritePageU64
  */
int8_t FLASH_WritePageU64( uint32_t writeAddr, const uint64_t *writeData, uint32_t lens )
{
  int8_t status = ERROR;
  uint32_t count = 0;

  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR  | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
  do {
    status = FLASH_ProgramDoubleWord(writeAddr + (count << 3), writeData[count]);
  } while ((++count < lens) && (status == FLASH_COMPLETE));
  FLASH_Lock();

  return status;
}

/**
  * @brief  FLASH_ReadPageU8
  */
void FLASH_ReadPageU8( uint32_t readAddr, uint8_t *readData, uint32_t lens )
{
  uint32_t count = 0;

  do {
    readData[count] = (uint8_t)(*(__IO uint32_t*)(readAddr + count));
  } while (++count < lens);
}

/**
  * @brief  FLASH_ReadPageU16
  */
void FLASH_ReadPageU16( uint32_t readAddr, uint16_t *readData, uint32_t lens )
{
  uint32_t count = 0;

  do {
    readData[count] = (uint16_t)(*(__IO uint32_t*)(readAddr + (count << 1)));
  } while (++count < lens);
}

/**
  * @brief  FLASH_ReadPageU32
  */
void FLASH_ReadPageU32( uint32_t readAddr, uint32_t *readData, uint32_t lens )
{
  uint32_t count = 0;

  do {
    readData[count] = (uint32_t)(*(__IO uint32_t*)(readAddr + (count << 2)));
  } while (++count < lens);
}

/**
  * @brief  FLASH_ReadPageU64
  */
void FLASH_ReadPageU64( uint32_t readAddr, uint64_t *readData, uint32_t lens )
{
  uint32_t count = 0;

  do {
    readData[count] = (uint64_t)(*(__IO uint32_t*)(readAddr + (count << 3)));
  } while (++count < lens);
}

/**
  * @brief  FLASH_GetSector
  */
uint32_t FLASH_GetSector( uint32_t addr )
{
  if      ((addr >= FLASH_ADDR_SECTOR_0)  && (addr < FLASH_ADDR_SECTOR_1))      return FLASH_Sector_0;
  else if ((addr >= FLASH_ADDR_SECTOR_1)  && (addr < FLASH_ADDR_SECTOR_2))      return FLASH_Sector_1;
  else if ((addr >= FLASH_ADDR_SECTOR_2)  && (addr < FLASH_ADDR_SECTOR_3))      return FLASH_Sector_2;
  else if ((addr >= FLASH_ADDR_SECTOR_3)  && (addr < FLASH_ADDR_SECTOR_4))      return FLASH_Sector_3;
  else if ((addr >= FLASH_ADDR_SECTOR_4)  && (addr < FLASH_ADDR_SECTOR_5))      return FLASH_Sector_4;
  else if ((addr >= FLASH_ADDR_SECTOR_5)  && (addr < FLASH_ADDR_SECTOR_6))      return FLASH_Sector_5;
  else if ((addr >= FLASH_ADDR_SECTOR_6)  && (addr < FLASH_ADDR_SECTOR_7))      return FLASH_Sector_6;
  else if ((addr >= FLASH_ADDR_SECTOR_7)  && (addr < FLASH_ADDR_SECTOR_8))      return FLASH_Sector_7;
  else if ((addr >= FLASH_ADDR_SECTOR_8)  && (addr < FLASH_ADDR_SECTOR_9))      return FLASH_Sector_8;
  else if ((addr >= FLASH_ADDR_SECTOR_9)  && (addr < FLASH_ADDR_SECTOR_10))     return FLASH_Sector_9;
  else if ((addr >= FLASH_ADDR_SECTOR_10) && (addr < FLASH_ADDR_SECTOR_11))     return FLASH_Sector_10;
  else  /*((addr >= FLASH_ADDR_SECTOR_11) && (addr < FLASH_ADDR_SECTOR_END)) */ return FLASH_Sector_11;
}

/**
  * @brief  FLASH_EraseSectors
  */
int8_t FLASH_EraseSectors( uint32_t eraseSector, uint32_t lens )
{
  int8_t status = ERROR;
  uint8_t count = 0;

  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR  | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
  do {
    status = FLASH_EraseSector(eraseSector + (count << 3), VoltageRange_3);
  } while ((++count < lens) && (status == FLASH_COMPLETE));
  FLASH_Lock();

  return status;
}

/*************************************** END OF FILE ****************************************/
