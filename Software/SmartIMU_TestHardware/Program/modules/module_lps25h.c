/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"

#include "module_lps25h.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define SPIx                        SPI1
#define SPIx_CLK_ENABLE()           __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx_SPEED_HIGH             SPI_BAUDRATEPRESCALER_4
#define SPIx_SPEED_LOW              SPI_BAUDRATEPRESCALER_256

#define SPIx_CSB_PIN                GPIO_PIN_0
#define SPIx_CSB_GPIO_PORT          GPIOB
#define SPIx_CSB_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_CSB_H()                __GPIO_SET(SPIx_CSB_GPIO_PORT, SPIx_CSB_PIN)
#define SPIx_CSB_L()                __GPIO_RST(SPIx_CSB_GPIO_PORT, SPIx_CSB_PIN)

#define SPIx_SCK_PIN                GPIO_PIN_5
#define SPIx_SCK_GPIO_PORT          GPIOA
#define SPIx_SCK_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_SCK_AF                 GPIO_AF5_SPI1

#define SPIx_SDO_PIN                GPIO_PIN_6
#define SPIx_SDO_GPIO_PORT          GPIOA
#define SPIx_SDO_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_SDO_AF                 GPIO_AF5_SPI1

#define SPIx_SDI_PIN                GPIO_PIN_7
#define SPIx_SDI_GPIO_PORT          GPIOA
#define SPIx_SDI_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_SDI_AF                 GPIO_AF5_SPI1

#define SPIx_INTB_PIN               GPIO_PIN_0
#define SPIx_INTB_GPIO_PORT         GPIOA
#define SPIx_INTB_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_INTB_IRQ               EXTI0_IRQn
/*====================================================================================================*/
/*====================================================================================================*/
static SPI_HandleTypeDef LPSx_InitStruct;
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25H_WriteReg
**功能 : Write Reg
**輸入 : writeAddr, writeData
**輸出 : None
**使用 : LPS25H_WriteReg(writeAddr, writeData);
**====================================================================================================*/
/*====================================================================================================*/
void LPS25H_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
  SPIx_CSB_L();
  SPI_RW(SPIx, writeAddr);
  SPI_RW(SPIx, writeData);
  SPIx_CSB_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25H_WriteRegs
**功能 : Write Regs
**輸入 : writeAddr, *writeData, lens
**輸出 : None
**使用 : LPS25H_WriteRegs(writeAddr, writeData, lens);
**====================================================================================================*/
/*====================================================================================================*/
void LPS25H_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
  SPIx_CSB_L();
  SPI_RW(SPIx, writeAddr);
  for(uint8_t i = 0; i < lens; i++)
    SPI_RW(SPIx, writeData[i]);
  SPIx_CSB_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25H_ReadReg
