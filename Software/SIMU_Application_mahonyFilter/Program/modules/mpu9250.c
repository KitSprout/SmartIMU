/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    mpu9250.c
  * @author  KitSprout
  * @date    12-Nov-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"
#include "modules\mpu9250.h"

/** @addtogroup STM32_Module
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
#define MPU92_SPIx                SPI1
#define MPU92_SPIx_CLK_ENABLE()   __HAL_RCC_SPI1_CLK_ENABLE()

#define MPU92_CSM_PIN             GPIO_PIN_4
#define MPU92_CSM_GPIO_PORT       GPIOA
#define MPU92_CSM_H()             __GPIO_SET(MPU92_CSM_GPIO_PORT, MPU92_CSM_PIN)
#define MPU92_CSM_L()             __GPIO_RST(MPU92_CSM_GPIO_PORT, MPU92_CSM_PIN)

#define MPU92_INTM_PIN            GPIO_Pin_0
#define MPU92_INTM_GPIO_PORT      GPIOA

#define MPU92_Delay(__TIME)       delay_ms(__TIME);

#define __USE_MAG_AK8963
#define MAG_READ_DELAY 256

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  MPU92_WriteReg
  * @param  writeAddr: 
  * @param  writeData: 
  * @retval None
  */
void MPU92_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
  MPU92_CSM_L();
  SPI_RW(MPU92_SPIx, writeAddr);
  SPI_RW(MPU92_SPIx, writeData);
  MPU92_CSM_H();
}

/**
  * @brief  MPU92_WriteRegs
  * @param  writeAddr: 
  * @param  writeData: 
  * @param  lens: 
  * @retval None
  */
void MPU92_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
  MPU92_CSM_L();
  SPI_RW(MPU92_SPIx, writeAddr);
  for (uint8_t i = 0; i < lens; i++) {
    SPI_RW(MPU92_SPIx, writeData[i]);
  }
  MPU92_CSM_H();
}

/**
  * @brief  MPU92_ReadReg
  * @param  readAddr: 
  * @retval read data
  */
uint8_t MPU92_ReadReg( uint8_t readAddr )
{
  uint8_t readData = 0;

  MPU92_CSM_L();
  SPI_RW(MPU92_SPIx, 0x80 | readAddr);
  readData = SPI_RW(MPU92_SPIx, 0x00);
  MPU92_CSM_H();

  return readData;
}

/**
  * @brief  MPU92_ReadRegs
  * @param  readAddr: 
  * @param  readData: 
  * @param  lens: 
  * @retval None
  */
void MPU92_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
  MPU92_CSM_L();
  SPI_RW(MPU92_SPIx, 0x80 | readAddr);
  for (uint8_t i = 0; i < lens; i++) {
    readData[i] = SPI_RW(MPU92_SPIx, 0x00);
  }
  MPU92_CSM_H();
}

/**
  * @brief  MPU92_Mag_WriteReg
  * @param  writeAddr: 
  * @param  writeData: 
  * @retval None
  */
void MPU92_Mag_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
  uint8_t  status = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU92_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR);
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_SLV4_REG, writeAddr);
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_SLV4_DO, writeData);
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
  MPU92_Delay(1);

  do {
    status = MPU92_ReadReg(MPU6500_I2C_MST_STATUS);
    MPU92_Delay(1);
  } while (((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));
}

/**
  * @brief  MPU92_Mag_WriteRegs
  * @param  writeAddr: 
  * @param  writeData: 
  * @param  lens: 
  * @retval None
  */
void MPU92_Mag_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
  uint8_t  status = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU92_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR);
  MPU92_Delay(1);
  for (uint8_t i = 0; i < lens; i++) {
    MPU92_WriteReg(MPU6500_I2C_SLV4_REG, writeAddr + i);
    MPU92_Delay(1);
    MPU92_WriteReg(MPU6500_I2C_SLV4_DO, writeData[i]);
    MPU92_Delay(1);
    MPU92_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
    MPU92_Delay(1);

    do {
      status = MPU92_ReadReg(MPU6500_I2C_MST_STATUS);
    } while (((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));
  }
}

