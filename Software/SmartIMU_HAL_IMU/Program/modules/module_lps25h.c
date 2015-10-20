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

#define SPIx_CS_PIN                 GPIO_PIN_1
#define SPIx_CS_GPIO_PORT           GPIOB
#define SPIx_CS_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_CS_AF                  GPIO_AF5_SPI1
#define SPIx_CS_H                   __GPIO_SET(SPIx_CS_GPIO_PORT, SPIx_CS_PIN)
#define SPIx_CS_L                   __GPIO_RST(SPIx_CS_GPIO_PORT, SPIx_CS_PIN)

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
/*====================================================================================================*/
/*====================================================================================================*/
static SPI_HandleTypeDef LPS25_HandleStruct;
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25H_ReadReg
**功能 : 讀暫存器
**輸入 : ReadAddr
**輸出 : ReadData
**使用 : LPS25H_ReadReg(ReadAddr, &DeviceID);
**====================================================================================================*/
/*====================================================================================================*/
void LPS25H_ReadReg( uint8_t ReadAddr, uint8_t *ReadData )
{
  SPIx_CS_L;
  SPI_RW(SPIx, 0x80 | ReadAddr);
  *ReadData = SPI_RW(SPIx, 0x00);
  SPIx_CS_H;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25H_WriteReg
**功能 : 寫暫存器
**輸入 : WriteAddr, WriteData
**輸出 : None
**使用 : LPS25H_WriteReg(WriteAddr, WriteData);
**====================================================================================================*/
/*====================================================================================================*/
void LPS25H_WriteReg( uint8_t WriteAddr, uint8_t WriteData )
{
  SPIx_CS_L;
  SPI_RW(SPIx, WriteAddr);
  SPI_RW(SPIx, WriteData);
  SPIx_CS_H;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25H_SetSpeed
**功能 : 設置 SPI 速度
**輸入 : SpeedSel
**輸出 : None
**使用 : LPS25H_SetSpeed(SPIx_SPEED_LOW);
**====================================================================================================*/
/*====================================================================================================*/
void LPS25H_SetSpeed( uint8_t SpeedSel )
{
  __HAL_SPI_DISABLE(&LPS25_HandleStruct);
  LPS25_HandleStruct.Init.BaudRatePrescaler = SpeedSel;
  HAL_SPI_Init(&LPS25_HandleStruct);
  __HAL_SPI_ENABLE(&LPS25_HandleStruct);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25H_Config
**功能 : 初始化 LPS25H
**輸入 : None
**輸出 : None
**使用 : LPS25H_Config();
**====================================================================================================*/
/*====================================================================================================*/
void LPS25H_GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* SPI Clk ******************************************************************/
  SPIx_CS_GPIO_CLK_ENABLE();
  SPIx_SCK_GPIO_CLK_ENABLE();
  SPIx_SDO_GPIO_CLK_ENABLE();
  SPIx_SDI_GPIO_CLK_ENABLE();

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin       = SPIx_CS_PIN;
  HAL_GPIO_Init(SPIx_CS_GPIO_PORT, &GPIO_InitStruct);

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

  SPIx_CS_H;  // 低電位有效
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25H_SPI_Config
**功能 : 初始化 LPS25H
**輸入 : None
**輸出 : None
**使用 : LPS25H_Config();
**====================================================================================================*/
/*====================================================================================================*/
void LPS25H_SPI_Config( void )
{
  /* SPI Clk ******************************************************************/
  SPIx_CLK_ENABLE();

  /* SPI Init ****************************************************************/
  LPS25_HandleStruct.Instance               = SPIx;
  LPS25_HandleStruct.Init.Mode              = SPI_MODE_MASTER;
  LPS25_HandleStruct.Init.Direction         = SPI_DIRECTION_2LINES;
  LPS25_HandleStruct.Init.DataSize          = SPI_DATASIZE_8BIT;
  LPS25_HandleStruct.Init.CLKPolarity       = SPI_POLARITY_HIGH;
  LPS25_HandleStruct.Init.CLKPhase          = SPI_PHASE_2EDGE;
  LPS25_HandleStruct.Init.NSS               = SPI_NSS_SOFT;
  LPS25_HandleStruct.Init.BaudRatePrescaler = SPIx_SPEED_LOW;
  LPS25_HandleStruct.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  LPS25_HandleStruct.Init.TIMode            = SPI_TIMODE_DISABLE;
  LPS25_HandleStruct.Init.CRCCalculation    = SPI_CRCCALCULATION_ENABLE;
  LPS25_HandleStruct.Init.CRCPolynomial     = 7;
  HAL_SPI_Init(&LPS25_HandleStruct);

  __HAL_SPI_ENABLE(&LPS25_HandleStruct);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25H_Init
**功能 : 初始化 LPS25H
**輸入 : None
**輸出 : Status
**使用 : Status = LPS25H_Init();
**====================================================================================================*/
/*====================================================================================================*/
//#define LPS25H_InitRegNum 2
uint8_t LPS25H_Init( void )
{
  uint8_t status = ERROR;

  LPS25H_GPIO_Config();
  LPS25H_SPI_Config();

//  uint8_t LPS25H_InitData[LPS25H_InitRegNum][2] = {0};

//  for(uint8_t i = 0; i < LPS25H_InitRegNum; i++) {
//    LPS25H_WriteReg(LPS25H_InitData[i][1], LPS25H_InitData[i][0]);
//    Delay_1ms(1);
//  }

  status = LPS25H_Check();
  if(status != SUCCESS)
    return ERROR;

  LPS25H_SetSpeed(SPIx_SPEED_HIGH);
  Delay_10ms(1);

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25H_Check
**功能 : LPS25H Check
**輸入 : None
**輸出 : Status
**使用 : Status = LPS25H_Check();
**====================================================================================================*/
/*====================================================================================================*/
uint8_t LPS25H_Check( void )
{
  uint8_t DeviceID = ERROR;

  LPS25H_ReadReg(LPS25H_WHO_AM_I, &DeviceID);
  if(DeviceID != LPS25H_Device_ID)
    return ERROR;

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LPS25H_GetData
**功能 : 讀取感測器資料
**輸入 : *ReadBuf
**輸出 : None
**使用 : LPS25H_GetData(ReadBuf);
**====================================================================================================*/
/*====================================================================================================*/
void LPS25H_GetData( uint8_t *ReadBuf )
{

}
/*====================================================================================================*/
/*====================================================================================================*/
