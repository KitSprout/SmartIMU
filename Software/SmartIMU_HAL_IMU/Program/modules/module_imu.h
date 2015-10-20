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


///* ---- Sensitivity --------------------------------------------------------- */

//#define MPU9250A_2g       ((float)0.000061035156f)  // 0.000061035156 g/LSB
//#define MPU9250A_4g       ((float)0.000122070312f)  // 0.000122070312 g/LSB
//#define MPU9250A_8g       ((float)0.000244140625f)  // 0.000244140625 g/LSB
//#define MPU9250A_16g      ((float)0.000488281250f)  // 0.000488281250 g/LSB

//#define MPU9250G_250dps   ((float)0.007633587786f)  // 0.007633587786 dps/LSB
//#define MPU9250G_500dps   ((float)0.015267175572f)  // 0.015267175572 dps/LSB
//#define MPU9250G_1000dps  ((float)0.030487804878f)  // 0.030487804878 dps/LSB
//#define MPU9250G_2000dps  ((float)0.060975609756f)  // 0.060975609756 dps/LSB

//#define MPU9250M_4800uT   ((float)0.6f)             // 0.6 uT/LSB

//#define MPU9250T_85degC   ((float)0.002995177763f)  // 0.002995177763 degC/LSB

///* ---- MPU6500 Reg In MPU9250 ---------------------------------------------- */

//#define MPU6500_Device_ID   ((uint8_t)0x71) // In MPU9250
//#define AK8963_Device_ID    ((uint8_t)0x48)
//#define LPS25H_Device_ID    ((uint8_t)0xBD)

/*=====================================================================================================*/
/*=====================================================================================================*/
void    IMU_GPIO_Config( void );
void    IMU_SPI_Config( void );
uint8_t IMU_Init( MPU_InitTypeDef *MPUx );
uint8_t IMU_deviceCheck( void );
void    IMU_getData( int16_t *dataIMU );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