/**
  * @brief  MPU92_Mag_ReadReg
  * @param  readAddr: 
  * @retval read data
  */
uint8_t MPU92_Mag_ReadReg( uint8_t readAddr )
{
  uint8_t status = 0;
  uint8_t readData = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU92_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR | 0x80);
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_SLV4_REG, readAddr);
  MPU92_Delay(1);
  MPU92_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
  MPU92_Delay(1);

  do {
    status = MPU92_ReadReg(MPU6500_I2C_MST_STATUS);
    MPU92_Delay(1);
  } while (((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));

  readData = MPU92_ReadReg(MPU6500_I2C_SLV4_DI);

  return readData;
}

/**
  * @brief  MPU92_Mag_ReadRegs
  * @param  readAddr: 
  * @param  readData: 
  * @param  lens: 
  * @retval None
  */
void MPU92_Mag_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
  uint8_t status = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU92_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR | 0x80);
  MPU92_Delay(1);
  for (uint8_t i = 0; i< lens; i++) {
    MPU92_WriteReg(MPU6500_I2C_SLV4_REG, readAddr + i);
    MPU92_Delay(1);
    MPU92_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
    MPU92_Delay(1);

    do {
      status = MPU92_ReadReg(MPU6500_I2C_MST_STATUS);
    } while (((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));

    readData[i] = MPU92_ReadReg(MPU6500_I2C_SLV4_DI);
    MPU92_Delay(1);
  }
}

/**
  * @brief  MPU92_Config
  * @param  IMUx: 
  * @retval SPIx_BASE
  */
uint32_t MPU92_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* SPI Clk ******************************************************************/
  MPU92_SPIx_CLK_ENABLE();

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin   = MPU92_CSM_PIN;
  HAL_GPIO_Init(MPU92_CSM_GPIO_PORT, &GPIO_InitStruct);

  MPU92_CSM_H();    // LOW ENABLE

  return ((uint32_t)MPU92_SPIx);
}

/**
  * @brief  MPU92_Init
  * @param  IMUx: 
  * @retval state 
  */
