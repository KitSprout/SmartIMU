/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_adc.c
  * @author  KitSprout
  * @date    13-Nov-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4_system.h"
#include "stm32f4_adc.h"

/** @addtogroup STM32_Driver
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
#define ADCx                    ADC1
#define ADCx_CLK_ENABLE()       __HAL_RCC_ADC1_CLK_ENABLE()
#define DMAx_CLK_ENABLE()       __HAL_RCC_DMA2_CLK_ENABLE()

#define ADCx_CHANNEL_PIN        GPIO_PIN_1
#define ADCx_CHANNEL_GPIO_PORT  GPIOB

#define ADCx_CHANNEL            ADC_CHANNEL_9

#define ADCx_DMA_CHANNEL        DMA_CHANNEL_0
#define ADCx_DMA_STREAM         DMA2_Stream0
#define ADCx_DMA_IRQn           DMA2_Stream0_IRQn

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
ADC_HandleTypeDef AdcHandle;
__IO uint16_t ADCxConvertedValue = 0;

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void HAL_ADC_MspInit( ADC_HandleTypeDef *hadc )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  static DMA_HandleTypeDef hdma_adc;

  /* ADC and DMA Clk ***********************************************************/
  ADCx_CLK_ENABLE();
  DMAx_CLK_ENABLE();

  /* ADC Pin *******************************************************************/
  GPIO_InitStruct.Pin  = ADCx_CHANNEL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

  /* DMA Init ******************************************************************/
  hdma_adc.Instance                 = ADCx_DMA_STREAM;
  hdma_adc.Init.Channel             = ADCx_DMA_CHANNEL;
  hdma_adc.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_adc.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_adc.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_adc.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdma_adc.Init.Mode                = DMA_CIRCULAR;
  hdma_adc.Init.Priority            = DMA_PRIORITY_HIGH;
  hdma_adc.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
  hdma_adc.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_HALFFULL;
  hdma_adc.Init.MemBurst            = DMA_MBURST_SINGLE;
  hdma_adc.Init.PeriphBurst         = DMA_PBURST_SINGLE;
  HAL_DMA_Init(&hdma_adc);
  __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc);

  /* DMA IT ********************************************************************/
  HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 0x0F, 0x0F);
  HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);
}

/**
  * @brief  ADC_Config
  * @param  None
  * @retval None
  */
void ADC_Config( void )
{
  ADC_ChannelConfTypeDef ADC_ChannelConfStruct;

  /* ADC Init ******************************************************************/
  AdcHandle.Instance                   = ADCx;
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.ScanConvMode          = DISABLE;
  AdcHandle.Init.ContinuousConvMode    = ENABLE;
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.NbrOfDiscConversion   = 0;
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.NbrOfConversion       = 1;
  AdcHandle.Init.DMAContinuousRequests = ENABLE;
  AdcHandle.Init.EOCSelection          = DISABLE;
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
    while (1) { ; }

  /* ADC Channel Init **********************************************************/
  ADC_ChannelConfStruct.Channel      = ADCx_CHANNEL;
  ADC_ChannelConfStruct.Rank         = 1;
  ADC_ChannelConfStruct.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  ADC_ChannelConfStruct.Offset       = 0;
  if (HAL_ADC_ConfigChannel(&AdcHandle, &ADC_ChannelConfStruct) != HAL_OK)
    while (1) { ; }

  /* ADC Start *****************************************************************/
  if(HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*)&ADCxConvertedValue, 1) != HAL_OK)
    while (1) { ; }
}

#define ADC_CONV  1.611328125f  // R = R2/(R1+R2) = 0.5, 3300mV / (2^12 * R)
uint16_t ADC_GetValue( void )
{
  return ((uint16_t)(ADCxConvertedValue * 1.611328125f));
}

void HAL_ADC_ConvCpltCallback( ADC_HandleTypeDef* AdcHandle )
{
  UNUSED(AdcHandle);
}

/*************************************** END OF FILE ****************************************/
