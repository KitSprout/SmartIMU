/* #include "module_imu.h" */

#ifndef __MODULE_IMU_H
#define __MODULE_IMU_H

#include "stm32f4xx_hal.h"

#include "Modules\module_mpu9250.h"
#include "Modules\module_lps25h.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
/*
|     |            ACCELEROMETER           |
| LPF |  BandW  |  Delay  | Noise | Sample |
+-----+---------+---------+-------+--------+
|  x  |  1130Hz |  0.75ms | 250Hz |  4kHz  |
|  0  |   460Hz |  1.94ms | 250Hz |  1kHz  |
|  1  |   184Hz |  5.80ms | 250Hz |  1kHz  |
|  2  |    92Hz |  7.80ms | 250Hz |  1kHz  |
|  3  |    41Hz | 11.80ms | 250Hz |  1kHz  |
|  4  |    20Hz | 19.80ms | 250Hz |  1kHz  |
|  5  |    10Hz | 35.70ms | 250Hz |  1kHz  |
|  6  |     5Hz | 66.96ms | 250Hz |  1kHz  |
|  7  |   460Hz |  1.94ms | 250Hz |  1kHz  |
*/
/*
|     |          GYROSCOPE         |    TEMPERATURE    |
| LPF |  BandW  |  Delay  | Sample |  BandW  |  Delay  |
+-----+---------+---------+--------+---------+---------+
|  x  |  8800Hz | 0.064ms |  32kHz |  4000Hz |  0.04ms |
|  x  |  3600Hz |  0.11ms |  32kHz |  4000Hz |  0.04ms |
|  0  |   250Hz |  0.97ms |   8kHz |  4000Hz |  0.04ms |
|  1  |   184Hz |   2.9ms |   1kHz |   188Hz |   1.9ms |
|  2  |    92Hz |   3.9ms |   1kHz |    98Hz |   2.8ms |
|  3  |    41Hz |   5.9ms |   1kHz |    42Hz |   4.8ms |
|  4  |    20Hz |   9.9ms |   1kHz |    20Hz |   8.3ms |
|  5  |    10Hz | 17.85ms |   1kHz |    10Hz |  13.4ms |
|  6  |     5Hz | 33.48ms |   1kHz |     5Hz |  18.6ms |
|  7  |  3600Hz |  0.17ms |   8kHz |  4000Hz |  0.04ms |
*/
/*
| ODR |  Pres  |  Temp  |
+-----+--------+--------+
|  0  | One-Shot Enable |
|  1  |    1Hz |    1Hz |
|  2  |    7Hz |    7Hz |
|  3  | 12.5Hz | 12.5Hz |
|  4  |   25Hz |   25Hz |
|  5  |     Reserved    |
*/

typedef enum {
  MPU_GyrAcc_ENABLE   = ENABLE,
  MPU_GyrAcc_DISABLE  = DISABLE
} MPU_GyrAccEN_TypeDef;
typedef enum {
  MPU_Mag_ENABLE      = ENABLE,
  MPU_Mag_DISABLE     = DISABLE
} MPU_MagEN_TypeDef;
typedef enum {
  LPS_PresTemp_ENABLE  = ENABLE,
  LPS_PresTemp_DISABLE = DISABLE
} LPS_PresTempEN_TypeDef;

typedef struct {
  MPU_GyrAccEN_TypeDef    MPU_GyrAcc_Enable;
  MPU_MagEN_TypeDef       MPU_Mag_Enable;
  LPS_PresTempEN_TypeDef  LPS_PresTemp_Enable;

  int16_t Gyr[3];
  int16_t Acc[3];
  int16_t Mag[3];
  int16_t ICTemp;
  int16_t Temp;
  int32_t Pres;

  int16_t MagASA[3];

  int16_t TempOffset;
  int32_t PresOffset;

  float GyrOffset[3];
  float AccOffset[3];
  float MagOffset[3];

  float GyrGain[3];
  float AccGain[6];
  float MagGain[6];
  float TempGain;
  float PresGain;

  float GyrSens;
  float AccSens;
  float MagSens;
  float ICTempSens;
  float TempSens;
  float PresSens;

  float GyrF[3];
  float AccF[3];
  float MagF[3];
  float ICTempF;
  float TempF;
  float PresF;

} IMU_DataTypeDef;

typedef struct {
  MPU_InitTypeDef InitMPU;
  LPS_InitTypeDef InitLPS;
  IMU_DataTypeDef *pIMU;
} IMU_InitTypeDef;
/*=====================================================================================================*/
/*=====================================================================================================*/
extern IMU_DataTypeDef IMU;
/*=====================================================================================================*/
/*=====================================================================================================*/
void    IMU_SetSpeed( uint8_t speedSel );
void    IMU_SetSensitivity( IMU_InitTypeDef *IMUx );
void    IMU_ClaerData( IMU_DataTypeDef *pIMU );

void    IMU_Config( void );
uint8_t IMU_Init( IMU_InitTypeDef *IMUx );

uint8_t IMU_deviceCheck( IMU_DataTypeDef *pIMU );
void    IMU_getData( IMU_DataTypeDef *pIMU );
void    IMU_getDataCorr( IMU_DataTypeDef *pIMU );
void    IMU_convToPhy( IMU_DataTypeDef *pIMU );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
