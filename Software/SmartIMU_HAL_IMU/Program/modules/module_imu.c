/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"

#include "module_imu.h"
#include "module_mpu9250.h"
#include "module_lps25h.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define _USE_MAGNETOMETER
#define _USE_BAROMETER
/*====================================================================================================*/
/*====================================================================================================*/
#define IMU_SPIx                    SPI1
#define IMU_SPIx_CLK_ENABLE()       __HAL_RCC_SPI1_CLK_ENABLE()
#define IMU_SPIx_SPEED_HIGH         SPI_BAUDRATEPRESCALER_4
#define IMU_SPIx_SPEED_LOW          SPI_BAUDRATEPRESCALER_256

#define IMU_CSM_PIN                 GPIO_PIN_4
#define IMU_CSM_GPIO_PORT           GPIOA
#define IMU_CSM_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define IMU_CSM_AF                  GPIO_AF5_SPI1
#define IMU_CSM_H()                 __GPIO_SET(IMU_CSM_GPIO_PORT, IMU_CSM_PIN)
#define IMU_CSM_L()                 __GPIO_RST(IMU_CSM_GPIO_PORT, IMU_CSM_PIN)

#define IMU_CSB_PIN                 GPIO_PIN_1
#define IMU_CSB_GPIO_PORT           GPIOB
#define IMU_CSB_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define IMU_CSB_AF                  GPIO_AF5_SPI1
#define IMU_CSB_H()                 __GPIO_SET(IMU_CSB_GPIO_PORT, IMU_CSB_PIN)
#define IMU_CSB_L()                 __GPIO_RST(IMU_CSB_GPIO_PORT, IMU_CSB_PIN)

#define IMU_SCK_PIN                 GPIO_PIN_5
#define IMU_SCK_GPIO_PORT           GPIOA
#define IMU_SCK_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define IMU_SCK_AF                  GPIO_AF5_SPI1

#define IMU_SDO_PIN                 GPIO_PIN_6
#define IMU_SDO_GPIO_PORT           GPIOA
#define IMU_SDO_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define IMU_SDO_AF                  GPIO_AF5_SPI1

#define IMU_SDI_PIN                 GPIO_PIN_7
#define IMU_SDI_GPIO_PORT           GPIOA
#define IMU_SDI_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define IMU_SDI_AF                  GPIO_AF5_SPI1

#define IMU_INTM_PIN                GPIO_PIN_3
#define IMU_INTM_GPIO_PORT          GPIOA
#define IMU_INTM_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define IMU_INTM_IRQ                EXTI3_IRQn

#define IMU_INTB_PIN                GPIO_PIN_0
#define IMU_INTB_GPIO_PORT          GPIOB
#define IMU_INTB_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define IMU_INTB_IRQ                EXTI0_IRQn
/*====================================================================================================*/
/*====================================================================================================*/
static SPI_HandleTypeDef MPU_HandleStruct;

