/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    string.c
  * @author  KitSprout
  * @date    16-Nov-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "algorithms\string.h"

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
  * @brief  num2Str
  * @param  type: 
  * @param  lens: 
  * @param  pStr:
  * @param  number:
  * @retval None
  */
void num2Str( StringType_t type, uint8_t lens, char *pStr, int32_t number )
{
  uint8_t i = 0;
  uint32_t tmpStr[48] = {0};
  uint32_t tmpNum = 1;

  switch (type) {

    case S_BIN:
    case S_OCT:
    case S_DEC:
    case S_HEX:
      for (i = 0; i < lens; i++) {
        tmpStr[i] = ((uint32_t)number) / tmpNum;
        tmpNum = tmpNum * type;
      }
      for (i = 0; i < lens; i++) {
        pStr[lens-i-1] = tmpStr[i] - tmpStr[i+1] * type;
        if (pStr[lens-i-1] > 9)
          pStr[lens-i-1] += 55;	// 65-10
        else
          pStr[lens-i-1] += 48;
      }
      pStr[lens] = '\0';
    break;

    case S_INT:
      if (number < 0) {
        pStr[0] = '-';
        number = (~number) + 1;
      }
      else {
        pStr[0] = '+';
      }
      for (i = 1; i < lens + 1; i++) {
        tmpStr[i-1] = ((uint32_t)number) / tmpNum;
        tmpNum = tmpNum * 10;
      }
      for (i = 1; i < lens + 1; i++) {
        pStr[lens-i+1] = tmpStr[i-1] - tmpStr[i] * 10;
        pStr[lens-i+1] += 48;
      }
      pStr[lens+1] = '\0';
      break;

    case S_FLOAT:
      break;
  }
}

/**
  * @brief  lenOfStr
  * @param  pStr: 
  * @retval string lengths
  */
uint16_t lenOfStr( char *pStr )
{
  uint16_t strLens = 0;

  if (pStr == NULL)
    return strLens;

  while (*(pStr++))
    strLens++;

  return strLens;
}

/*************************************** END OF FILE ****************************************/
