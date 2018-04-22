/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_uarte.c
 *  @author  KitSprout
 *  @date    22-Apr-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_uarte.h"

/** @addtogroup NRF5x_Driver
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  UARTE_Init
 */
void UARTE_Init( UARTE_InitTypeDef *huarte )
{
  nrf_gpio_cfg_output(huarte->PinTXD);
  nrf_gpio_cfg_input(huarte->PinRXD, NRF_GPIO_PIN_NOPULL);

  huarte->Instance->PSEL.TXD = huarte->PinTXD;
  huarte->Instance->PSEL.RXD = huarte->PinRXD;
  huarte->Instance->PSEL.CTS = NRF_UARTE_PSEL_DISCONNECTED;
  huarte->Instance->PSEL.RTS = NRF_UARTE_PSEL_DISCONNECTED;

  if (huarte->HardwareFlowControl == NRF_UARTE_HWFC_ENABLED) {
    nrf_gpio_cfg_output(huarte->PinRTS);
    nrf_gpio_cfg_input(huarte->PinCTS, NRF_GPIO_PIN_NOPULL);
    huarte->Instance->PSEL.CTS = huarte->PinCTS;
    huarte->Instance->PSEL.RTS = huarte->PinRTS;
  }

  huarte->Instance->CONFIG   = huarte->Parity | huarte->HardwareFlowControl;
  huarte->Instance->BAUDRATE = huarte->BaudRate;

  UARTE_Start(huarte);
}

/**
 *  @brief  UARTE_Cmd
 */
void UARTE_Cmd( UARTE_InitTypeDef *huarte, uint32_t state )
{
  if (state != ENABLE) {
    huarte->Instance->ENABLE = UARTE_ENABLE_ENABLE_Disabled;
  }
  else {
    huarte->Instance->ENABLE = UARTE_ENABLE_ENABLE_Enabled;
  }
}

/**
 *  @brief  UARTE_InterruptCmd
 */
void UARTE_InterruptCmd( UARTE_InitTypeDef *huarte, uint32_t mode, uint32_t state )
{
  if (state == ENABLE) {
    huarte->Instance->INTENSET = mode;
  }
  else {
    huarte->Instance->INTENCLR = mode;
  }
}

/**
 *  @brief  UARTE_Start
 */
void UARTE_Start( UARTE_InitTypeDef *huarte )
{
  UARTE_EVENTS_TXDRDY(huarte->Instance) = RESET;
  UARTE_EVENTS_RXDRDY(huarte->Instance) = RESET;
}

/**
 *  @brief  UARTE_Set_TxBuffer
 */
void UARTE_Set_TxBuffer( UARTE_InitTypeDef *huarte, uint8_t *buffer, uint32_t lens )
{
  huarte->Instance->TXD.PTR = (uint32_t)buffer;
  huarte->Instance->TXD.MAXCNT = lens;
}

/**
 *  @brief  UARTE_Set_RxBuffer
 */
void UARTE_Set_RxBuffer( UARTE_InitTypeDef *huarte, uint8_t *buffer, uint32_t lens )
{
  huarte->Instance->RXD.PTR = (uint32_t)buffer;
  huarte->Instance->RXD.MAXCNT = lens;
}

/**
 *  @brief  UARTE_SendByte
 */
void UARTE_SendByte( UARTE_InitTypeDef *huarte, uint8_t *sendByte )
{
  UARTE_EVENTS_ENDTX(huarte->Instance) = RESET;
  UARTE_EVENTS_TXSTOPPED(huarte->Instance) = RESET;
  huarte->Instance->TXD.PTR    = (uint32_t)sendByte;
  huarte->Instance->TXD.MAXCNT = 1;
  UARTE_TASKS_STARTTX(huarte->Instance);
  while ((UARTE_EVENTS_ENDTX(huarte->Instance) != SET) && (UARTE_EVENTS_TXSTOPPED(huarte->Instance) != SET));
}

/**
 *  @brief  UARTE_RecvByte
 */
void UARTE_RecvByte( UARTE_InitTypeDef *huarte, uint8_t *recvByte )
{
  UARTE_EVENTS_ENDRX(huarte->Instance) = RESET;
  UARTE_EVENTS_RXTO(huarte->Instance) = RESET;
  huarte->Instance->RXD.PTR    = (uint32_t)recvByte;
  huarte->Instance->RXD.MAXCNT = 1;
  UARTE_TASKS_STARTRX(huarte->Instance);
  while ((UARTE_EVENTS_ENDRX(huarte->Instance) != SET) && (UARTE_EVENTS_RXTO(huarte->Instance) != SET) && (UARTE_EVENTS_ERROR(huarte->Instance) != SET));
}

/**
 *  @brief  UARTE_SendData
 */
