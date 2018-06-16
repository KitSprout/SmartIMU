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
 *  @date    16-Jun-2018
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
#define UARTE_POLLING     (0U)
#define UARTE_INTERRUPT   (1U)
#define UARTE_KSERIAL     (2U)
#define UARTE_MODE        UARTE_POLLING

#define TIMER_TICK_FREQ   1000

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
#if UARTE_MODE == UARTE_KSERIAL
static uint32_t sec = 0;
static uint32_t msc = 0;
#endif

/* Prototypes ------------------------------------------------------------------------------*/
#if UARTE_MODE == UARTE_INTERRUPT
void serial_receive_event( void );
#elif UARTE_MODE == UARTE_KSERIAL
void timer_tick_event( void );
#endif

/* Functions -------------------------------------------------------------------------------*/

int main( void )
{
  bsp_gpio_init();

#if UARTE_MODE == UART_POLLING

  bsp_serial_init(NULL);
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


#elif UARTE_MODE == UARTE_INTERRUPT

  bsp_serial_init(serial_receive_event);
  while (1) {
    __WFI();
  }


#elif UARTE_MODE == UARTE_KSERIAL
#define TIMER_TICK_FREQ   1000
  bsp_timer_init(timer_tick_event, TIMER_TICK_FREQ);
  bsp_timer_enable(ENABLE);
  bsp_serial_init(NULL);

  while (1) {
    LED_Toggle();

    int16_t buff[2];
    buff[0] = sec;
    buff[1] = msc * (1000.0f / TIMER_TICK_FREQ);
    kSerial_SendPacket(NULL, buff, 2, KS_I16);
  }


#else
  #error UARTE_MODE ERROR!!!

#endif
}

#if UARTE_MODE == UARTE_INTERRUPT
void serial_receive_event( void )
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

#elif UARTE_MODE == UARTE_KSERIAL
void timer_tick_event( void )
{
  if (++msc >= TIMER_TICK_FREQ) {
    msc = 0;
    sec++;
  }
}

#endif

/*************************************** END OF FILE ****************************************/
