/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_rcc.h"
#include "modules\module_serial.h"
#include "modules\module_imu.h"

#include "experiment_stm32f4.h"
/*====================================================================================================*/
/*====================================================================================================*/
void System_Init( void )
{
  uint8_t state = ERROR;
  MPU_InitTypeDef MPU_InitStruct;

  HAL_Init();
  SystemClock_Config();
  GPIO_Config();
  Serial_Config();

  LED_R_Reset;
  Delay_100ms(1);

  MPU_InitStruct.MPU_Gyr_FullScale     = MPU_GyrFS_2000dps;
  MPU_InitStruct.MPU_Gyr_LowPassFilter = MPU_GyrLPS_41Hz;
  MPU_InitStruct.MPU_Acc_FullScale     = MPU_AccFS_4g;
  MPU_InitStruct.MPU_Acc_LowPassFilter = MPU_AccLPS_41Hz;
  state = IMU_Init(&MPU_InitStruct);
  while(state != SUCCESS) {
    LED_G_Toggle;
    Delay_100ms(1);
  }

  LED_G_Reset;
  Delay_100ms(1);
}

int main( void )
{
  int16_t dataIMU[10] = {0};

  System_Init();

  while(1) {
    LED_B_Toggle;
    Delay_100ms(1);

    IMU_getData(dataIMU);
    printf("AccX:%d\tAccY:%d\tAccZ:%d\tGyrX:%d\tGyrY:%d\tGyrZ:%d\tMagX:%d\tMagY:%d\tMagZ:%d\t\r\n", dataIMU[1], dataIMU[2], dataIMU[3], dataIMU[4], dataIMU[5], dataIMU[6], dataIMU[7], dataIMU[8], dataIMU[9]);
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
void GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Clk ******************************************************************/
  LED_R_GPIO_CLK_ENABLE();
  LED_G_GPIO_CLK_ENABLE();
  LED_B_GPIO_CLK_ENABLE();

  /* GPIO Pin ******************************************************************/
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin   = LED_R_PIN;
  HAL_GPIO_Init(LED_R_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin   = LED_G_PIN;
  HAL_GPIO_Init(LED_G_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin   = LED_B_PIN;
  HAL_GPIO_Init(LED_B_GPIO_PORT, &GPIO_InitStruct);

  /* GPIO Pin ******************************************************************/
  LED_R_Set;
  LED_G_Set;
  LED_B_Set;
}
/*====================================================================================================*/
/*====================================================================================================*/
