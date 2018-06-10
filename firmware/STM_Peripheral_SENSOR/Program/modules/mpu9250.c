/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    mpu9250.c
 *  @author  KitSprout
 *  @date    09-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"
#include "modules\mpu9250.h"

/** @addtogroup STM32_Module
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
#define ENABLE_DEBUG_LOG      (0U)
#define AUX_READ_TIMEOUT      (200)

/* Macro -----------------------------------------------------------------------------------*/
#define MPU92_Delay(__MS)     delay_ms(__MS)

/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static float32_t gyr_sensadj[3] = {1, 1, 1};
static float32_t acc_sensadj[3] = {1, 1, 1};

#if ENABLE_INTERNAL_MAG_AK8963
static float32_t mag_sensadj[3] = {1, 1, 1};
static float32_t AK8963_ASA[3] = {0};
#endif

/* Prototypes ------------------------------------------------------------------------------*/
static uint32_t MPU92_DeviceCheck( void );

#if ENABLE_INTERNAL_MAG_AK8963
static uint32_t MPU92_AUX_AK8963_DeviceCheck( void );
static void     MPU92_AUX_AK8963_Init( void );
#endif

/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  MPU92_WriteReg
 */
void MPU92_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
  IMU_CSM_L();
  SPI_RW(hImu.handle, writeAddr);
  SPI_RW(hImu.handle, writeData);
  IMU_CSM_H();
}

/**
 *  @brief  MPU92_WriteRegs
 */
void MPU92_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
  IMU_CSM_L();
  SPI_RW(hImu.handle, writeAddr);
  for (uint32_t i = 0; i < lens; i++) {
    SPI_RW(hImu.handle, writeData[i]);
  }
  IMU_CSM_H();
}

/**
 *  @brief  MPU92_ReadReg
 */
uint8_t MPU92_ReadReg( uint8_t readAddr )
{
  uint8_t readData;

  IMU_CSM_L();
  SPI_RW(hImu.handle, 0x80 | readAddr);
  readData = SPI_RW(hImu.handle, 0x00);
  IMU_CSM_H();

  return readData;
}

/**
 *  @brief  MPU92_ReadRegs
 */
void MPU92_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
  IMU_CSM_L();
  SPI_RW(hImu.handle, 0x80 | readAddr);
  for (uint32_t i = 0; i < lens; i++) {
    readData[i] = SPI_RW(hImu.handle, 0x00);
  }
  IMU_CSM_H();
}

/**
 *  @brief  MPU92_AUX_WriteReg
 */
void MPU92_AUX_WriteReg( uint8_t slaveAddr, uint8_t writeAddr, uint8_t writeData )
{
  uint8_t  status;
  uint32_t timeout = AUX_READ_TIMEOUT;

  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_SLV4_ADDR, slaveAddr);
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_SLV4_REG, writeAddr);
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_SLV4_DO, writeData);
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);

  do {
    MPU92_Delay(1);
    status = MPU92_ReadReg(MPU6500_I2C_MST_STATUS);
  } while (((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));
  MPU92_Delay(1);
}

/**
 *  @brief  MPU92_AUX_ReadReg
 */
uint8_t MPU92_AUX_ReadReg( uint8_t slaveAddr, uint8_t readAddr )
{
  uint8_t status;
  uint8_t readData;
  uint32_t timeout = AUX_READ_TIMEOUT;

  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_SLV4_ADDR, slaveAddr | 0x80);
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_SLV4_REG, readAddr);
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);

  do {
    MPU92_Delay(1);
    status = MPU92_ReadReg(MPU6500_I2C_MST_STATUS);
  } while (((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));
  MPU92_Delay(1);
  readData = MPU92_ReadReg(MPU6500_I2C_SLV4_DI);

  return readData;
}

/**
 *  @brief  MPU92_AUX_SlaveConfig
 */
