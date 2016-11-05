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
  * @date    11-Oct-2016
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
#define IMU_SPIx                    SPI3
#define IMU_SPIx_CLK_ENABLE()       RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE)

#define IMU_CSM_PIN                 GPIO_Pin_15
#define IMU_CSM_GPIO_PORT           GPIOA
#define IMU_CSM_H()                 __GPIO_SET(IMU_CSM_GPIO_PORT, IMU_CSM_PIN)
#define IMU_CSM_L()                 __GPIO_RST(IMU_CSM_GPIO_PORT, IMU_CSM_PIN)

#define __USE_MAG_AK8963
#define MAG_READ_DELAY 256

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  MPU9250_WriteReg
  * @param  writeAddr: 
  * @param  writeData: 
  * @retval None
  */
void MPU9250_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
  IMU_CSM_L();
  SPI_RW(IMU_SPIx, writeAddr);
  SPI_RW(IMU_SPIx, writeData);
  IMU_CSM_H();
}

/**
  * @brief  MPU9250_WriteRegs
  * @param  writeAddr: 
  * @param  writeData: 
  * @param  lens: 
  * @retval None
  */
void MPU9250_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
  IMU_CSM_L();
  SPI_RW(IMU_SPIx, writeAddr);
  for(uint8_t i = 0; i < lens; i++) {
    SPI_RW(IMU_SPIx, writeData[i]);
  }
  IMU_CSM_H();
}

/**
  * @brief  MPU9250_ReadReg
  * @param  readAddr: 
  * @retval read data
  */
uint8_t MPU9250_ReadReg( uint8_t readAddr )
{
  uint8_t readData = 0;

  IMU_CSM_L();
  SPI_RW(IMU_SPIx, 0x80 | readAddr);
  readData = SPI_RW(IMU_SPIx, 0x00);
  IMU_CSM_H();

  return readData;
}

/**
  * @brief  MPU9250_ReadRegs
  * @param  readAddr: 
  * @param  readData: 
  * @param  lens: 
  * @retval None
  */
void MPU9250_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
  IMU_CSM_L();
  SPI_RW(IMU_SPIx, 0x80 | readAddr);
  for (uint8_t i = 0; i < lens; i++) {
    readData[i] = SPI_RW(IMU_SPIx, 0x00);
  }
  IMU_CSM_H();
}

/**
  * @brief  MPU9250_Mag_WriteReg
  * @param  writeAddr: 
  * @param  writeData: 
  * @retval None
  */
void MPU9250_Mag_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
  uint8_t  status = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU9250_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_REG, writeAddr);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_DO, writeData);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
  delay_ms(1);

  do {
    status = MPU9250_ReadReg(MPU6500_I2C_MST_STATUS);
    delay_ms(1);
  } while(((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));
}

/**
  * @brief  MPU9250_Mag_WriteRegs
  * @param  writeAddr: 
  * @param  writeData: 
  * @param  lens: 
  * @retval None
  */
void MPU9250_Mag_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
  uint8_t  status = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU9250_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR);
  delay_ms(1);
  for (uint8_t i = 0; i < lens; i++) {
    MPU9250_WriteReg(MPU6500_I2C_SLV4_REG, writeAddr + i);
    delay_ms(1);
    MPU9250_WriteReg(MPU6500_I2C_SLV4_DO, writeData[i]);
    delay_ms(1);
    MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
    delay_ms(1);

    do {
      status = MPU9250_ReadReg(MPU6500_I2C_MST_STATUS);
    } while (((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));
  }
}

/**
  * @brief  MPU9250_Mag_ReadReg
  * @param  readAddr: 
  * @retval read data
  */
uint8_t MPU9250_Mag_ReadReg( uint8_t readAddr )
{
  uint8_t status = 0;
  uint8_t readData = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU9250_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR | 0x80);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_REG, readAddr);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
  delay_ms(1);

  do {
    status = MPU9250_ReadReg(MPU6500_I2C_MST_STATUS);
    delay_ms(1);
  } while (((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));

  readData = MPU9250_ReadReg(MPU6500_I2C_SLV4_DI);

  return readData;
}

/**
  * @brief  MPU9250_Mag_ReadRegs
  * @param  readAddr: 
  * @param  readData: 
  * @param  lens: 
  * @retval None
  */
void MPU9250_Mag_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
  uint8_t status = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU9250_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR | 0x80);
  delay_ms(1);
  for (uint8_t i = 0; i< lens; i++) {
    MPU9250_WriteReg(MPU6500_I2C_SLV4_REG, readAddr + i);
    delay_ms(1);
    MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
    delay_ms(1);

    do {
      status = MPU9250_ReadReg(MPU6500_I2C_MST_STATUS);
    } while (((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));

    readData[i] = MPU9250_ReadReg(MPU6500_I2C_SLV4_DI);
    delay_ms(1);
  }
}

/*************************************** END OF FILE ****************************************/
