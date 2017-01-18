/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    lps22hb.h
  * @author  KitSprout
  * @date    19-Jan-2017
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __LPS22HB_H
#define __LPS22HB_H

#ifdef __cplusplus
 extern "C" {
#endif

#define __LPS22
#define __USE_BAROMETER

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "algorithms\mathUnit.h"

/* Exported types --------------------------------------------------------------------------*/
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
} LPS_ConfigTypeDef;

/* Exported constants ----------------------------------------------------------------------*/

/* ---- LPS25H Reg  --------------------------------------------------------- */

#define LPS22HB_I2C_ADDR      ((uint8_t)0xB8)   // 1011_10xb
#define LPS22HB_DeviceID      ((uint8_t)0xB1)

#define LPS22HB_INTERRUPT_CFG ((uint8_t)0x0B)
#define LPS22HB_THS_P_L       ((uint8_t)0x0C)
#define LPS22HB_THS_P_H       ((uint8_t)0x0D)
#define LPS22HB_WHO_AM_I      ((uint8_t)0x0F)
#define LPS22HB_CTRL_REG1     ((uint8_t)0x10)
#define LPS22HB_CTRL_REG2     ((uint8_t)0x11)
#define LPS22HB_CTRL_REG3     ((uint8_t)0x12)
#define LPS22HB_FIFO_CTRL     ((uint8_t)0x14)
#define LPS22HB_REF_P_XL      ((uint8_t)0x15)
#define LPS22HB_REF_P_L       ((uint8_t)0x16)
#define LPS22HB_REF_P_H       ((uint8_t)0x17)
#define LPS22HB_RPDS_L        ((uint8_t)0x18)
#define LPS22HB_RPDS_H        ((uint8_t)0x19)
#define LPS22HB_RES_CONF      ((uint8_t)0x1A)
#define LPS22HB_INT_SOURCE    ((uint8_t)0x25)
#define LPS22HB_FIFO_STATUS   ((uint8_t)0x26)
#define LPS22HB_STATUS        ((uint8_t)0x27)
#define LPS22HB_PRESS_OUT_XL  ((uint8_t)0x28)
#define LPS22HB_PRESS_OUT_L   ((uint8_t)0x29)
#define LPS22HB_PRESS_OUT_H   ((uint8_t)0x2A)
#define LPS22HB_TEMP_OUT_L    ((uint8_t)0x2B)
#define LPS22HB_TEMP_OUT_H    ((uint8_t)0x2C)
#define LPS22HB_LPFP_RES      ((uint8_t)0x33)

/* Exported functions ----------------------------------------------------------------------*/  
void     LPS22_WriteReg( uint8_t writeAddr, uint8_t writeData );
void     LPS22_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens );
uint8_t  LPS22_ReadReg( uint8_t readAddr );
void     LPS22_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens );

void     LPS22_Config( void );
int8_t   LPS22_Init( LPS_ConfigTypeDef *LPSx );
int8_t   LPS22_DeviceCheck( void );
void     LPS22_GetSensitivity( float32_t *sensitivity );
int8_t   LPS22_GetRawData( int32_t *data );

#ifdef __cplusplus
}
#endif

#endif
