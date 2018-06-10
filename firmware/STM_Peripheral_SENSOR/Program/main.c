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
 *  @date    09-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\kSerial.h"
#include "stm32f4xx_bsp.h"

/** @addtogroup STM32_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
#define TIMER_TICK_FREQ   1000

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static uint32_t sec = 0;
static uint32_t msc = 0;
static uint32_t sample_count = 0;
static uint32_t print_count = 0;

/* Prototypes ------------------------------------------------------------------------------*/
void timer_tick_event( void );

/* Functions -------------------------------------------------------------------------------*/

int main( void )
{
  float32_t gyr[3] = {0}, acc[3] = {0}, mag[3] = {0}, bar[2] = {0};

  bsp_gpio_init();
  bsp_timer_init(timer_tick_event, TIMER_TICK_FREQ);
  bsp_uart_serial_init(NULL);
  bsp_sensor_init();

  // start timer
  bsp_timer_enable(ENABLE);

  while (1) {

    if (sample_count >= (TIMER_TICK_FREQ / 200)) {  // 200Hz
      sample_count = 0;
      bsp_sensor_gyr_acc_mag_data(gyr, acc, mag);
      bsp_sensor_bar_data(bar);
    }

#if 0
    if (print_count >= (TIMER_TICK_FREQ / 200)) {  // 200Hz
      print_count = 0;
      LED_B_Toggle();

      int16_t buff[13];
      buff[0]  = sec;
      buff[1]  = msc * (1000.0f / TIMER_TICK_FREQ);
      buff[2]  = (int16_t)(gyr[0] * 935); // +- 34.906585 * 935
      buff[3]  = (int16_t)(gyr[1] * 935);
      buff[4]  = (int16_t)(gyr[2] * 935);
      buff[5]  = (int16_t)(acc[0] * 835); // +- 39.2 * 835
      buff[6]  = (int16_t)(acc[1] * 835);
      buff[7]  = (int16_t)(acc[2] * 835);
      buff[8]  = (int16_t)(mag[0] * 65);  // +- 500 * 65
      buff[9]  = (int16_t)(mag[1] * 65);
      buff[10] = (int16_t)(mag[2] * 65);
      buff[11] = (int16_t)(bar[0] * 26);  // +- 1260 * 26
      buff[12] = (int16_t)(bar[1] * 100);
      kSerial_SendPacket(NULL, buff, 13, KS_I16);
    }

#else
    if (print_count >= (TIMER_TICK_FREQ / 10)) {  // 10Hz
      print_count = 0;
      LED_G_Toggle();
      printf("[G] %8.3f, %8.3f, %8.3f ", gyr[0], gyr[1], gyr[2]);
      printf("[A] %8.3f, %8.3f, %8.3f ", acc[0], acc[1], acc[2]);
      printf("[M] %8.3f, %8.3f, %8.3f ", mag[0], mag[1], mag[2]);
      printf("[B] %8.3f, %8.3f ", bar[0], bar[1]);
      printf("\r\n");
    }

#endif
  }
}

void timer_tick_event( void )
{
  sample_count++;
  print_count++;
  if (++msc == TIMER_TICK_FREQ) {
    msc = 0;
    sec++;
  }
}

/*************************************** END OF FILE ****************************************/
