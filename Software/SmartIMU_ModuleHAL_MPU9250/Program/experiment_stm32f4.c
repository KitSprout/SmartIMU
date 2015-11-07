/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\module_serial.h"
#include "modules\module_mpu9250.h"

#include "experiment_stm32f4.h"
/*====================================================================================================*/
/*====================================================================================================*/
void System_Init( void )
{
  uint8_t state = ERROR;
  MPU_InitTypeDef MPU_InitStruct;

  HAL_Init();
  GPIO_Config();
  Serial_Config();

  LED_B_Reset();
  MPU_InitStruct.MPU_Gyr_FullScale     = MPU_GyrFS_2000dps;
  MPU_InitStruct.MPU_Gyr_LowPassFilter = MPU_GyrLPS_41Hz;
  MPU_InitStruct.MPU_Acc_FullScale     = MPU_AccFS_4g;
  MPU_InitStruct.MPU_Acc_LowPassFilter = MPU_AccLPS_41Hz;
  state = MPU9250_Init(&MPU_InitStruct);
  printf("\fMPU9250 Init ... ");
  if(state != SUCCESS) {
    printf("ERROR\r\n");
    while(1) {
      LED_R_Toggle();
      Delay_100ms(1);
    }
  }
  else {
    printf("SUCCESS\r\n");
  }
  LED_R_Set();
  LED_B_Set();
  Delay_100ms(1);
}

int main( void )
{
  int16_t dataIMU[10] = {0};

  System_Init();

  while(1) {
    LED_G_Toggle();
    Delay_100ms(2);

    MPU9250_getData(dataIMU);
    printf("AccX:%5i\tAccY:%5i\tAccZ:%5i\tGyrX:%5i\tGyrY:%5i\tGyrZ:%5i\tMagX:%5i\tMagY:%5i\tMagZ:%5i\r\n", dataIMU[1], dataIMU[2], dataIMU[3], dataIMU[4], dataIMU[5], dataIMU[6], dataIMU[7], dataIMU[8], dataIMU[9]);
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