#define MPU6500_InitRegNum  11
#define AK8963_InitRegNum   5
int8_t MPU92_Init( MPU_ConfigTypeDef *IMUx )
{
  int8_t status = ERROR;
  uint8_t MPU6500_InitData[MPU6500_InitRegNum][2] = {
    {0x80, MPU6500_PWR_MGMT_1},     /* [0]  Reset Device                  */
    {0x04, MPU6500_PWR_MGMT_1},     /* [1]  Clock Source                  */
    {0x10, MPU6500_INT_PIN_CFG},    /* [2]  Set INT_ANYRD_2CLEAR          */
    {0x01, MPU6500_INT_ENABLE},     /* [3]  Set RAW_RDY_EN                */
    {0x00, MPU6500_PWR_MGMT_2},     /* [4]  Enable Acc & Gyro             */
    {0x00, MPU6500_SMPLRT_DIV},     /* [5]  Sample Rate Divider           */
    {0x00, MPU6500_GYRO_CONFIG},    /* [6]  default : +-250dps            */
    {0x00, MPU6500_ACCEL_CONFIG},   /* [7]  default : +-2G                */
    {0x00, MPU6500_CONFIG},         /* [8]  default : GyrLPS_250Hz        */
    {0x00, MPU6500_ACCEL_CONFIG_2}, /* [9]  default : AccLPS_460Hz        */
    {0x30, MPU6500_USER_CTRL},      /* [10] Set I2C_MST_EN, I2C_IF_DIS    */
  };
  uint8_t AK8963_InitData[AK8963_InitRegNum][2] = {
    {0x01, AK8963_CNTL2},           /* [0]  Reset Device                  */
    {0x00, AK8963_CNTL1},           /* [1]  Power-down mode               */
    {0x0F, AK8963_CNTL1},           /* [2]  Fuse ROM access mode          */
                                    /*      Read sensitivity adjustment   */
    {0x00, AK8963_CNTL1},           /* [3]  Power-down mode               */
    {0x06, AK8963_CNTL1},           /* [4]  Continuous measurement mode 2 */
  };

  MPU6500_InitData[6][0] = IMUx->MPU_Gyr_FullScale;       /* [6] MPU6500_GYRO_CONFIG */
  MPU6500_InitData[7][0] = IMUx->MPU_Acc_FullScale;       /* [7] MPU6500_ACCEL_CONFIG */
  MPU6500_InitData[8][0] = IMUx->MPU_Gyr_LowPassFilter;   /* [8] MPU6500_CONFIG */
  MPU6500_InitData[9][0] = IMUx->MPU_Acc_LowPassFilter;   /* [9] MPU6500_ACCEL_CONFIG_2 */

  for (uint32_t i = 0; i < MPU6500_InitRegNum; i++) {
    MPU92_Delay(2);
    MPU92_WriteReg(MPU6500_InitData[i][1], MPU6500_InitData[i][0]);
  }

  MPU92_Delay(2);
  status = MPU92_DeviceCheck();
  if (status != SUCCESS)
    return ERROR;

#if defined(__USE_MAG_AK8963)
  AK8963_InitData[4][0] |= IMUx->MPU_Mag_FullScale;        /* [4] AK8963_CNTL1 */

  MPU92_Delay(2);
  MPU92_Mag_WriteReg(AK8963_InitData[0][1], AK8963_InitData[0][0]);
  MPU92_Delay(2);
  MPU92_Mag_WriteReg(AK8963_InitData[1][1], AK8963_InitData[1][0]);
  MPU92_Delay(2);
  MPU92_Mag_WriteReg(AK8963_InitData[2][1], AK8963_InitData[2][0]);
#if 0   /* ASA read */
  uint8_t ASA[3] = {0};
  float32_t sensAdj[3] = {0};

  MPU92_Delay(2);
  MPU92_Mag_ReadRegs(AK8963_ASAX, ASA, 3);
  for (uint8_t i = 0; i < 3; i++) {
    sensAdj[i] = (ASA[i] + 128) / 256.0;
  }
//  printf("ASA = %i, %i, %i\r\n", ASA[0], ASA[1], ASA[2]);
//  printf("sensAdj = %f, %f, %f\r\n", sensAdj[0], sensAdj[1], sensAdj[2]);
#endif
  MPU92_Delay(2);
  MPU92_Mag_WriteReg(AK8963_InitData[3][1], AK8963_InitData[3][0]);
  MPU92_Delay(2);
  MPU92_Mag_WriteReg(AK8963_InitData[4][1], AK8963_InitData[4][0]);

  /* config mpu9250 i2c */
  MPU92_Delay(2);
  MPU92_WriteReg(MPU6500_I2C_MST_CTRL, 0x5D);
  MPU92_Delay(2);
  MPU92_WriteReg(MPU6500_I2C_SLV0_ADDR, AK8963_I2C_ADDR | 0x80);
  MPU92_Delay(2);
  MPU92_WriteReg(MPU6500_I2C_SLV0_REG, AK8963_ST1);
  MPU92_Delay(2);
  MPU92_WriteReg(MPU6500_I2C_SLV0_CTRL, MPU6500_I2C_SLVx_EN | 8);
  MPU92_Delay(2);
  MPU92_WriteReg(MPU6500_I2C_SLV4_CTRL, 0x09);
  MPU92_Delay(2);
  MPU92_WriteReg(MPU6500_I2C_MST_DELAY_CTRL, 0x81);
#endif

  MPU92_Delay(100);

  return SUCCESS;
}

int8_t MPU92_DeviceCheck( void )
{
  uint8_t deviceID = 0x00;

  deviceID = MPU92_ReadReg(MPU6500_WHO_AM_I);
  if (deviceID != MPU6500_DeviceID) {
    return ERROR;
  }

#if defined(__USE_MAG_AK8963)
  MPU92_Delay(10);
  deviceID = MPU92_Mag_ReadReg(AK8963_WIA);
  if (deviceID != AK8963_DeviceID) {
    return ERROR;
  }
#endif

  return SUCCESS;
}

/**
  * @brief  MPU92_GetRawData
  * @param  sensitivity: point to float32_t
            sensitivity[0] - gyro
            sensitivity[1] - accel
            sensitivity[2] - magnetic
            sensitivity[3] - temperature scale
            sensitivity[4] - temperature offset
  * @retval None
  */
