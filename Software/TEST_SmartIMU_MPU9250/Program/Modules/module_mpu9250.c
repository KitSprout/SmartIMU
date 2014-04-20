/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_spi.h"
#include "module_mpu9250.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define SPIx                  SPI1
#define SPIx_CLK              RCC_APB2Periph_SPI1

#define SPIx_CSM_PIN          GPIO_Pin_3
#define SPIx_CSM_GPIO_PORT    GPIOA
#define SPIx_CSM_GPIO_CLK     RCC_AHB1Periph_GPIOA

#define SPIx_SCK_PIN          GPIO_Pin_5
#define SPIx_SCK_GPIO_PORT    GPIOA
#define SPIx_SCK_GPIO_CLK     RCC_AHB1Periph_GPIOA
#define SPIx_SCK_SOURCE       GPIO_PinSource5
#define SPIx_SCK_AF           GPIO_AF_SPI1

#define SPIx_SDO_PIN          GPIO_Pin_6
#define SPIx_SDO_GPIO_PORT    GPIOA
#define SPIx_SDO_GPIO_CLK     RCC_AHB1Periph_GPIOA
#define SPIx_SDO_SOURCE       GPIO_PinSource6
#define SPIx_SDO_AF           GPIO_AF_SPI1

#define SPIx_SDI_PIN          GPIO_Pin_7
#define SPIx_SDI_GPIO_PORT    GPIOA
#define SPIx_SDI_GPIO_CLK     RCC_AHB1Periph_GPIOA
#define SPIx_SDI_SOURCE       GPIO_PinSource7
#define SPIx_SDI_AF           GPIO_AF_SPI1

#define IMU_CSM   PAO(3)
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_ReadReg
**功能 : 讀暫存器
**輸入 : ReadAddr
**輸出 : ReadData
**使用 : MPU9250_ReadReg(ReadAddr, &DeviceID);
**====================================================================================================*/
/*====================================================================================================*/
static void MPU9250_ReadReg( u8 ReadAddr, u8 *ReadData )
{
  IMU_CSM = 0;
  SPI_RW(SPIx, 0x80 | ReadAddr);
  *ReadData = SPI_RW(SPIx, 0xFF);
  IMU_CSM = 1;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_WriteReg
**功能 : 寫暫存器
**輸入 : WriteAddr, WriteData
**輸出 : None
**使用 : MPU9250_WriteReg(WriteAddr, WriteData);
**====================================================================================================*/
/*====================================================================================================*/
static void MPU9250_WriteReg( u8 WriteAddr, u8 WriteData )
{
  IMU_CSM = 0;
  SPI_RW(SPIx, WriteAddr);
  SPI_RW(SPIx, WriteData);
  IMU_CSM = 1;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : MPU9250_ReadRegs
**功能 : 連續讀暫存器
**輸入 : ReadAddr, *ReadBuf, Bytes
**輸出 : None
**使用 : MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, ReadBuf, 14);
**=====================================================================================================*/
/*=====================================================================================================*/
static void MPU9250_ReadRegs( u8 ReadAddr, u8 *ReadBuf, u8 Bytes )
{
  u8 i = 0;

  IMU_CSM = 0;
  SPI_RW(SPIx, 0x80 | ReadAddr);
  for(i=0; i<Bytes; i++)
    ReadBuf[i] = SPI_RW(SPIx, 0xFF);
  IMU_CSM = 1;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Config
**功能 : 初始化 MPU9250
**輸入 : None
**輸出 : None
**使用 : MPU9250_Config();
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef SPI_InitStruct;

  /* SPI Clk Init ************************************************************/
  RCC_AHB1PeriphClockCmd(SPIx_CSM_GPIO_CLK | SPIx_SCK_GPIO_CLK |
                         SPIx_SDO_GPIO_CLK | SPIx_SDI_GPIO_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(SPIx_CLK, ENABLE);

  /* SPI AF ******************************************************************/
  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
  GPIO_PinAFConfig(SPIx_SDO_GPIO_PORT, SPIx_SDO_SOURCE, SPIx_SDO_AF);
  GPIO_PinAFConfig(SPIx_SDI_GPIO_PORT, SPIx_SDI_SOURCE, SPIx_SDI_AF);

  /* CSM PA4  */
  GPIO_InitStruct.GPIO_Pin = SPIx_CSM_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPIx_CSM_GPIO_PORT, &GPIO_InitStruct);
  /* SCK PA5  */
  GPIO_InitStruct.GPIO_Pin = SPIx_SCK_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);
  /* SDO PA6  */
  GPIO_InitStruct.GPIO_Pin = SPIx_SDO_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPIx_SDO_GPIO_PORT, &GPIO_InitStruct);
  /* SDI PA7  */
  GPIO_InitStruct.GPIO_Pin = SPIx_SDI_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPIx_SDI_GPIO_PORT, &GPIO_InitStruct);

  IMU_CSM = 1;  // 低電位有效

  /* SPI Init ****************************************************************/
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   // Full Duplex
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;                        // Master Mode
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;                    // Data Size 8 bit
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;                          // Transitioned On The Falling Edge
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;                         // Latched On the Rising Edge
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;                            // Software NSS Signal
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;   // fsck = APB2 84MHz / 4 = 21MHz
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;                   // MSB First
  SPI_InitStruct.SPI_CRCPolynomial = 7;
  SPI_Init(SPIx, &SPI_InitStruct);

  SPI_Cmd(SPIx, ENABLE);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Init
**功能 : 初始化 MPU9250
**輸入 : None
**輸出 : None
**使用 : MPU9250_Init();
**====================================================================================================*/
/*====================================================================================================*/
#define MPU9250_InitRegNum 10
void MPU9250_Init( void )
{
  u8 i = 0;
  u8 MPU6500_Init_Data[MPU9250_InitRegNum][2] = {
      {0x80, MPU6500_PWR_MGMT_1},     // Reset Device
      {0x01, MPU6500_PWR_MGMT_1},     // Clock Source
      {0x00, MPU6500_PWR_MGMT_2},     // Enable Acc & Gyro
      {0x07, MPU6500_CONFIG},         // 
      {0x18, MPU6500_GYRO_CONFIG},    // +-2000dps
      {0x08, MPU6500_ACCEL_CONFIG},   // +-4G
      {0x00, MPU6500_ACCEL_CONFIG_2}, // Set Acc Data Rates
      {0x30, MPU6500_INT_PIN_CFG},    // 
      {0x40, MPU6500_I2C_MST_CTRL},   // I2C Speed 348 kHz
      {0x20, MPU6500_USER_CTRL},      // Enable AUX
    };

  for(i=0; i<MPU9250_InitRegNum; i++) {
    MPU9250_WriteReg(MPU6500_Init_Data[i][1], MPU6500_Init_Data[i][0]);
    Delay_1ms(1);
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Check
**功能 : MPU9250 Check
**輸入 : None
**輸出 : Status
**使用 : Status = MPU9250_Check();
**====================================================================================================*/
/*====================================================================================================*/
u8 MPU9250_Check( void )
{
  u8 DeviceID;

  /* MPU6500 */
  DeviceID = 0x00;
  MPU9250_ReadReg(MPU6500_WHO_AM_I, &DeviceID);
  if(DeviceID != MPU6500_Device_ID)
    return ERROR;

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Read
**功能 : 讀取感測器資料
**輸入 : *ReadBuf
**輸出 : None
**使用 : MPU9250_Read(ReadBuf);
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_Read( u8 *ReadBuf )
{
  MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, ReadBuf, 14);
}
/*====================================================================================================*/
/*====================================================================================================*/
