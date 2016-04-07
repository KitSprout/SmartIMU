/* #include "algorithm_compare.h" */

#ifndef __ALGORITHM_COMPARE_H
#define __ALGORITHM_COMPARE_H

#include "stm32f4xx_hal.h"
/*====================================================================================================*/
/*====================================================================================================*/
int8_t Cmp_ArrU8( const uint8_t *arr1, const uint8_t *arr2, uint32_t lens );
int8_t Cmp_ArrU16( const uint16_t *arr1, const uint16_t *arr2, uint32_t lens );
int8_t Cmp_ArrU32( const uint32_t *arr1, const uint32_t *arr2, uint32_t lens );
int8_t Cmp_ArrU64( const uint64_t *arr1, const uint64_t *arr2, uint32_t lens );
int8_t Cmp_ArrU8U16( const uint8_t *arr1, const uint16_t *arr2, uint32_t lens );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
