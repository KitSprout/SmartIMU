/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    KTerminal.c
  * @author  KitSprout
  * @date    24-Jan-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\imu.h"
#include "applications\kTerminal.h"

/** @addtogroup STM32_Application
  * @{
  */

/* Private define --------------------------------------------------------------------------*/
#define RECVBUF_LENS 24

/* Private typedef -------------------------------------------------------------------------*/
const uint8_t KST_COMMAND[KST_LENS][6] = {
  {"KS"},     //  0, [R]  KST check, return OK
  {"REST"},   //  1, [W]  device reset (b1)
  {"ORGL"},   //  2, [W]  set to default (b1)
  {"INFO"},   //  3, [R]  get board information 
  {"DEVID"},  //  4, [RW] set/get device id (b4)
  {"ODR"},    //  5, [RW] set/get data output rate (b4)
  {"BAUD"},   //  6, [RW] set/get baudrate (b6)
  {"MODE"},   //  7, [RW] set/get device mode (b1)
  {"CALIB"},  //  8, [W]  calib sensor (b1)
  {"EOUT"},   //  9, [RW] set/get device output state (b1)
  {"DOUT"},   // 10, [R]  get data
  {"EXIT"}    // 11, [C]  exti KST
};

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
extern IMU_DataTypeDef IMU;

static USART_TypeDef *KSTUart;
static uint8_t commandBuf[RECVBUF_LENS] = {0};
static uint8_t KST_COMMAND_LENS[KST_LENS] = {0};

__IO uint8_t KST_Cmd = DISABLE;
__IO uint8_t KST_Eout = DISABLE;

/* Private function prototypes -------------------------------------------------------------*/
static void KST_FUNC_REST( uint8_t cmd, uint32_t data );
static void KST_FUNC_ORGL( uint8_t cmd, uint32_t data );
static void KST_FUNC_INFO( uint8_t cmd );
static void KST_FUNC_DEVID( uint8_t cmd, uint32_t data );
static void KST_FUNC_ODR( uint8_t cmd, uint32_t data );
static void KST_FUNC_BAUD( uint8_t cmd, uint32_t data );
static void KST_FUNC_MODE( uint8_t cmd, uint32_t data );
static void KST_FUNC_CALIB( uint8_t cmd, uint32_t data );
static void KST_FUNC_EOUT( uint8_t cmd, uint32_t data );
static void KST_FUNC_DOUT( uint8_t cmd );
static void KST_FUNC_EXIT( void );

/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  KSTRecv
  */
static uint8_t KSTRecv( void )
{
  uint8_t recvByte;
  while (!(KSTUart->SR & USART_FLAG_RXNE));
  recvByte = (uint16_t)(KSTUart->DR & (uint16_t)0x01FF);
  return recvByte;
}

/**
  * @brief  KSTSend
  */
static void KSTSend( uint8_t *sendData, uint16_t lens )
{
  do {
    KSTUart->DR = (*sendData++ & (uint16_t)0x01FF);
    while (!(KSTUart->SR & USART_FLAG_TC));
  } while (--lens);
}

/**
  * @brief  KSTSendStr
  */
static void KSTSendStr( char *pWord )
{
  uint8_t lens = lenOfStr(pWord);
  KSTSend((uint8_t*)pWord, lens);
}

/**
  * @brief  getCommandInfo
  */
static void getCommandInfo( uint8_t* command, uint8_t lens, uint8_t *cmd, uint32_t *data )
{
  // search command
  for (uint8_t i = 1; i < KST_LENS; i++) {
    // check command lens - get
    if (lens == KST_COMMAND_LENS[i]) {
      uint8_t check = 0;
      for (uint8_t j = 0; j < KST_COMMAND_LENS[i]; j++) {
        if (command[j] == KST_COMMAND[i][j]) {
          check++;
        }
      }
      if (check == KST_COMMAND_LENS[i]) {
        *cmd = i;
        return;
      }
    }
    // check command lens - set
    else if (lens > KST_COMMAND_LENS[i]) {
      uint8_t check = 0;
      for (uint8_t j = 0; j < KST_COMMAND_LENS[i]; j++) {
        if (command[j] == KST_COMMAND[i][j]) {
          check++;
        }
      }
      if (check == KST_COMMAND_LENS[i]) {
        uint8_t index = KST_COMMAND_LENS[i];
        uint32_t ten = 1;
        if (command[index] == '=') {
          *cmd = i | 0x80;
          *data = 0;
          for (uint8_t j = lens - 1; j > index; j--) {
            if ((command[j] - 48) > 9) {
              *cmd = KST_ERROR;
              return;
            }
            else {
              *data += (command[j] - 48) * ten;
              ten *= 10;
            }
          }
          return;
        }
      }
    }
  }
}

