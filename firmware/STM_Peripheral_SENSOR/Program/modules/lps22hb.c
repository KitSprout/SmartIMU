/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    lps22hb.c
 *  @author  KitSprout
 *  @date    09-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"
#include "modules\lps22hb.h"

/** @addtogroup STM32_Module
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
#define ENABLE_DEBUG_LOG      (0U)

/* Macro -----------------------------------------------------------------------------------*/
#define LPS22_Delay(__MS)     delay_ms(__MS)

/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/\
static float32_t bar_sensadj[2] = {1, 1};

/* Prototypes ------------------------------------------------------------------------------*/
static uint32_t LPS22_DeviceCheck( void );

/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  LPS22_WriteReg
 */
void LPS22_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
  IMU_CSB_L();
  SPI_RW(hImu.handle, writeAddr);
  SPI_RW(hImu.handle, writeData);
  IMU_CSB_H();
}

/**
 *  @brief  LPS22_WriteRegs
 */
void LPS22_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
  IMU_CSB_L();
  SPI_RW(hImu.handle, writeAddr);
  for (uint32_t i = 0; i < lens; i++) {
    SPI_RW(hImu.handle, writeData[i]);
  }
  IMU_CSB_H();
}

/**
 *  @brief  LPS22_ReadReg
 */
uint8_t LPS22_ReadReg( uint8_t readAddr )
{
  uint8_t readData;

  IMU_CSB_L();
  SPI_RW(hImu.handle, 0x80 | readAddr);
  readData = SPI_RW(hImu.handle, 0x00);
  IMU_CSB_H();

  return readData;
}

/**
 *  @brief  LPS22_ReadRegs
 */
void LPS22_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
  IMU_CSB_L();
  SPI_RW(hImu.handle, 0x80 | readAddr);
  for (uint32_t i = 0; i < lens; i++) {
    readData[i] = SPI_RW(hImu.handle, 0x00);
  }
  IMU_CSB_H();
}

/**
 *  @brief  LPS22_Config
 */
void LPS22_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* CS Pin */
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin   = IMU_CSB_PIN;
  HAL_GPIO_Init(IMU_CSB_GPIO_PORT, &GPIO_InitStruct);

  IMU_CSB_H();  // LOW ENABLE
}

/**
 *  @brief  LPS22_Init
 */
uint32_t LPS22_Init( void )
{
  uint8_t treg = 0;

  if (LPS22_DeviceCheck() != KS_OK) {
    return KS_ERROR;
  }

  /* Normal mode (low-noise mode) */
  treg = LPS22_ReadReg(LPS22HB_RES_CONF);
  treg &= 0x02;
  treg |= 0x00;
  LPS22_WriteReg(LPS22HB_RES_CONF, treg);
  LPS22_Delay(5);

  /* Control register 1 */
  treg  = 0x00;
  treg |= 0x50; // Output data rate, 75 Hz
  treg |= 0x00; // Low-pass filter disabled
  treg |= 0x02; // Block data update, enable
  LPS22_WriteReg(LPS22HB_CTRL_REG1, treg);
  LPS22_Delay(5);

  /* Control register 2 */
  treg = LPS22_ReadReg(LPS22HB_CTRL_REG2);
  treg &= 0xED;
  treg |= 0x10;
  LPS22_WriteReg(LPS22HB_RES_CONF, treg);
  LPS22_Delay(5);

  bar_sensadj[0] = 1.0 / 4096;
  bar_sensadj[1] = 1.0 / 100;
#if ENABLE_DEBUG_LOG
  printf(" >>> LPS22HB SENS : %8.6f %8.6f\r\n", bar_sensadj[0], bar_sensadj[1]);
#endif

  return KS_OK;
}

/**
 *  @brief  LPS22_DeviceCheck
 */
static uint32_t LPS22_DeviceCheck( void )
{
  uint8_t deviceID;

  deviceID = LPS22_ReadReg(LPS22HB_WHO_AM_I);
#if ENABLE_DEBUG_LOG
  printf(" >>> LPS22HB      : 0x%02X\r\n", deviceID);
#endif
  if (deviceID != LPS22HB_DEVICE_ID) {
    return KS_ERROR;
  }

  return KS_OK;
}

/**
 *  @brief  LPS22_GetRawData
 */
uint32_t LPS22_GetRawData( int32_t *raw )
{
  uint8_t buff[3];
  uint32_t tmp = 0;

  LPS22_ReadRegs(LPS22HB_PRESS_OUT_XL, buff, 3);

  for (uint32_t i = 0; i < 3; i++) {
    tmp |= (((uint32_t)buff[i]) << (i << 3));
  }

  /* convert the 2's complement 24 bit to 2's complement 32 bit */
  if (tmp & 0x00800000) {
    tmp |= 0xFF000000;
  }

  raw[0] = (int32_t)tmp;

  LPS22_ReadRegs(LPS22HB_TEMP_OUT_L, buff, 2);
  tmp = (((uint16_t)buff[1]) << 8) + (uint16_t)buff[0];

  raw[1] = (int16_t)tmp;

  return KS_OK;
}

/**
 *  @brief  LPS22_GetRawData
 */
uint32_t LPS22_GetData( float32_t *data )
{
  int32_t raw[2] = {0};
  uint32_t status;

  status = LPS22_GetRawData(raw);
  data[0] = raw[0] * bar_sensadj[0];
  data[1] = raw[1] * bar_sensadj[1];

  return status;
}

/*************************************** END OF FILE ****************************************/
