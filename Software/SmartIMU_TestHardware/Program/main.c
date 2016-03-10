/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\module_imu.h"

#include "smartimu.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define GPIO_A_PINs       GPIO_PIN_15
#define GPIO_A_GPIO_PORT  GPIOA
#define GPIO_A_Set()      HAL_GPIO_WritePin(GPIO_A_GPIO_PORT, GPIO_A_PINs, GPIO_PIN_SET)
#define GPIO_A_Reset()    HAL_GPIO_WritePin(GPIO_A_GPIO_PORT, GPIO_A_PINs, GPIO_PIN_RESET)
#define GPIO_A_Toggle()   HAL_GPIO_TogglePin(GPIO_A_GPIO_PORT, GPIO_A_PINs)

#define GPIO_B_PINs       GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9
#define GPIO_B_GPIO_PORT  GPIOB
#define GPIO_B_Set()      HAL_GPIO_WritePin(GPIO_B_GPIO_PORT, GPIO_B_PINs, GPIO_PIN_SET)
#define GPIO_B_Reset()    HAL_GPIO_WritePin(GPIO_B_GPIO_PORT, GPIO_B_PINs, GPIO_PIN_RESET)
#define GPIO_B_Toggle()   HAL_GPIO_TogglePin(GPIO_B_GPIO_PORT, GPIO_B_PINs)

#define GPIO_C_PINs       GPIO_PIN_13 | GPIO_PIN_14
#define GPIO_C_GPIO_PORT  GPIOC
#define GPIO_C_Set()      HAL_GPIO_WritePin(GPIO_C_GPIO_PORT, GPIO_C_PINs, GPIO_PIN_SET)
#define GPIO_C_Reset()    HAL_GPIO_WritePin(GPIO_C_GPIO_PORT, GPIO_C_PINs, GPIO_PIN_RESET)
#define GPIO_C_Toggle()   HAL_GPIO_TogglePin(GPIO_C_GPIO_PORT, GPIO_C_PINs)

void GPIO_EX_Set( void )    { GPIO_A_Set();     GPIO_B_Set();     GPIO_C_Set();    }
void GPIO_EX_Reset( void )  { GPIO_A_Reset();   GPIO_B_Reset();   GPIO_C_Reset();  }
void GPIO_EX_Toggle( void ) { GPIO_A_Toggle();  GPIO_B_Toggle();  GPIO_C_Toggle(); }

void SIMU_GPIO_EX_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Clk ******************************************************************/
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* GPIO Pin ******************************************************************/
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin   = GPIO_A_PINs;
  HAL_GPIO_Init(GPIO_A_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin   = GPIO_B_PINs;
  HAL_GPIO_Init(GPIO_B_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin   = GPIO_C_PINs;
  HAL_GPIO_Init(GPIO_C_GPIO_PORT, &GPIO_InitStruct);

  /* GPIO Pin ******************************************************************/
  GPIO_EX_Set();
}
/*====================================================================================================*/
/*====================================================================================================*/
int main( void )
{
  SIMU_Init();
  SIMU_GPIO_EX_Config();

  while(1) {
    LED_G_Toggle();
    GPIO_EX_Toggle();
//    printf("IMU : %6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\r\n", IMU.Gyr[0], IMU.Gyr[1], IMU.Gyr[2], IMU.Acc[0], IMU.Acc[1], IMU.Acc[2], IMU.Mag[0], IMU.Mag[1], IMU.Mag[2]);
    delay_ms(100);
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
