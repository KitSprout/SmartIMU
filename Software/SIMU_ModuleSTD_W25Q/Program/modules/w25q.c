/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    w25q.c
  * @author  KitSprout
  * @date    11-Oct-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"
#include "modules\w25q.h"

/** @addtogroup STM32_Module
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
#define W25Q_SPIx                 SPI2
#define W25Q_SPIx_CLK_ENABLE()    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE)

#define W25Q_SCK_PIN              GPIO_Pin_13
#define W25Q_SCK_GPIO_PORT        GPIOB
#define W25Q_SCK_AF               GPIO_AF_SPI2
#define W25Q_SCK_SOURCE           GPIO_PinSource13

#define W25Q_SDO_PIN              GPIO_Pin_14
#define W25Q_SDO_GPIO_PORT        GPIOB
#define W25Q_SDO_AF               GPIO_AF_SPI2
#define W25Q_SDO_SOURCE           GPIO_PinSource14

#define W25Q_SDI_PIN              GPIO_Pin_15
#define W25Q_SDI_GPIO_PORT        GPIOB
#define W25Q_SDI_AF               GPIO_AF_SPI2
#define W25Q_SDI_SOURCE           GPIO_PinSource15

#define W25Q_CSF_PIN              GPIO_Pin_8
#define W25Q_CSF_GPIO_PORT        GPIOA
#define W25Q_CSF_H()              __GPIO_SET(W25Q_CSF_GPIO_PORT, W25Q_CSF_PIN)
#define W25Q_CSF_L()              __GPIO_RST(W25Q_CSF_GPIO_PORT, W25Q_CSF_PIN)

#define W25Q_WP_PIN              GPIO_Pin_12
#define W25Q_WP_GPIO_PORT        GPIOB
#define W25Q_WP_H()              __GPIO_SET(W25Q_WP_GPIO_PORT, W25Q_WP_PIN)
#define W25Q_WP_L()              __GPIO_RST(W25Q_WP_GPIO_PORT, W25Q_WP_PIN)

#define W25Q_HOD_PIN              GPIO_Pin_0
#define W25Q_HOD_GPIO_PORT        GPIOB
#define W25Q_HOD_H()              __GPIO_SET(W25Q_HOD_GPIO_PORT, W25Q_HOD_PIN)
#define W25Q_HOD_L()              __GPIO_RST(W25Q_HOD_GPIO_PORT, W25Q_HOD_PIN)

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  W25Q_GetDeviceID
  * @param  None
  * @retval device id
  */
uint16_t W25Q_GetDeviceID( void )
{
  uint16_t deviceID = 0;

  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, 0x90);
  SPI_RW(W25Q_SPIx, 0x00);
  SPI_RW(W25Q_SPIx, 0x00);
  SPI_RW(W25Q_SPIx, 0x00);
  deviceID |= SPI_RW(W25Q_SPIx, 0xFF) << 8;
  deviceID |= SPI_RW(W25Q_SPIx, 0xFF);
  W25Q_CSF_H();

  return deviceID;
}

/**
  * @brief  W25Q_Config
  * @param  None
  * @retval None
  */
void W25Q_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef SPI_InitStruct;

  /* SPI Clk ******************************************************************/
  W25Q_SPIx_CLK_ENABLE();

  /* SPI AF *******************************************************************/
  GPIO_PinAFConfig(W25Q_SCK_GPIO_PORT, W25Q_SCK_SOURCE, W25Q_SCK_AF);
  GPIO_PinAFConfig(W25Q_SDO_GPIO_PORT, W25Q_SDO_SOURCE, W25Q_SDO_AF);
  GPIO_PinAFConfig(W25Q_SDI_GPIO_PORT, W25Q_SDI_SOURCE, W25Q_SDI_AF);

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = W25Q_CSF_PIN;
  GPIO_Init(W25Q_CSF_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = W25Q_SCK_PIN;
  GPIO_Init(W25Q_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = W25Q_SDO_PIN;
  GPIO_Init(W25Q_SDO_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = W25Q_SDI_PIN;
  GPIO_Init(W25Q_SDI_GPIO_PORT, &GPIO_InitStruct);

  W25Q_CSF_H();  // LOW ENABLE

  /* SPI Init ****************************************************************/
  SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;
  SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL              = SPI_CPOL_High;
  SPI_InitStruct.SPI_CPHA              = SPI_CPHA_2Edge;
  SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial     = 7;
  SPI_Init(W25Q_SPIx, &SPI_InitStruct);

  SPI_Cmd(W25Q_SPIx, ENABLE); 
}

/**
  * @brief  W25Q_Init
  * @param  None
  * @retval None
  */
void W25Q_Init( void )
{
  uint16_t deviceID = 0;

  deviceID = W25Q_GetDeviceID();

  switch (deviceID) {
    case W25Q80_ID:     break;
    case W25Q16_ID:     break;
    case W25Q32_ID:     break;
    case W25Q64_ID:     break;
    case W25Q128_ID:    break;
    default:            break;
  }
}

/**
  * @brief  W25Q_GetStatus
  * @param  None
  * @retval status
  */
uint8_t W25Q_GetStatus( void )   
{
  uint8_t status = 0;

  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, W25Q_CMD_READ_STATUS);
  status = SPI_RW(W25Q_SPIx, 0xFF);
  W25Q_CSF_H();

  return status;
}

/**
  * @brief  W25Q_SetStatus
  * @param  None
  * @retval status
  */
void W25Q_SetStatus( uint8_t status )   
{
  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, W25Q_CMD_WRITE_STATUS);
  SPI_RW(W25Q_SPIx, status);
  W25Q_CSF_H();
}

