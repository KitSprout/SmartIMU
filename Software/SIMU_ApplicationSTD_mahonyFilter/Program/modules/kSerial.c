/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    kSerial.c
  * @author  KitSprout
  * @date    12-Oct-2016
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "kSerial.h"

/** @addtogroup STM32_Module
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
static ptrSerial pSerialFunc = NULL;

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  kSerial_config
  * @param  pSerial: point to serial send byte function
  * @retval None
  */
void kSerial_config( ptrSerial pSerial )
{
  pSerialFunc = pSerial;
}

static uint8_t getTypeSize( uint8_t type )
{
  switch (type) {
    case KS_INT8:     return 1;
    case KS_UINT8:    return 1;
    case KS_INT16:    return 2;
    case KS_UINT16:   return 2;
    case KS_INT32:    return 4;
    case KS_UINT32:   return 4;
    case KS_INT64:    return 8;
    case KS_UINT64:   return 8;
    case KS_FLOAT32:  return 4;
    case KS_FLOAT64:  return 8;
    default:          return 0;
  }
}

/**
  * @brief  kSerial_sendData
  * @param  data: point to send data
  * @param  lens: data lengths
  * @param  type: data type
  * @retval None
  */
void kSerial_sendData( void *data, uint8_t lens, uint8_t type )
{
  static uint16_t sequenceNum = 0;

  uint8_t packet[256] = {0};
  uint8_t *pPacket = packet;

  uint16_t dataBytes  = lens * getTypeSize(type);
  uint16_t packetSize = dataBytes + 8;
  uint16_t packetInfo = ((uint16_t)type << 8) | dataBytes;

  sequenceNum++;
  packet[0] = 'K';                                      /* header 'K'  */
  packet[1] = 'S';                                      /* header 'S'  */
  packet[2] = (packetInfo & 0x00FF);                    /* lens & type */
  packet[3] = (packetInfo & 0xFF00) >> 8;               /* lens & type */
  for (uint8_t count = 0; count < dataBytes; count++)   /* data        */
    packet[count + 4] = ((uint8_t*)data)[count];
  packet[packetSize - 4] = (sequenceNum & 0x00FF);      /* seqNum L    */
  packet[packetSize - 3] = (sequenceNum & 0xFF00) >> 8; /* seqNum H    */
  packet[packetSize - 2] = '\r';                        /* end 'r'     */
  packet[packetSize - 1] = '\n';                        /* end 'n'     */

  do {
    pSerialFunc(*pPacket++);
  } while (--packetSize);
}

/*************************************** END OF FILE ****************************************/
