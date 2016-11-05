/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    w25q.h
  * @author  KitSprout
  * @date    11-Oct-2016
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __W25Q_H
#define __W25Q_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types --------------------------------------------------------------------------*/
typedef enum {
  W25Q80_ID  = 0xEF13,
  W25Q16_ID  = 0xEF14,
  W25Q32_ID  = 0xEF15,
  W25Q64_ID  = 0xEF16,
  W25Q128_ID = 0xEF17
} W25Qx_ID_t;

/* Exported constants ----------------------------------------------------------------------*/
#define W25Q_CMD_WRITE_ENABLE       ((uint8_t)0x06)
#define W25Q_CMD_WRITE_DISABLE      ((uint8_t)0x04)
#define W25Q_CMD_READ_STATUS        ((uint8_t)0x05)
#define W25Q_CMD_WRITE_STATUS       ((uint8_t)0x01)
#define W25Q_CMD_CHIP_ERASE         ((uint8_t)0xC7)
#define W25Q_CMD_POWER_DOWN         ((uint8_t)0xB9)
#define W25Q_CMD_RELEASE_POWER_DOWN ((uint8_t)0xAB)
#define W25Q_CMD_DEVICE_ID          ((uint8_t)0x90)
#define W25Q_CMD_JEDEC_DEVICE_ID    ((uint8_t)0x9F)
#define W25Q_CMD_PAGE_PROGRAM       ((uint8_t)0x02)
#define W25Q_CMD_SECTOR_ERASE       ((uint8_t)0x20)   //  4 KB 
#define W25Q_CMD_BLOCK_ERASE_32     ((uint8_t)0x52)   // 32 KB
#define W25Q_CMD_BLOCK_ERASE_64     ((uint8_t)0xD8)   // 64 KB
#define W25Q_CMD_READ_DATA          ((uint8_t)0x03)
#define W25X_CMD_FAST_READ          ((uint8_t)0x0B)
#define W25X_CMD_FAST_READ_DUAL     ((uint8_t)0x3B)

/* Exported functions ----------------------------------------------------------------------*/  
void     W25Q_Config( void );
void     W25Q_Init( void );
uint16_t W25Q_GetDeviceID( void );
void     W25Q_ReadData( uint8_t *readBuf, uint32_t readAddr, uint16_t lens );
void     W25Q_WritePage( uint8_t *writeBuf, uint32_t writeAddr, uint16_t lens );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
