/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    imu.c
 *  @author  KitSprout
 *  @date    09-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"
#include "modules\imu.h"

/** @addtogroup STM32_Module
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static uint8_t IMU_TX_BUFFER[IMU_MAX_TXBUF] = {0};
static uint8_t IMU_RX_BUFFER[IMU_MAX_RXBUF] = {0};

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  IMU_Config
 */
void IMU_Config( void )
{
  hImu.pTxBuf = IMU_TX_BUFFER;
  hImu.pRxBuf = IMU_RX_BUFFER;

#if defined(__MPU9250_H)
  MPU92_Config();
#endif

#if defined(__LPS22HB_H)
  LPS22_Config();
#endif

  /* SPI Init ****************************************************************/
  hImu.handle->Instance               = IMU_SPIx;
  hImu.handle->Init.Mode              = SPI_MODE_MASTER;
  hImu.handle->Init.Direction         = SPI_DIRECTION_2LINES;
  hImu.handle->Init.DataSize          = SPI_DATASIZE_8BIT;
  hImu.handle->Init.CLKPolarity       = SPI_POLARITY_HIGH;
  hImu.handle->Init.CLKPhase          = SPI_PHASE_2EDGE;
  hImu.handle->Init.NSS               = SPI_NSS_SOFT;
  hImu.handle->Init.BaudRatePrescaler = IMU_SPIx_SPEED_LOW;
  hImu.handle->Init.FirstBit          = SPI_FIRSTBIT_MSB;
  hImu.handle->Init.TIMode            = SPI_TIMODE_DISABLE;
  hImu.handle->Init.CRCCalculation    = SPI_CRCCALCULATION_ENABLE;
  hImu.handle->Init.CRCPolynomial     = 7;
  HAL_SPI_Init(hImu.handle);

  __HAL_SPI_ENABLE(hImu.handle);
}

/**
 *  @brief  IMU_Init
 */
uint32_t IMU_Init( void )
{
#if defined(__MPU9250_H)
  if (MPU92_Init() != KS_OK) {
    return KS_ERROR;
  }
#endif

#if defined(__LPS22HB_H)
  if (LPS22_Init() != KS_OK) {
    return KS_ERROR;
  }
#endif

  SPI_SetSpeed(hImu.handle, IMU_SPIx_SPEED_HIGH);
  delay_ms(10);

  return KS_OK;
}

/*************************************** END OF FILE ****************************************/
