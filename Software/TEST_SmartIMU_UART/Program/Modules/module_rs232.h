/* #include "module_rs232.h" */

#ifndef __MODULE_RS232_H
#define __MODULE_RS232_H

#include "stm32f4xx.h"
/*====================================================================================================*/
/*====================================================================================================*/
void RS232_Config( void );
void RS232_SendStr( u8 *pWord );
void RS232_SendNum( u8 Type, u8 NumLen, s32 SendData );
void RS232_SendData( u8 *SendData, u16 DataLen );
void RS232_RecvStr( u8 *pWord );
void RS232_RecvData( u8 *RecvData, u16 DataLen );
void RS232_VisualScope( u8 *SendBuf );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
