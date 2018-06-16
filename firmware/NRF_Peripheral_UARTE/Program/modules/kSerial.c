/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kSerial.c
 *  @author  KitSprout
 *  @date    22-Apr-2018
 *  @brief   kSerial packet format :
 *           byte 1   : header 'K' (75)   [HK]
 *           byte 2   : header 'S' (83)   [HS]
 *           byte 3   : total length      [L ]
 *           byte 4   : data type         [T ]
 *           byte 5   : parameter 1       [P1]
 *           byte 6   : parameter 2       [P2]
 *            ...
 *           byte L-2 : data              [DN]
 *           byte L-1 : finish '\r' (13)  [ER]
 *           byte L   : finish '\n' (10)  [EN]
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "modules\serial.h"
#include "modules\kSerial.h"

/** @addtogroup STM32_Module
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
#define KS_MAX_SEND_BUFF_SIZE   (1024)
#define KS_MAX_RECV_BUFF_SIZE   (1024 + 1024)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
#if KSERIAL_SEND_ENABLE
static uint8_t ksSendBuffer[KS_MAX_SEND_BUFF_SIZE] = {0};
//static uint32_t (*pKSerialSend)( uint8_t *, uint32_t) = NULL;
#define pKSerialSend( __DATA, __LENS )  Serial_SendData(__DATA, __LENS)
#endif

#if KSERIAL_RECV_ENABLE
static uint8_t ksRecvBuffer[KS_MAX_RECV_BUFF_SIZE] = {0};
//static uint32_t (*pKSerialRecv)( uint8_t *, uint32_t) = NULL;
//static uint8_t (*pKSerialRecvByte)( void ) = NULL;
#define pKSerialRecvByte()  Serial_RecvByte()
#endif

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

#if KSERIAL_SEND_ENABLE
/**
 *  @brief  kSerial_SetSendFunction
 */
//void kSerial_SetSendFunction( uint32_t (*fptr)(uint8_t *, uint32_t) )
//{
//  pKSerialSend = fptr;
//}
#endif

#if KSERIAL_RECV_ENABLE
/**
 *  @brief  kSerial_SetSendFunction
 */
//void kSerial_SetRecvByteFunction( uint8_t (*fptr)(void) )
//{
//  pKSerialRecvByte = fptr;
//}
#endif

/**
 *  @brief  kSerial_Check
 */
uint32_t kSerial_Check( uint8_t *packet, void *param, uint16_t *lens, uint8_t *type )
{
  if ((packet[0] == 'K') && (packet[1] == 'S')) {
    *lens = (((uint16_t)packet[3] << 2) & 0x0300) | packet[2];
    if (*lens < 8) {
      return KS_ERROR;
    }
    if ((packet[*lens - 2] == '\r') && (packet[*lens - 1] == '\n')) {
      *type = packet[3] & 0x3F;
      ((uint8_t*)param)[0] = packet[4];
      ((uint8_t*)param)[1] = packet[5];
      return KS_OK;
    }
  }

  return KS_ERROR;
}

/**
 *  @brief  kSerial_Pack
 */
uint32_t kSerial_Pack( uint8_t *packet, void *param, void *data, const uint16_t lens, const uint8_t type )
{
  const uint16_t packetDataBytes = lens * (type & 0x0F);
  const uint16_t packetTotalBytes = packetDataBytes + 8;
  const uint8_t lensHiBit = (packetTotalBytes & 0x0300) >> 2;

  packet[0] = 'K';                        /* header 'K'   */
  packet[1] = 'S';                        /* header 'S'   */
  packet[2] = packetTotalBytes & 0x00FF;  /* total length */
  packet[3] = lensHiBit | type;           /* data type    */

  if (param != NULL) {
    packet[4] = ((uint8_t*)param)[0];     /* parameter 1  */
    packet[5] = ((uint8_t*)param)[1];     /* parameter 2  */
  }
  else {
    packet[4] = 0;
    packet[5] = 0;
  }

  for (uint32_t i = 0; i < packetDataBytes; i++) {
    packet[6 + i] = ((uint8_t*)data)[i];  /* data ....... */
  }

  packet[6 + packetDataBytes] = '\r';      /* finish '\r'  */
  packet[7 + packetDataBytes] = '\n';      /* finish '\n'  */

  return KS_OK;
}

/**
 *  @brief  kSerial_Unpack
 */
uint32_t kSerial_Unpack( uint8_t *packet, void *param, void *data, uint16_t *lens, uint8_t *type )
{
  if ((packet[0] == 'K') && (packet[1] == 'S')) {
    *lens = (((uint16_t)packet[3] << 2) & 0x0300) | packet[2];
    if (*lens < 8) {
      return KS_ERROR;
    }
    if ((packet[*lens - 2] == '\r') && (packet[*lens - 1] == '\n')) {
      *type = packet[3] & 0x3F;
      ((uint8_t*)param)[0] = packet[4];
      ((uint8_t*)param)[1] = packet[5];
      for (uint32_t i = 0; i < *lens; i++ ) {
        ((uint8_t*)data)[i] = packet[6 + i];
      }
      return KS_OK;
    }
  }

  return KS_ERROR;
}

#if KSERIAL_SEND_ENABLE
/**
 *  @brief  kSerial_SendPacket
 */
uint32_t kSerial_SendPacket( void *param, void *data, const uint16_t lens, const uint8_t type )
{
  uint32_t totalLens;

  kSerial_Pack(ksSendBuffer, param, data, lens, type);
  totalLens = (((uint16_t)ksSendBuffer[3] << 2) & 0x0300) | ksSendBuffer[2];
  pKSerialSend(ksSendBuffer, totalLens);

  return KS_OK;
}
#endif

#if KSERIAL_RECV_ENABLE
/**
 *  @brief  kSerial_RecvPacket
 */
uint32_t kSerial_RecvPacket( void *param, void *data, uint16_t *lens, uint8_t *type )
{
  static uint16_t point = 0;
  static uint16_t index = 0;
  static uint16_t bytes = 0;

  uint32_t state;

  ksRecvBuffer[point] = pKSerialRecvByte();

  if (point > 1) {
    if ((ksRecvBuffer[point - 2] == 'K') && (ksRecvBuffer[point - 1] == 'S')) {
      index = point - 2;
      bytes = (((uint16_t)ksRecvBuffer[3] << 2) & 0x0300) | ksRecvBuffer[2];
    }
    if ((point - index + 1) == bytes) {
      state = kSerial_Unpack(&(ksRecvBuffer[index]), param, data, lens, type);
      if (state == KS_OK) {
        point = 0;
        index = 0;
        bytes = 0;
        return KS_OK;
      }
    }
  }

  if (++point >= KS_MAX_RECV_BUFF_SIZE) {
    point = 0;
  }

  return KS_ERROR;
}
#endif

/**
 *  @brief  kSerial_GetPacketDataLens
 */
uint16_t kSerial_GetPacketDataLens( uint16_t lens, uint8_t type )
{
  switch (type & 0x0F) {
    case 0x01:  break;
    case 0x02:  lens >>= 1; break;
    case 0x04:  lens >>= 2; break;
    case 0x08:  lens >>= 3; break;
    default:    return 0;
  }
  return lens;
}

/*************************************** END OF FILE ****************************************/
