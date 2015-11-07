/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"

#include "module_mpu9250.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define SPIx                        SPI1
#define SPIx_CLK_ENABLE()           __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx_SPEED_HIGH             SPI_BAUDRATEPRESCALER_32
#define SPIx_SPEED_LOW              SPI_BAUDRATEPRESCALER_256

#define SPIx_CS_PIN                 GPIO_PIN_4
#define SPIx_CS_GPIO_PORT           GPIOA
#define SPIx_CS_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_CS_AF                  GPIO_AF5_SPI1
#define SPIx_CS_H()                 __GPIO_SET(SPIx_CS_GPIO_PORT, SPIx_CS_PIN)
#define SPIx_CS_L()                 __GPIO_RST(SPIx_CS_GPIO_PORT, SPIx_CS_PIN)

#define SPIx_SCK_PIN                GPIO_PIN_5
#define SPIx_SCK_GPIO_PORT          GPIOA
#define SPIx_SCK_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_SCK_AF                 GPIO_AF5_SPI1

#define SPIx_SDO_PIN                GPIO_PIN_6
#define SPIx_SDO_GPIO_PORT          GPIOA
#define SPIx_SDO_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_SDO_AF                 GPIO_AF5_SPI1

#define SPIx_SDI_PIN                GPIO_PIN_7
#define SPIx_SDI_GPIO_PORT          GPIOA
#define SPIx_SDI_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_SDI_AF                 GPIO_AF5_SPI1

#define SPIx_INTM_PIN               GPIO_PIN_1
#define SPIx_INTM_GPIO_PORT         GPIOA
#define SPIx_INTM_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_INTM_IRQ               EXTI1_IRQn

#define _USE_MAG_AK8963
#define MAG_READ_DELAY 256
/*====================================================================================================*/
/*====================================================================================================*/
static SPI_HandleTypeDef MPU_HandleStruct;
#ifdef _USE_MAG_AK8963
static int16_t AK8963_ASA[3] = {0};
#endif
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_WriteReg
**功能 : Write Reg
**輸入 : writeAddr, writeData
**輸出 : None
**使用 : MPU9250_WriteReg(writeAddr, writeData);
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
  SPIx_CS_L();
  SPI_RW(SPIx, writeAddr);
  SPI_RW(SPIx, writeData);
  SPIx_CS_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_WriteRegs
**功能 : Write Regs
**輸入 : writeAddr, *writeData, lens
**輸出 : None
**使用 : MPU9250_WriteRegs(writeAddr, writeData, lens);
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
  SPIx_CS_L();
  SPI_RW(SPIx, writeAddr);
  for(uint8_t i = 0; i < lens; i++)
    SPI_RW(SPIx, writeData[i]);
  SPIx_CS_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_ReadReg
