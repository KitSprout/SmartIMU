/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    main.c
 *  @author  KitSprout
 *  @date    22-Apr-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "modules\serial.h"
#include "modules\kSerial.h"
#include "nrf5x_bsp.h"

/** @addtogroup NRF5x_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
#define UART_POLLING      (0U)
//#define UART_INTERRUPT    (1U)
#define UART_KSERIAL      (2U)
#define UAER_MODE         UART_POLLING

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
#if UAER_MODE == UART_KSERIAL
static uint32_t sec = 0, msc = 0;
#endif

/* Prototypes ------------------------------------------------------------------------------*/
#if UAER_MODE == UART_INTERRUPT
void IRQEvent_SerialRecv( void );
#elif UAER_MODE == UART_KSERIAL
void IRQEvent_TimerTick( void );
#endif

/* Functions -------------------------------------------------------------------------------*/

int main( void )
{
  BSP_CLOCK_Config();
  BSP_GPIO_Config();

#if UAER_MODE == UART_POLLING

  BSP_UART_SERIAL_Config(NULL);
  uint8_t recvByte = 0;
  while (1) {
    LED_Toggle();
    recvByte = Serial_RecvByte();
    if (recvByte == 0x0D) { // if press enter
      printf("\r\n");
    }
    else {
      Serial_SendByte(recvByte);
    }
  }


#elif UAER_MODE == UART_INTERRUPT

  BSP_UART_SERIAL_Config(IRQEvent_SerialRecv);
  while (1) {
//    LED_Toggle();
    delay_ms(100);
  }


#elif UAER_MODE == UART_KSERIAL
#define TIMER_TICK_FREQ   1000

  BSP_TIMER_Config(IRQEvent_TimerTick, TIMER_TICK_FREQ);
  BSP_UART_SERIAL_Config(NULL);
  while (1) {
    LED_Toggle();
//    delay_ms(100);

    int16_t buff[2];
    buff[0] = sec;
    buff[1] = msc * (1000.0f / TIMER_TICK_FREQ);
    kSerial_SendPacket(NULL, buff, 2, KS_I16);
  }


#else
  #error UAER_MODE ERROR!!!

#endif
}

#if UAER_MODE == UART_INTERRUPT
void IRQEvent_SerialRecv( void )
{
  uint8_t recvByte = hSerial.pRxBuf[0];

  LED_Toggle();
  if (recvByte == 0x0D) {
    printf("\r\n");
  }
  else {
    Serial_SendByte(recvByte);
  }
}

#elif UAER_MODE == UART_KSERIAL
void IRQEvent_TimerTick( void )
{
  if (++msc == TIMER_TICK_FREQ) {
    msc = 0;
    sec++;
  }
}

#endif

/*************************************** END OF FILE ****************************************/
