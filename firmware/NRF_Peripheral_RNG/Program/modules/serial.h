/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    serial.h
 *  @author  KitSprout
 *  @date    22-Apr-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __SERIAL_H
#define __SERIAL_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_uarte.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
extern UARTE_InitTypeDef hSerial;

/* Functions -------------------------------------------------------------------------------*/
void      Serial_Config( void );
void      Serial_Start( void );
void      Serial_Stop( void );

void      Serial_SendByte( uint8_t sendByte );
uint8_t   Serial_RecvByte( void );
uint32_t  Serial_SendData( uint8_t *sendData, uint32_t lens );
uint32_t  Serial_RecvData( uint8_t *recvData, uint32_t lens );
uint32_t  Serial_SendDataWaitTimeout( uint8_t *sendData, uint32_t lens, uint32_t timuout );
uint32_t  Serial_RecvDataWaitTimeout( uint8_t *recvData, uint32_t lens, uint32_t timeout );


#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
