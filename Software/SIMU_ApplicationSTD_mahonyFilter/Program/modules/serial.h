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
void    Serial_Config( void );
void    Serial_SendByte( uint8_t sendByte );
void    Serial_SendData( uint8_t *sendData, uint16_t lens );
void    Serial_SendStr( char *pWord );
void    Serial_SendNum( StringType_t type, uint8_t lens, int32_t number );
uint8_t Serial_RecvByte( void );
void    Serial_RecvData( uint8_t *recvData, uint16_t lens );
int8_t  Serial_RecvDataWTO( uint8_t *recvData, uint16_t lens, int32_t timeoutMs );
void    Serial_RecvStr( char *pWord );
int8_t  Serial_RecvStrWTO( char *pWord, int32_t timeoutMs );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/

