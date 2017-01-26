/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_flash.h
  * @author  KitSprout
  * @date    26-Jan-2017
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STM32F4_FLASH_H
#define __STM32F4_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types --------------------------------------------------------------------------*/
/* Exported constants ----------------------------------------------------------------------*/
#define FLASH_ADDR_SECTOR_0   ((uint32_t)0x08000000)  // Sector 0,   16 Kbytes,   16 KBytes
#define FLASH_ADDR_SECTOR_1   ((uint32_t)0x08004000)  // Sector 1,   16 Kbytes,   32 KBytes
#define FLASH_ADDR_SECTOR_2   ((uint32_t)0x08008000)  // Sector 2,   16 Kbytes,   48 KBytes
#define FLASH_ADDR_SECTOR_3   ((uint32_t)0x0800C000)  // Sector 3,   16 Kbytes,   64 KBytes
#define FLASH_ADDR_SECTOR_4   ((uint32_t)0x08010000)  // Sector 4,   64 Kbytes,  128 KBytes
#define FLASH_ADDR_SECTOR_5   ((uint32_t)0x08020000)  // Sector 5,  128 Kbytes,  256 KBytes
#define FLASH_ADDR_SECTOR_6   ((uint32_t)0x08040000)  // Sector 6,  128 Kbytes,  384 KBytes
#define FLASH_ADDR_SECTOR_7   ((uint32_t)0x08060000)  // Sector 7,  128 Kbytes,  512 KBytes
#define FLASH_ADDR_SECTOR_8   ((uint32_t)0x08080000)  // Sector 8,  128 Kbytes,  640 KBytes
#define FLASH_ADDR_SECTOR_9   ((uint32_t)0x080A0000)  // Sector 9,  128 Kbytes,  768 KBytes
#define FLASH_ADDR_SECTOR_10  ((uint32_t)0x080C0000)  // Sector 10, 128 Kbytes,  896 KBytes
#define FLASH_ADDR_SECTOR_11  ((uint32_t)0x080E0000)  // Sector 11, 128 Kbytes, 1024 KBytes

/* Exported functions ----------------------------------------------------------------------*/  
int8_t   FLASH_WritePageU8( uint32_t writeAddr, const uint8_t *writeData, uint32_t lens );
int8_t   FLASH_WritePageU16( uint32_t writeAddr, const uint16_t *writeData, uint32_t lens );
int8_t   FLASH_WritePageU32( uint32_t writeAddr, const uint32_t *writeData, uint32_t lens );
int8_t   FLASH_WritePageU64( uint32_t writeAddr, const uint64_t *writeData, uint32_t lens );

void     FLASH_ReadPageU8( uint32_t readAddr, uint8_t *readData, uint32_t lens );
void     FLASH_ReadPageU16( uint32_t readAddr, uint16_t *readData, uint32_t lens );
void     FLASH_ReadPageU32( uint32_t readAddr, uint32_t *readData, uint32_t lens );
void     FLASH_ReadPageU64( uint32_t readAddr, uint64_t *readData, uint32_t lens );

uint32_t FLASH_GetSector( uint32_t addr );
int8_t   FLASH_EraseSectors( uint32_t eraseSector, uint32_t lens );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
