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
#include "modules\serial.h"

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

void UARTE0_IRQHandler( void )
{
  hSerial.RxEventCallback();
}

//void TWIM0_TWIS0_IRQHandler( void )
//void SPIM0_SPIS0_IRQHandler( void )
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
//void COMP_IRQHandler( void )
//void SWI0_EGU0_IRQHandler( void )
//void SWI1_EGU1_IRQHandler( void )
//void SWI2_IRQHandler( void )
//void SWI3_IRQHandler( void )
//void SWI4_IRQHandler( void )
//void SWI5_IRQHandler( void )
//void PWM0_IRQHandler( void )
//void PDM_IRQHandler( void )

/*************************************** END OF FILE ****************************************/