**功能 : Read Reg
**輸入 : readAddr
**輸出 : readData
**使用 : readData = MPU9250_ReadReg(readAddr);
**====================================================================================================*/
/*====================================================================================================*/
uint8_t MPU9250_ReadReg( uint8_t readAddr )
{
  uint8_t readData = 0;

  SPIx_CS_L();
  SPI_RW(SPIx, 0x80 | readAddr);
  readData = SPI_RW(SPIx, 0x00);
  SPIx_CS_H();

  return readData;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : MPU9250_ReadRegs
**功能 : Read Regs
**輸入 : readAddr, *readData, lens
**輸出 : None
**使用 : MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, readData, 14);
**=====================================================================================================*/
/*=====================================================================================================*/
void MPU9250_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
  SPIx_CS_L();
  SPI_RW(SPIx, 0x80 | readAddr);
  for(uint8_t i = 0; i < lens; i++)
    readData[i] = SPI_RW(SPIx, 0x00);
  SPIx_CS_H();
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : MPU9250_Mag_WriteReg
**功能 : AK8963 Write Reg
**輸入 : writeAddr, writeData
**輸出 : None
**使用 : MPU9250_Mag_WriteReg(AK8963_CNTL2, 0x01);
**=====================================================================================================*/
/*=====================================================================================================*/
#ifdef _USE_MAG_AK8963
void MPU9250_Mag_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
  uint8_t  status = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU9250_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR);
  Delay_1ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_REG, writeAddr);
  Delay_1ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_DO, writeData);
  Delay_1ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
  Delay_1ms(1);

  do {
    status = MPU9250_ReadReg(MPU6500_I2C_MST_STATUS);
    Delay_1ms(1);
  } while(((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));
}
#endif
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : MPU9250_Mag_WriteRegs
**功能 : AK8963 Write Regs
**輸入 : writeAddr, *writeData, lens
**輸出 : None
**使用 : MPU9250_Mag_WriteRegs(AK8963_CNTL2, 0x01);
**=====================================================================================================*/
/*=====================================================================================================*/
#ifdef _USE_MAG_AK8963
void MPU9250_Mag_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
  uint8_t  status = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU9250_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR);
  Delay_1ms(1);
  for(uint8_t i = 0; i < lens; i++) {
    MPU9250_WriteReg(MPU6500_I2C_SLV4_REG, writeAddr + i);
    Delay_1ms(1);
    MPU9250_WriteReg(MPU6500_I2C_SLV4_DO, writeData[i]);
    Delay_1ms(1);
    MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
    Delay_1ms(1);

    do {
      status = MPU9250_ReadReg(MPU6500_I2C_MST_STATUS);
    } while(((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));
  }
}
#endif
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : MPU9250_Mag_ReadReg
**功能 : AK8963 Read Reg
**輸入 : readAddr
**輸出 : readData
**使用 : DeviceID = MPU9250_Mag_ReadReg(AK8963_WIA);
**=====================================================================================================*/
/*=====================================================================================================*/
#ifdef _USE_MAG_AK8963
uint8_t MPU9250_Mag_ReadReg( uint8_t readAddr )
{
  uint8_t status = 0;
  uint8_t readData = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU9250_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR | 0x80);
  Delay_1ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_REG, readAddr);
  Delay_1ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
  Delay_1ms(1);

  do {
    status = MPU9250_ReadReg(MPU6500_I2C_MST_STATUS);
    Delay_1ms(1);
  } while(((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));

  readData = MPU9250_ReadReg(MPU6500_I2C_SLV4_DI);

  return readData;
}
#endif
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : MPU9250_Mag_ReadRegs
**功能 : AK8963 Read Regs
**輸入 : readAddr, *readData, lens
**輸出 : None
**使用 : DeviceID = MPU9250_Mag_ReadRegs(AK8963_WIA);
**=====================================================================================================*/
/*=====================================================================================================*/
#ifdef _USE_MAG_AK8963
void MPU9250_Mag_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
  uint8_t status = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU9250_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR | 0x80);
  Delay_1ms(1);
  for(uint8_t i = 0; i< lens; i++) {
    MPU9250_WriteReg(MPU6500_I2C_SLV4_REG, readAddr + i);
    Delay_1ms(1);
    MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
    Delay_1ms(1);

    do {
      status = MPU9250_ReadReg(MPU6500_I2C_MST_STATUS);
    } while(((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));

    readData[i] = MPU9250_ReadReg(MPU6500_I2C_SLV4_DI);
    Delay_1ms(1);
  }
}
#endif
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_SetSpeed
**功能 : 設置 SPI 速度
**輸入 : SpeedSel
**輸出 : None
**使用 : MPU9250_SetSpeed(SPIx_SPEED_LOW);
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_SetSpeed( uint8_t SpeedSel )
{
  __HAL_SPI_DISABLE(&MPU_HandleStruct);
  MPU_HandleStruct.Init.BaudRatePrescaler = SpeedSel;
  HAL_SPI_Init(&MPU_HandleStruct);
  __HAL_SPI_ENABLE(&MPU_HandleStruct);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Config
**功能 : 初始化 MPU9250
**輸入 : None
**輸出 : None
**使用 : MPU9250_Config();
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* SPI Clk ******************************************************************/
  SPIx_CS_GPIO_CLK_ENABLE();
  SPIx_SCK_GPIO_CLK_ENABLE();
  SPIx_SDO_GPIO_CLK_ENABLE();
  SPIx_SDI_GPIO_CLK_ENABLE();
  SPIx_INTM_GPIO_CLK_ENABLE();

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin       = SPIx_CS_PIN;
  HAL_GPIO_Init(SPIx_CS_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
  GPIO_InitStruct.Alternate = SPIx_SCK_AF;
  HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = SPIx_SDO_PIN;
  GPIO_InitStruct.Alternate = SPIx_SDO_AF;
  HAL_GPIO_Init(SPIx_SDO_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = SPIx_SDI_PIN;
  GPIO_InitStruct.Alternate = SPIx_SDI_AF;
  HAL_GPIO_Init(SPIx_SDI_GPIO_PORT, &GPIO_InitStruct);

  SPIx_CS_H();  // 低電位有效

  // EXIT Config
  GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Pin   = SPIx_INTM_PIN;
  HAL_GPIO_Init(SPIx_INTM_GPIO_PORT, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(SPIx_INTM_IRQ, 14, 0);
  HAL_NVIC_EnableIRQ(SPIx_INTM_IRQ);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_SPI_Config
**功能 : 初始化 MPU9250
**輸入 : None
**輸出 : None
**使用 : MPU9250_SPI_Config();
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_SPI_Config( void )
{
  /* SPI Clk ******************************************************************/
  SPIx_CLK_ENABLE();

  /* SPI Init ****************************************************************/
  MPU_HandleStruct.Instance               = SPIx;
  MPU_HandleStruct.Init.Mode              = SPI_MODE_MASTER;
  MPU_HandleStruct.Init.Direction         = SPI_DIRECTION_2LINES;
  MPU_HandleStruct.Init.DataSize          = SPI_DATASIZE_8BIT;
  MPU_HandleStruct.Init.CLKPolarity       = SPI_POLARITY_HIGH;
  MPU_HandleStruct.Init.CLKPhase          = SPI_PHASE_2EDGE;
  MPU_HandleStruct.Init.NSS               = SPI_NSS_SOFT;
  MPU_HandleStruct.Init.BaudRatePrescaler = SPIx_SPEED_LOW;
  MPU_HandleStruct.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  MPU_HandleStruct.Init.TIMode            = SPI_TIMODE_DISABLE;
  MPU_HandleStruct.Init.CRCCalculation    = SPI_CRCCALCULATION_ENABLE;
  MPU_HandleStruct.Init.CRCPolynomial     = 7;
  HAL_SPI_Init(&MPU_HandleStruct);

  __HAL_SPI_ENABLE(&MPU_HandleStruct);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Init
**功能 : 初始化 MPU9250
**輸入 : None
**輸出 : status
**使用 : status = MPU9250_Init(&MPU_InitStruct);
**====================================================================================================*/
/*====================================================================================================*/
#define MPU6500_InitRegNum 11
uint8_t MPU9250_Init( MPU_InitTypeDef *MPUx )
{
  uint8_t status = ERROR;
#ifdef _USE_MAG_AK8963
  uint8_t tmpRead[3] = {0};
#endif
  uint8_t MPU6500_InitData[MPU6500_InitRegNum][2] = {
    {0x80, MPU6500_PWR_MGMT_1},     // [0]  Reset Device
    {0x04, MPU6500_PWR_MGMT_1},     // [1]  Clock Source
    {0x10, MPU6500_INT_PIN_CFG},    // [2]  Set INT_ANYRD_2CLEAR
    {0x01, MPU6500_INT_ENABLE},     // [3]  Set RAW_RDY_EN
    {0x00, MPU6500_PWR_MGMT_2},     // [4]  Enable Acc & Gyro
    {0x00, MPU6500_SMPLRT_DIV},     // [5]  Sample Rate Divider
    {0x18, MPU6500_GYRO_CONFIG},    // [6]  default : +-2000dps
    {0x08, MPU6500_ACCEL_CONFIG},   // [7]  default : +-4G
    {0x07, MPU6500_CONFIG},         // [8]  default : LPS_41Hz
    {0x03, MPU6500_ACCEL_CONFIG_2}, // [9]  default : LPS_41Hz
    {0x30, MPU6500_USER_CTRL},      // [10] Set I2C_MST_EN, I2C_IF_DIS
  };

  MPU9250_GPIO_Config();
  MPU9250_SPI_Config();

  MPU6500_InitData[6][0] = MPUx->MPU_Gyr_FullScale;       // MPU6500_GYRO_CONFIG
  MPU6500_InitData[8][0] = MPUx->MPU_Gyr_LowPassFilter;   // MPU6500_CONFIG
  MPU6500_InitData[7][0] = MPUx->MPU_Acc_FullScale;       // MPU6500_ACCEL_CONFIG
  MPU6500_InitData[9][0] = MPUx->MPU_Acc_LowPassFilter;   // MPU6500_ACCEL_CONFIG_2

  for(uint8_t i = 0; i < MPU6500_InitRegNum; i++) {
    MPU9250_WriteReg(MPU6500_InitData[i][1], MPU6500_InitData[i][0]);
    Delay_1ms(1);
  }

  status = MPU9250_Check();
  if(status != SUCCESS)
    return ERROR;

  Delay_10ms(1);

#ifdef _USE_MAG_AK8963
  MPU9250_Mag_WriteReg(AK8963_CNTL2, 0x01);       // Reset Device
  Delay_1ms(1);
  MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x10);       // Power-down mode
  Delay_1ms(1);
  MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x1F);       // Fuse ROM access mode
  Delay_1ms(1);
  MPU9250_Mag_ReadRegs(AK8963_ASAX, tmpRead, 3);  // Read sensitivity adjustment values
  Delay_1ms(1);
  MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x10);       // Power-down mode
  Delay_1ms(1);

	AK8963_ASA[0] = (int16_t)(tmpRead[0]) + 128;
	AK8963_ASA[1] = (int16_t)(tmpRead[1]) + 128;
	AK8963_ASA[2] = (int16_t)(tmpRead[2]) + 128;

  MPU9250_WriteReg(MPU6500_I2C_MST_CTRL, 0x5D);
  Delay_1ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV0_ADDR, AK8963_I2C_ADDR | 0x80);
  Delay_1ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV0_REG, AK8963_ST1);
  Delay_1ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV0_CTRL, MPU6500_I2C_SLVx_EN | 8);
  Delay_1ms(1);

  MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x16);       // Continuous measurement mode 2
  Delay_1ms(1);

	MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, 0x09);
	Delay_1ms(1);
	MPU9250_WriteReg(MPU6500_I2C_MST_DELAY_CTRL, 0x81);
	Delay_100ms(1);