/**
  * @brief  W25Q_WriteCMD
  * @param  state: 
  * @retval None
  */
void W25Q_WriteCMD( uint8_t state )
{
  W25Q_CSF_L();
  if (state == ENABLE) {
    SPI_RW(W25Q_SPIx, W25Q_CMD_WRITE_ENABLE);
  }
  else {
    SPI_RW(W25Q_SPIx, W25Q_CMD_WRITE_DISABLE);
  }
  W25Q_CSF_H();
}

/**
  * @brief  W25Q_WaitBusy
  * @param  None
  * @retval None
  */
void W25Q_WaitBusy( void )   
{   
  while ((W25Q_GetStatus() & 0x01) == 0x01);
}

/**
  * @brief  W25Q_PowerDown
  * @param  None
  * @retval None
  */
void W25Q_PowerDown( void )
{
  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, W25Q_CMD_POWER_DOWN);
  W25Q_CSF_H();
  delay_ms(1);
}

/**
  * @brief  W25Q_WakeUp
  * @param  None
  * @retval None
  */
void W25Q_WakeUp( void )
{
  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, W25Q_CMD_RELEASE_POWER_DOWN);
  W25Q_CSF_H();
  delay_ms(1);
}

/**
  * @brief  W25Q_EraseChip
  * @param  None
  * @retval None
  */
void W25Q_EraseChip( void )
{
  W25Q_WriteCMD(ENABLE);
  W25Q_WaitBusy();

  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, W25Q_CMD_CHIP_ERASE);
  W25Q_CSF_H();

  W25Q_WaitBusy();
}

/**
  * @brief  W25Q_EraseSector
  * @param  eraseAddr: 
  * @retval None
  */
void W25Q_EraseSector( uint32_t eraseAddr )   
{
  eraseAddr *= 4096;

  W25Q_WriteCMD(ENABLE);
  W25Q_WaitBusy();

  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, W25Q_CMD_SECTOR_ERASE);
  SPI_RW(W25Q_SPIx, (uint8_t)(eraseAddr >> 16));
  SPI_RW(W25Q_SPIx, (uint8_t)(eraseAddr >> 8));
  SPI_RW(W25Q_SPIx, (uint8_t)eraseAddr);
  W25Q_CSF_H();

  W25Q_WaitBusy();
}

/**
  * @brief  W25Q_ReadData
  * @param  readBuf: 
  * @param  readAddr: 
  * @param  lens: 
  * @retval None
  */
void W25Q_ReadData( uint8_t *readBuf, uint32_t readAddr, uint16_t lens )
{
  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, W25Q_CMD_READ_DATA);
  SPI_RW(W25Q_SPIx, (uint8_t)(readAddr >> 16));
  SPI_RW(W25Q_SPIx, (uint8_t)(readAddr >> 8));
  SPI_RW(W25Q_SPIx, (uint8_t)readAddr);
  for(uint16_t i = 0; i < lens; i++) {
    readBuf[i] = SPI_RW(W25Q_SPIx, 0xFF);
  }
  W25Q_CSF_H();
}

/**
  * @brief  W25Q_WritePage
  * @param  writeBuf: 
  * @param  writeAddr: 
  * @param  lens: 
  * @retval None
  */
void W25Q_WritePage( uint8_t *writeBuf, uint32_t writeAddr, uint16_t lens )
{
  W25Q_WriteCMD(ENABLE);
  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, W25Q_CMD_PAGE_PROGRAM);
  SPI_RW(W25Q_SPIx, (uint8_t)(writeAddr >> 16));
  SPI_RW(W25Q_SPIx, (uint8_t)(writeAddr >> 8));
  SPI_RW(W25Q_SPIx, (uint8_t)writeAddr);
  for(uint16_t i = 0; i < lens; i++) {
    SPI_RW(W25Q_SPIx, writeBuf[i]);
  }
  W25Q_CSF_H();
  W25Q_WaitBusy();
}

/*************************************** END OF FILE ****************************************/