static void MPU92_AUX_SlaveConfig( uint8_t slaveNum, uint8_t slaveAddr, uint8_t readAddr, uint8_t readLens )
{
  uint8_t reg;
  uint8_t offset = slaveNum * 3;

  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_SLV0_ADDR + offset, slaveAddr | 0x80);
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_SLV0_REG + offset, readAddr);
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_SLV0_CTRL + offset, MPU6500_I2C_SLVx_EN | readLens);
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_SLV4_CTRL, 0x09);
  MPU92_Delay(1);
  reg = MPU92_ReadReg(MPU6500_I2C_MST_DELAY_CTRL);
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_MST_DELAY_CTRL, reg | (0x01 << slaveNum));
}

/**
 *  @brief  MPU92_Config
 */
void MPU92_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* CS Pin */
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin   = IMU_CSM_PIN;
  HAL_GPIO_Init(IMU_CSM_GPIO_PORT, &GPIO_InitStruct);

  IMU_CSM_H();  // LOW ENABLE
}

/**
 *  @brief  MPU92_Init
 */
uint32_t MPU92_Init( void )
{
  uint8_t res;

  if (MPU92_DeviceCheck() != KS_OK) {
    return KS_ERROR;
  }

  MPU92_Delay(10);
  MPU92_WriteReg(MPU6500_PWR_MGMT_1,         0x80);   // Reset Device
  MPU92_Delay(100);
  MPU92_WriteReg(MPU6500_PWR_MGMT_1,         0x04);   // Clock Source
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_INT_PIN_CFG,        0x10);   // INT_ANYRD_2CLEAR
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_INT_ENABLE,         0x01);   // Set RAW_RDY_EN
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_PWR_MGMT_2,         0x00);   // Enable Accel & Gyro
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_SMPLRT_DIV,         0x00);   // Sample Rate Divider, INTERNAL_SAMPLE_RATE = 1KHz
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_GYRO_CONFIG,        0x18);   // 0x00:250dps, 0x08:500dps, 0x10:1000dps, 0x18:2000dps
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_ACCEL_CONFIG,       0x08);   // 0x00:2g, 0x08:4g, 0x10:8g, 0x18:16g
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_CONFIG,             0x00);   // gyro low pass filter
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_ACCEL_CONFIG_2,     0x05);   // accel low pass filter
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_USER_CTRL,          0x30);   // Set I2C_MST_EN, I2C_IF_DIS
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_MST_CTRL,       0x5D);   // aux i2c 400kHz
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_MST_DELAY_CTRL, 0x80);   // 
  MPU92_Delay(1);

  res = MPU92_ReadReg(MPU6500_GYRO_CONFIG) & 0x18;
  switch (res) {
    case 0x00:  gyr_sensadj[0] = gyr_sensadj[1] = gyr_sensadj[2] = 1.0 / 131.0;   break;
    case 0x08:  gyr_sensadj[0] = gyr_sensadj[1] = gyr_sensadj[2] = 1.0 / 65.5;    break;
    case 0x10:  gyr_sensadj[0] = gyr_sensadj[1] = gyr_sensadj[2] = 1.0 / 32.8;    break;
    case 0x18:  gyr_sensadj[0] = gyr_sensadj[1] = gyr_sensadj[2] = 1.0 / 16.4;    break;
  }

  res = MPU92_ReadReg(MPU6500_ACCEL_CONFIG) & 0x18;
  switch (res) {
    case 0x00:  acc_sensadj[0] = acc_sensadj[1] = acc_sensadj[2] = 1.0 / 16384;   break;
    case 0x08:  acc_sensadj[0] = acc_sensadj[1] = acc_sensadj[2] = 1.0 / 8192;    break;
    case 0x10:  acc_sensadj[0] = acc_sensadj[1] = acc_sensadj[2] = 1.0 / 4096;    break;
    case 0x18:  acc_sensadj[0] = acc_sensadj[1] = acc_sensadj[2] = 1.0 / 2048;    break;
  }

#if ENABLE_DEBUG_LOG
  printf(" >>> MPU9250 SENS : %8.6f %8.6f %8.6f, %8.6f %8.6f %8.6f\r\n", gyr_sensadj[0], gyr_sensadj[1], gyr_sensadj[2], acc_sensadj[0], acc_sensadj[1], acc_sensadj[2]);
