/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_flash.h
 *  @author  KitSprout
 *  @date    16-Jun-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __NRF5x_FLASH_H
#define __NRF5x_FLASH_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"

/* Define ----------------------------------------------------------------------------------*/
#define FLASH_PAGE_SIZE   (NRF_FICR->CODEPAGESIZE)    // Code memory page size, nrf51: 1024 bytes
#define FLASH_SIZE        (NRF_FICR->CODESIZE)        // Code memory size in number of pages

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
void      FLASH_WriteDataU8( const uint32_t writeAddr, const uint8_t *writeData, uint32_t lens );
void      FLASH_WriteDataU16( const uint32_t writeAddr, const uint16_t *writeData, uint32_t lens );
void      FLASH_WriteDataU32( const uint32_t writeAddr, const uint32_t *writeData, uint32_t lens );
void      FLASH_ReadDataU8( const uint32_t readAddr, uint8_t *readData, const uint32_t lens );
void      FLASH_ReadDataU16( const uint32_t readAddr, uint16_t *readData, const uint32_t lens );
void      FLASH_ReadDataU32( const uint32_t readAddr, uint32_t *readData, const uint32_t lens );
void      FLASH_ErasePage( uint32_t erasePageAddr );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
