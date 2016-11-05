/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    kSerial.h
  * @author  KitSprout
  * @date    12-Oct-2016
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __KSERIAL_H
#define __KSERIAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "algorithms\mathUnit.h"

/* Exported types --------------------------------------------------------------------------*/
typedef enum {
  KS_INT8    = 0x00,  /* 8'b0000_xxxx */
  KS_INT16   = 0x10,  /* 8'b0001_xxxx */
  KS_INT32   = 0x20,  /* 8'b0010_xxxx */
  KS_INT64   = 0x30,  /* 8'b0011_xxxx */
  KS_UINT8   = 0x40,  /* 8'b0100_xxxx */
  KS_UINT16  = 0x50,  /* 8'b0101_xxxx */
  KS_UINT32  = 0x60,  /* 8'b0110_xxxx */
  KS_UINT64  = 0x70,  /* 8'b0111_xxxx */
//  KS_RECV1 = 0x80,  /* 8'b1000_xxxx */
//  KS_RECV. = 0x90,  /* 8'b1001_xxxx */
  KS_FLOAT32 = 0xA0,  /* 8'b1010_xxxx */
  KS_FLOAT64 = 0xB0,  /* 8'b1011_xxxx */
//  KS_CMD01 = 0xC0,  /* 8'b1100_xxxx */
//  KS_CMD02 = 0xD0,  /* 8'b1101_xxxx */
//  KS_CMD03 = 0xE0,  /* 8'b1110_xxxx */
//  KS_CMD04 = 0xF0,  /* 8'b1111_xxxx */
} KSerial_TypeDef;

typedef void (*ptrSerial)(uint8_t);

/* Exported constants ----------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------*/  
void kSerial_config( ptrSerial pSerial );
void kSerial_sendData( void *signalData, uint8_t lens, uint8_t type );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