#endif

  MPU9250_SetSpeed(SPIx_SPEED_HIGH);
  Delay_10ms(1);

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Check
**功能 : check Device ID
**輸入 : None
**輸出 : Status
**使用 : Status = MPU9250_Check();
**====================================================================================================*/
/*====================================================================================================*/
uint8_t MPU9250_Check( void )
{
  uint8_t DeviceID = ERROR;

  DeviceID = MPU9250_ReadReg(MPU6500_WHO_AM_I);
  if(DeviceID != MPU6500_Device_ID)
    return ERROR;

#ifdef _USE_MAG_AK8963
  DeviceID = MPU9250_Mag_ReadReg(AK8963_WIA);
  if(DeviceID != AK8963_Device_ID)
    return ERROR;
#endif

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_getData
**功能 : read IMU data
**輸入 : *dataIMU
**輸出 : None
**使用 : MPU9250_getData(dataIMU);
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_getData( int16_t *dataIMU )
{
  uint8_t tmpRead[22] = {0};

#ifdef _USE_MAG_AK8963
  MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, tmpRead, 22);
#else
  MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, tmpRead, 14);
#endif

  dataIMU[0] = (Byte16(int16_t, tmpRead[6],  tmpRead[7]));    // Temp
  dataIMU[1] = (Byte16(int16_t, tmpRead[0],  tmpRead[1]));    // Acc.X
  dataIMU[2] = (Byte16(int16_t, tmpRead[2],  tmpRead[3]));    // Acc.Y
  dataIMU[3] = (Byte16(int16_t, tmpRead[4],  tmpRead[5]));    // Acc.Z
  dataIMU[4] = (Byte16(int16_t, tmpRead[8],  tmpRead[9]));    // Gyr.X
  dataIMU[5] = (Byte16(int16_t, tmpRead[10], tmpRead[11]));   // Gyr.Y
  dataIMU[6] = (Byte16(int16_t, tmpRead[12], tmpRead[13]));   // Gyr.Z
#ifdef _USE_MAG_AK8963
  if(!(tmpRead[14] & AK8963_STATUS_DRDY) || (tmpRead[14] & AK8963_STATUS_DOR) || (tmpRead[21] & AK8963_STATUS_HOFL))
    return;

  dataIMU[7] = (Byte16(int16_t, tmpRead[16], tmpRead[15]));   // Mag.X
  dataIMU[8] = (Byte16(int16_t, tmpRead[18], tmpRead[17]));   // Mag.Y
  dataIMU[9] = (Byte16(int16_t, tmpRead[20], tmpRead[19]));   // Mag.Z

//  dataIMU[7] = ((long)dataIMU[7] * AK8963_ASA[0]) >> 8;
//  dataIMU[8] = ((long)dataIMU[8] * AK8963_ASA[1]) >> 8;
//  dataIMU[9] = ((long)dataIMU[9] * AK8963_ASA[2]) >> 8;
#endif  
}
/*==============================================================================================*/
/*==============================================================================================*/
