/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    boardConfig.h
 *  @author  KitSprout
 *  @date    21-Apr-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __BOARDCONFIG_H
#define __BOARDCONFIG_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
/* Define ----------------------------------------------------------------------------------*/

#define KS_HW_BOARD_NAME              "SmartIMU"
#define KS_HW_MCU_NAME                "NRF52810"

#define KS_HW_USE_CLOCK_SOUCE_INT     (0U)
#define KS_HW_USE_CLOCK_SOUCE_EXT     (1U)
#define KS_HW_HCLOCK_SOUCE            KS_HW_USE_CLOCK_SOUCE_EXT
#define KS_HW_LCLOCK_SOUCE            KS_HW_USE_CLOCK_SOUCE_INT

#define KS_SYSCLK                     SystemCoreClock


/* -------- LED and KEY */

#define LED_PIN                       12
#define LED_PORT                      NRF_P0
#define LED_Set()                     __GPIO_SET(LED_PORT, LED_PIN)
#define LED_Reset()                   __GPIO_RST(LED_PORT, LED_PIN)
#define LED_Toggle()                  __GPIO_TOG(LED_PORT, LED_PIN)
#define LED_On()                      LED_Set()
#define LED_Off()                     LED_Reset()

#define KEY_PIN                       25
#define KEY_PORT                      NRF_P0
#define KEY_Read()                    (__GPIO_READ(KEY_PORT, KEY_PIN) != SET)


/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
