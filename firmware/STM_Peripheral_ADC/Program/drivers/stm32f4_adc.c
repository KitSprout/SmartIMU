/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32f4_adc.c
 *  @author  KitSprout
 *  @date    10-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_adc.h"

/** @addtogroup STM32_Driver
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static ADC_HandleTypeDef hadc_1;
static __IO uint16_t ADC_CONV_BUFF[ADC_MAX_SAMPLEBUF][ADC_MAX_CHANNEL] = {0};

AdcHandle_st hAdc1 = {
  .handle     = &hadc_1,
  .convBuffer = (uint16_t*)ADC_CONV_BUFF,
};

/* Prototypes ------------------------------------------------------------------------------*/
/* MSP Functions ---------------------------------------------------------------------------*/

void HAL_ADC_MspInit( ADC_HandleTypeDef *hadc )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  static DMA_HandleTypeDef hadc_dma;

  if (hadc->Instance == ADCx) {

    /* ADC and DMA Clk ***********************************************************/
    ADCx_CLK_ENABLE();
    ADCx_DMA_CLK_ENABLE();

    /* ADC Pin *******************************************************************/
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    GPIO_InitStruct.Pin  = ADCx_CHANNEL_PIN;
    HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

    /* DMA Init ******************************************************************/
    hadc_dma.Instance                 = ADCx_DMA_STREAM;
    hadc_dma.Init.Channel             = ADCx_DMA_CHANNEL;
    hadc_dma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hadc_dma.Init.PeriphInc           = DMA_PINC_DISABLE;
    hadc_dma.Init.MemInc              = DMA_MINC_ENABLE;
    hadc_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hadc_dma.Init.MemDataAlignment    = DMA_PDATAALIGN_HALFWORD;
    hadc_dma.Init.Mode                = DMA_CIRCULAR;
    hadc_dma.Init.Priority            = DMA_PRIORITY_HIGH;
    hadc_dma.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hadc_dma.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_HALFFULL;
    hadc_dma.Init.MemBurst            = DMA_MBURST_SINGLE;
    hadc_dma.Init.PeriphBurst         = DMA_PBURST_SINGLE;
    HAL_DMA_Init(&hadc_dma);
    __HAL_LINKDMA(hadc, DMA_Handle, hadc_dma);

    /* DMA IT ********************************************************************/
    HAL_NVIC_SetPriority(ADCx_DMA_IRQn, ADCx_DMA_IRQn_PREEMPT, ADCx_DMA_IRQn_SUB);
    HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);
  }
}

/**
 *  @brief ADC MSP De-Initialization
 */
void HAL_ADC_MspDeInit( ADC_HandleTypeDef *hadc )
{
  if (hadc->Instance == ADCx) {
    ADCx_FORCE_RESET();
    ADCx_RELEASE_RESET();
    HAL_GPIO_DeInit(ADCx_CHANNEL_GPIO_PORT, ADCx_CHANNEL_PIN);
  }
}

/**
 *  @brief  Regular conversion complete callback in non blocking mode 
 */
void HAL_ADC_ConvCpltCallback( ADC_HandleTypeDef* hadc )
{
  if (hadc->Instance == ADCx) {
    // ...
  }
}

/**
  * @brief  Regular conversion half DMA transfer callback in non blocking mode 
  */
void HAL_ADC_ConvHalfCpltCallback( ADC_HandleTypeDef* hadc )
{
  if (hadc->Instance == ADCx) {
    // ...
  }
}

/**
 *  @brief  Analog watchdog callback in non blocking mode 
 */
void HAL_ADC_LevelOutOfWindowCallback( ADC_HandleTypeDef* hadc )
{
  if (hadc->Instance == ADCx) {
    // ...
  }
}

/**
 *  @brief  Error ADC callback.
 */
void HAL_ADC_ErrorCallback( ADC_HandleTypeDef *hadc )
{
  if (hadc->Instance == ADCx) {
    // ...
  }
}

/**
 *  @brief  Injected conversion complete callback in non blocking mode 
 */
void HAL_ADCEx_InjectedConvCpltCallback( ADC_HandleTypeDef* hadc )
{
  if (hadc->Instance == ADCx) {
    // ...
  }
}

/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  ADC_Config
 */
void ADC_Config( void )
{
  ADC_ChannelConfTypeDef ADC_ChannelConfStruct;

  /* ADC Init ******************************************************************/
  hadc_1.Instance                   = ADCx;
  hadc_1.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
  hadc_1.Init.Resolution            = ADC_RESOLUTION_12B;
  hadc_1.Init.ScanConvMode          = DISABLE;
  hadc_1.Init.ContinuousConvMode    = ENABLE;
  hadc_1.Init.DiscontinuousConvMode = DISABLE;
  hadc_1.Init.NbrOfDiscConversion   = 0;
  hadc_1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc_1.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
  hadc_1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  hadc_1.Init.NbrOfConversion       = ADC_MAX_CHANNEL;
  hadc_1.Init.DMAContinuousRequests = ENABLE;
  hadc_1.Init.EOCSelection          = DISABLE;
  if (HAL_ADC_Init(&hadc_1) != HAL_OK)
    while (1) { ; }

  /* ADC Channel Init **********************************************************/
  ADC_ChannelConfStruct.Channel      = ADCx_CHANNEL;
  ADC_ChannelConfStruct.Rank         = ADCx_RANK;
  ADC_ChannelConfStruct.SamplingTime = ADCx_SAMPLETIME;
  ADC_ChannelConfStruct.Offset       = 0;
  if (HAL_ADC_ConfigChannel(&hadc_1, &ADC_ChannelConfStruct) != HAL_OK)
    while (1) { ; }

  /* ADC Start *****************************************************************/
  if (HAL_ADC_Start_DMA(&hadc_1, (uint32_t*)ADC_CONV_BUFF, ADC_MAX_CHANNEL * ADC_MAX_SAMPLEBUF) != HAL_OK)
    while (1) { ; }
}

/**
 *  @brief  ADC_GetValue
 */
uint16_t ADC_GetValue( uint8_t index )
{
  return ((uint16_t)(ADC_CONV_BUFF[0][index]));
}

/**
 *  @brief  ADC_GetAveValue
 */
void ADC_GetAveValue( uint16_t *aveAdc )
{
  for (uint32_t i = 0; i < ADC_MAX_CHANNEL; i++) {
    uint32_t tmp = 0;
    for (uint32_t j = 0; j < ADC_MAX_SAMPLEBUF; j++) {
      tmp += ADC_CONV_BUFF[j][i];
    }
    aveAdc[i] = tmp >> 3;
  }
}

/*************************************** END OF FILE ****************************************/