uint32_t UARTE_SendData( UARTE_InitTypeDef *huarte, uint8_t *sendData, uint32_t lens )
{
  uint32_t endtx, txstopped;

  if ((sendData == NULL) || (lens == 0)) {
    return KS_ERROR;
  }

  UARTE_EVENTS_ENDTX(huarte->Instance) = RESET;
  UARTE_EVENTS_TXSTOPPED(huarte->Instance) = RESET;
  huarte->Instance->TXD.PTR    = (uint32_t)sendData;
  huarte->Instance->TXD.MAXCNT = lens;
  UARTE_TASKS_STARTTX(huarte->Instance);
  do {
    endtx     = UARTE_EVENTS_ENDTX(huarte->Instance);
    txstopped = UARTE_EVENTS_TXSTOPPED(huarte->Instance);
  } while ((!endtx) && (!txstopped));

  if (txstopped) {
    return KS_ERROR;
  }

  return KS_OK;
}

/**
 *  @brief  UARTE_RecvData
 */
uint32_t UARTE_RecvData( UARTE_InitTypeDef *huarte, uint8_t *recvData, uint32_t lens )
{
  uint32_t endrx, rxto, error;

  if ((recvData == NULL) || (lens == 0)) {
    return KS_ERROR;
  }

  UARTE_EVENTS_ENDRX(huarte->Instance) = RESET;
  UARTE_EVENTS_RXTO(huarte->Instance) = RESET;
  huarte->Instance->RXD.PTR    = (uint32_t)recvData;
  huarte->Instance->RXD.MAXCNT = lens;
  UARTE_TASKS_STARTRX(huarte->Instance);
  do {
    endrx = UARTE_EVENTS_ENDRX(huarte->Instance);
    rxto  = UARTE_EVENTS_RXTO(huarte->Instance);
    error = UARTE_EVENTS_ERROR(huarte->Instance);
  } while ((!endrx) && (!rxto) && (!error));

  if (rxto) {
    return KS_TIMEOUT;
  }
  else if (error) {
    return KS_ERROR;
  }

  return KS_OK;
}

/**
 *  @brief  UARTE_SendDataWaitTimeout
 */
uint32_t UARTE_SendDataWaitTimeout( UARTE_InitTypeDef *huarte, uint8_t *sendData, uint32_t lens, uint32_t timeout )
{
  uint32_t txto = RESET, endtx, txstopped;

  if ((sendData == NULL) || (lens == 0)) {
    return KS_ERROR;
  }

  UARTE_EVENTS_ENDTX(huarte->Instance) = RESET;
  UARTE_EVENTS_TXSTOPPED(huarte->Instance) = RESET;
  huarte->Instance->TXD.PTR    = (uint32_t)sendData;
  huarte->Instance->TXD.MAXCNT = lens;
  UARTE_TASKS_STARTTX(huarte->Instance);
  do {
    endtx     = UARTE_EVENTS_ENDTX(huarte->Instance);
    txstopped = UARTE_EVENTS_TXSTOPPED(huarte->Instance);

    if (--timeout) {
      delay_us(1);
    }
    else {
      // stop
      txto = SET;
    }
  } while ((!txto) && (!endtx) && (!txstopped));

  if (txto) {
    return KS_TIMEOUT;
  }
  else if (txstopped) {
    return KS_ERROR;
  }

  return KS_OK;
}

/**
 *  @brief  UARTE_RecvDataWaitTimeout
 */
uint32_t UARTE_RecvDataWaitTimeout( UARTE_InitTypeDef *huarte, uint8_t *recvData, uint32_t lens, uint32_t timeout )
{
  uint32_t endrx, rxto, error;

  if ((recvData == NULL) || (lens == 0)) {
    return KS_ERROR;
  }

  UARTE_EVENTS_ENDRX(huarte->Instance) = RESET;
  UARTE_EVENTS_RXTO(huarte->Instance) = RESET;
  huarte->Instance->RXD.PTR    = (uint32_t)recvData;
  huarte->Instance->RXD.MAXCNT = lens;
  UARTE_TASKS_STARTRX(huarte->Instance);
  do {
    endrx = UARTE_EVENTS_ENDRX(huarte->Instance);
    rxto  = UARTE_EVENTS_RXTO(huarte->Instance);
    error = UARTE_EVENTS_ERROR(huarte->Instance);

    if (--timeout) {
      delay_us(1);
    }
    else {
      // stop
      rxto = SET;
    }
  } while ((!endrx) && (!rxto) && (!error));

  if (rxto) {
    return KS_TIMEOUT;
  }
  else if (error) {
    return KS_ERROR;
  }

  return KS_OK;
}

/*************************************** END OF FILE ****************************************/