void MPU92_GetSensitivity( MPU_ConfigTypeDef *IMUx, float32_t *sensitivity )
{
  float64_t scale;

  /* Set gyroscope sensitivity (dps/LSB) */
#if defined(__USE_GYROSCOPE)
  switch (IMUx->MPU_Gyr_FullScale) {
    case MPU_GyrFS_250dps:    scale = 250.0;    break;
    case MPU_GyrFS_500dps:    scale = 500.0;    break;
    case MPU_GyrFS_1000dps:   scale = 1000.0;   break;
    case MPU_GyrFS_2000dps:   scale = 2000.0;   break;
    default:                  scale = 0.0;      break;
  }
  sensitivity[0] = scale / 32768.0;
#else
  sensitivity[0] = 0.0f;
#endif

  /* Set accelerometer sensitivity (g/LSB) */
#if defined(__USE_ACCELEROMETER)
  switch (IMUx->MPU_Acc_FullScale) {
    case MPU_AccFS_2g:    scale = 2.0;    break;
    case MPU_AccFS_4g:    scale = 4.0;    break;
    case MPU_AccFS_8g:    scale = 8.0;    break;
    case MPU_AccFS_16g:   scale = 16.0;   break;
    default:              scale = 0.0f;   break;
  }
  sensitivity[1] = scale / 32768.0;
#else
  sensitivity[1] = 0.0f;
#endif

  /* Set magnetometer sensitivity (uT/LSB) */
#if defined(__USE_MAGNETOMETER)
  sensitivity[2] = 0.6;  /* +-4800uT */
#else
  sensitivity[2] = 0.0f;
#endif

  /* Set ictemperature sensitivity (degC/LSB) */
#if defined(__USE_ICTEMPERATURE)
  sensitivity[3] = 1.0 / 333.87;
  sensitivity[4] = 21.0f;
#else
  sensitivity[3] = 0.0f;
  sensitivity[4] = 0.0f;
#endif
}

/**
  * @brief  MPU92_GetRawData
  * @param  data: point to int16_t
  * @retval return 1 : AK8963 data update
            return 0 : AK8963 data not update
  */
int8_t MPU92_GetRawData( int16_t *data )
{
#if defined(__USE_MAG_AK8963)
  uint8_t readBuf[22] = {0};
  MPU92_ReadRegs(MPU6500_ACCEL_XOUT_H, readBuf, 22);    /* Read Gyr, Acc, Mag */
#else
  uint8_t readBuf[14] = {0};
  MPU92_ReadRegs(MPU6500_ACCEL_XOUT_H, readBuf, 14);    /* Read Gyr, Acc */
#endif

  data[0] = (int16_t)(readBuf[8]  << 8) | readBuf[9];   /* Gyr.X */
  data[1] = (int16_t)(readBuf[10] << 8) | readBuf[11];  /* Gyr.Y */
  data[2] = (int16_t)(readBuf[12] << 8) | readBuf[13];  /* Gyr.Z */
  data[3] = (int16_t)(readBuf[0]  << 8) | readBuf[1];   /* Acc.X */
  data[4] = (int16_t)(readBuf[2]  << 8) | readBuf[3];   /* Acc.Y */
  data[5] = (int16_t)(readBuf[4]  << 8) | readBuf[5];   /* Acc.Z */
  data[6] = (int16_t)(readBuf[6]  << 8) | readBuf[7];   /* ICTemp */

#if defined(__USE_MAG_AK8963)
  if (!(!(readBuf[14] & AK8963_STATUS_DRDY) || (readBuf[14] & AK8963_STATUS_DOR) || (readBuf[21] & AK8963_STATUS_HOFL))) {
    data[7] = (int16_t)(readBuf[16] << 8) | readBuf[15];  /* Mag.X */
    data[8] = (int16_t)(readBuf[18] << 8) | readBuf[17];  /* Mag.Y */
    data[9] = (int16_t)(readBuf[20] << 8) | readBuf[19];  /* Mag.Z */
    return 1;
  }
#endif
  return 0;
}

/*************************************** END OF FILE ****************************************/
