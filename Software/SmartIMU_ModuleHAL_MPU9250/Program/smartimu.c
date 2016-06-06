/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\module_serial.h"
#include "modules\module_mpu9250.h"
#include "applications\app_kSerial.h"

#include "smartimu.h"
/*====================================================================================================*/
/*====================================================================================================*/
void SIMU_Init( void )
{
  HAL_Init();

  SIMU_GPIO_Config();
  SIMU_UART_Config();
  SIMU_MPU9250_Config();
}

void SIMU_Loop( void )
{
  int16_t imu[10] = {0};

  while(1) {
    LED_G_Toggle();
    delay_ms(100);

    MPU9250_getData(imu);
//    Serial_SendDataMATLAB(imu, 10);
//    kSerial_sendData(imu + 1, KS_INT16, 9);
    printf("AX:%5i\tAY:%5i\tAZ:%5i\tGX:%5i\tGY:%5i\tGZ:%5i\tMX:%5i\tMY:%5i\tMZ:%5i\r\n", imu[1], imu[2], imu[3], imu[4], imu[5], imu[6], imu[7], imu[8], imu[9]);
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
