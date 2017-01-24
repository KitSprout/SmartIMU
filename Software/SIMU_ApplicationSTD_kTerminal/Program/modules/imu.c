/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    imu.c
  * @author  KitSprout
  * @date    19-Jan-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"
#include "modules\imu.h"

#include <stdio.h>
#include <string.h>

/** @addtogroup STM32_Module
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
static SPI_InitTypeDef SPI_InitStruct;
IMU_DataTypeDef IMU;

/* Private function prototypes -------------------------------------------------------------*/
static void IMU_InitData( IMU_DataTypeDef *imux );
static void IMU_SetSensitivity( IMU_InitTypeDef *IMUx );
static void IMU_MergeScaleStrength( IMU_DataTypeDef *imux );
//static void IMU_MergeScaleCalib( IMU_DataTypeDef *imux );

/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  IMU_Config
  */
void IMU_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

#if defined(__MPU92)
  MPU92_Config();
#endif

#if defined(__LPS22)
  LPS22_Config();
#endif

  /* SPI Clk ******************************************************************/
  IMU_SPIx_CLK_ENABLE();

  /* SPI AF *******************************************************************/
  GPIO_PinAFConfig(IMU_SCK_GPIO_PORT, IMU_SCK_SOURCE, IMU_SCK_AF);
  GPIO_PinAFConfig(IMU_SDO_GPIO_PORT, IMU_SDO_SOURCE, IMU_SDO_AF);
  GPIO_PinAFConfig(IMU_SDI_GPIO_PORT, IMU_SDI_SOURCE, IMU_SDI_AF);

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = IMU_SCK_PIN;
  GPIO_Init(IMU_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = IMU_SDO_PIN;
  GPIO_Init(IMU_SDO_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = IMU_SDI_PIN;
  GPIO_Init(IMU_SDI_GPIO_PORT, &GPIO_InitStruct);

  /* SPI Init ****************************************************************/
  SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;
  SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL              = SPI_CPOL_High;
  SPI_InitStruct.SPI_CPHA              = SPI_CPHA_2Edge;
  SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = IMU_SPIx_SPEED_LOW;
  SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial     = 7;
  SPI_Init(IMU_SPIx, &SPI_InitStruct);

  SPI_Cmd(IMU_SPIx, ENABLE);
}

/**
  * @brief  IMU_Init
  */
int8_t IMU_Init( IMU_InitTypeDef *IMUx )
{
  int8_t status;

#if defined(__MPU92)
  status = MPU92_Init(&IMUx->InitMPU);
  if (status != SUCCESS) {
    return ERROR;
  }
#endif

#if defined(__LPS22)
  status = LPS22_Init(&IMUx->InitLPS);
  if (status != SUCCESS) {
    return ERROR;
  }
#endif

  SPI_SetSpeed(IMU_SPIx, &SPI_InitStruct, IMU_SPIx_SPEED_HIGH);
  delay_ms(10);

  IMU_InitData(IMUx->Data);
  IMU_SetSensitivity(IMUx);
  IMU_MergeScaleStrength(IMUx->Data);
//  IMU_MergeScaleCalib(IMUx->Data);

  return SUCCESS;
}

/**
  * @brief  IMU_GetRawData
  */
int8_t IMU_GetRawData( IMU_DataTypeDef *imux )
{
  int8_t status;

#if defined(__MPU92)
  int16_t data16[10];

  status = MPU92_GetRawData(data16);
  imux->gyrRaw[0] = data16[0];    /* Gyr.X */
  imux->gyrRaw[1] = data16[1];    /* Gyr.Y */
  imux->gyrRaw[2] = data16[2];    /* Gyr.Z */
  imux->accRaw[0] = data16[3];    /* Acc.X */
  imux->accRaw[1] = data16[4];    /* Acc.Y */
  imux->accRaw[2] = data16[5];    /* Acc.Z */
  imux->ictempRaw = data16[6];    /* ICTemp */

#if defined(__USE_MAGNETOMETER)
  if (status == 1) {
    imux->magRaw[0] = data16[7];  /* Mag.X */
    imux->magRaw[1] = data16[8];  /* Mag.Y */
    imux->magRaw[2] = data16[9];  /* Mag.Z */
  }
#endif
#endif

#if defined(__LPS22)
  int32_t data32[2];

  status = LPS22_GetRawData(data32);
  if (status == 1) {
    imux->baroRaw[0] = data32[0];
    imux->baroRaw[1] = data32[1];
  }
#endif
  return status;
}

/**
  * @brief  IMU_GetCalibData
  */
void IMU_GetCalibData( IMU_DataTypeDef *imux )
{
#if defined(__USE_MAGNETOMETER)
  float32_t tmp[3] = {0};
#endif

  IMU_GetRawData(imux);

  imux->gyrData[0] = (imux->gyrRaw[0] - imux->gyrOffset[0]) * imux->gyrCalib[0];   /* Gyr.X */
  imux->gyrData[1] = (imux->gyrRaw[1] - imux->gyrOffset[1]) * imux->gyrCalib[1];   /* Gyr.Y */
  imux->gyrData[2] = (imux->gyrRaw[2] - imux->gyrOffset[2]) * imux->gyrCalib[2];   /* Gyr.Z */

  imux->accData[0] = (imux->accCalib[0] * imux->accRaw[0] + imux->accCalib[1] * imux->accRaw[1] + imux->accCalib[2] * imux->accRaw[2]) + imux->accOffset[0];  /* Acc.X */
  imux->accData[1] = (imux->accCalib[3] * imux->accRaw[0] + imux->accCalib[4] * imux->accRaw[1] + imux->accCalib[5] * imux->accRaw[2]) + imux->accOffset[1];  /* Acc.X */
  imux->accData[2] = (imux->accCalib[6] * imux->accRaw[0] + imux->accCalib[7] * imux->accRaw[1] + imux->accCalib[8] * imux->accRaw[2]) + imux->accOffset[2];  /* Acc.X */

  imux->ictempData = imux->ictempRaw * imux->ictempScale + imux->ictempOffset;

#if defined(__USE_MAGNETOMETER)
  tmp[0] = imux->magRaw[0] - imux->magOffset[0];   /* Mag.X */
  tmp[1] = imux->magRaw[1] - imux->magOffset[1];   /* Mag.Y */
  tmp[2] = imux->magRaw[2] - imux->magOffset[2];   /* Mag.Z */

  imux->magData[0] = imux->magCalib[0] * tmp[0] + imux->magCalib[1] * tmp[1] + imux->magCalib[2] * tmp[2];
  imux->magData[1] = imux->magCalib[3] * tmp[0] + imux->magCalib[4] * tmp[1] + imux->magCalib[5] * tmp[2];
  imux->magData[2] = imux->magCalib[6] * tmp[0] + imux->magCalib[7] * tmp[1] + imux->magCalib[8] * tmp[2];
#endif

#if defined(__USE_BAROMETER)
  imux->baroData[0] = imux->baroRaw[0];
  imux->baroData[1] = imux->baroRaw[1];
#endif
}

/**
  * @brief  IMU_GetScaleData
  */
void IMU_GetScaleData( IMU_DataTypeDef *imux )
{
  IMU_GetCalibData(imux);

  imux->gyrData[0] = imux->gyrData[0] * imux->gyrScale[0];    /* Gyr.X */
  imux->gyrData[1] = imux->gyrData[1] * imux->gyrScale[1];    /* Gyr.Y */
  imux->gyrData[2] = imux->gyrData[2] * imux->gyrScale[2];    /* Gyr.Z */

  imux->accData[0] = imux->accData[0] * imux->accScale[0];    /* Acc.X */
  imux->accData[1] = imux->accData[1] * imux->accScale[1];    /* Acc.Y */
  imux->accData[2] = imux->accData[2] * imux->accScale[2];    /* Acc.Z */

#if defined(__USE_MAGNETOMETER)
  imux->magData[0] = imux->magData[0] * imux->magScale[0];    /* Mag.X */
  imux->magData[1] = imux->magData[1] * imux->magScale[1];    /* Mag.Y */
  imux->magData[2] = imux->magData[2] * imux->magScale[2];    /* Mag.Z */
#endif

#if defined(__USE_BAROMETER)
  imux->baroData[0] = imux->baroData[0] * imux->baroScale[0]; /* Mag.X */
  imux->baroData[1] = imux->baroData[1] * imux->baroScale[1]; /* Mag.Y */
#endif
}

/**
  * @brief  IMU_InitData
  */
static void IMU_InitData( IMU_DataTypeDef *imux )
{
  memset(imux, 0, sizeof(IMU_DataTypeDef));

  imux->gyrCalib[0] = 1.0f;
  imux->gyrCalib[1] = 1.0f;
  imux->gyrCalib[2] = 1.0f;

  imux->accStrength = 1.0f;
  imux->accCalib[0] = 1.0f;
  imux->accCalib[4] = 1.0f;
  imux->accCalib[8] = 1.0f;

  imux->magStrength = 1.0f;
  imux->magCalib[0] = 1.0f;
  imux->magCalib[4] = 1.0f;
  imux->magCalib[8] = 1.0f;

#if 0
  /* set gyroscope parameters */
  imux->gyrOffset[0] = -5.266666666666667f;
  imux->gyrOffset[1] = -7.883333333333334f;
  imux->gyrOffset[2] = -8.783333333333333f;

  /* set accelerometer parameters */
  imux->accStrength  =  9.8f;
  imux->accCalib[0]  =  1.000591271010064f;
  imux->accCalib[1]  =  0.000586893913887f;
  imux->accCalib[2]  = -0.003436732180952f;
  imux->accCalib[3]  = -0.001485988936797f;
  imux->accCalib[4]  =  1.000157846160545f;
  imux->accCalib[5]  = -0.011743938010525f;
  imux->accCalib[6]  = -0.007978426891497f;
  imux->accCalib[7]  = -0.000416245697376f;
  imux->accCalib[8]  =  0.995341160302247f;
  imux->accOffset[0] =  42.55588470081017f;
  imux->accOffset[1] = -97.96568104396205f;
  imux->accOffset[2] =  316.0875468994960f;

  /* set magnetometer parameters */
//  imux->magStrength  =  212.4818615767788f;
  imux->magCalib[0]  =  1.019823067566470f;
  imux->magCalib[1]  = -0.003356411418474f;
  imux->magCalib[2]  = -0.014430324672665f;
  imux->magCalib[3]  = -0.003356411418474f;
  imux->magCalib[4]  =  1.015454920456150f;
  imux->magCalib[5]  =  0.018734747904013f;
  imux->magCalib[6]  = -0.014430324672665f;
  imux->magCalib[7]  =  0.018734747904013f;
  imux->magCalib[8]  =  0.966196977587016f;
  imux->magOffset[0] =  16.228349462609184f;
  imux->magOffset[1] =  170.7384291607371f;
  imux->magOffset[2] = -58.094694639781515f;
#endif
}

/**
  * @brief  IMU_SetSensitivity
  */
static void IMU_SetSensitivity( IMU_InitTypeDef *IMUx )
{
  float32_t scale[5];

#if defined(__MPU92)
  MPU92_GetSensitivity(&IMUx->InitMPU, scale);

  /* Set gyroscope sensitivity (dps/LSB) */
  IMUx->Data->gyrScale[0] = scale[0];
  IMUx->Data->gyrScale[1] = scale[0];
  IMUx->Data->gyrScale[2] = scale[0];

  /* Set accelerometer sensitivity (g/LSB) */
  IMUx->Data->accScale[0] = scale[1];
  IMUx->Data->accScale[1] = scale[1];
  IMUx->Data->accScale[2] = scale[1];

  /* Set magnetometer sensitivity (uT/LSB) */
  IMUx->Data->magScale[0] = scale[2];
  IMUx->Data->magScale[1] = scale[2];
  IMUx->Data->magScale[2] = scale[2];

  /* Set ictemperature sensitivity (degC/LSB) */
  IMUx->Data->ictempScale  = scale[3];
  IMUx->Data->ictempOffset = scale[4];
#endif

#if defined(__LPS22)
  LPS22_GetSensitivity(scale);

  /* Set barometer pressure sensitivity (hPa/LSB) */
  IMUx->Data->baroScale[0] = scale[0];

  /* Set barometer temperature sensitivity (degC/LSB) */
  IMUx->Data->baroScale[1] = scale[1];
#endif
}

/**
  * @brief  IMU_MergeScaleStrength
  */
static void IMU_MergeScaleStrength( IMU_DataTypeDef *imux )
{
  /* Merge accelerometer scale and sensitivity (g/LSB) */
  imux->accScale[0] = imux->accScale[0] * imux->accStrength;
  imux->accScale[1] = imux->accScale[1] * imux->accStrength;
  imux->accScale[2] = imux->accScale[2] * imux->accStrength;

  /* Merge magnetometer scale and sensitivity (uT/LSB) */
  imux->magScale[0] = imux->magScale[0] * imux->magStrength;
  imux->magScale[1] = imux->magScale[1] * imux->magStrength;
  imux->magScale[2] = imux->magScale[2] * imux->magStrength;
}

/**
  * @brief  IMU_MergeScaleCalib
  */
//static void IMU_MergeScaleCalib( IMU_DataTypeDef *imux )
//{
//  /* Merge gyroscope scale and calibration (dps/LSB) */
//  imux->gyrCalib[0] = imux->gyrCalib[0] * imux->gyrScale[0];
//  imux->gyrCalib[1] = imux->gyrCalib[1] * imux->gyrScale[1];
//  imux->gyrCalib[2] = imux->gyrCalib[2] * imux->gyrScale[2];

//  /* Merge accelerometer scale and sensitivity (g/LSB) */
//  imux->accCalib[0] = imux->accCalib[0] * imux->accScale[0];
//  imux->accCalib[1] = imux->accCalib[1] * imux->accScale[1];
//  imux->accCalib[2] = imux->accCalib[2] * imux->accScale[2];
//  imux->accCalib[3] = imux->accCalib[3] * imux->accScale[0];
//  imux->accCalib[4] = imux->accCalib[4] * imux->accScale[1];
//  imux->accCalib[5] = imux->accCalib[5] * imux->accScale[2];
//  imux->accCalib[6] = imux->accCalib[6] * imux->accScale[0];
//  imux->accCalib[7] = imux->accCalib[7] * imux->accScale[1];
//  imux->accCalib[8] = imux->accCalib[8] * imux->accScale[2];

//  /* Merge magnetometer scale and sensitivity (uT/LSB) */
//  imux->magCalib[0] = imux->magCalib[0] * imux->magScale[0];
//  imux->magCalib[1] = imux->magCalib[1] * imux->magScale[1];
//  imux->magCalib[2] = imux->magCalib[2] * imux->magScale[2];
//  imux->magCalib[3] = imux->magCalib[3] * imux->magScale[0];
//  imux->magCalib[4] = imux->magCalib[4] * imux->magScale[1];
//  imux->magCalib[5] = imux->magCalib[5] * imux->magScale[2];
//  imux->magCalib[6] = imux->magCalib[6] * imux->magScale[0];
//  imux->magCalib[7] = imux->magCalib[7] * imux->magScale[1];
//  imux->magCalib[8] = imux->magCalib[8] * imux->magScale[2];
//}

/**
  * @brief  IMU_PrintData
  */
void IMU_PrintData( IMU_DataTypeDef *imux )
{
  printf("\r\n");
  printf("- Print IMU Data -----------------\r\n");
  printf("G_raw : %f, %f, %f\r\n", imux->gyrRaw[0], imux->gyrRaw[1], imux->gyrRaw[2]);
  printf("A_raw : %f, %f, %f\r\n", imux->accRaw[0], imux->accRaw[1], imux->accRaw[2]);
#if defined(__USE_MAGNETOMETER)
  printf("M_raw : %f, %f, %f\r\n", imux->magRaw[0], imux->magRaw[1], imux->magRaw[2]);
#endif
  printf("T_raw : %f\r\n", imux->ictempRaw);
#if defined(__USE_BAROMETER)
  printf("B_raw : %f, %f\r\n", imux->baroRaw[0], imux->baroRaw[1]);
#endif

  printf("\r\n");
  printf("G_offset : %f, %f, %f\r\n", imux->gyrOffset[0], imux->gyrOffset[1], imux->gyrOffset[2]);
  printf("A_offset : %f, %f, %f\r\n", imux->accOffset[0], imux->accOffset[1], imux->accOffset[2]);
#if defined(__USE_MAGNETOMETER)
  printf("M_offset : %f, %f, %f\r\n", imux->magOffset[0], imux->magOffset[1], imux->magOffset[2]);
#endif
  printf("T_offset : %f\r\n", imux->ictempOffset);

  printf("\r\n");
  printf("G_data : %f, %f, %f\r\n", imux->gyrData[0], imux->gyrData[1], imux->gyrData[2]);
  printf("A_data : %f, %f, %f\r\n", imux->accData[0], imux->accData[1], imux->accData[2]);
#if defined(__USE_MAGNETOMETER)
  printf("M_data : %f, %f, %f\r\n", imux->magData[0], imux->magData[1], imux->magData[2]);
#endif
  printf("T_data : %f\r\n", imux->ictempData);
#if defined(__USE_BAROMETER)
  printf("B_data : %f, %f\r\n", imux->baroData[0], imux->baroData[1]);
#endif

  printf("\r\n");
  printf("G_calib : %f, %f, %f\r\n", imux->gyrCalib[0], imux->gyrCalib[1], imux->gyrCalib[2]);
  printf("A_calib : %f, %f, %f\r\n", imux->accCalib[0], imux->accCalib[1], imux->accCalib[2]);
  printf("          %f, %f, %f\r\n", imux->accCalib[3], imux->accCalib[4], imux->accCalib[5]);
  printf("          %f, %f, %f\r\n", imux->accCalib[6], imux->accCalib[7], imux->accCalib[8]);
#if defined(__USE_MAGNETOMETER)
  printf("M_calib : %f, %f, %f\r\n", imux->magCalib[0], imux->magCalib[1], imux->magCalib[2]);
  printf("          %f, %f, %f\r\n", imux->magCalib[3], imux->magCalib[4], imux->magCalib[5]);
  printf("          %f, %f, %f\r\n", imux->magCalib[6], imux->magCalib[7], imux->magCalib[8]);
#endif

  printf("\r\n");
  printf("G_scale : %f, %f, %f\r\n", imux->gyrScale[0], imux->gyrScale[1], imux->gyrScale[2]);
  printf("A_scale : %f, %f, %f\r\n", imux->accScale[0], imux->accScale[1], imux->accScale[2]);
#if defined(__USE_MAGNETOMETER)
  printf("M_scale : %f, %f, %f\r\n", imux->magScale[0], imux->magScale[1], imux->magScale[2]);
#endif
  printf("T_scale : %f\r\n", imux->ictempScale);
#if defined(__USE_BAROMETER)
  printf("B_scale : %f, %f\r\n", imux->baroScale[0], imux->baroScale[1]);
#endif

  printf("\r\n");
#if defined(__USE_MAGNETOMETER)
  printf("A_strength : %f\r\n", imux->accStrength);
  printf("M_strength : %f\r\n", imux->magStrength);
#endif

  printf("----------------------------------\r\n");
  printf("\r\n\r\n");
}

/*************************************** END OF FILE ****************************************/