#endif

#if ENABLE_INTERNAL_MAG_AK8963
  if (MPU92_AUX_AK8963_DeviceCheck() != KS_OK) {
    return KS_ERROR;
  }
  MPU92_AUX_AK8963_Init();
  MPU92_AUX_SlaveConfig(1, AK8963_I2C_ADDR, AK8963_ST1, 8);   // add ak8963 to i2c slave 0
#endif

  MPU92_Delay(5);

  return KS_OK;
}

/**
 *  @brief  MPU92_DeviceCheck
 */
static uint32_t MPU92_DeviceCheck( void )
{
  uint8_t deviceID;

  deviceID = MPU92_ReadReg(MPU6500_WHO_AM_I);
#if ENABLE_DEBUG_LOG
  printf("\r\n");
  printf(" >>> MPU9250      : 0x%02X\r\n", deviceID);
#endif
  if (deviceID != MPU6500_DeviceID) {
    return KS_ERROR;
  }

  return KS_OK;
}

/**
 *  @brief  MPU92_AUX_AK8963_DeviceCheck
 */
#if ENABLE_INTERNAL_MAG_AK8963
static uint32_t MPU92_AUX_AK8963_DeviceCheck( void )
{
  uint8_t deviceID;

  MPU92_Delay(1);
  deviceID = MPU92_AUX_ReadReg(AK8963_I2C_ADDR, AK8963_WIA);
#if ENABLE_DEBUG_LOG
  printf(" >>> AK8963       : 0x%02X\r\n", deviceID);
#endif
  if (deviceID != AK8963_DeviceID) {
    return KS_ERROR;
  }

  return KS_OK;
}

/**
 *  @brief  MPU92_AUX_AK8963_Init
 */
static void MPU92_AUX_AK8963_Init( void )
{
  uint8_t res, asa[3] = {0};

  MPU92_Delay(1);
  MPU92_AUX_WriteReg(AK8963_I2C_ADDR, AK8963_CNTL2, 0x01);    // Reset Device
  MPU92_Delay(50);
  MPU92_AUX_WriteReg(AK8963_I2C_ADDR, AK8963_CNTL1, 0x00);    // Power-down mode
  MPU92_Delay(1);
  MPU92_AUX_WriteReg(AK8963_I2C_ADDR, AK8963_CNTL1, 0x1F);    // Fuse ROM access mode, Read sensitivity adjustment
  MPU92_Delay(10);
  asa[0] = MPU92_AUX_ReadReg(AK8963_I2C_ADDR, AK8963_ASAX);
  MPU92_Delay(1);
  asa[1] = MPU92_AUX_ReadReg(AK8963_I2C_ADDR, AK8963_ASAY);
  MPU92_Delay(1);
  asa[2] = MPU92_AUX_ReadReg(AK8963_I2C_ADDR, AK8963_ASAZ);
  MPU92_Delay(1);
#if ENABLE_DEBUG_LOG
  printf(" >>> AK8963 ASA   : %02X %02X %02X\r\n", asa[0], asa[1], asa[2]);
#endif
  MPU92_AUX_WriteReg(AK8963_I2C_ADDR, AK8963_CNTL1, 0x00);    // Power-down mode
  MPU92_Delay(10);
  MPU92_AUX_WriteReg(AK8963_I2C_ADDR, AK8963_CNTL1, 0x16);    // Continuous measurement mode 2 & 16-bit
  MPU92_Delay(10);

  AK8963_ASA[0] = (asa[0] - 128) * 0.5 / 128 + 1;
  AK8963_ASA[1] = (asa[1] - 128) * 0.5 / 128 + 1;
  AK8963_ASA[2] = (asa[2] - 128) * 0.5 / 128 + 1;
#if ENABLE_DEBUG_LOG
  printf(" >>> AK8963 ASA   : %8.6f %8.6f %8.6f\r\n", AK8963_ASA[0], AK8963_ASA[1], AK8963_ASA[2]);
#endif

  res = MPU92_AUX_ReadReg(AK8963_I2C_ADDR, AK8963_CNTL1) & 0x10;
  switch (res) {
    case 0x00:  mag_sensadj[0] = mag_sensadj[1] = mag_sensadj[2] = 0.6;   break;
    case 0x10:  mag_sensadj[0] = mag_sensadj[1] = mag_sensadj[2] = 0.15;  break;
  }
#if ENABLE_DEBUG_LOG
  printf(" >>> AK8963 SENS  : %8.6f %8.6f %8.6f\r\n", mag_sensadj[0], mag_sensadj[1], mag_sensadj[2]);
#endif

  mag_sensadj[0] *= AK8963_ASA[0];
  mag_sensadj[1] *= AK8963_ASA[1];
  mag_sensadj[2] *= AK8963_ASA[2];
#if ENABLE_DEBUG_LOG
  printf(" >>> AK8963 SENS  : %8.6f %8.6f %8.6f\r\n", mag_sensadj[0], mag_sensadj[1], mag_sensadj[2]);
#endif
}
#endif

