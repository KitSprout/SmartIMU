/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    simuDev_bsp.c
  * @author  KitSprout
  * @date    12-Oct-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\kSerial.h"
#include "modules\imu.h"
#include "simuDev_bsp.h"
#include "simuDev_sample.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
pFunc IRQEven_TIM2 = NULL;
pFunc IRQEven_UART1 = NULL;

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void SIMU_GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable all GPIO Clk *******************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  /* GPIO all analog input *****************************************************/
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_All;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_Init(GPIOC, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_All & (~(GPIO_Pin_13 | GPIO_Pin_14));
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* GPIO Pin ******************************************************************/
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = LED_R_PIN;
  GPIO_Init(LED_R_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = LED_G_PIN;
  GPIO_Init(LED_G_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = LED_B_PIN;
  GPIO_Init(LED_B_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_InitStruct.GPIO_Pin   = KEY_PIN;
  GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStruct);

  LED_R_Set();
  LED_G_Set();
  LED_B_Set();
}

void SIMU_UART_Config( pFunc pUARTx )
{
  IRQEven_UART1 = pUARTx;

  Serial_Config();
  kSerial_config(Serial_SendByte);
  printf("\r\nHello World!\r\n\r\n");
}

extern IMU_DataTypeDef IMU;
void SIMU_IMU_Config( pFunc pTIMx )
{
  NVIC_InitTypeDef NVIC_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  IMU_InitTypeDef IMU_InitStruct;

  IRQEven_TIM2 = pTIMx;

  IMU_Config();
  delay_ms(50);

  printf("IMU Init ... ");
  IMU_InitStruct.Data                       = &IMU;
  IMU_InitStruct.Init.MPU_Gyr_FullScale     = MPU_GyrFS_2000dps;
  IMU_InitStruct.Init.MPU_Gyr_LowPassFilter = MPU_GyrLPS_41Hz;
  IMU_InitStruct.Init.MPU_Acc_FullScale     = MPU_AccFS_4g;
  IMU_InitStruct.Init.MPU_Acc_LowPassFilter = MPU_AccLPS_41Hz;
  IMU_InitStruct.Init.MPU_Mag_FullScale     = MPU_MagFS_16b;
  if (IMU_Init(&IMU_InitStruct) != SUCCESS) {
    printf("ERROR\r\n");
    while (1) {
      LED_R_Toggle();
      delay_ms(100);
    }
  }
  printf("SUCCESS\r\n\r\n");
  delay_ms(50);

  /* TIMX Clk ******************************************************************/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* NVIC Config ***************************************************************/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStruct.NVIC_IRQChannel                   = TIM2_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

  /* TIM Base Config ************************************************************/
  TIM_TimeBaseStruct.TIM_Prescaler     = (SystemCoreClock / 2 / (SAMPLE_RATE_FREQ * 1000)) - 1;
  TIM_TimeBaseStruct.TIM_Period        = 1000 - 1;
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStruct.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

  /* TIM Enable *****************************************************************/
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM2, ENABLE);

//  IMU_PrintData(&IMU);
//  while(1);
}

/*************************************** END OF FILE ****************************************/
