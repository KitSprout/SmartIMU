/* #include "stm32f4_flash.h" */

#ifndef __STM32F4_FLASH_H
#define __STM32F4_FLASH_H

#include "stm32f4xx_hal.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define FLASH_ADDR_SECTOR_0   ((uint32_t)0x08000000) // Sector 0,   16 Kbytes,   16 KBytes
#define FLASH_ADDR_SECTOR_1   ((uint32_t)0x08004000) // Sector 1,   16 Kbytes,   32 KBytes
#define FLASH_ADDR_SECTOR_2   ((uint32_t)0x08008000) // Sector 2,   16 Kbytes,   48 KBytes
#define FLASH_ADDR_SECTOR_3   ((uint32_t)0x0800C000) // Sector 3,   16 Kbytes,   64 KBytes
#define FLASH_ADDR_SECTOR_4   ((uint32_t)0x08010000) // Sector 4,   64 Kbytes,  128 KBytes
#define FLASH_ADDR_SECTOR_5   ((uint32_t)0x08020000) // Sector 5,  128 Kbytes,  256 KBytes
#define FLASH_ADDR_SECTOR_6   ((uint32_t)0x08040000) // Sector 6,  128 Kbytes,  384 KBytes
#define FLASH_ADDR_SECTOR_7   ((uint32_t)0x08060000) // Sector 7,  128 Kbytes,  512 KBytes
/*=====================================================================================================*/
/*=====================================================================================================*/
int8_t   Flash_WritePageU8( uint32_t writeAddr, const uint8_t *writeData, uint32_t lens );
int8_t   Flash_WritePageU16( uint32_t writeAddr, const uint16_t *writeData, uint32_t lens );
int8_t   Flash_WritePageU32( uint32_t writeAddr, const uint32_t *writeData, uint32_t lens );
int8_t   Flash_WritePageU64( uint32_t writeAddr, const uint64_t *writeData, uint32_t lens );

void     Flash_ReadPageU8( uint32_t readAddr, uint8_t *readData, uint32_t lens );
void     Flash_ReadPageU16( uint32_t readAddr, uint16_t *readData, uint32_t lens );
void     Flash_ReadPageU32( uint32_t readAddr, uint32_t *readData, uint32_t lens );
void     Flash_ReadPageU64( uint32_t readAddr, uint64_t *readData, uint32_t lens );

uint32_t Flash_GetSector( uint32_t addr );
int8_t   Flash_ErasePages( uint32_t erasePageStart, uint32_t nbSectors );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif	 
