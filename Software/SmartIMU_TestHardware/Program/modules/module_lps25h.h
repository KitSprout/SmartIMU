/* #include "module_lps25h.h" */

#ifndef __MODULE_LPS25H_H
#define __MODULE_LPS25H_H

#include "stm32f4xx_hal.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
/*
| ODR |  Pres  |  Temp  |
+-----+--------+--------+
|  0  | One-Shot Enable |
|  1  |    1Hz |    1Hz |
|  2  |    7Hz |    7Hz |
|  3  | 12.5Hz | 12.5Hz |
|  4  |   25Hz |   25Hz |
|  5  |     Reserved    |
*/

typedef enum {
  LPS_TempRES_8   = 0x00,
  LPS_TempRES_16  = 0x04,
  LPS_TempRES_32  = 0x08,
  LPS_TempRES_64  = 0x0C,
} LPS_TempRES_TypeDef;
typedef enum {
  LPS_PresRES_8   = 0x00,
  LPS_PresRES_32  = 0x01,
  LPS_PresRES_128 = 0x02,
  LPS_PresRES_512 = 0x03,
} LPS_PresRES_TypeDef;
typedef enum {
  LPS_ODR_OneShot = 0x00,
  LPS_ODR_1Hz     = 0x10,
  LPS_ODR_7Hz     = 0x20,
  LPS_ODR_12P5Hz  = 0x30,
  LPS_ODR_25Hz    = 0x40,
} LPS_ODR_TypeDef;
typedef enum {
  LPS_FIFO_MODE_BYPASS            = 0x00,
  LPS_FIFO_MODE_FIFO              = 0x20,
  LPS_FIFO_MODE_STREAM            = 0x40,
  LPS_FIFO_MODE_STREAM_TO_FIFO    = 0x60,
  LPS_FIFO_MODE_BYPASS_TO_STREAM  = 0x80,
  LPS_FIFO_MODE_NOT_AVAILABLE     = 0xA0,
  LPS_FIFO_MODE_FIFO_MEAN         = 0xC0,
  LPS_FIFO_MODE_BYPASS_TO_FIFO    = 0xE0,
} LPS_FIFO_MODE_TypeDef;
typedef enum {
  LPS_FIFO_WTM_2  = 0x01,
  LPS_FIFO_WTM_4  = 0x03,
  LPS_FIFO_WTM_8  = 0x07,
  LPS_FIFO_WTM_16 = 0x0F,
  LPS_FIFO_WTM_32 = 0x1F,
} LPS_FIFO_WTM_TypeDef;

typedef struct {
  LPS_TempRES_TypeDef   LPS_TempRES;
  LPS_PresRES_TypeDef   LPS_PresRES;
  LPS_ODR_TypeDef       LPS_ODR;
  LPS_FIFO_MODE_TypeDef LPS_FIFO_MODE;
  LPS_FIFO_WTM_TypeDef  LPS_FIFO_WTM;
} LPS_InitTypeDef;

/* ---- Sensitivity --------------------------------------------------------- */

#define LPS25H_P_hPa         ((float)0.000244140625f)  // 0.000244140625f hPa/LSB
#define LPS25H_T_degC        ((float)0.002083333333f)  // 0.002083333333f degC/LSB

/* ---- LPS25H Reg  --------------------------------------------------------- */

#define LPS25H_I2C_ADDR      ((uint8_t)0xB8) // 1011_10xb
#define LPS25H_Device_ID     ((uint8_t)0xBD)

#define LPS25H_REF_P_XL      ((uint8_t)0x08)
#define LPS25H_REF_P_L       ((uint8_t)0x09)
#define LPS25H_REF_P_H       ((uint8_t)0x0A)
#define LPS25H_WHO_AM_I      ((uint8_t)0x0F)
#define LPS25H_RES_CONF      ((uint8_t)0x10)
#define LPS25H_CTRL_REG1     ((uint8_t)0x20)
#define LPS25H_CTRL_REG2     ((uint8_t)0x21)
#define LPS25H_CTRL_REG3     ((uint8_t)0x22)
#define LPS25H_CTRL_REG4     ((uint8_t)0x23)
#define LPS25H_INT_CFG       ((uint8_t)0x24)
#define LPS25H_INT_SOURCE    ((uint8_t)0x25)
#define LPS25H_STATUS_REG    ((uint8_t)0x27)
#define LPS25H_PRESS_POUT_XL ((uint8_t)0x28)
#define LPS25H_PRESS_OUT_L   ((uint8_t)0x29)
#define LPS25H_PRESS_OUT_H   ((uint8_t)0x2A)
#define LPS25H_TEMP_OUT_L    ((uint8_t)0x2B)
#define LPS25H_TEMP_OUT_H    ((uint8_t)0x2C)
#define LPS25H_FIFO_CTRL     ((uint8_t)0x2E)
#define LPS25H_FIFO_STATUS   ((uint8_t)0x2F)
#define LPS25H_THS_P_L       ((uint8_t)0x30)
#define LPS25H_THS_P_H       ((uint8_t)0x31)
#define LPS25H_RPDS_L        ((uint8_t)0x39)
#define LPS25H_RPDS_H        ((uint8_t)0x3A)
/*=====================================================================================================*/
/*=====================================================================================================*/
void    LPS25H_WriteReg( uint8_t writeAddr, uint8_t writeData );
void    LPS25H_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens );
uint8_t LPS25H_ReadReg( uint8_t readAddr );
void    LPS25H_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens );

void    LPS25H_SetSpeed( uint8_t speedSel );

void    LPS25H_Config( void );
uint8_t LPS25H_Init( void );

uint8_t LPS25H_Check( void );
void    LPS25H_getData( uint8_t *ReadBuf );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
