/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "experiment_stm32f4.h"
#include "module_imu.h"
#include "algorithm_ahrs.h"
/*====================================================================================================*/
/*====================================================================================================*/
void SysTick_Handler( void )
{
  Sensor_Read(SampleRateFreg);

  Acc.TrueX  = Acc.X*MPU9250A_4g;       // g/LSB
  Acc.TrueY  = Acc.Y*MPU9250A_4g;       // g/LSB
  Acc.TrueZ  = Acc.Z*MPU9250A_4g;       // g/LSB
  Gyr.TrueX  = Gyr.X*MPU9250G_2000dps;  // dps/LSB
  Gyr.TrueY  = Gyr.Y*MPU9250G_2000dps;  // dps/LSB
  Gyr.TrueZ  = Gyr.Z*MPU9250G_2000dps;  // dps/LSB

#ifdef USE_SENSOR_MAG
  Mag.TrueX  = Mag.X*MPU9250M_4800uT;   // uT/LSB
  Mag.TrueY  = Mag.Y*MPU9250M_4800uT;   // uT/LSB
  Mag.TrueZ  = Mag.Z*MPU9250M_4800uT;   // uT/LSB
#endif

#ifdef USE_SENSOR_TEMP
  Temp.TrueT = Temp.T*MPU9250T_85degC;  // degC/LSB
#endif

  AHRS_Update();
}
/*====================================================================================================*/
/*====================================================================================================*/
void HardFault_Handler( void )
{
  while(1);
}
void MemManage_Handler( void )
{
  while(1);
}
void BusFault_Handler( void )
{
  while(1);
}
void UsageFault_Handler( void )
{
  while(1);
}
void SVC_Handler( void )
{
  while(1);
}
void DebugMon_Handler( void )
{
  while(1);
}
void PendSV_Handler( void )
{
  while(1);
}
void NMI_Handler( void )
{
  while(1);
}
/*====================================================================================================*/
/*====================================================================================================*/
