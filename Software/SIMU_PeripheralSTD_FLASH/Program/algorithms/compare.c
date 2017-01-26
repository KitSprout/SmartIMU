/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    compare.c
  * @author  KitSprout
  * @date    26-Jan-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "algorithms\compare.h"

/** @addtogroup STM32_Algorithm
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  CMP_ArrayU8
  */
int8_t CMP_ArrayU8( const uint8_t *arr1, const uint8_t *arr2, uint32_t lens )
{
  while (lens--) {
    if (arr1[lens] != arr2[lens]) {
      return HAL_ERROR;
    }
  }
  return HAL_OK;
}

/**
  * @brief  CMP_ArrayU16
  */
int8_t CMP_ArrayU16( const uint16_t *arr1, const uint16_t *arr2, uint32_t lens )
{
  while (lens--) {
    if (arr1[lens] != arr2[lens]) {
      return HAL_ERROR;
    }
  }
  return HAL_OK;
}

/**
  * @brief  CMP_ArrayU32
  */
int8_t CMP_ArrayU32( const uint32_t *arr1, const uint32_t *arr2, uint32_t lens )
{
  while (lens--) {
    if (arr1[lens] != arr2[lens]) {
      return HAL_ERROR;
    }
  }
  return HAL_OK;
}

/**
  * @brief  CMP_ArrayU64
  */
int8_t CMP_ArrayU64( const uint64_t *arr1, const uint64_t *arr2, uint32_t lens )
{
  while (lens--) {
    if (arr1[lens] != arr2[lens]) {
      return HAL_ERROR;
    }
  }
  return HAL_OK;
}

/*************************************** END OF FILE ****************************************/
