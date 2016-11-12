/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    smartimu_bsp.c
  * @author  KitSprout
  * @date    12-Nov-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\kSerial.h"
#include "modules\imu.h"
#include "smartimu_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
TIM_HandleTypeDef Timer2Handle;

pFunc IRQEven_TIM2 = NULL;
pFunc IRQEven_UART6 = NULL;

extern IMU_DataTypeDef IMU;

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void SIMU_GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

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

  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  GPIO_InitStruct.Pin   = KEY_PIN;
  HAL_GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStruct);

  LED_R_Set();
  LED_G_Set();
  LED_B_Set();
}

void SIMU_UART_Config( pFunc pUARTx )
{
  uint8_t interrupt = (pUARTx == NULL) ? DISABLE : ENABLE;
  IRQEven_UART6 = pUARTx;

  Serial_Config(interrupt);
  kSerial_config(Serial_SendByte);
  printf("\r\nHello World!\r\n\r\n");
}

void SIMU_IMU_Config( pFunc pTIMx, uint32_t sampleFreq )
{
  IMU_InitTypeDef IMU_InitStruct;

  IRQEven_TIM2 = pTIMx;

  IMU_Config();
  delay_ms(50);

  printf("IMU Init ... ");
  IMU_InitStruct.Data                           = &IMU;
  IMU_InitStruct.InitMPU.MPU_Gyr_FullScale      = MPU_GyrFS_2000dps;
  IMU_InitStruct.InitMPU.MPU_Gyr_LowPassFilter  = MPU_GyrLPS_41Hz;
  IMU_InitStruct.InitMPU.MPU_Acc_FullScale      = MPU_AccFS_4g;
  IMU_InitStruct.InitMPU.MPU_Acc_LowPassFilter  = MPU_AccLPS_41Hz;
  IMU_InitStruct.InitMPU.MPU_Mag_FullScale      = MPU_MagFS_16b;
  if(IMU_Init(&IMU_InitStruct) != SUCCESS) {
    printf("ERROR\r\n");
    while(1) {
      LED_R_Toggle();
      delay_ms(100);
    }
  }
  printf("SUCCESS\r\n\r\n");
  delay_ms(50);

  /* TIMX Clk ******************************************************************/
  __HAL_RCC_TIM2_CLK_ENABLE();

  /* NVIC Config ***************************************************************/
  HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);

  /* TIM Base Config ************************************************************/
  Timer2Handle.Instance               = TIM2;
  Timer2Handle.Init.Prescaler         = (SystemCoreClock / (sampleFreq * 1000)) - 1;
  Timer2Handle.Init.Period            = 1000 - 1;
  Timer2Handle.Init.ClockDivision     = 0;
  Timer2Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  Timer2Handle.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&Timer2Handle);

  /* TIM Enable *****************************************************************/
  HAL_TIM_Base_Start_IT(&Timer2Handle);
}

/*************************************** END OF FILE ****************************************/
