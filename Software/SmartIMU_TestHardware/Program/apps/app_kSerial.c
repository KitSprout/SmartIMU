/*=====================================================================================================*/
/*=====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\module_serial.h"

#include "app_kSerial.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
static uint16_t calChecksum( uint8_t *data, const uint8_t dataLens )
{
  uint8_t count = 0;
  uint32_t checkSum = 0;

  for(count = 0; count < dataLens; count++)
    checkSum += (data[count] << 8) | (data[count + 1]);

  return (uint16_t)((dataLens & 0x01) ? checkSum : checkSum ^ (uint32_t)data[count]);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void KSerial_serialScope( void *signalData, uint8_t type, uint8_t lens )
{
  const uint8_t dataLens = lens << ((type & 0x60) >> 5);

  uint8_t packet[32] = {0};           // tmpData lens >= 2 * lens + 4
  uint8_t *pPacket = packet;
  uint8_t packetLens = dataLens + 6;  // all packet bytes
  uint16_t checkSum = 0;

  packet[0] = 'S';                                      // 'S' - Signal
  packet[1] = type | (dataLens & 0x1F);                 // data type & lens
  for(uint8_t count = 0; count < dataLens; count++)     // data
    packet[count + 2] = ((uint8_t*)signalData)[count];
  checkSum = calChecksum(packet + 2, lens);
  packet[packetLens - 4] = Byte8H(checkSum);            // checksum H
  packet[packetLens - 3] = Byte8L(checkSum);            // checksum L
  packet[packetLens - 2] = '\r';                        // 'r'
  packet[packetLens - 1] = '\n';                        // 'n'

  do {
    Serial_SendByte(*pPacket++);
  } while(--packetLens);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
