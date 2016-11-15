/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    serial.h
  * @author  KitSprout
  * @date    6-Oct-2016
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __SERIAL_H
#define __SERIAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include <stdio.h>
#include "algorithms\string.h"

/* Exported types --------------------------------------------------------------------------*/
/* Exported constants ----------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------*/  
void   Serial_Config( void );

int8_t Serial_SendData( uint8_t *sendData, uint16_t lens, uint32_t timuout );
int8_t Serial_RecvData( uint8_t *recvData, uint16_t lens, uint32_t timeout );
int8_t Serial_SendDataIT( uint8_t *sendData, uint16_t lens);
int8_t Serial_RecvDataIT( uint8_t *recvData, uint16_t lens );
int8_t Serial_SendDataDMA( uint8_t *sendData, uint16_t lens );
int8_t Serial_RecvDataDMA( uint8_t *recvData, uint16_t lens );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/