/**
  * @brief  KTerminal_Init
  */
void KTerminal_Init( USART_TypeDef *USARTx )
{
  KSTUart = USARTx;

  for (uint8_t i = 0; i < KST_LENS; i++) {
    uint8_t cmdLens = lenOfStr((char*)&KST_COMMAND[i]);
    KST_COMMAND_LENS[i] = cmdLens;
  }
}

/**
  * @brief  KTerminal_IRQ
  */
void KTerminal_IRQ( void )
{
  if (KST_Cmd) {
    // recv data
    for (uint8_t i = 1; i < RECVBUF_LENS; i++) {
      commandBuf[i-1] = commandBuf[i];
    }
    commandBuf[RECVBUF_LENS-1] = KSTRecv();
    // check "/r/n"
    if ((commandBuf[RECVBUF_LENS-2] == 13) && (commandBuf[RECVBUF_LENS-1] == 10)) {
      // find newest "KS"
      for (int8_t i = RECVBUF_LENS - 3; i > 0; i--) {
        if ((commandBuf[i-1] == 'K') && (commandBuf[i] == 'S')) {
          uint8_t index = i - 1;
          uint8_t lens  = RECVBUF_LENS - 4 - index;
          uint8_t cmd = KST_ERROR;
          uint32_t data = 0;
          switch (lens) {
            case 0:   // CMD[KS]
              KSTSendStr("OK\r\n");
              break;
            case 1:   // NO CMD
            case 2:
            case 3:
              cmd = 0;
              KSTSend(&cmd, 1);
  //            KSTSendStr("[ERROR] WRONG CMD!!\r\n");
              break;
            default:  // SET/GET CMD
              if (commandBuf[index+2] == '+') {
                getCommandInfo(&commandBuf[index+3], lens - 1, &cmd, &data);
                switch (cmd & 0x7F) {
                  case KST_REST:    KST_FUNC_REST(cmd, data);       break;
                  case KST_ORGL:    KST_FUNC_ORGL(cmd, data);       break;
                  case KST_INFO:    KST_FUNC_INFO(cmd);             break;
                  case KST_DEVID:   KST_FUNC_DEVID(cmd, data);      break;
                  case KST_ODR:     KST_FUNC_ODR(cmd, data);        break;
                  case KST_BAUD:    KST_FUNC_BAUD(cmd, data);       break;
                  case KST_MODE:    KST_FUNC_MODE(cmd, data);       break;
                  case KST_CALIB:   KST_FUNC_CALIB(cmd, data);      break;
                  case KST_EOUT:    KST_FUNC_EOUT(cmd, data);       break;
                  case KST_DOUT:    KST_FUNC_DOUT(cmd);             break;
                  case KST_EXIT:    KST_FUNC_EXIT();                break;
                  default:
                    cmd = 0;
                    KSTSend(&cmd, 1);
  //                  KSTSendStr("[ERROR] WRONG CMD!!\r\n");
                  break;
                }
              }
              break;
          }
          return;
        }
      }
    }
  }
}

/* Command functions -----------------------------------------------------------------------*/

static void KST_FUNC_REST( uint8_t cmd, uint32_t data )
{
  if ((cmd & 0x80) && (data == ENABLE)) {
    KSTSendStr("[REST] RESET DEVICE\r\n");
    KSTSendStr("OK\r\n");
  }
  else {
    cmd = 0;
    KSTSend(&cmd, 1);
  }
}

static void KST_FUNC_ORGL( uint8_t cmd, uint32_t data )
{
  if ((cmd & 0x80) && (data == ENABLE)) {
    KSTSendStr("[ORGL] SET TO DEFAULT\r\n");
    KSTSendStr("OK\r\n");
  }
  else {
    cmd = 0;
    KSTSend(&cmd, 1);
  }
}

static void KST_FUNC_INFO( uint8_t cmd )
{
  if (cmd & 0x80) {
    cmd = 0;
    KSTSend(&cmd, 1);
  }
  else {
    KSTSendStr(" ---- INFO -------------------- \r\n");
    KSTSendStr(" Controller:  STM32F412RGT6\r\n");
    KSTSendStr(" Sensor:      MPU9250 / LPS22HB\r\n");
    KSTSendStr(" Flash:       W25Q128FV\r\n");
    KSTSendStr(" Crystal:      16 MHz\r\n");
    KSTSendStr(" Max Freq:    100 MHz\r\n");
    KSTSendStr(" APB1 Freq:    50 MHz\r\n");
    KSTSendStr(" APB2 Freq:   100 MHz\r\n");
    KSTSendStr(" ------------------------------ \r\n");
  }
}

