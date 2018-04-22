/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_it.c
 *  @author  KitSprout
 *  @date    22-Apr-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_timer.h"

/** @addtogroup NRF5x_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
extern TIMER_BaseInitTypeDef htimer;
extern TIMER_ChannelInitTypeDef htimerCC;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void NMI_Handler( void ) { while(1); }
void HardFault_Handler( void ) { while(1); }
void MemoryManagement_Handler( void ) { while(1); }
void BusFault_Handler( void ) { while(1); }
void UsageFault_Handler( void ) { while(1); }
void SVC_Handler( void ) { while(1); }
//void DebugMon_Handler( void )
void PendSV_Handler( void ) { while(1); }
//void Default_Handler( void )

//void SysTick_Handler( void )
//void POWER_CLOCK_IRQHandler( void )
//void RADIO_IRQHandler( void )
//void UARTE0_UART0_IRQHandler( void )
//void SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler( void )
//void SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler( void )
//void NFCT_IRQHandler( void )
//void GPIOTE_IRQHandler( void )
//void SAADC_IRQHandler( void )

void TIMER0_IRQHandler( void )
{
  if (TIMER_EVENTS_COPPARE(htimer.Instance, htimerCC.Channel) != RESET) {
    TIMER_EVENTS_COPPARE(htimer.Instance, htimerCC.Channel) = RESET;
    TIMER_TASKS_CLEAR(htimer.Instance);
    htimerCC.EventCallback();
  }
}

//void TIMER1_IRQHandler( void )
//void TIMER2_IRQHandler( void )
//void RTC0_IRQHandler( void )
//void TEMP_IRQHandler( void )
//void RNG_IRQHandler( void )
//void ECB_IRQHandler( void )
//void CCM_AAR_IRQHandler( void )
//void WDT_IRQHandler( void )
//void RTC1_IRQHandler( void )
//void QDEC_IRQHandler( void )
//void COMP_LPCOMP_IRQHandler( void )
//void SWI0_EGU0_IRQHandler( void )
//void SWI1_EGU1_IRQHandler( void )
//void SWI2_EGU2_IRQHandler( void )
//void SWI3_EGU3_IRQHandler( void )
//void SWI4_EGU4_IRQHandler( void )
//void SWI5_EGU5_IRQHandler( void )
//void TIMER3_IRQHandler( void )
//void TIMER4_IRQHandler( void )
//void PWM0_IRQHandler( void )
//void PDM_IRQHandler( void )
//void MWU_IRQHandler( void )
//void PWM1_IRQHandler( void )
//void PWM2_IRQHandler( void )
//void SPIM2_SPIS2_SPI2_IRQHandler( void )
//void RTC2_IRQHandler( void )
//void I2S_IRQHandler( void )
//void FPU_IRQHandler( void )
//void USBD_IRQHandler( void )
//void UARTE1_IRQHandler( void )
//void QSPI_IRQHandler( void )
//void CRYPTOCELL_IRQHandler( void )
//void SPIM3_IRQHandler( void )
//void PWM3_IRQHandler( void )

/*************************************** END OF FILE ****************************************/
