/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"

#include "module_imu.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define IMU_SPIx                    SPI1
#define IMU_SPIx_CLK_ENABLE()       __HAL_RCC_SPI1_CLK_ENABLE()
#define IMU_SPIx_SPEED_HIGH         SPI_BAUDRATEPRESCALER_2
#define IMU_SPIx_SPEED_LOW          SPI_BAUDRATEPRESCALER_256

#define IMU_CSM_PIN                 GPIO_PIN_4
#define IMU_CSM_GPIO_PORT           GPIOA
#define IMU_CSM_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define IMU_CSM_H()                 __GPIO_SET(IMU_CSM_GPIO_PORT, IMU_CSM_PIN)
#define IMU_CSM_L()                 __GPIO_RST(IMU_CSM_GPIO_PORT, IMU_CSM_PIN)

#define IMU_CSB_PIN                 GPIO_PIN_0
#define IMU_CSB_GPIO_PORT           GPIOB
#define IMU_CSB_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
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

//#define IMU_INTM_PIN                GPIO_PIN_1
//#define IMU_INTM_GPIO_PORT          GPIOA
//#define IMU_INTM_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
//#define IMU_INTM_IRQ                EXTI1_IRQn

//#define IMU_INTB_PIN                GPIO_PIN_0
//#define IMU_INTB_GPIO_PORT          GPIOA
//#define IMU_INTB_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
//#define IMU_INTB_IRQ                EXTI0_IRQn

#define _USE_MAGNETOMETER
#define _USE_BAROMETER
/*====================================================================================================*/
/*====================================================================================================*/
IMU_DataTypeDef IMU;

