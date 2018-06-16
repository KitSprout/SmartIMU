/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_bsp.c
 *  @author  KitSprout
 *  @date    16-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_clock.h"
#include "drivers\nrf5x_gpiote.h"
#include "nrf5x_bsp.h"

/** @addtogroup NRF5x_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
GPIOTE_InitTypeDef hKey;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void bsp_gpio_init( void )
{
  CLOCK_Config();

  nrf_gpio_cfg_output(LED_PIN);
  nrf_gpio_cfg_input(KEY_PIN, NRF_GPIO_PIN_PULLUP);

  LED_Off();
}

void bsp_gpiote_key_init( pFunc event )
{
  hKey.Instance      = KEY_GPIOTEx;
  hKey.Pin           = KEY_PIN;
  hKey.Line          = KEY_GPIOTEx_LINE;
  hKey.Mode          = KEY_GPIOTEx_MODE;
  hKey.Polarity      = KEY_GPIOTEx_POLARITY;
  hKey.OutInit       = KEY_GPIOTEx_OUTINIT;
  GPIOTE_Init(&hKey);

  if (event != NULL) {
    hKey.EventCallback = event;
    NVIC_SetPriority(KEY_GPIOTEx_IRQn, KEY_GPIOTEx_IRQn_PRIORITY);
    NVIC_EnableIRQ(KEY_GPIOTEx_IRQn);
    GPIOTE_InterruptCmd(&hKey, ENABLE);
  }
}

/*************************************** END OF FILE ****************************************/
