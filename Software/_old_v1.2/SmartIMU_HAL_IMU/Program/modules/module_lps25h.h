/* #include "module_lps25h.h" */

#ifndef __MODULE_LPS25H_H
#define __MODULE_LPS25H_H

#include "stm32f4xx_hal.h"
/*=====================================================================================================*/
/*=====================================================================================================*/

/* ---- Sensitivity --------------------------------------------------------- */

#define LPS25H_I2C_ADDR       ((uint8_t)0xB8) // 1011_10xb
#define LPS25H_Device_ID      ((uint8_t)0xBD)

#define LPS25H_REF_P_XL       ((uint8_t)0x08)
#define LPS25H_REF_P_L        ((uint8_t)0x09)
#define LPS25H_REF_P_H        ((uint8_t)0x0A)
#define LPS25H_WHO_AM_I       ((uint8_t)0x0F)
#define LPS25H_RES_CONF       ((uint8_t)0x10)
#define LPS25H_CTRL_REG1      ((uint8_t)0x20)
#define LPS25H_CTRL_REG2      ((uint8_t)0x21)
#define LPS25H_CTRL_REG3      ((uint8_t)0x22)
#define LPS25H_CTRL_REG4      ((uint8_t)0x23)
#define LPS25H_INT_CFG        ((uint8_t)0x24)
#define LPS25H_INT_SOURCE     ((uint8_t)0x25)
#define LPS25H_STATUS_REG     ((uint8_t)0x27)
#define LPS25H_PRESS_POUT_XL  ((uint8_t)0x28)
#define LPS25H_PRESS_OUT_L    ((uint8_t)0x29)
#define LPS25H_PRESS_OUT_H    ((uint8_t)0x2A)
#define LPS25H_TEMP_OUT_L     ((uint8_t)0x2B)
#define LPS25H_TEMP_OUT_H     ((uint8_t)0x2C)
#define LPS25H_FIFO_CTRL      ((uint8_t)0x2E)
#define LPS25H_FIFO_STATUS    ((uint8_t)0x3F)
#define LPS25H_THS_P_L        ((uint8_t)0x30)
#define LPS25H_THS_P_H        ((uint8_t)0x31)
#define LPS25H_RPDS_L         ((uint8_t)0x39)
#define LPS25H_RPDS_H         ((uint8_t)0x3A)
/*=====================================================================================================*/
/*=====================================================================================================*/
void    LPS25H_ReadReg( uint8_t ReadAddr, uint8_t *ReadData );
void    LPS25H_WriteReg( uint8_t WriteAddr, uint8_t WriteData );
void    LPS25H_SetSpeed( uint8_t SpeedSel );

void    LPS25H_GPIO_Config( void );
void    LPS25H_SPI_Config( void );
uint8_t LPS25H_Init( void );
uint8_t LPS25H_Check( void );
void    LPS25H_getData( uint8_t *ReadBuf );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
