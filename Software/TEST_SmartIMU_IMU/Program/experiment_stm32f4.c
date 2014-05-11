/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
#include "experiment_stm32f4.h"
#include "module_rs232.h"
#include "module_imu.h"
#include "module_mpu9250.h"
#include "algorithm_ahrs.h"
#include "algorithm_string.h"
/*====================================================================================================*/
/*====================================================================================================*/
void System_Init( void )
{
  SystemInit();

  GPIO_Config();
  RS232_Config();
  Sensor_Config();

  Sensor_Init();

  AHRS_Init(&NumQ, &AngE);

  /* Systick Setup */
  if(SysTick_Config((u32)(SystemCoreClock/(float)SampleRateFreg)))
    while(1);
}
/*====================================================================================================*/
/*====================================================================================================*/
int main( void )
{
  u8 UART_BUF[32] = {0};

  System_Init();

  while(1) {
    LED = !LED;
    Delay_10ms(2);

    UART_BUF[0]  = Byte8L((s16)(Acc.TrueX*1000)); // Acc.X 1 mg/LSB
    UART_BUF[1]  = Byte8H((s16)(Acc.TrueX*1000));
    UART_BUF[2]  = Byte8L((s16)(Acc.TrueY*1000)); // Acc.Y 1 mg/LSB
    UART_BUF[3]  = Byte8H((s16)(Acc.TrueY*1000));
    UART_BUF[4]  = Byte8L((s16)(Acc.TrueZ*1000)); // Acc.Z 1 mg/LSB
    UART_BUF[5]  = Byte8H((s16)(Acc.TrueZ*1000));

    UART_BUF[6]  = Byte8L((s16)(Gyr.TrueX*100));  // Gyr.X 10 mdps/LSB
    UART_BUF[7]  = Byte8H((s16)(Gyr.TrueX*100));
    UART_BUF[8]  = Byte8L((s16)(Gyr.TrueY*100));  // Gyr.Y 10 mdps/LSB
    UART_BUF[9]  = Byte8H((s16)(Gyr.TrueY*100));
    UART_BUF[10] = Byte8L((s16)(Gyr.TrueZ*100));  // Gyr.Z 10 mdps/LSB
    UART_BUF[11] = Byte8H((s16)(Gyr.TrueZ*100));

#ifdef USE_SENSOR_MAG
    UART_BUF[12] = Byte8L((s16)(Mag.TrueX));      // 100 nTesla/LSB
    UART_BUF[13] = Byte8L((s16)(Mag.TrueX));
    UART_BUF[14] = Byte8L((s16)(Mag.TrueY));      // 100 nTesla/LSB
    UART_BUF[15] = Byte8H((s16)(Mag.TrueY));
    UART_BUF[16] = Byte8L((s16)(Mag.TrueZ));      // 100 nTesla/LSB
    UART_BUF[17] = Byte8H((s16)(Mag.TrueZ));
#endif

#ifdef USE_SENSOR_TEMP
    UART_BUF[18] = Byte8L((s16)(Temp.TrueT*100)); // 0.01 degC/LSB
    UART_BUF[19] = Byte8H((s16)(Temp.TrueT*100));
#endif

    UART_BUF[20] = Byte8L((s16)(AngE.Pitch*100)); // 0.01 deg/LSB
    UART_BUF[21] = Byte8H((s16)(AngE.Pitch*100)); // 0.01 deg/LSB
    UART_BUF[22] = Byte8L((s16)(AngE.Roll*100));  // 0.01 deg/LSB
    UART_BUF[23] = Byte8H((s16)(AngE.Roll*100));  // 0.01 deg/LSB
    UART_BUF[24] = Byte8L((s16)(AngE.Yaw*10));    // 100 nTesla/LSB
    UART_BUF[25] = Byte8H((s16)(AngE.Yaw*10));    // 0.1 deg/LSB

#ifdef USE_SENSOR_BARO
    UART_BUF[26] = Byte8L((s16)(Baro.Temp*100));  // 0.01 degC/LSB
    UART_BUF[27] = Byte8H((s16)(Baro.Temp*100));

    UART_BUF[28] = Byte8L((s16)(Baro.Press*10));  // 0.1 mbar/LSB
    UART_BUF[29] = Byte8H((s16)(Baro.Press*10));

    UART_BUF[30] = Byte8L((s16)(Baro.Height));
    UART_BUF[31] = Byte8H((s16)(Baro.Height));
#endif

    RS232_VisualScope(UART_BUF+20); // Print Acc
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
void GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* LED Clk Init *************************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  /* LED PC13 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  LED = 0;
}
/*====================================================================================================*/
/*====================================================================================================*/
