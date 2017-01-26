/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    compare.h
  * @author  KitSprout
  * @date    26-Jan-2017
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __COMPARE_H
#define __COMPARE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types --------------------------------------------------------------------------*/
/* Exported constants ----------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------*/  
int8_t CMP_ArrayU8( const uint8_t *arr1, const uint8_t *arr2, uint32_t lens );
int8_t CMP_ArrayU16( const uint16_t *arr1, const uint16_t *arr2, uint32_t lens );
int8_t CMP_ArrayU32( const uint32_t *arr1, const uint32_t *arr2, uint32_t lens );
int8_t CMP_ArrayU64( const uint64_t *arr1, const uint64_t *arr2, uint32_t lens );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
