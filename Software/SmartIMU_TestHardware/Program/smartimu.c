/*====================================================================================================*/
/*====================================================================================================*/
#include <stdlib.h>
#include <string.h>

#include "drivers\stm32f4_system.h"
#include "modules\module_serial.h"
#include "modules\module_imu.h"
#include "algorithms\algorithm_mathUnit.h"
#include "algorithms\algorithm_moveAve.h"
#include "algorithms\algorithm_newtonMethod.h"
#include "apps\app_kSerial.h"

#include "smartimu.h"
#include "smartimu_bsp.h"
/*====================================================================================================*/
/*====================================================================================================*/
__IO SensorState_TypeDef SensorState = SENSOR_DEBUG; // SENSOR_CORR_SEL, SENSOR_ALG;

uint16_t timeSec = 0;
uint16_t timeMin = 0;

static int16_t *FIFO_X, *FIFO_Y, *FIFO_Z;

#define CORR_SAMPLE 16
double accCorrData[CORR_SAMPLE * 3] = {0};
double magCorrData[CORR_SAMPLE * 3] = {0};

#define row_X 9
#define col_X 1
double arrayX[row_X * col_X] = { 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0 };

Matrix_TypeDef matrixX   = { row_X, col_X, arrayX, MSTATE_NONE };
Matrix_TypeDef matrixAcc = { 3, CORR_SAMPLE, accCorrData, MSTATE_NONE };
Matrix_TypeDef matrixMag = { 3, CORR_SAMPLE, magCorrData, MSTATE_NONE };

