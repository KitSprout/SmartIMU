/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32f4xx_bsp.c
 *  @author  KitSprout
 *  @date    08-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_tim.h"
#include "modules\serial.h"
#include "modules\kSerial.h"
#include "modules\imu.h"
#include "stm32f4xx_bsp.h"

/** @addtogroup STM32_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
void HW_BoardInformation( void );

/* Functions -------------------------------------------------------------------------------*/

void bsp_gpio_init( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  HAL_Init();

  /* Enable all GPIO Clk *******************************************************/
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* GPIO all analog input *****************************************************/
  GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  GPIO_InitStruct.Pin   = GPIO_PIN_All & (~(GPIO_PIN_13 | GPIO_PIN_14));
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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

  LED_R_Off();
  LED_G_Off();
  LED_B_Off();
}

void bsp_timer_init( pFunc event, uint32_t freq )
{
  hTimer10.tickEventCallback = event;
  Timer10_Config(TIMx_CLOCK(freq, 1000), 1000);
}

void bsp_timer_enable( uint32_t state )
{
  Timer10_Cmd(state);
}

void bsp_uart_serial_init( pFunc event )
{
  hSerial.rxEventCallback = event;

  Serial_Config();

//  HW_BoardInformation();
//  printf("\r\nHello World!\r\n\r\n");

  printf("\f");
}

void bsp_sensor_init( void )
{
  uint32_t status;

  printf("ks-imu sensor init ... ");

  IMU_Config();
  delay_ms(50);

  status = IMU_Init();
  if (status != KS_OK) {
    printf("error");
    while (1) {
      LED_R_Toggle();
      delay_ms(100);
    }
  }
  else {
    printf("ok");
  }
  printf("\r\n");
}

uint32_t bsp_sensor_gyr_acc_mag_data( float32_t gyr[3], float32_t acc[3], float32_t mag[3] )
{
  float32_t data[9] = {0};
  uint32_t status;

  status = MPU92_GetData(data);

  // conv. to ENU
  gyr[0] = -data[1] * 0.0174532925f;  // rad/s
  gyr[1] = -data[0] * 0.0174532925f;
  gyr[2] = -data[2] * 0.0174532925f;
  acc[0] =  data[4] * 9.8f;           // m/s^2
  acc[1] =  data[3] * 9.8f;
  acc[2] =  data[5] * 9.8f;
  mag[0] = -data[6];                  // uT
  mag[1] = -data[7];
  mag[2] =  data[8];

  return status;
}

uint32_t bsp_sensor_bar_data( float32_t bar[2] )
{
  float32_t data[2] = {0};
  uint32_t status;

  status = LPS22_GetData(data);

  bar[0] = data[0];  // hpa
  bar[1] = data[1];  // degC

  return status;
}

/*************************************** END OF FILE ****************************************/
