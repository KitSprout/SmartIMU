/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_radio.c
 *  @author  KitSprout
 *  @date    16-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_radio.h"

/** @addtogroup NRF5x_Driver
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static uint8_t radio_packet[256] = {0};

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  RADIO_Init
 */
void RADIO_Init( RADIO_InitTypeDef *hradio )
{
  // config pa/lna
  nrf_gpio_cfg_output(RADIO_RXEN_PIN);
  nrf_gpio_cfg_output(RADIO_TXEN_PIN);

  RADIO_RXEN_On();
  RADIO_TXEN_Off();

  // tx power, radio frequence, transmission speed
  RADIOx->TXPOWER   = hradio->TxPower   << RADIO_TXPOWER_TXPOWER_Pos;
  RADIOx->FREQUENCY = hradio->Frequency << RADIO_FREQUENCY_FREQUENCY_Pos;
  RADIOx->MODE      = hradio->Mode      << RADIO_MODE_MODE_Pos;

  // crc check
  RADIOx->CRCCNF = (hradio->CRC << RADIO_CRCCNF_LEN_Pos);
  switch (hradio->CRC) {
    case RADIO_CRCCNF_LEN_One:
      RADIOx->CRCINIT = 0xFFUL;     // Initial Value
      RADIOx->CRCPOLY = 0x107UL;    // CRC poly: x^8 + x^2^x^1 + 1
      break;
    case RADIO_CRCCNF_LEN_Two:
      RADIOx->CRCINIT = 0xFFFFUL;   // Initial Value
      RADIOx->CRCPOLY = 0x11021UL;  // CRC poly: x^16 + x^12^x^5 + 1
      break;
    case RADIO_CRCCNF_LEN_Three:
      break;
  }

  // tx / rx address
  RADIOx->TXADDRESS   = hradio->TxAddr;
  RADIOx->RXADDRESSES = hradio->RxAddr;

  // packet config
  RADIOx->PCNF0 = (RADIO_PACKET_S1_FIELD_SIZE       << RADIO_PCNF0_S1LEN_Pos) |
                  (RADIO_PACKET_S0_FIELD_SIZE       << RADIO_PCNF0_S0LEN_Pos) |
                  (RADIO_PACKET_LENGTH_FIELD_SIZE   << RADIO_PCNF0_LFLEN_Pos);
  RADIOx->PCNF1 = (RADIO_PCNF1_WHITEEN_Disabled     << RADIO_PCNF1_WHITEEN_Pos) |
                  (RADIO_PCNF1_ENDIAN_Big           << RADIO_PCNF1_ENDIAN_Pos)  |
                  (RADIO_PACKET_BASE_ADDRESS_LENGTH << RADIO_PCNF1_BALEN_Pos)   |
                  (RADIO_PACKET_STATIC_LENGTH       << RADIO_PCNF1_STATLEN_Pos) |
                  (RADIO_PACKET_PAYLOAD_MAXSIZE     << RADIO_PCNF1_MAXLEN_Pos);

  // enable rss measure automatic
  NRF_RADIO->SHORTS |= RADIO_SHORTS_ADDRESS_RSSISTART_Msk;

  // set buffer
  RADIOx->PACKETPTR = (uint32_t)radio_packet;







  // base address 0 - 1
  RADIOx->BASE0 = hradio->BaseAddr[0];
  RADIOx->BASE1 = hradio->BaseAddr[1];

  // address prefix 0 - 7
  RADIOx->PREFIX0 = BYTE32(hradio->Prefix[7], hradio->Prefix[6], hradio->Prefix[5], hradio->Prefix[4]);
  RADIOx->PREFIX1 = BYTE32(hradio->Prefix[3], hradio->Prefix[2], hradio->Prefix[1], hradio->Prefix[0]);
}

