/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_radio.h
 *  @author  KitSprout
 *  @date    18-Apr-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __NRF5x_RADIO_H
#define __NRF5x_RADIO_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"

/* Exported types --------------------------------------------------------------------------*/

typedef struct {
//  NRF_RADIO_Type *Instance;

  uint32_t TxPower;
  // RADIO_TXPOWER_TXPOWER_Pos4dBm  - +4 dBm
  // RADIO_TXPOWER_TXPOWER_Pos3dBm  - +3 dBm
  // RADIO_TXPOWER_TXPOWER_0dBm     -  0 dBm
  // RADIO_TXPOWER_TXPOWER_Neg4dBm  - -4 dBm
  // RADIO_TXPOWER_TXPOWER_Neg8dBm  - -8 dBm
  // RADIO_TXPOWER_TXPOWER_Neg12dBm - -12 dBm
  // RADIO_TXPOWER_TXPOWER_Neg16dBm - -16 dBm
  // RADIO_TXPOWER_TXPOWER_Neg20dBm - -20 dBm
  // RADIO_TXPOWER_TXPOWER_Neg30dBm - -40 dBm (Deprecated enumerator)
  // RADIO_TXPOWER_TXPOWER_Neg40dBm - -40 dBm
  uint32_t Frequency;
  // RF Frequency = 2400 + Frequency (MHz)
  uint32_t Mode;
  // RADIO_MODE_MODE_Nrf_2Mbit   -   2Mbit/s Nordic propietary radio mode.
  // RADIO_MODE_MODE_Nrf_1Mbit   -   1Mbit/s Nordic propietary radio mode.
  // RADIO_MODE_MODE_Nrf_250Kbit - 250kbit/s Nordic propietary radio mode. (Deprecated enumerator)
  // RADIO_MODE_MODE_Ble_1Mbit   -   1Mbit/s Bluetooth Low Energy.
  uint32_t CRC;
  // RADIO_CRCCNF_LEN_Disabled  - CRC length is 0 and CRC calculation is disabled
  // RADIO_CRCCNF_LEN_One       - CRC length is 1 byte and CRC calculation is enabled
  // RADIO_CRCCNF_LEN_Two       - CRC length is 2 bytes and CRC calculation is enabled
  // RADIO_CRCCNF_LEN_Three     - CRC length is 3 bytes and CRC calculation is enabled
  uint8_t *Prefix;
  uint8_t TxAddr;
  uint8_t RxAddr;
  uint32_t *BaseAddr;
  uint32_t *PacketPtr;
} RADIO_InitTypeDef;

/* Exported constants ----------------------------------------------------------------------*/
#define RADIOx                            NRF_RADIO

#define RADIO_PACKET_S1_FIELD_SIZE        (0UL)   // Packet S1 field size in bits.
#define RADIO_PACKET_S0_FIELD_SIZE        (0UL)   // Packet S0 field size in bytes.
#define RADIO_PACKET_LENGTH_FIELD_SIZE    (0UL)   // Packet length field size in bits.
#define RADIO_PACKET_BASE_ADDRESS_LENGTH  (4UL)   // Packet base address length field size in bytes
#define RADIO_PACKET_STATIC_LENGTH        (248UL) // Packet static length in bytes
#define RADIO_PACKET_PAYLOAD_MAXSIZE      (248UL) // Packet payload maximum size in bytes

/* Exported macro --------------------------------------------------------------------------*/
#define RADIO_TASKS_TXEN(__INS)       __INS->TASKS_TXEN         // Enable RADIO in TX mode
#define RADIO_TASKS_RXEN(__INS)       __INS->TASKS_RXEN         // Enable RADIO in RX mode
#define RADIO_TASKS_START(__INS)      __INS->TASKS_START        // Start RADIO
#define RADIO_TASKS_STOP(__INS)       __INS->TASKS_STOP         // Stop RADIO
#define RADIO_TASKS_DISABLE(__INS)    __INS->TASKS_DISABLE      // Disable RADIO
#define RADIO_TASKS_RSSISTART(__INS)  __INS->TASKS_RSSISTART    // Start the RSSI and take one single sample of the receive signal strength.
#define RADIO_TASKS_RSSISTOP(__INS)   __INS->TASKS_RSSISTOP     // Stop the RSSI measurement
#define RADIO_TASKS_BCSTART(__INS)    __INS->TASKS_BCSTART      // Start the bit counter
#define RADIO_TASKS_BCSTOP(__INS)     __INS->TASKS_BCSTOP       // Stop the bit counter

#define RADIO_EVENTS_READY(__INS)     __INS->EVENTS_READY       // RADIO has ramped up and is ready to be started
#define RADIO_EVENTS_ADDRESS(__INS)   __INS->EVENTS_ADDRESS     // Address sent or received
#define RADIO_EVENTS_PAYLOAD(__INS)   __INS->EVENTS_PAYLOAD     // Packet payload sent or received
#define RADIO_EVENTS_END(__INS)       __INS->EVENTS_END         // Packet sent or received
#define RADIO_EVENTS_DISABLED(__INS)  __INS->EVENTS_DISABLED    // RADIO has been disabled
#define RADIO_EVENTS_DEVMATCH(__INS)  __INS->EVENTS_DEVMATCH    // A device address match occurred on the last received packet
#define RADIO_EVENTS_DEVMISS(__INS)   __INS->EVENTS_DEVMISS     // No device address match occurred on the last received packet
#define RADIO_EVENTS_RSSIEND(__INS)   __INS->EVENTS_RSSIEND     // Sampling of receive signal strength complete.
#define RADIO_EVENTS_BCMATCH(__INS)   __INS->EVENTS_BCMATCH     // Bit counter reached bit count value.
#define RADIO_EVENTS_CRCOK(__INS)     __INS->EVENTS_CRCOK       // Packet received with CRC ok
#define RADIO_EVENTS_CRCERROR(__INS)  __INS->EVENTS_CRCERROR    // Packet received with CRC error

/* Exported functions ----------------------------------------------------------------------*/
void      RADIO_Init( RADIO_InitTypeDef *hradio );
void      RADIO_Disable( void );
uint32_t  RADIO_RecvPacket( uint16_t *sequence, uint8_t *pdata, uint16_t *lens, int8_t *rssi );
uint32_t  RADIO_SendPacket( uint16_t  sequence, uint8_t *pdata, uint16_t  lens );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
