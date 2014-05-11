/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "module_imu.h"
#include "module_mpu9250.h"
#include "module_ms5611.h"
/*====================================================================================================*/
/*====================================================================================================*/
SensorAcc Acc = {0};
SensorGyr Gyr = {0};

#ifdef USE_SENSOR_MAG
SensorMag Mag = {0};
#endif

#ifdef USE_SENSOR_TEMP
SensorTemp Temp = {0};
#endif

#ifdef USE_SENSOR_BARO
SensorBaro Baro = {0};
#endif
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Sensor_Config
**功能 : Sensor Config
**輸入 : None
**輸出 : None
**使用 : Sensor_Config();
**====================================================================================================*/
/*====================================================================================================*/
void Sensor_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /* INT PA1 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  MPU9250_Config();

#ifdef USE_SENSOR_BARO
  MS5611_Config();
#endif
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Sensor_Init
**功能 : 初始化 Sensor
**輸入 : None
**輸出 : None
**使用 : Sensor_Init();
**====================================================================================================*/
/*====================================================================================================*/
void Sensor_Init( void )
{
//  MPU_InitTypeDef MPU_InitStruct;

  Acc.X = 0;
  Acc.Y = 0;
  Acc.Z = 0;
  Acc.OffsetX = +7;
  Acc.OffsetY = +146;
  Acc.OffsetZ = -291;
  Acc.TrueX = 0.0f;
  Acc.TrueY = 0.0f;
  Acc.TrueZ = 0.0f;

  Gyr.X = 0;
  Gyr.Y = 0;
  Gyr.Z = 0;
  Gyr.OffsetX = 0;
  Gyr.OffsetY = 0;
  Gyr.OffsetZ = 0;
  Gyr.TrueX = 0.0f;
  Gyr.TrueY = 0.0f;
  Gyr.TrueZ = 0.0f;

#ifdef USE_SENSOR_MAG
  Mag.X = 0;
  Mag.Y = 0;
  Mag.Z = 0;
  Mag.AdjustX = 0;
  Mag.AdjustY = 0;
  Mag.AdjustZ = 0;
  Mag.TrueX = 0.0f;
  Mag.TrueY = 0.0f;
  Mag.TrueZ = 0.0f;
#endif

#ifdef USE_SENSOR_TEMP
  Temp.T = 0;
  Temp.OffsetT = TEMP_OFFSET;
  Temp.TrueT = 0.0f;
#endif

#ifdef USE_SENSOR_BARO
  Baro.Temp = 0.0f;
  Baro.Press = 0.0f;
  Baro.Height = 0.0f;
#endif

//  MPU_InitStruct.MPU_LowPassFilter = MPU_LPS_42Hz;
//  MPU_InitStruct.MPU_Acc_FullScale = MPU_AccFS_4g;
//  MPU_InitStruct.MPU_Gyr_FullScale = MPU_GyrFS_2000dps;
//  MPU9150_Init(&MPU_InitStruct);
  MPU9250_Init();
  while(MPU9250_Check() != SUCCESS);

#ifdef USE_SENSOR_BARO
  MS5611_Init();
#endif
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Sensor_Read
**功能 : Sensor Read
**輸入 : ReadBuf
**輸出 : None
**使用 : Sensor_Read(SampleRateFreg);
**====================================================================================================*/
/*====================================================================================================*/
void Sensor_Read( u16 ReadFreg )
{
  u8 ReadBuf[20] = {0};

#ifdef USE_SENSOR_BARO
  static s8 ReadCount = 0;
  static s32 Baro_Buf[2] = {0}; // 沒加 static 資料會有問題
#endif

  MPU9250_Read(ReadBuf);

  Acc.X  = Byte16(s16, ReadBuf[0],  ReadBuf[1]);  // Acc.X
  Acc.Y  = Byte16(s16, ReadBuf[2],  ReadBuf[3]);  // Acc.Y
  Acc.Z  = Byte16(s16, ReadBuf[4],  ReadBuf[5]);  // Acc.Z
  Gyr.X  = Byte16(s16, ReadBuf[8],  ReadBuf[9]);  // Gyr.X
  Gyr.Y  = Byte16(s16, ReadBuf[10], ReadBuf[11]); // Gyr.Y
  Gyr.Z  = Byte16(s16, ReadBuf[12], ReadBuf[13]); // Gyr.Z

#ifdef USE_SENSOR_MAG
  Mag.Z  = Byte16(s16, ReadBuf[14], ReadBuf[15]); // Mag.X
  Mag.Z  = Byte16(s16, ReadBuf[16], ReadBuf[17]); // Mag.Y
  Mag.Z  = Byte16(s16, ReadBuf[18], ReadBuf[19]); // Mag.Z
#endif

#ifdef USE_SENSOR_TEMP
  Temp.T = Byte16(s16, ReadBuf[6],  ReadBuf[7]);  // Temp
#endif

#ifdef USE_SENSOR_BARO
  if(ReadCount == 0) {
    MS5611_Read(Baro_Buf, MS5611_D1_OSR_4096);
    Baro.Temp   = (fp32)(Baro_Buf[0]*0.01f);
    Baro.Press  = (fp32)(Baro_Buf[1]*0.01f);
    Baro.Height = (fp32)((Baro_Buf[1]-101333)*9.5238f);
    ReadCount = (u16)(ReadFreg/MS5611_RespFreq_4096)+1;
  }
  ReadCount--;
#endif
}
/*====================================================================================================*/
/*====================================================================================================*/