void RADIO_Disable( void )
{
  RADIOx->SHORTS = RESET;
  RADIO_EVENTS_DISABLED(RADIOx) = RESET;
#ifdef NRF51
  RADIOx->TEST = RESET;
#endif
  RADIO_EVENTS_DISABLED(RADIOx) = SET;
  while (RADIO_EVENTS_DISABLED(RADIOx) == RESET) {;}
  RADIO_EVENTS_DISABLED(RADIOx) = RESET;

  RADIO_RXEN_Off();
  RADIO_TXEN_Off();
}

/**
 *  @brief  RADIO_RecvPacket
 */
uint32_t RADIO_RecvPacket( uint16_t *sequence, uint8_t *pdata, uint16_t *lens, int8_t *rssi )
{
  uint32_t status = KS_OK;
  uint32_t crc32 = 0;

  RADIO_RXEN_On();
  RADIO_TXEN_Off();

  RADIO_EVENTS_READY(RADIOx) = RESET;
  RADIO_TASKS_RXEN(RADIOx) = SET;
  while (RADIO_EVENTS_READY(RADIOx) == RESET);

  RADIO_EVENTS_END(RADIOx) = RESET;
  RADIO_TASKS_START(RADIOx) = SET;
  while (RADIO_EVENTS_END(RADIOx) == RESET);
  if (RADIOx->CRCSTATUS == SET) {
    *rssi = -RADIOx->RSSISAMPLE;
    *lens = BYTE16(radio_packet[1], radio_packet[0]);
    *sequence = BYTE16(radio_packet[3], radio_packet[2]);
    for (uint16_t i = 0; i < *lens; i++) {
      pdata[i] = radio_packet[i + 8];
      crc32 += radio_packet[i + 8];
    }
    if (crc32 != BYTE32(radio_packet[7], radio_packet[6], radio_packet[5], radio_packet[4])) {
      status = KS_ERROR;
    }
    printf("[%08X %08X] ", crc32, BYTE32(radio_packet[7], radio_packet[6], radio_packet[5], radio_packet[4]));
  }
  else {
    status = KS_ERROR;
  }

  RADIO_EVENTS_DISABLED(RADIOx) = RESET;
  RADIO_TASKS_DISABLE(RADIOx) = SET;
  while (RADIO_EVENTS_DISABLED(RADIOx) == RESET);

  return status;
}

/**
 *  @brief  RADIO_SendPacket
 */
uint32_t RADIO_SendPacket( uint16_t sequence, uint8_t *pdata, uint16_t lens )
{
  uint32_t crc32 = 0;

  RADIO_RXEN_On();
  RADIO_TXEN_On();

  if (lens > (RADIO_PACKET_PAYLOAD_MAXSIZE - 8)) {
    return KS_ERROR;
  }

  radio_packet[0] = (lens & 0x00FF) >> 0;       // lens
  radio_packet[1] = (lens & 0xFF00) >> 8;       // lens
  radio_packet[2] = (sequence & 0x00FF) >> 0;   // sequence
  radio_packet[3] = (sequence & 0xFF00) >> 8;   // sequence
  while (lens--) {
    radio_packet[lens + 8] = pdata[lens];
    crc32 += pdata[lens];
  }
  radio_packet[4] = (crc32 & 0x000000FF) >> 0;    // checksum
  radio_packet[5] = (crc32 & 0x0000FF00) >> 8;    // checksum
  radio_packet[6] = (crc32 & 0x00FF0000) >> 16;   // checksum
  radio_packet[7] = (crc32 & 0xFF000000) >> 24;   // checksum

  // send packet
  RADIO_EVENTS_READY(RADIOx) = RESET;
  RADIO_TASKS_TXEN(RADIOx) = SET;
  while (RADIO_EVENTS_READY(RADIOx) == RESET);

  RADIO_EVENTS_END(RADIOx) = RESET;
  RADIO_TASKS_START(RADIOx) = SET;
  while (RADIO_EVENTS_END(RADIOx) == RESET);

  RADIO_EVENTS_DISABLED(RADIOx) = RESET;
  RADIO_TASKS_DISABLE(RADIOx) = SET;
  while (RADIO_EVENTS_DISABLED(RADIOx) == RESET);

  return KS_OK;
}

/*************************************** END OF FILE ****************************************/