**功能 : Read Reg
**輸入 : readAddr
**輸出 : readData
**使用 : readData = LPS25H_ReadReg(readAddr);
**====================================================================================================*/
/*====================================================================================================*/
uint8_t LPS25H_ReadReg( uint8_t readAddr )
{
  uint8_t readData = 0;

  SPIx_CSB_L();
  SPI_RW(SPIx, 0x80 | readAddr);
  readData = SPI_RW(SPIx, 0x00);
  SPIx_CSB_H();

  return readData;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : LPS25H_ReadRegs
**功能 : Read Regs
**輸入 : readAddr, *readData, lens
**輸出 : None
**使用 : LPS25H_ReadRegs(readAddr, readData, lens);
**=====================================================================================================*/
/*=====================================================================================================*/
void LPS25H_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
  SPIx_CSB_L();
  SPI_RW(SPIx, 0x80 | readAddr);
  for(uint8_t i = 0; i < lens; i++)
    readData[i] = SPI_RW(SPIx, 0x00);
  SPIx_CSB_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25H_SetSpeed
**功能 : Set SPI Speed
**輸入 : SpeedSel
**輸出 : None
**使用 : LPS25H_SetSpeed(SPIx_SPEED_LOW);
**====================================================================================================*/
/*====================================================================================================*/
void LPS25H_SetSpeed( uint8_t speedSel )
{
  __HAL_SPI_DISABLE(&LPSx_InitStruct);
  LPSx_InitStruct.Init.BaudRatePrescaler = speedSel;
  HAL_SPI_Init(&LPSx_InitStruct);
  __HAL_SPI_ENABLE(&LPSx_InitStruct);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25H_Config
**功能 : LPS25H Config
**輸入 : None
**輸出 : None
**使用 : LPS25H_Config();
**====================================================================================================*/
/*====================================================================================================*/
void LPS25H_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* SPI Clk ******************************************************************/
  SPIx_CSB_GPIO_CLK_ENABLE();
  SPIx_SCK_GPIO_CLK_ENABLE();
  SPIx_SDO_GPIO_CLK_ENABLE();
  SPIx_SDI_GPIO_CLK_ENABLE();
  SPIx_INTB_GPIO_CLK_ENABLE();
  SPIx_CLK_ENABLE();

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin       = SPIx_CSB_PIN;
  HAL_GPIO_Init(SPIx_CSB_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
  GPIO_InitStruct.Alternate = SPIx_SCK_AF;
  HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = SPIx_SDO_PIN;
  GPIO_InitStruct.Alternate = SPIx_SDO_AF;
  HAL_GPIO_Init(SPIx_SDO_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = SPIx_SDI_PIN;
  GPIO_InitStruct.Alternate = SPIx_SDI_AF;
  HAL_GPIO_Init(SPIx_SDI_GPIO_PORT, &GPIO_InitStruct);

  SPIx_CSB_H();  // LOW ENABLE

  // EXIT Config
  GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Pin   = SPIx_INTB_PIN;
  HAL_GPIO_Init(SPIx_INTB_GPIO_PORT, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(SPIx_INTB_IRQ, 14, 0);
  HAL_NVIC_EnableIRQ(SPIx_INTB_IRQ);

  /* SPI Init ****************************************************************/
  LPSx_InitStruct.Instance               = SPIx;
  LPSx_InitStruct.Init.Mode              = SPI_MODE_MASTER;
  LPSx_InitStruct.Init.Direction         = SPI_DIRECTION_2LINES;
  LPSx_InitStruct.Init.DataSize          = SPI_DATASIZE_8BIT;
  LPSx_InitStruct.Init.CLKPolarity       = SPI_POLARITY_HIGH;
  LPSx_InitStruct.Init.CLKPhase          = SPI_PHASE_2EDGE;
  LPSx_InitStruct.Init.NSS               = SPI_NSS_SOFT;
  LPSx_InitStruct.Init.BaudRatePrescaler = SPIx_SPEED_LOW;
  LPSx_InitStruct.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  LPSx_InitStruct.Init.TIMode            = SPI_TIMODE_DISABLE;
  LPSx_InitStruct.Init.CRCCalculation    = SPI_CRCCALCULATION_ENABLE;
  LPSx_InitStruct.Init.CRCPolynomial     = 7;
  HAL_SPI_Init(&LPSx_InitStruct);

  __HAL_SPI_ENABLE(&LPSx_InitStruct);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25H_Init
**功能 : Init LPS25H
**輸入 : None
**輸出 : status
**使用 : status = LPS25H_Init();
**====================================================================================================*/
/*====================================================================================================*/
//#define LPS25H_InitRegNum 2
uint8_t LPS25H_Init( void )
{
  uint8_t status = ERROR;
//  uint8_t LPS25H_InitData[LPS25H_InitRegNum][2] = {0};

  status = LPS25H_Check();
  if(status != SUCCESS)
    return ERROR;

  delay_ms(10);

//  for(uint8_t i = 0; i < LPS25H_InitRegNum; i++) {
//    Delay_1ms(2);
//    LPS25H_WriteReg(LPS25H_InitData[i][1], LPS25H_InitData[i][0]);
//  }

  LPS25H_SetSpeed(SPIx_SPEED_HIGH);
  delay_ms(10);

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25H_Check
**功能 : Check Device ID
**輸入 : None
**輸出 : Status
**使用 : Status = LPS25H_Check();
**====================================================================================================*/
/*====================================================================================================*/
uint8_t LPS25H_Check( void )
{
  uint8_t deviceID = ERROR;

  deviceID = LPS25H_ReadReg(LPS25H_WHO_AM_I);
  if(deviceID != LPS25H_Device_ID)
    return ERROR;

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25H_getData
**功能 : Get Sensor Data
**輸入 : *dataIMU
**輸出 : None
**使用 : LPS25H_getData(dataIMU);
**====================================================================================================*/
/*====================================================================================================*/
void LPS25H_getData( uint8_t *ReadBuf )
{

}
/*====================================================================================================*/
/*====================================================================================================*/