int16_t IMU_Buf[10] = {0};
/*====================================================================================================*/
/*====================================================================================================*/
void SIMU_Init( void )
{
  IMU_InitTypeDef IMU_InitStruct;

  HAL_Init();
  SIMU_GPIO_Config();
  Serial_Config();
  IMU_Config();

  LED_B_Reset();
  delay_ms(100);
  printf("\r\nHello World!\r\n\r\n");

  IMU.MPU_GyrAcc_Enable   = MPU_GyrAcc_ENABLE;
  IMU.MPU_Mag_Enable      = MPU_Mag_ENABLE;
  IMU.LPS_PresTemp_Enable = LPS_PresTemp_ENABLE;

  IMU_InitStruct.pIMU                          = &IMU;
  IMU_InitStruct.InitMPU.MPU_Gyr_FullScale     = MPU_GyrFS_2000dps;
  IMU_InitStruct.InitMPU.MPU_Gyr_LowPassFilter = MPU_GyrLPS_41Hz;
  IMU_InitStruct.InitMPU.MPU_Acc_FullScale     = MPU_AccFS_4g;
  IMU_InitStruct.InitMPU.MPU_Acc_LowPassFilter = MPU_AccLPS_41Hz;
  IMU_InitStruct.InitMPU.MPU_Mag_FullScale     = MPU_MagFS_16b;
  IMU_InitStruct.InitLPS.LPS_PresRES           = LPS_PresRES_8;
  IMU_InitStruct.InitLPS.LPS_TempRES           = LPS_TempRES_8;
  IMU_InitStruct.InitLPS.LPS_ODR               = LPS_ODR_12P5Hz;
  while(IMU_Init(&IMU_InitStruct) != SUCCESS) {
    LED_R_Toggle();
    delay_ms(100);
  }

  LED_R_Set();
  LED_G_Set();
  LED_B_Set();
  delay_ms(100);

  SIMU_TIM_Config();
}
/*====================================================================================================*/
/*====================================================================================================*/
void SIMU_UpdateEven( TIM_HandleTypeDef *htim )
{
  static uint16_t timeCount = 0;
  static uint32_t correctionTimes = 0;

  uint16_t corrState = 0;

  /* time count */
  timeCount++;
  if(timeCount == SampleRateFreg) {
    timeCount = 0;
    timeSec++;
    if(timeSec == 60) {   // 0~59
      timeSec = 0;
      timeMin++;
      if(timeMin == 60)
        timeMin = 0;
    }
  }

  /* 200 Hz, Read Accelerometer, Gyroscope, Magnetometer */
//  IMU_getDataCorr(&IMU);

  #define MAFIFO_SIZE       (SampleRateFreg)      // 200
  #define CORRECTION_TIME   (SampleRateFreg * 2)  // 2 seconds
  switch(SensorState) {

    /************************** Correction Select *******************************/
    case SENSOR_CORR_SEL:
      // read flash
      FIFO_X = (int16_t*)malloc(MAFIFO_SIZE * sizeof(int16_t));
      FIFO_Y = (int16_t*)malloc(MAFIFO_SIZE * sizeof(int16_t));
      FIFO_Z = (int16_t*)malloc(MAFIFO_SIZE * sizeof(int16_t));
      SensorState = SENSOR_GYRO;
      break;

    /************************** Correction Gyro *********************************/
    case SENSOR_GYRO:
      IMU_getData(&IMU);
      switch((uint16_t)(correctionTimes / SampleRateFreg)) {
        case 0:   // clear memory
          LED_R_Reset();
          LED_G_Set();
          LED_B_Set();
          memset(FIFO_X, 0, MAFIFO_SIZE * sizeof(int16_t));
          memset(FIFO_Y, 0, MAFIFO_SIZE * sizeof(int16_t));
          memset(FIFO_Z, 0, MAFIFO_SIZE * sizeof(int16_t));
          correctionTimes = SampleRateFreg;
          break;
        case 1:   // simple moving average & wait to fill fifo
        case 2:
          IMU.Gyr[0] = (int16_t)MoveAve_SMA(IMU.Gyr[0], FIFO_X, MAFIFO_SIZE, MAFIFO_SIZE >> 1);
          IMU.Gyr[1] = (int16_t)MoveAve_SMA(IMU.Gyr[1], FIFO_Y, MAFIFO_SIZE, MAFIFO_SIZE >> 1);
          IMU.Gyr[2] = (int16_t)MoveAve_SMA(IMU.Gyr[2], FIFO_Z, MAFIFO_SIZE, MAFIFO_SIZE >> 1);
          correctionTimes++;
          break;
        case 3:   // calculate offset
          LED_R_Set();
          LED_G_Set();
          LED_B_Set();
          IMU.GyrOffset[0] = IMU.Gyr[0];
          IMU.GyrOffset[1] = IMU.Gyr[1];
          IMU.GyrOffset[2] = IMU.Gyr[2];
          correctionTimes = 0;
          SensorState = SENSOR_CORR_END;
//          SensorState = SENSOR_ACCEL;
          break;
      }
      break;

    /************************** Correction Accel ********************************/
    case SENSOR_ACCEL:
      IMU_getData(&IMU);
      corrState = (uint16_t)(correctionTimes / CORRECTION_TIME);
      switch(corrState) {
        case 0:   // clear memory
          LED_R_Set();
          LED_G_Set();
          LED_B_Set();
          memset(FIFO_X, 0, MAFIFO_SIZE * sizeof(int16_t));
          memset(FIFO_Y, 0, MAFIFO_SIZE * sizeof(int16_t));
          memset(FIFO_Z, 0, MAFIFO_SIZE * sizeof(int16_t));
          correctionTimes = CORRECTION_TIME;
          break;
        case 1:     // simple moving average & wait to fill fifo
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
          if((correctionTimes % CORRECTION_TIME) == 0)
            LED_B_Toggle();
          accCorrData[CORR_SAMPLE * 0 + corrState - 1] = (int16_t)MoveAve_SMA(IMU.Acc[0], FIFO_X, MAFIFO_SIZE, MAFIFO_SIZE >> 1);
          accCorrData[CORR_SAMPLE * 1 + corrState - 1] = (int16_t)MoveAve_SMA(IMU.Acc[1], FIFO_Y, MAFIFO_SIZE, MAFIFO_SIZE >> 1);
          accCorrData[CORR_SAMPLE * 2 + corrState - 1] = (int16_t)MoveAve_SMA(IMU.Acc[2], FIFO_Z, MAFIFO_SIZE, MAFIFO_SIZE >> 1);
          correctionTimes++;
          break;
        case 17:  // calculate parameter
          LED_R_Set();
          LED_G_Set();
          LED_B_Set();
          correctionTimes = 0;
          SensorState = SENSOR_CORR_END;
//          SensorState = SENSOR_MAG;
          break;
      }
      break;

    /************************** Correction Mag **********************************/
    case SENSOR_MAG:
      IMU_getData(&IMU);
      corrState = (uint16_t)(correctionTimes / CORRECTION_TIME);
      switch(corrState) {
        case 0:   // clear memory
          LED_R_Set();
          LED_G_Set();
          LED_B_Set();
          memset(FIFO_X, 0, MAFIFO_SIZE * sizeof(int16_t));
          memset(FIFO_Y, 0, MAFIFO_SIZE * sizeof(int16_t));
          memset(FIFO_Z, 0, MAFIFO_SIZE * sizeof(int16_t));
          correctionTimes = CORRECTION_TIME;
          break;
        case 1:     // simple moving average & wait to fill fifo
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
          if((correctionTimes % CORRECTION_TIME) == 0)
            LED_G_Toggle();
          magCorrData[CORR_SAMPLE * 0 + corrState - 1] = (int16_t)MoveAve_SMA(IMU.Mag[0], FIFO_X, MAFIFO_SIZE, MAFIFO_SIZE >> 1);
          magCorrData[CORR_SAMPLE * 1 + corrState - 1] = (int16_t)MoveAve_SMA(IMU.Mag[1], FIFO_Y, MAFIFO_SIZE, MAFIFO_SIZE >> 1);
          magCorrData[CORR_SAMPLE * 2 + corrState - 1] = (int16_t)MoveAve_SMA(IMU.Mag[2], FIFO_Z, MAFIFO_SIZE, MAFIFO_SIZE >> 1);
          correctionTimes++;
          break;
        case 17:  // calculate parameter
          LED_R_Set();
          LED_G_Set();
          LED_B_Set();
          correctionTimes = 0;
          SensorState = SENSOR_CORR_END;
          break;
      }
      break;

    /************************** Correction End **********************************/
    case SENSOR_CORR_END:
      free(FIFO_X);
      free(FIFO_Y);
      free(FIFO_Z);

      __HAL_TIM_DISABLE(htim);
      LED_R_Reset();
      LED_G_Reset();
      LED_B_Reset();
      newtonMethod(&matrixX, &matrixAcc, 0.4, 8192);
      IMU.AccGain[0]   = (float)arrayX[0];
      IMU.AccGain[1]   = (float)arrayX[1];
      IMU.AccGain[2]   = (float)arrayX[2];
      IMU.AccGain[3]   = (float)arrayX[3];
      IMU.AccGain[4]   = (float)arrayX[4];
      IMU.AccGain[5]   = (float)arrayX[5];
      IMU.AccOffset[0] = (float)arrayX[6];
      IMU.AccOffset[1] = (float)arrayX[7];
      IMU.AccOffset[2] = (float)arrayX[8];
      LED_R_Set();
      LED_G_Set();
      LED_B_Set();
      __HAL_TIM_ENABLE(htim);

      // save to flash
      SensorState = SENSOR_ALG;
      break;

    /************************** Algorithm ***************************************/
    case SENSOR_ALG:
      IMU_convToPhy(&IMU);
      break;

    /************************** Debug *******************************************/
    case SENSOR_DEBUG:
      IMU_getData(&IMU);
      IMU_Buf[0] = IMU.Acc[0];
      IMU_Buf[1] = IMU.Acc[1];
      IMU_Buf[2] = IMU.Acc[2];
      IMU_Buf[3] = IMU.Gyr[0];
      IMU_Buf[4] = IMU.Gyr[1];
      IMU_Buf[5] = IMU.Gyr[2];
      IMU_Buf[6] = IMU.Mag[0];
      IMU_Buf[7] = IMU.Mag[1];
      IMU_Buf[8] = IMU.Mag[2];
      Serial_SendDataMATLAB(IMU_Buf, 10);
//      printf("%5d,%5d,%5d\r\n", IMU.Acc[0], IMU.Acc[1], IMU.Acc[2]);
      break;

    /************************** Error *******************************************/
    default:
      while(1);
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
