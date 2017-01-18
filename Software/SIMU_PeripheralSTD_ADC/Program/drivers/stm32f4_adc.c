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
  * @date    18-Jan-2016
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
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
static __IO uint16_t AdcConvBuf[BAT_MAX_CONVBUF]= {0};

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void ADC_Config( void )
{
  ADC_InitTypeDef ADC_InitStruct;
  ADC_CommonInitTypeDef ADC_CommonInitStruct;
  DMA_InitTypeDef DMA_InitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;

  /* ADC Clk *******************************************************************/
  BAT_ADCx_CLK_ENABLE();
  BAT_DMAx_CLK_ENABLE();

  /* DMA Init ******************************************************************/
  DMA_InitStruct.DMA_Channel            = BAT_ADCx_DMA_CHANNEL;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&BAT_ADCx->DR;
  DMA_InitStruct.DMA_Memory0BaseAddr    = (uint32_t)AdcConvBuf;
  DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  DMA_InitStruct.DMA_BufferSize         = BAT_MAX_CONVBUF;
  DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Disable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct.DMA_Mode               = DMA_Mode_Circular;
  DMA_InitStruct.DMA_Priority           = DMA_Priority_High;
  DMA_InitStruct.DMA_FIFOMode           = DMA_FIFOMode_Disable;         
  DMA_InitStruct.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_InitStruct.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
  DMA_Init(BAT_ADCx_DMA_STREAM, &DMA_InitStruct);
  DMA_Cmd(BAT_ADCx_DMA_STREAM, ENABLE);

  /* GPIO Init *****************************************************************/
  GPIO_InitStruct.GPIO_Pin  = BAT_ADCx_CHANNEL_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(BAT_ADCx_CHANNEL_PORT, &GPIO_InitStruct);

  /* ADC Common Init ***********************************************************/
  ADC_CommonInitStruct.ADC_Mode             = ADC_Mode_Independent;
  ADC_CommonInitStruct.ADC_Prescaler        = ADC_Prescaler_Div2;
  ADC_CommonInitStruct.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStruct);

  /* ADC Init ******************************************************************/
  ADC_InitStruct.ADC_Resolution           = ADC_Resolution_12b;
  ADC_InitStruct.ADC_ScanConvMode         = DISABLE;
  ADC_InitStruct.ADC_ContinuousConvMode   = ENABLE;
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStruct.ADC_ExternalTrigConv     = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStruct.ADC_DataAlign            = ADC_DataAlign_Right;
  ADC_InitStruct.ADC_NbrOfConversion      = 1;
  ADC_Init(BAT_ADCx, &ADC_InitStruct);

  /* ADC Channel ***************************************************************/
  ADC_RegularChannelConfig(BAT_ADCx, BAT_ADCx_CHANNEL, BAT_ADCx_RANK, BAT_ADCx_SAMPLECYCLES);

  /* ADC ENABLE ****************************************************************/
  ADC_DMARequestAfterLastTransferCmd(BAT_ADCx, ENABLE);
  ADC_DMACmd(BAT_ADCx, ENABLE);
  ADC_Cmd(BAT_ADCx, ENABLE);

  ADC_SoftwareStartConv(BAT_ADCx);
}

uint16_t ADC_GetValue( void )
{
  return ((uint16_t)(AdcConvBuf[0] * 1.611328125f));
}

/*************************************** END OF FILE ****************************************/
