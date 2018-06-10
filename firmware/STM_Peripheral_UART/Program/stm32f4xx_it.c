/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32f4xx_it.c
 *  @author  KitSprout
 *  @date    03-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_tim.h"
#include "modules\serial.h"

/** @addtogroup STM32_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void NMI_Handler( void ) { while(1); }
void HardFault_Handler( void ) { while(1); }
void MemManage_Handler( void ) { while(1); }
void BusFault_Handler( void ) { while(1); }
void UsageFault_Handler( void ) { while(1); }
void SVC_Handler( void ) {}
void DebugMon_Handler( void ) {}
void PendSV_Handler( void ) {}
void SysTick_Handler( void ) { HAL_IncTick(); }

//void WWDG_IRQHandler( void )
//void PVD_IRQHandler( void )
//void TAMP_STAMP_IRQHandler( void )
//void RTC_WKUP_IRQHandler( void )
//void FLASH_IRQHandler( void )
//void RCC_IRQHandler( void )
//void EXTI0_IRQHandler( void )
//void EXTI1_IRQHandler( void )
//void EXTI2_IRQHandler( void )
//void EXTI3_IRQHandler( void )
//void EXTI4_IRQHandler( void )
//void DMA1_Stream0_IRQHandler( void )
//void DMA1_Stream1_IRQHandler( void )
//void DMA1_Stream2_IRQHandler( void )
//void DMA1_Stream3_IRQHandler( void )
//void DMA1_Stream4_IRQHandler( void )
//void DMA1_Stream5_IRQHandler( void )
//void DMA1_Stream6_IRQHandler( void )
//void ADC_IRQHandler( void )
//void EXTI9_5_IRQHandler( void )
//void TIM1_BRK_TIM9_IRQHandler( void )

void TIM1_UP_TIM10_IRQHandler( void )
{
  if (__HAL_TIM_GET_FLAG(hTimer10.handle, TIM_FLAG_UPDATE) != RESET) {
    if (__HAL_TIM_GET_IT_SOURCE(hTimer10.handle, TIM_IT_UPDATE) != RESET) {
      __HAL_TIM_CLEAR_IT(hTimer10.handle, TIM_IT_UPDATE);
      hTimer10.tickEventCallback();
    }
  }
}

//void TIM1_TRG_COM_TIM11_IRQHandler( void )
//void TIM1_CC_IRQHandler( void )
//void TIM2_IRQHandler( void )
//void TIM3_IRQHandler( void )
//void TIM4_IRQHandler( void )
//void I2C1_EV_IRQHandler( void )
//void I2C1_ER_IRQHandler( void )
//void I2C2_EV_IRQHandler( void )
//void I2C2_ER_IRQHandler( void )
//void SPI1_IRQHandler( void )
//void SPI2_IRQHandler( void )

void USART1_IRQHandler( void )
{
#if KS_FW_UART_HAL_LIBRARY
  HAL_UART_IRQHandler(hSerial.handle);

#else
  if (__HAL_UART_GET_IT_SOURCE(hSerial.handle, UART_IT_TXE) != RESET) {
    __HAL_UART_GET_IT_SOURCE(hSerial.handle, UART_IT_TXE);
    HAL_UART_TxCpltCallback(hSerial.handle);
  }
  if (__HAL_UART_GET_IT_SOURCE(hSerial.handle, UART_IT_RXNE) != RESET) {
    __HAL_UART_GET_IT_SOURCE(hSerial.handle, UART_IT_RXNE);
    HAL_UART_RxCpltCallback(hSerial.handle);
  }

#endif
}

//void USART2_IRQHandler( void )
//void EXTI15_10_IRQHandler( void )
//void RTC_Alarm_IRQHandler( void )
//void OTG_FS_WKUP_IRQHandler( void )
//void DMA1_Stream7_IRQHandler( void )
//void SDIO_IRQHandler( void )
//void TIM5_IRQHandler( void )
//void SPI3_IRQHandler( void )
//void DMA2_Stream0_IRQHandler( void )
//void DMA2_Stream1_IRQHandler( void )
//void DMA2_Stream2_IRQHandler( void )
//void DMA2_Stream3_IRQHandler( void )
//void DMA2_Stream4_IRQHandler( void )
//void OTG_FS_IRQHandler( void )
//void DMA2_Stream5_IRQHandler( void )
//void DMA2_Stream6_IRQHandler( void )
//void DMA2_Stream7_IRQHandler( void )
//void USART6_IRQHandler( void )
//void I2C3_EV_IRQHandler( void )
//void I2C3_ER_IRQHandler( void )
//void FPU_IRQHandler( void )
//void SPI4_IRQHandler( void )
//void SPI5_IRQHandler( void )

/*************************************** END OF FILE ****************************************/
