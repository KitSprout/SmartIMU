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
#include "drivers\nrf5x_radio.h"
#include "modules\serial.h"
#include "nrf5x_bsp.h"

/** @addtogroup NRF5x_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
#define MAX_LENS 128

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static int8_t rssi = 0;
static uint8_t packet[256];
static uint16_t lens = 0;
static uint16_t sequence = 0;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

int main( void )
{
  bsp_gpio_init();
  bsp_serial_init(NULL);
  bsp_radio_init();

  if (KEY_Read()) {
    // RECV
    uint32_t status;
    printf(" >>> RECV MODE\r\n");
    while (1) {
      LED_Toggle();
      status = RADIO_RecvPacket(&sequence, packet, &lens, &rssi);
      if (status == KS_OK) {
        printf("[OK]");
      }
      else {
        printf("[ER]");
      }
      printf("[%06i][%03i][%04i dBm]", sequence, lens, rssi);
      printf("\r\n");
    }
  }
  else {
    // SEND
    printf(" >>> SEND MODE\r\n");
    while (1) {
      LED_Toggle();
      delay_ms(50);

      sequence++;
      for (uint32_t i = 0; i < MAX_LENS; i++) {
        packet[i] = i;
      }
      RADIO_SendPacket(sequence, packet, MAX_LENS);
    }
  }
}

/*************************************** END OF FILE ****************************************/
