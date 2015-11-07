/* #include "module_serial.h" */

#ifndef __MODULE_SERIAL_H
#define __MODULE_SERIAL_H

#include "stm32f4xx_hal.h"
#include "algorithms\algorithm_string.h"
/*====================================================================================================*/
/*====================================================================================================*/
void    Serial_Config( void );

void    Serial_SendByte( uint8_t sendByte );
uint8_t Serial_RecvByte( void );

void    Serial_SendData( uint8_t *sendData, uint16_t lens );
void    Serial_RecvData( uint8_t *recvData, uint16_t lens );
int8_t  Serial_RecvDataWTO( uint8_t *recvData, uint16_t lens, uint32_t timeoutMs );

void    Serial_SendStr( char *pWord );
void    Serial_RecvStr( char *pWord );
int8_t  Serial_RecvStrWTO( char *pWord, uint32_t timeoutMs );

void    Serial_SendNum( StrType type, uint8_t lens, int32_t sendData );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