static SPI_HandleTypeDef IMU_HandleStruct;
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_SetSpeed
**功能 : Set SPI Speed
**輸入 : SpeedSel
**輸出 : None
**使用 : IMU_SetSpeed(SPIx_SPEED_LOW);
**====================================================================================================*/
/*====================================================================================================*/
void IMU_SetSpeed( uint8_t speedSel )
{
  __HAL_SPI_DISABLE(&IMU_HandleStruct);
  IMU_HandleStruct.Init.BaudRatePrescaler = speedSel;
  HAL_SPI_Init(&IMU_HandleStruct);
  __HAL_SPI_ENABLE(&IMU_HandleStruct);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_ClaerData
**功能 : Clear Data
**輸入 : *pIMU
**輸出 : None
**使用 : IMU_ClaerData(IMU);
**====================================================================================================*/
/*====================================================================================================*/
void IMU_ClaerData( IMU_DataTypeDef *pIMU )
{
  for(uint8_t i = 0; i < 3; i++ ) {
    pIMU->Gyr[i] = 0;
    pIMU->Acc[i] = 0;
    pIMU->Mag[i] = 0;
    pIMU->MagASA[i] = 0;
    pIMU->GyrOffset[i] = 0;
    pIMU->AccOffset[i] = 0;
    pIMU->MagOffset[i] = 0;
    pIMU->GyrGain[i] = 0.0f;
    pIMU->GyrF[i] = 0.0f;
    pIMU->AccF[i] = 0.0f;
    pIMU->MagF[i] = 0.0f;
  }

  pIMU->AccGain[0] = 1.0f;
  pIMU->AccGain[1] = 0.0f;
  pIMU->AccGain[2] = 0.0f;
  pIMU->AccGain[3] = 1.0f;
  pIMU->AccGain[4] = 0.0f;
  pIMU->AccGain[5] = 1.0f;

  pIMU->MagGain[0] = 1.0f;
  pIMU->MagGain[1] = 0.0f;
  pIMU->MagGain[2] = 0.0f;
  pIMU->MagGain[3] = 1.0f;
  pIMU->MagGain[4] = 0.0f;
  pIMU->MagGain[5] = 1.0f;

  pIMU->Temp = 0;
  pIMU->Pres = 0;
  pIMU->TempOffset = 0;
  pIMU->PresOffset = 0;
  pIMU->TempGain = 1.0f;
  pIMU->PresGain = 1.0f;
  pIMU->GyrSens = 0.0f;
  pIMU->AccSens = 0.0f;
  pIMU->MagSens = 0.0f;
  pIMU->TempSens = 0.0f;
  pIMU->PresSens = 0.0f;
  pIMU->TempF = 0.0f;
  pIMU->PresF = 0.0f;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_SetSensitivity
**功能 : Set Sensitivity
**輸入 : *IMUx
**輸出 : None
**使用 : IMU_SetSensitivity(IMUx);
**====================================================================================================*/
/*====================================================================================================*/
void IMU_SetSensitivity( IMU_InitTypeDef *IMUx )
{
  if(IMUx->pIMU->MPU_GyrAcc_Enable == MPU_GyrAcc_ENABLE) {
    switch(IMUx->InitMPU.MPU_Gyr_FullScale) { // dps/LSB
      case MPU_GyrFS_250dps:  IMUx->pIMU->GyrSens = MPU9250G_250dps;  break;
      case MPU_GyrFS_500dps:  IMUx->pIMU->GyrSens = MPU9250G_500dps;  break;
      case MPU_GyrFS_1000dps: IMUx->pIMU->GyrSens = MPU9250G_1000dps; break;
      case MPU_GyrFS_2000dps: IMUx->pIMU->GyrSens = MPU9250G_2000dps; break;
      default :               IMUx->pIMU->GyrSens = MPU9250G_2000dps; break;
    }
    switch(IMUx->InitMPU.MPU_Acc_FullScale) { // g/LSB
      case MPU_AccFS_2g:  IMUx->pIMU->AccSens = MPU9250A_2g;  break;
      case MPU_AccFS_4g:  IMUx->pIMU->AccSens = MPU9250A_4g;  break;
      case MPU_AccFS_8g:  IMUx->pIMU->AccSens = MPU9250A_8g;  break;
      case MPU_AccFS_16g: IMUx->pIMU->AccSens = MPU9250A_16g; break;
      default :           IMUx->pIMU->AccSens = MPU9250A_4g;  break;
    }
    IMUx->pIMU->ICTempSens = MPU9250T_85degC; // degC/LSB
  }

  if(IMUx->pIMU->MPU_Mag_Enable == MPU_Mag_ENABLE) {
    IMUx->pIMU->MagSens    = MPU9250M_4800uT; // uT/LSB
  }

  if(IMUx->pIMU->LPS_PresTemp_Enable == LPS_PresTemp_ENABLE) {
    IMUx->pIMU->TempSens   = LPS25H_T_degC;   // degC/LSB
    IMUx->pIMU->PresSens   = LPS25H_P_hPa;    // hPa/LSB
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_Config
**功能 : GPIO Config
**輸入 : None
**輸出 : None
**使用 : IMU_Config();
**====================================================================================================*/
/*====================================================================================================*/
void IMU_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* SPI Clk ******************************************************************/
  IMU_CSM_GPIO_CLK_ENABLE();
  IMU_CSB_GPIO_CLK_ENABLE();
  IMU_SCK_GPIO_CLK_ENABLE();
  IMU_SDO_GPIO_CLK_ENABLE();
  IMU_SDI_GPIO_CLK_ENABLE();
//  IMU_INTM_GPIO_CLK_ENABLE();
//  IMU_INTB_GPIO_CLK_ENABLE();
  IMU_SPIx_CLK_ENABLE();

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

//  // EXIT Config
//  GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING;
//  GPIO_InitStruct.Pull  = GPIO_PULLUP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

//  GPIO_InitStruct.Pin   = IMU_INTM_PIN;
//  HAL_GPIO_Init(IMU_INTM_GPIO_PORT, &GPIO_InitStruct);

//  HAL_NVIC_SetPriority(IMU_INTM_IRQ, 14, 0);
//  HAL_NVIC_EnableIRQ(IMU_INTM_IRQ);

#ifdef _USE_BAROMETER
//  GPIO_InitStruct.Pin   = IMU_INTB_PIN;
//  HAL_GPIO_Init(IMU_INTB_GPIO_PORT, &GPIO_InitStruct);

//  HAL_NVIC_SetPriority(IMU_INTB_IRQ, 14, 0);
//  HAL_NVIC_EnableIRQ(IMU_INTB_IRQ);
#endif

  /* CS Pin Set High ******************************************************************/
  IMU_CSM_H();
  IMU_CSB_H();

  /* SPI Init ****************************************************************/
  IMU_HandleStruct.Instance               = IMU_SPIx;
  IMU_HandleStruct.Init.Mode              = SPI_MODE_MASTER;
  IMU_HandleStruct.Init.Direction         = SPI_DIRECTION_2LINES;
  IMU_HandleStruct.Init.DataSize          = SPI_DATASIZE_8BIT;
  IMU_HandleStruct.Init.CLKPolarity       = SPI_POLARITY_HIGH;
  IMU_HandleStruct.Init.CLKPhase          = SPI_PHASE_2EDGE;
  IMU_HandleStruct.Init.NSS               = SPI_NSS_SOFT;
  IMU_HandleStruct.Init.BaudRatePrescaler = IMU_SPIx_SPEED_LOW;
  IMU_HandleStruct.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  IMU_HandleStruct.Init.TIMode            = SPI_TIMODE_DISABLE;
  IMU_HandleStruct.Init.CRCCalculation    = SPI_CRCCALCULATION_ENABLE;
  IMU_HandleStruct.Init.CRCPolynomial     = 7;
  HAL_SPI_Init(&IMU_HandleStruct);

  __HAL_SPI_ENABLE(&IMU_HandleStruct);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_Init
**功能 : IMU Init
**輸入 : None
**輸出 : None
**使用 : status = IMU_Init(&IMUx_InitStruct);
**====================================================================================================*/
/*====================================================================================================*/
#define MPU6500_InitRegNum 11
//#define LPS25H_InitRegNum  2
uint8_t IMU_Init( IMU_InitTypeDef *IMUx )
{
  uint8_t i = 0;
  uint8_t status = ERROR;
  uint8_t tmpRead[3] = {0};
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

  IMU_ClaerData(IMUx->pIMU);
  IMU_SetSensitivity(IMUx);

  MPU6500_InitData[6][0] = IMUx->InitMPU.MPU_Gyr_FullScale;       // MPU6500_GYRO_CONFIG
  MPU6500_InitData[7][0] = IMUx->InitMPU.MPU_Acc_FullScale;       // MPU6500_ACCEL_CONFIG
  MPU6500_InitData[8][0] = IMUx->InitMPU.MPU_Gyr_LowPassFilter;   // MPU6500_CONFIG
  MPU6500_InitData[9][0] = IMUx->InitMPU.MPU_Acc_LowPassFilter;   // MPU6500_ACCEL_CONFIG_2

  for(i = 0; i < MPU6500_InitRegNum; i++) {
    delay_ms(2);
    MPU9250_WriteReg(MPU6500_InitData[i][1], MPU6500_InitData[i][0]);
  }

  status = IMU_deviceCheck(IMUx->pIMU);
  if(status != SUCCESS)
    return ERROR;

  delay_ms(10);

  if(IMUx->pIMU->MPU_Mag_Enable == MPU_Mag_ENABLE) {
    MPU9250_Mag_WriteReg(AK8963_CNTL2, 0x01);       // Reset Device
    delay_ms(2);
    MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x10);       // Power-down mode
    delay_ms(2);
    MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x1F);       // Fuse ROM access mode
    delay_ms(2);
    MPU9250_Mag_ReadRegs(AK8963_ASAX, tmpRead, 3);  // Read sensitivity adjustment values
    delay_ms(2);
    MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x10);       // Power-down mode
    delay_ms(2);

    IMUx->pIMU->MagASA[0] = (int16_t)(tmpRead[0]) + 128;
    IMUx->pIMU->MagASA[1] = (int16_t)(tmpRead[1]) + 128;
    IMUx->pIMU->MagASA[2] = (int16_t)(tmpRead[2]) + 128;

    MPU9250_WriteReg(MPU6500_I2C_MST_CTRL, 0x5D);
    delay_ms(2);
    MPU9250_WriteReg(MPU6500_I2C_SLV0_ADDR, AK8963_I2C_ADDR | 0x80);
    delay_ms(2);
    MPU9250_WriteReg(MPU6500_I2C_SLV0_REG, AK8963_ST1);
    delay_ms(2);
    MPU9250_WriteReg(MPU6500_I2C_SLV0_CTRL, MPU6500_I2C_SLVx_EN | 8);
    delay_ms(2);

    MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x16);       // Continuous measurement mode 2
    delay_ms(2);

    MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, 0x09);
    delay_ms(2);
    MPU9250_WriteReg(MPU6500_I2C_MST_DELAY_CTRL, 0x81);
    delay_ms(100);
  }

  if(IMUx->pIMU->LPS_PresTemp_Enable == LPS_PresTemp_ENABLE) {
//    for(uint8_t i = 0; i < LPS25H_InitRegNum; i++) {
//      Delay_1ms(2);
//      LPS25H_WriteReg(LPS25H_InitData[i][1], LPS25H_InitData[i][0]);
//    }
  }

  IMU_SetSpeed(IMU_SPIx_SPEED_HIGH);
  delay_ms(10);

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_deviceCheck
**功能 : Check Device ID
**輸入 : *pIMU
**輸出 : status
**使用 : status = IMU_deviceCheck(pIMU);
**====================================================================================================*/
/*====================================================================================================*/
uint8_t IMU_deviceCheck( IMU_DataTypeDef *pIMU )
{
  uint8_t deviceID = ERROR;

  if(pIMU->MPU_GyrAcc_Enable == MPU_GyrAcc_ENABLE) {
    deviceID = MPU9250_ReadReg(MPU6500_WHO_AM_I);
    if(deviceID != MPU6500_Device_ID)
      return ERROR;
  }

  if(pIMU->MPU_Mag_Enable == MPU_Mag_ENABLE) {
    deviceID = MPU9250_Mag_ReadReg(AK8963_WIA);
    if(deviceID != AK8963_Device_ID)
      return ERROR;
  }

  if(pIMU->LPS_PresTemp_Enable == LPS_PresTemp_ENABLE) {
    deviceID = LPS25H_ReadReg(LPS25H_WHO_AM_I);
    if(deviceID != LPS25H_Device_ID)
      return ERROR;
  }

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_getData
**功能 : Get IMU Data
**輸入 : *pIMU
**輸出 : None
**使用 : IMU_getData(pIMU);
**====================================================================================================*/
/*====================================================================================================*/
void IMU_getData( IMU_DataTypeDef *pIMU )
{
  uint8_t tmpRead[22] = {0};

  if((pIMU->MPU_GyrAcc_Enable == MPU_GyrAcc_ENABLE) && (pIMU->MPU_Mag_Enable == MPU_Mag_ENABLE)) {
    MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, tmpRead, 22);    // Read Gyr, Acc, Mag
  }
  else if(pIMU->MPU_GyrAcc_Enable == MPU_GyrAcc_ENABLE) {
    MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, tmpRead, 14);    // Read Gyr, Acc
  }
  else if(pIMU->MPU_Mag_Enable == MPU_Mag_ENABLE) {
    MPU9250_ReadRegs(MPU6500_EXT_SENS_DATA_00, tmpRead + 14, 8); // Read Mag
  }

  if(pIMU->MPU_GyrAcc_Enable == MPU_GyrAcc_ENABLE) {
    pIMU->Gyr[0] = (Byte16(int16_t, tmpRead[8],  tmpRead[9]));    // Gyr.X
    pIMU->Gyr[1] = (Byte16(int16_t, tmpRead[10], tmpRead[11]));   // Gyr.Y
    pIMU->Gyr[2] = (Byte16(int16_t, tmpRead[12], tmpRead[13]));   // Gyr.Z
    pIMU->Acc[0] = (Byte16(int16_t, tmpRead[0],  tmpRead[1]));    // Acc.X
    pIMU->Acc[1] = (Byte16(int16_t, tmpRead[2],  tmpRead[3]));    // Acc.Y
    pIMU->Acc[2] = (Byte16(int16_t, tmpRead[4],  tmpRead[5]));    // Acc.Z
    pIMU->ICTemp = (Byte16(int16_t, tmpRead[6],  tmpRead[7]));    // Temp
  }

  if(pIMU->MPU_Mag_Enable == MPU_Mag_ENABLE) {
    if(!(!(tmpRead[14] & AK8963_STATUS_DRDY) || (tmpRead[14] & AK8963_STATUS_DOR) || (tmpRead[21] & AK8963_STATUS_HOFL))) {
      pIMU->Mag[0] = (Byte16(int16_t, tmpRead[16], tmpRead[15]));   // Mag.X
      pIMU->Mag[1] = (Byte16(int16_t, tmpRead[18], tmpRead[17]));   // Mag.Y
      pIMU->Mag[2] = (Byte16(int16_t, tmpRead[20], tmpRead[19]));   // Mag.Z
    }
  }

  if(pIMU->LPS_PresTemp_Enable == LPS_PresTemp_ENABLE) {
//    LPS25H_ReadReg(LPS25H_WHO_AM_I, );
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_getDataCorr
**功能 : Get Correction Data
**輸入 : *pIMU
**輸出 : None
**使用 : IMU_getDataCorr(pIMU);
**====================================================================================================*/
/*====================================================================================================*/
void IMU_getDataCorr( IMU_DataTypeDef *pIMU )
{
  double tmpData[3] = {0};

  IMU_getData(pIMU);

  if(pIMU->MPU_GyrAcc_Enable == MPU_GyrAcc_ENABLE) {
    pIMU->Gyr[0] = (int16_t)(pIMU->Gyr[0] - pIMU->GyrOffset[0]);  // Gyr.X
    pIMU->Gyr[1] = (int16_t)(pIMU->Gyr[1] - pIMU->GyrOffset[1]);  // Gyr.Y
    pIMU->Gyr[2] = (int16_t)(pIMU->Gyr[2] - pIMU->GyrOffset[2]);  // Gyr.Z

    tmpData[0] = pIMU->Acc[0] - pIMU->AccOffset[0];   // Acc.X
    tmpData[1] = pIMU->Acc[1] - pIMU->AccOffset[1];   // Acc.Y
    tmpData[2] = pIMU->Acc[2] - pIMU->AccOffset[2];   // Acc.Z

    pIMU->Acc[0] = pIMU->AccGain[0] * tmpData[0] + pIMU->AccGain[1] * tmpData[1] + pIMU->AccGain[2] * tmpData[2];
    pIMU->Acc[1] = pIMU->AccGain[1] * tmpData[0] + pIMU->AccGain[3] * tmpData[1] + pIMU->AccGain[4] * tmpData[2];
    pIMU->Acc[2] = pIMU->AccGain[2] * tmpData[0] + pIMU->AccGain[4] * tmpData[1] + pIMU->AccGain[5] * tmpData[2];
  }

  if(pIMU->MPU_Mag_Enable == MPU_Mag_ENABLE) {
    tmpData[0] = pIMU->Mag[0] - pIMU->MagOffset[0];   // Mag.X
    tmpData[1] = pIMU->Mag[1] - pIMU->MagOffset[1];   // Mag.Y
    tmpData[2] = pIMU->Mag[2] - pIMU->MagOffset[2];   // Mag.Z

    pIMU->Acc[0] = pIMU->AccGain[0] * tmpData[0] + pIMU->AccGain[1] * tmpData[1] + pIMU->AccGain[2] * tmpData[2];
    pIMU->Acc[1] = pIMU->AccGain[1] * tmpData[0] + pIMU->AccGain[3] * tmpData[1] + pIMU->AccGain[4] * tmpData[2];
    pIMU->Acc[2] = pIMU->AccGain[2] * tmpData[0] + pIMU->AccGain[4] * tmpData[1] + pIMU->AccGain[5] * tmpData[2];
  }

  if(pIMU->LPS_PresTemp_Enable == LPS_PresTemp_ENABLE) {
    pIMU->Temp -= pIMU->TempOffset;   // Temp
    pIMU->Pres -= pIMU->PresOffset;   // Pres
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : IMU_convToPhy
**功能 : Convert Data
**輸入 : *pIMU
**輸出 : None
**使用 : IMU_convToPhy(pIMU);
**====================================================================================================*/
/*====================================================================================================*/
void IMU_convToPhy( IMU_DataTypeDef *pIMU )
{
  if(pIMU->MPU_GyrAcc_Enable == MPU_GyrAcc_ENABLE) {
    pIMU->GyrF[0] = pIMU->Gyr[0] * pIMU->GyrSens;     // Gyr.X
    pIMU->GyrF[1] = pIMU->Gyr[1] * pIMU->GyrSens;     // Gyr.Y
    pIMU->GyrF[2] = pIMU->Gyr[2] * pIMU->GyrSens;     // Gyr.Z
    pIMU->AccF[0] = pIMU->Acc[0] * pIMU->AccSens;     // Acc.X
    pIMU->AccF[1] = pIMU->Acc[1] * pIMU->AccSens;     // Acc.Y
    pIMU->AccF[2] = pIMU->Acc[2] * pIMU->AccSens;     // Acc.Z
    pIMU->ICTempF = pIMU->ICTemp * pIMU->ICTempSens;  // ICTemp
  }

  if(pIMU->MPU_Mag_Enable == MPU_Mag_ENABLE) {
    pIMU->MagF[0] = pIMU->Mag[0] * pIMU->MagSens;     // Mag.X
    pIMU->MagF[1] = pIMU->Mag[1] * pIMU->MagSens;     // Mag.Y
    pIMU->MagF[2] = pIMU->Mag[2] * pIMU->MagSens;     // Mag.Z
  }

  if(pIMU->LPS_PresTemp_Enable == LPS_PresTemp_ENABLE) {
    pIMU->TempF = pIMU->Temp * pIMU->TempSens;        // Temp
    pIMU->PresF = pIMU->Pres * pIMU->PresSens;        // Pres
  }
}
/*==============================================================================================*/
/*==============================================================================================*/
