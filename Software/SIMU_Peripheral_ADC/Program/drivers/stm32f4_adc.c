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
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
static ADC_HandleTypeDef AdcHandle;
static __IO uint16_t AdcConvBuf[BAT_MAX_CONVBUF]= {0};

AdcHandle_st hAdc1 = {
  .handle    = &AdcHandle,
  .convValue = (uint16_t*)AdcConvBuf,
};

/* Private function prototypes -------------------------------------------------------------*/

/* MSP functions ---------------------------------------------------------------------------*/

void HAL_ADC_MspInit( ADC_HandleTypeDef *hadc )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  static DMA_HandleTypeDef hadcDma;

  if (hadc->Instance == BAT_ADCx) {

    /* ADC and DMA Clk ***********************************************************/
    BAT_ADCx_CLK_ENABLE();
    BAT_DMAx_CLK_ENABLE();

    /* ADC Pin *******************************************************************/
    GPIO_InitStruct.Pin  = BAT_ADCx_CHANNEL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BAT_ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

    /* DMA Init ******************************************************************/
    hadcDma.Instance                 = BAT_ADCx_DMA_STREAM;
    hadcDma.Init.Channel             = BAT_ADCx_DMA_CHANNEL;
    hadcDma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hadcDma.Init.PeriphInc           = DMA_PINC_DISABLE;
    hadcDma.Init.MemInc              = DMA_MINC_ENABLE;
    hadcDma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hadcDma.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    hadcDma.Init.Mode                = DMA_CIRCULAR;
    hadcDma.Init.Priority            = DMA_PRIORITY_HIGH;
    hadcDma.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hadcDma.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_HALFFULL;
    hadcDma.Init.MemBurst            = DMA_MBURST_SINGLE;
    hadcDma.Init.PeriphBurst         = DMA_PBURST_SINGLE;
    HAL_DMA_Init(&hadcDma);
    __HAL_LINKDMA(hadc, DMA_Handle, hadcDma);

    /* DMA IT ********************************************************************/
    HAL_NVIC_SetPriority(BAT_ADCx_DMA_IRQn, 0x0F, 0x0F);
    HAL_NVIC_EnableIRQ(BAT_ADCx_DMA_IRQn);
  }
}

/**
  * @brief ADC MSP De-Initialization
  */
void HAL_ADC_MspDeInit( ADC_HandleTypeDef *hadc )
{
  if (hadc->Instance == BAT_ADCx) {
    BAT_ADCx_FORCE_RESET();
    BAT_ADCx_RELEASE_RESET();
    HAL_GPIO_DeInit(BAT_ADCx_CHANNEL_GPIO_PORT, BAT_ADCx_CHANNEL_PIN);
  }
}

/**
  * @brief  Regular conversion complete callback in non blocking mode 
  */
void HAL_ADC_ConvCpltCallback( ADC_HandleTypeDef* hadc )
{
  if (hadc->Instance == BAT_ADCx) {
    // ...
  }
}

/**
  * @brief  Regular conversion half DMA transfer callback in non blocking mode 
  */
void HAL_ADC_ConvHalfCpltCallback( ADC_HandleTypeDef* hadc )
{
  if (hadc->Instance == BAT_ADCx) {
    // ...
  }
}

/**
  * @brief  Analog watchdog callback in non blocking mode 
  */
void HAL_ADC_LevelOutOfWindowCallback( ADC_HandleTypeDef* hadc )
{
  if (hadc->Instance == BAT_ADCx) {
    // ...
  }
}

/**
  * @brief  Error ADC callback.
  */
void HAL_ADC_ErrorCallback( ADC_HandleTypeDef *hadc )
{
  if (hadc->Instance == BAT_ADCx) {
    // ...
  }
}

/**
  * @brief  Injected conversion complete callback in non blocking mode 
  */
void HAL_ADCEx_InjectedConvCpltCallback( ADC_HandleTypeDef* hadc )
{
  if (hadc->Instance == BAT_ADCx) {
    // ...
  }
}

/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  ADC_Config
  */
void ADC_Config( void )
{
  ADC_ChannelConfTypeDef ADC_ChannelConfStruct;

  /* ADC Init ******************************************************************/
  AdcHandle.Instance                   = BAT_ADCx;
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.ScanConvMode          = DISABLE;
  AdcHandle.Init.ContinuousConvMode    = ENABLE;
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.NbrOfDiscConversion   = 0;
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.NbrOfConversion       = BAT_MAX_CONVBUF;
  AdcHandle.Init.DMAContinuousRequests = ENABLE;
  AdcHandle.Init.EOCSelection          = DISABLE;
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
    while (1) { ; }

  /* ADC Channel Init **********************************************************/
  ADC_ChannelConfStruct.Channel      = BAT_ADCx_CHANNEL;
  ADC_ChannelConfStruct.Rank         = BAT_ADCx_RANK;
  ADC_ChannelConfStruct.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  ADC_ChannelConfStruct.Offset       = 0;
  if (HAL_ADC_ConfigChannel(&AdcHandle, &ADC_ChannelConfStruct) != HAL_OK)
    while (1) { ; }

  /* ADC Start *****************************************************************/
  if(HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*)AdcConvBuf, BAT_MAX_CONVBUF) != HAL_OK)
    while (1) { ; }
}

uint16_t ADC_GetValue( void )
{
  return ((uint16_t)(AdcConvBuf[0] * 1.611328125f));
}

/*************************************** END OF FILE ****************************************/
