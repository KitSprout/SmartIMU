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
#include "drivers\nrf5x_rng.h"
#include "modules\serial.h"
#include "nrf5x_bsp.h"

/** @addtogroup NRF5x_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
extern RNG_InitTypeDef hrng;
uint8_t randNum;

/* Prototypes ------------------------------------------------------------------------------*/
void rng_generate_event( void );

/* Functions -------------------------------------------------------------------------------*/

int main( void )
{
  bsp_gpio_init();
  bsp_rng_init(rng_generate_event);
  bsp_serial_init(NULL);

  while (1) {
    LED_Toggle();
    delay_ms(100);

//    randNum = RNG_GetRandomValue(&hrng);
    printf("random number : %3i\r\n", randNum);
  }
}

void rng_generate_event( void )
{
  randNum = RNG_GetRandomValue(&hrng);
}

/*************************************** END OF FILE ****************************************/
