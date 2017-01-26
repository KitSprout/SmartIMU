/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    main.c
  * @author  KitSprout
  * @date    26-Jan-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_flash.h"
#include "modules\serial.h"
#include "stm32f4xx_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
pFunc JumpToApp;
uint32_t JumpAddr;

uint8_t  binaryFile[4096] = {0};
uint32_t binaryFileSize   = 0;

/* Private function prototypes -------------------------------------------------------------*/
void     IAP_Init( void );
void     IAP_JumpToApp( void );
uint32_t IAP_Download( uint8_t *binaryFile );
void     IAP_Upload( uint8_t *binaryFile, uint32_t binaryFileSize );

/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  HAL_InitTick();
  BSP_GPIO_Config();

  if (KEY_Read()) {
    LED_R_Reset();
    IAP_Init();
    delay_ms(100);
    binaryFileSize = IAP_Download(binaryFile);
    IAP_Upload(binaryFile, binaryFileSize);
    delay_ms(1000);
    LED_R_Set();
    IAP_JumpToApp();
  }
  else {
    IAP_JumpToApp();
  }
}

/**
  * @brief  IAP_Init
  */
void IAP_Init( void )
{
  BSP_UART_Config(NULL);
}

/**
  * @brief  IAP_JumpToApp
  */
void IAP_JumpToApp( void )
{
  if (((*(__IO uint32_t*)IAP_APPLICATION_ADDR) & 0x2FFE0000 ) == 0x20000000) {

    /* Jump to user application */
    JumpAddr = *(__IO uint32_t*)(IAP_APPLICATION_ADDR + 4);
    JumpToApp = (pFunc)JumpAddr;

    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t*)IAP_APPLICATION_ADDR);
    JumpToApp();
  }
}

/**
  * @brief  IAP_Download
  */
uint32_t IAP_Download( uint8_t *binaryFile )
{
  uint8_t fileInfo[16] = {0};
  uint32_t offset = 0;
  uint32_t *fileSize = (uint32_t*)(&fileInfo[4]);
  uint32_t *recvLens = (uint32_t*)(&fileInfo[8]);
  uint32_t *recvCRC  = (uint32_t*)(&fileInfo[12]);

  Serial_RecvData(fileInfo, 16);
  if ((fileInfo[0] == 'K') && (fileInfo[1] == 'S') && (fileInfo[2] == 'U') && (fileInfo[3] == 'L')) {
    uint32_t crc = 0;
    for (uint8_t i = 0; i < 12; i++) {
      crc += fileInfo[i];
    }
    if (crc == *recvCRC) {
      uint32_t quotient  = *fileSize / *recvLens;
      uint32_t remainder = *fileSize % *recvLens;
      while (quotient--) {
        Serial_RecvData(binaryFile + offset, *recvLens);
        offset += *recvLens;
      }
      Serial_RecvData(binaryFile + offset, remainder);
    }
  }

  return *fileSize;
}


/**
  * @brief  IAP_Upload
  */
void IAP_Upload( uint8_t *binaryFile, uint32_t binaryFileSize )
{
  uint32_t offset = 0;
  uint32_t quotient  = binaryFileSize / 1024;
  uint32_t remainder = binaryFileSize % 1024;
  uint32_t clearSector = FLASH_GetSector(IAP_APPLICATION_ADDR);

  FLASH_EraseSectors(clearSector, 1);
  while (quotient--) {
    FLASH_WritePageU8(IAP_APPLICATION_ADDR + offset, binaryFile + offset, 1024);
    offset += 1024;
    delay_ms(100);
  }
  FLASH_WritePageU8(IAP_APPLICATION_ADDR + offset, binaryFile + offset, remainder);
  delay_ms(100);
}

/*************************************** END OF FILE ****************************************/
