/*====================================================================================================*/
/*====================================================================================================*/
#include "Dirvers\stm32f4_system.h"

#include "experiment_stm32f4.h"
/*====================================================================================================*/
/*====================================================================================================*/
/*
System Clock source            = PLL (HSE)
SYSCLK(Hz)                     = 180000000
HCLK(Hz)                       = 180000000
AHB Prescaler                  = 1
APB1 Prescaler                 = 4
APB2 Prescaler                 = 2
HSE Frequency(Hz)              = 8000000
PLL_M                          = 8
PLL_N                          = 360
PLL_P                          = 2
PLL_Q                          = 7
VDD(V)                         = 3.3
Main regulator output voltage  = Scale1 mode
Flash Latency(WS)              = 5
*/
void HAL_MspInit( void )
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef state = HAL_OK;

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM       = 8;
  RCC_OscInitStruct.PLL.PLLN       = 360;
  RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ       = 7;
  RCC_OscInitStruct.PLL.PLLR       = 2;
  state = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(state != HAL_OK)
    while(1) { ; }

  state = HAL_PWREx_EnableOverDrive();
  if(state != HAL_OK)
    while(1) { ; }

  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  state = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if(state != HAL_OK)
    while(1) { ; }
}
/*====================================================================================================*/
/*====================================================================================================*/
