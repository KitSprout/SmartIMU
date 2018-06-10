/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32f4xx_bsp.h
 *  @author  KitSprout
 *  @date    03-Jun-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STM32F4XX_BSP_H
#define __STM32F4XX_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
void      bsp_gpio_init( void );
void      bsp_timer_init( pFunc event, uint32_t freq );
void      bsp_timer_enable( uint32_t state );
void      bsp_uart_serial_init( pFunc event );
void      bsp_sensor_init( void );
uint32_t  bsp_sensor_gyr_acc_mag_data( float32_t gyr[3], float32_t acc[3], float32_t mag[3] );
uint32_t  bsp_sensor_bar_data( float32_t bar[2] );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