#ifdef _USE_MAGNETOMETER
static int16_t AK8963_ASA[3] = {0};
#endif
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_SetSpeed
**功能 : Set SPI Speed
**輸入 : SpeedSel
**輸出 : None
**使用 : IMU_SetSpeed(SPIx_SPEED_LOW);
**====================================================================================================*/
/*====================================================================================================*/
static void IMU_SetSpeed( uint8_t SpeedSel )
{
  __HAL_SPI_DISABLE(&MPU_HandleStruct);
  MPU_HandleStruct.Init.BaudRatePrescaler = SpeedSel;
  HAL_SPI_Init(&MPU_HandleStruct);
  __HAL_SPI_ENABLE(&MPU_HandleStruct);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_GPIO_Config
**功能 : GPIO Config
**輸入 : None
**輸出 : None
**使用 : IMU_GPIO_Config();
**====================================================================================================*/
/*====================================================================================================*/
void IMU_GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* SPI Clk ******************************************************************/
  IMU_CSM_GPIO_CLK_ENABLE();
  IMU_CSB_GPIO_CLK_ENABLE();
  IMU_SCK_GPIO_CLK_ENABLE();
  IMU_SDO_GPIO_CLK_ENABLE();
  IMU_SDI_GPIO_CLK_ENABLE();
  IMU_INTM_GPIO_CLK_ENABLE();
  IMU_INTB_GPIO_CLK_ENABLE();

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin       = IMU_CSM_PIN;
  HAL_GPIO_Init(IMU_CSM_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = IMU_CSB_PIN;
  HAL_GPIO_Init(IMU_CSB_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin       = IMU_SCK_PIN;
  GPIO_InitStruct.Alternate = IMU_SCK_AF;
  HAL_GPIO_Init(IMU_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = IMU_SDO_PIN;
  GPIO_InitStruct.Alternate = IMU_SDO_AF;
  HAL_GPIO_Init(IMU_SDO_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = IMU_SDI_PIN;
  GPIO_InitStruct.Alternate = IMU_SDI_AF;
  HAL_GPIO_Init(IMU_SDI_GPIO_PORT, &GPIO_InitStruct);

  /* CS Pin Set High ******************************************************************/
  IMU_CSM_H();
  IMU_CSB_H();

  // EXIT Config
  GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin   = IMU_INTM_PIN;
  HAL_GPIO_Init(IMU_INTM_GPIO_PORT, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(IMU_INTM_IRQ, 14, 0);
  HAL_NVIC_EnableIRQ(IMU_INTM_IRQ);

#ifdef _USE_BAROMETER
  GPIO_InitStruct.Pin   = IMU_INTB_PIN;
  HAL_GPIO_Init(IMU_INTB_GPIO_PORT, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(IMU_INTB_IRQ, 14, 0);
  HAL_NVIC_EnableIRQ(IMU_INTB_IRQ);
#endif
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_SPI_Config
**功能 : IMU Config
**輸入 : None
**輸出 : None
**使用 : IMU_SPI_Config();
**====================================================================================================*/
/*====================================================================================================*/
void IMU_SPI_Config( void )
{
  /* SPI Clk ******************************************************************/
  IMU_SPIx_CLK_ENABLE();

  /* SPI Init ****************************************************************/
  MPU_HandleStruct.Instance               = IMU_SPIx;
  MPU_HandleStruct.Init.Mode              = SPI_MODE_MASTER;
  MPU_HandleStruct.Init.Direction         = SPI_DIRECTION_2LINES;
  MPU_HandleStruct.Init.DataSize          = SPI_DATASIZE_8BIT;
  MPU_HandleStruct.Init.CLKPolarity       = SPI_POLARITY_HIGH;
  MPU_HandleStruct.Init.CLKPhase          = SPI_PHASE_2EDGE;
  MPU_HandleStruct.Init.NSS               = SPI_NSS_SOFT;
  MPU_HandleStruct.Init.BaudRatePrescaler = IMU_SPIx_SPEED_LOW;
  MPU_HandleStruct.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  MPU_HandleStruct.Init.TIMode            = SPI_TIMODE_DISABLE;
  MPU_HandleStruct.Init.CRCCalculation    = SPI_CRCCALCULATION_ENABLE;
  MPU_HandleStruct.Init.CRCPolynomial     = 7;
  HAL_SPI_Init(&MPU_HandleStruct);

  __HAL_SPI_ENABLE(&MPU_HandleStruct);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_SPI_Config
**功能 : IMU Config
**輸入 : None
**輸出 : None
**使用 : IMU_SPI_Config();
**====================================================================================================*/
/*====================================================================================================*/
#define MPU6500_InitRegNum 11
//#define LPS25H_InitRegNum  2
uint8_t IMU_Init( MPU_InitTypeDef *MPUx )
{
  uint8_t status = ERROR;
#ifdef _USE_MAGNETOMETER
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

  IMU_GPIO_Config();
  IMU_SPI_Config();

  MPU6500_InitData[6][0] = MPUx->MPU_Gyr_FullScale;       // MPU6500_GYRO_CONFIG
  MPU6500_InitData[8][0] = MPUx->MPU_Gyr_LowPassFilter;   // MPU6500_CONFIG
  MPU6500_InitData[7][0] = MPUx->MPU_Acc_FullScale;       // MPU6500_ACCEL_CONFIG
  MPU6500_InitData[9][0] = MPUx->MPU_Acc_LowPassFilter;   // MPU6500_ACCEL_CONFIG_2

  for(uint8_t i = 0; i < MPU6500_InitRegNum; i++) {
    MPU9250_WriteReg(MPU6500_InitData[i][1], MPU6500_InitData[i][0]);
    Delay_1ms(1);
  }

  status = IMU_deviceCheck();
  if(status != SUCCESS)
    return ERROR;

  Delay_10ms(1);

#ifdef _USE_MAGNETOMETER
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

#ifdef _USE_BAROMETER
//  for(uint8_t i = 0; i < LPS25H_InitRegNum; i++) {
//    LPS25H_WriteReg(LPS25H_InitData[i][1], LPS25H_InitData[i][0]);
//    Delay_10ms(1);
//  }
#endif

  IMU_SetSpeed(IMU_SPIx_SPEED_HIGH);
  Delay_10ms(1);

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_deviceCheck
**功能 : Device Check
**輸入 : None
**輸出 : Status
**使用 : Status = IMU_deviceCheck();
**====================================================================================================*/
/*====================================================================================================*/
uint8_t IMU_deviceCheck( void )
{
  uint8_t DeviceID = ERROR;

  DeviceID = MPU9250_ReadReg(MPU6500_WHO_AM_I);
  if(DeviceID != MPU6500_Device_ID)
     return ERROR;

#ifdef _USE_MAGNETOMETER
  DeviceID = MPU9250_Mag_ReadReg(AK8963_WIA);
  if(DeviceID != AK8963_Device_ID)
     return ERROR;
#endif

#ifdef _USE_BAROMETER
  LPS25H_ReadReg(LPS25H_WHO_AM_I, &DeviceID);
  if(DeviceID != LPS25H_Device_ID)
    return ERROR;
#endif

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_getData
**功能 : read IMU data
**輸入 : *dataIMU
**輸出 : None
**使用 : IMU_getData(dataIMU);
**====================================================================================================*/
/*====================================================================================================*/
void IMU_getData( int16_t *dataIMU )
{
#ifdef _USE_MAGNETOMETER
  uint8_t tmpRead[22] = {0};
  MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, tmpRead, 22);
#else
  uint8_t tmpRead[14] = {0};
  MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, tmpRead, 14);
#endif

  dataIMU[0] = (Byte16(int16_t, tmpRead[6],  tmpRead[7]));    // Temp
  dataIMU[1] = (Byte16(int16_t, tmpRead[0],  tmpRead[1]));    // Acc.X
  dataIMU[2] = (Byte16(int16_t, tmpRead[2],  tmpRead[3]));    // Acc.Y
  dataIMU[3] = (Byte16(int16_t, tmpRead[4],  tmpRead[5]));    // Acc.Z
  dataIMU[4] = (Byte16(int16_t, tmpRead[8],  tmpRead[9]));    // Gyr.X
  dataIMU[5] = (Byte16(int16_t, tmpRead[10], tmpRead[11]));   // Gyr.Y
  dataIMU[6] = (Byte16(int16_t, tmpRead[12], tmpRead[13]));   // Gyr.Z
#ifdef _USE_MAGNETOMETER
	if(!(tmpRead[14] & AK8963_STATUS_DRDY) || (tmpRead[14] & AK8963_STATUS_DOR))
		return;
	if(tmpRead[21] & AK8963_STATUS_HOFL)
		return;

  dataIMU[7] = (Byte16(int16_t, tmpRead[16], tmpRead[15]));   // Mag.X
  dataIMU[8] = (Byte16(int16_t, tmpRead[18], tmpRead[17]));   // Mag.Y
  dataIMU[9] = (Byte16(int16_t, tmpRead[20], tmpRead[19]));   // Mag.Z

//	dataIMU[7] = ((long)dataIMU[7] * AK8963_ASA[0]) >> 8;
//	dataIMU[8] = ((long)dataIMU[8] * AK8963_ASA[1]) >> 8;
//	dataIMU[9] = ((long)dataIMU[9] * AK8963_ASA[2]) >> 8;
#endif

//#ifdef _USE_BAROMETER
//  LPS25H_ReadReg(LPS25H_WHO_AM_I, );
//#endif
}
/*==============================================================================================*/
/*==============================================================================================*/
