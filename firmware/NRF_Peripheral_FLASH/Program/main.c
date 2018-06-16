/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    main.c
 *  @author  KitSprout
 *  @date    16-Jun-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_flash.h"
#include "modules\serial.h"
#include "nrf5x_bsp.h"

/** @addtogroup NRF5x_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
#define BUFF_SIZE   1024

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
uint32_t w_data[BUFF_SIZE] = {0};
uint32_t r_data[BUFF_SIZE] = {0};

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

int main( void )
{
  uint32_t pg_size;
  uint32_t pg_num;
  uint32_t address;
  uint32_t error;

  bsp_gpio_init();
  bsp_serial_init(NULL);

  pg_size = FLASH_PAGE_SIZE;
  pg_num  = FLASH_SIZE - 1;  // Use last page in flash
  address = pg_size * pg_num;
  printf("address = 0x%08X, page size = %d, page num = %d\n\r", address, pg_size, pg_num);

  for (uint32_t i = 0; i < BUFF_SIZE; i++) {
    w_data[i] = i;
  }

  delay_ms(500);
  printf("flash  8-bit read / write ... ");
  error = 0;
  for (uint32_t i = 0; i < BUFF_SIZE; i++) {
    r_data[i] = 0;
  }
  FLASH_ErasePage(address);
  FLASH_WriteDataU8(address, (uint8_t*)w_data, BUFF_SIZE << 2);
  FLASH_ReadDataU8(address, (uint8_t*)r_data, BUFF_SIZE << 2);
  for (uint32_t i = 0; i < BUFF_SIZE; i++)
  {
    if (w_data[i] != r_data[i]) {
      error++;
    }
  }
  if (error == 0) {
    printf("OK\n\r");
  }
  else {
    printf("ERROR (%i / %i)\n\r", error, BUFF_SIZE);
  }

  delay_ms(500);
  printf("flash 16-bit read / write ... ");
  error = 0;
  for (uint32_t i = 0; i < BUFF_SIZE; i++) {
    r_data[i] = 0;
  }
  FLASH_ErasePage(address);
  FLASH_WriteDataU16(address, (uint16_t*)w_data, BUFF_SIZE << 1);
  FLASH_ReadDataU16(address, (uint16_t*)r_data, BUFF_SIZE << 1);
  for (uint32_t i = 0; i < BUFF_SIZE; i++) {
    if (w_data[i] != r_data[i]) {
      error++;
    }
  }
  if (error == 0) {
    printf("OK\n\r");
  }
  else {
    printf("ERROR (%i / %i)\n\r", error, BUFF_SIZE);
  }

  delay_ms(500);
  printf("flash 32-bit read / write ... ");
  error = 0;
  for (uint32_t i = 0; i < BUFF_SIZE; i++) {
    r_data[i] = 0;
  }
  FLASH_ErasePage(address);
  FLASH_WriteDataU32(address, w_data, BUFF_SIZE);
  FLASH_ReadDataU32(address, r_data, BUFF_SIZE);
  for (uint32_t i = 0; i < BUFF_SIZE; i++) {
    if (w_data[i] != r_data[i]) {
      error++;
    }
  }
  if (error == 0) {
    printf("OK\n\r");
  }
  else {
    printf("ERROR (%i / %i)\n\r", error, BUFF_SIZE);
  }

  delay_ms(500);
  printf("\r\nflash test ... ok\r\n");

  while (1) {
    LED_Toggle();
    delay_ms(100);
  }
}

/*************************************** END OF FILE ****************************************/