static void KST_FUNC_DEVID( uint8_t cmd, uint32_t data )
{
  if (cmd & 0x80) {
//    KSTSendStr("[NAME] SET DEVICE NAME\r\n");
    printf("[DEVID] %i\r\n", data);
    KSTSendStr("OK\r\n");
  }
  else {
    KSTSendStr(KS_HW_BOARD_NAME);
    KSTSendStr("\r\n");
  }
}

static void KST_FUNC_ODR( uint8_t cmd, uint32_t data )
{
  if (cmd & 0x80) {
//    KSTSendStr("[ODR] SET OUTPUT DATA RATE\r\n");
    switch (data) {
      case 1:
      case 5:
      case 10:
      case 20:
      case 50:
      case 100:
      case 200:
      case 250:
      case 400:
      case 800:
      case 1000:
      case 2000:
      case 4000:
      case 8000:
        printf("[ODR] %i\r\n", data);
        KSTSendStr("OK\r\n");
        break;
    }
  }
  else {
    KSTSendStr("[ODR] GET OUTPUT DATA RATE\r\n");
  }
}

static void KST_FUNC_BAUD( uint8_t cmd, uint32_t data )
{
  if (cmd & 0x80) {
//    KSTSendStr("[BAUD] SET UART BAUD\r\n");
    switch (data) {
      case 1200:
      case 2400:
      case 4800:
      case 9600:
      case 19200:
      case 38400:
      case 57600:
      case 115200:
      case 256000:
      case 460800:
      case 921600:
        printf("[BAUD] %i\r\n", data);
        KSTSendStr("OK\r\n");
        break;
    }
  }
  else {
    printf("%i\r\n", SERIAL_BAUDRATE);
//    KSTSendStr("[BAUD] GET UART BAUD\r\n");
  }
}

static void KST_FUNC_MODE( uint8_t cmd, uint32_t data )
{
  if (cmd & 0x80) {
//    KSTSendStr("[MODE] SET DEVICE MODE\r\n");
    printf("[MODE] %i\r\n", data);
    KSTSendStr("OK\r\n");
  }
  else {
    KSTSendStr("[MODE] GET DEVICE MODE\r\n");
  }
}

static void KST_FUNC_CALIB( uint8_t cmd, uint32_t data )
{
  if (cmd & 0x80) {
    switch (data) {
      case 0:
        KSTSendStr("[CALIB] CALIB - GYRO\r\n");
        KSTSendStr("OK\r\n");
        break;
      case 1:
        KSTSendStr("[CALIB] CALIB - ACCEL\r\n");
        KSTSendStr("OK\r\n");
        break;
      case 2:
        KSTSendStr("[CALIB] CALIB - MAG\r\n");
        KSTSendStr("OK\r\n");
        break;
    }
  }
  else {
    cmd = 0;
    KSTSend(&cmd, 1);
  }
}

static void KST_FUNC_EOUT( uint8_t cmd, uint32_t data )
{
  if (cmd & 0x80) {
    if (data == ENABLE) {
      KSTSendStr("[EOUT] ENABLE DATA OUTPUT\r\n");
      KST_Eout = ENABLE;
      KSTSendStr("OK\r\n");
    }
    else if (data == DISABLE) {
      KSTSendStr("[EOUT] DISABLE DATA OUTPUT\r\n");
      KST_Eout = DISABLE;
      KSTSendStr("OK\r\n");
    }
  }
  else {
    if (KST_Eout) {
      KSTSendStr("[EOUT] ENABLE\r\n");
    }
    else {
      KSTSendStr("[EOUT] DISABLE\r\n");
    }
  }
}

static void KST_FUNC_DOUT( uint8_t cmd )
{
  if (cmd & 0x80) {
    cmd = 0;
    KSTSend(&cmd, 1);
  }
  else {
    KSTSendStr("[DOUT] GET SENSOR DATA\r\n");
    IMU_GetRawData(&IMU);
    printf("GX:%5.0f\tGY:%5.0f\tGZ:%5.0f\tAX:%5.0f\tAY:%5.0f\tAZ:%5.0f\tMX:%5.0f\tMY:%5.0f\tMZ:%5.0f\r\n",
      IMU.gyrRaw[0], IMU.gyrRaw[1], IMU.gyrRaw[2],
      IMU.accRaw[0], IMU.accRaw[1], IMU.accRaw[2],
      IMU.magRaw[0], IMU.magRaw[1], IMU.magRaw[2]
    );
  }
}

static void KST_FUNC_EXIT( void )
{
  KST_Cmd = DISABLE;
  KSTSendStr("[EXIT] EXIT KST\r\n");
  KSTSendStr("OK\r\n");
  LED_R_Set();
}


/*************************************** END OF FILE ****************************************/
