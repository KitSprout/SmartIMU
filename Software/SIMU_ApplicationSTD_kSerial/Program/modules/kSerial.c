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
  * @date    28-Jan-2017
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
#define SERIAL_PACKET_MAX_SIZE  256

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
static USART_TypeDef *kSerialUart;

static uint16_t sequenceNum = 0;
static uint8_t ksPacket[SERIAL_PACKET_MAX_SIZE] = {0};

static const uint16_t typeTable[10][2] = {
  { 0x0000, 0 },  /* KS_INT8      8'b0000_xxxx*/
  { 0x1000, 1 },  /* KS_INT16     8'b0001_xxxx*/
  { 0x2000, 2 },  /* KS_INT32     8'b0010_xxxx*/
  { 0x3000, 3 },  /* KS_INT64     8'b0011_xxxx*/
  { 0x4000, 0 },  /* KS_UINT8     8'b0100_xxxx*/
  { 0x5000, 1 },  /* KS_UINT16    8'b0101_xxxx*/
  { 0x6000, 2 },  /* KS_UINT32    8'b0110_xxxx*/
  { 0x7000, 3 },  /* KS_UINT64    8'b0111_xxxx*/
  { 0xA000, 2 },  /* KS_FLOAT32   8'b1010_xxxx*/
  { 0xB000, 3 },  /* KS_FLOAT64   8'b1011_xxxx*/
//  { 0x8000, 0 },  /* KS_RECV1     8'b1000_xxxx*/
//  { 0x9000, 0 },  /* KS_RECV2     8'b1001_xxxx*/
//  { 0xC000, 0 },  /* KS_RECV3     8'b1100_xxxx*/
//  { 0xD000, 0 },  /* KS_RECV4     8'b1101_xxxx*/
//  { 0xE000, 0 },  /* KS_RECV5     8'b1110_xxxx*/
//  { 0xF000, 0 }   /* KS_RECV6     8'b1111_xxxx*/
};

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  kSerial_Recv
  */
//static void kSerial_Recv( uint8_t *recvData, uint16_t lens )
//{
//  do {
//    while (!(kSerialUart->SR & USART_FLAG_RXNE));
//    *recvData++ = (uint16_t)(kSerialUart->DR & (uint16_t)0x01FF);
//  } while (--lens);
//}

/**
  * @brief  kSerial_Send
  */
static void kSerial_Send( uint8_t *sendData, uint16_t lens )
{
  do {
    kSerialUart->DR = (*sendData++ & (uint16_t)0x01FF);
    while (!(kSerialUart->SR & USART_FLAG_TC));
  } while (--lens);
}

/**
  * @brief  kSerial_Config
  */
void kSerial_Config( USART_TypeDef *USARTx )
{
  kSerialUart = USARTx;

  ksPacket[0] = 'K';  /* header 'K' */
  ksPacket[1] = 'S';  /* header 'S' */
}

/**
  * @brief  kSerial_sendData
  */
void kSerial_SendData( void *data, const uint8_t lens, const uint8_t type )
{
  const uint16_t dataBytes  = lens << typeTable[type][1];
  const uint16_t packetInfo = typeTable[type][0] | dataBytes;

  uint8_t *pPacket = ksPacket;
  uint16_t packetSize = dataBytes + 8;

  sequenceNum++;
//  ksPacket[0] = 'K';                                      /* header 'K'  */
//  ksPacket[1] = 'S';                                      /* header 'S'  */
  ksPacket[2] = (packetInfo & 0x00FF);                    /* lens & type */
  ksPacket[3] = (packetInfo & 0xFF00) >> 8;               /* lens & type */
  for (uint8_t count = 0; count < dataBytes; count++)     /* data        */
    ksPacket[count + 4] = ((uint8_t*)data)[count];
  ksPacket[packetSize - 4] = (sequenceNum & 0x00FF);      /* seqNum L    */
  ksPacket[packetSize - 3] = (sequenceNum & 0xFF00) >> 8; /* seqNum H    */
  ksPacket[packetSize - 2] = '\r';                        /* end 'r'     */
  ksPacket[packetSize - 1] = '\n';                        /* end 'n'     */

  kSerial_Send(pPacket, packetSize);
}

/*************************************** END OF FILE ****************************************/