/**
 *  @brief  MPU92_GetRawData
 */
uint32_t MPU92_GetRawData( int16_t *raw )
{
#if ENABLE_INTERNAL_MAG_AK8963
  uint8_t readBuf[22] = {0};
  MPU92_ReadRegs(MPU6500_ACCEL_XOUT_H, readBuf, 22);    /* Gyr, Acc, Mag, Temp */
#else
  uint8_t readBuf[14] = {0};
  MPU92_ReadRegs(MPU6500_ACCEL_XOUT_H, readBuf, 14);    /* Gyr, Acc, Temp */
#endif

  raw[0] = (int16_t)(readBuf[6]  << 8) | readBuf[7];   /* ICTemp */
  raw[1] = (int16_t)(readBuf[8]  << 8) | readBuf[9];   /* Gyr.X */
  raw[2] = (int16_t)(readBuf[10] << 8) | readBuf[11];  /* Gyr.Y */
  raw[3] = (int16_t)(readBuf[12] << 8) | readBuf[13];  /* Gyr.Z */
  raw[4] = (int16_t)(readBuf[0]  << 8) | readBuf[1];   /* Acc.X */
  raw[5] = (int16_t)(readBuf[2]  << 8) | readBuf[3];   /* Acc.Y */
  raw[6] = (int16_t)(readBuf[4]  << 8) | readBuf[5];   /* Acc.Z */

#if ENABLE_INTERNAL_MAG_AK8963
  raw[7] = (int16_t)(readBuf[16] << 8) | readBuf[15];  /* Mag.X */
  raw[8] = (int16_t)(readBuf[18] << 8) | readBuf[17];  /* Mag.Y */
  raw[9] = (int16_t)(readBuf[20] << 8) | readBuf[19];  /* Mag.Z */
#endif

  return KS_OK;
}

/**
 *  @brief  MPU92_GetData
 */
uint32_t MPU92_GetData( float32_t *data )
{
  int16_t raw[10] = {0};
  uint32_t status;

  status = MPU92_GetRawData(raw);

  data[0] = raw[1] * gyr_sensadj[0];  // dps
  data[1] = raw[2] * gyr_sensadj[1];
  data[2] = raw[3] * gyr_sensadj[2];
  data[3] = raw[4] * acc_sensadj[0];  // g
  data[4] = raw[5] * acc_sensadj[1];
  data[5] = raw[6] * acc_sensadj[2];
#if ENABLE_INTERNAL_MAG_AK8963
  data[6] = raw[7] * mag_sensadj[0];  // uT
  data[7] = raw[8] * mag_sensadj[1];
  data[8] = raw[9] * mag_sensadj[2];
#else
  data[6] = 0;  // uT
  data[7] = 0;
  data[8] = 0;
#endif

  return status;
}

/*************************************** END OF FILE ****************************************/
