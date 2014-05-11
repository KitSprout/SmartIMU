/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_spi.h"
#include "module_ms5611.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define SPIx                  SPI2
#define SPIx_CLK              RCC_APB1Periph_SPI2

#define SPIx_CSB_PIN          GPIO_Pin_10
#define SPIx_CSB_GPIO_PORT    GPIOB
#define SPIx_CSB_GPIO_CLK     RCC_AHB1Periph_GPIOB

#define SPIx_SCK_PIN          GPIO_Pin_13
#define SPIx_SCK_GPIO_PORT    GPIOB
#define SPIx_SCK_GPIO_CLK     RCC_AHB1Periph_GPIOB
#define SPIx_SCK_SOURCE       GPIO_PinSource13
#define SPIx_SCK_AF           GPIO_AF_SPI2

#define SPIx_SDO_PIN          GPIO_Pin_14
#define SPIx_SDO_GPIO_PORT    GPIOB
#define SPIx_SDO_GPIO_CLK     RCC_AHB1Periph_GPIOB
#define SPIx_SDO_SOURCE       GPIO_PinSource14
#define SPIx_SDO_AF           GPIO_AF_SPI2

#define SPIx_SDI_PIN          GPIO_Pin_15
#define SPIx_SDI_GPIO_PORT    GPIOB
#define SPIx_SDI_GPIO_CLK     RCC_AHB1Periph_GPIOB
#define SPIx_SDI_SOURCE       GPIO_PinSource15
#define SPIx_SDI_AF           GPIO_AF_SPI2

#define IMU_CSB   PBO(10)
/*====================================================================================================*/
/*====================================================================================================*/
static void MS5611_Reset( void );
static void MS5611_ReadPROM( void );
static void MS5611_ConvADC( u8 ADC_ConvMode );
static void MS5611_ReadADC( u8 ADC_Sel );
static void MS5611_Calculate( void );

static __IO struct MS5611_ST {
  u16 C[7];
  u32 D[3];
  s32 Temp;
  s32 Press;
} COEFF;
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MS5611_WriteCMD
**功能 : 寫資料
**輸入 : WriteData
**輸出 : None
**使用 : MS5611_WriteCMD(WriteCMD);
**====================================================================================================*/
/*====================================================================================================*/
static void MS5611_WriteCMD( u8 WriteCMD )
{
  SPI_RW(SPIx, WriteCMD);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MS5611_ReadData
**功能 : 讀資料
**輸入 : *ReadData, ReadNum
**輸出 : ReadData
**使用 : MS5611_ReadData(ReadData, ReadNum);
**====================================================================================================*/
/*====================================================================================================*/
static void MS5611_ReadData( u8 *ReadData, u8 ReadNum )
{
  *ReadData = SPI_RW(SPIx, 0xFF);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MS5611_Config
**功能 : 配置 IO
**輸入 : None
**輸出 : None
**使用 : MS5611_Config();
**====================================================================================================*/
/*====================================================================================================*/
void MS5611_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef SPI_InitStruct;

  /* SPI Clk Init ************************************************************/
  RCC_AHB1PeriphClockCmd(SPIx_CSB_GPIO_CLK | SPIx_SCK_GPIO_CLK |
                         SPIx_SDO_GPIO_CLK | SPIx_SDI_GPIO_CLK, ENABLE);
  RCC_APB1PeriphClockCmd(SPIx_CLK, ENABLE);

  /* SPI AF ******************************************************************/
  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
  GPIO_PinAFConfig(SPIx_SDO_GPIO_PORT, SPIx_SDO_SOURCE, SPIx_SDO_AF);
  GPIO_PinAFConfig(SPIx_SDI_GPIO_PORT, SPIx_SDI_SOURCE, SPIx_SDI_AF);

  /* CSB PB10  */
  GPIO_InitStruct.GPIO_Pin = SPIx_CSB_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPIx_CSB_GPIO_PORT, &GPIO_InitStruct);
  /* SCK PB13  */
  GPIO_InitStruct.GPIO_Pin = SPIx_SCK_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);
  /* SDO PB14  */
  GPIO_InitStruct.GPIO_Pin = SPIx_SDO_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPIx_SDO_GPIO_PORT, &GPIO_InitStruct);
  /* SDI PB15  */
  GPIO_InitStruct.GPIO_Pin = SPIx_SDI_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPIx_SDI_GPIO_PORT, &GPIO_InitStruct);

  IMU_CSB = 1;  // 低電位有效

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
**函數 : MS5611_Init
**功能 : 初始化 MS5611
**輸入 : COEFF
**輸出 : None
**使用 : MS5611_Init();
**====================================================================================================*/
/*====================================================================================================*/
void MS5611_Init( void )
{
  /* Reset */
  MS5611_Reset();

  /* Read PROM */
  MS5611_ReadPROM();
  Delay_1ms(1);

  /* D1, D2 Conversion */
  MS5611_ConvADC(MS5611_D1_OSR_4096);
  Delay_1ms(10);
  MS5611_ReadADC(MS5611_ADC_D1);

  MS5611_ConvADC(MS5611_D2_OSR_4096);
  Delay_1ms(10);
  MS5611_ReadADC(MS5611_ADC_D2);

  /* Calculate */
  MS5611_Calculate();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MS5611_Reset
**功能 : Rset
**輸入 : None
**輸出 : None
**使用 : MS5611_Reset();
**====================================================================================================*/
/*====================================================================================================*/
static void MS5611_Reset( void )
{
  IMU_CSB = 0;
  MS5611_WriteCMD(MS5611_RESET);
  Delay_1ms(5);
  IMU_CSB = 1;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MS5611_ReadPROM
**功能 : Read PROM
**輸入 : COEFF
**輸出 : None
**使用 : MS5611_ReadPROM();
**====================================================================================================*/
/*====================================================================================================*/
static void MS5611_ReadPROM( void )
{
  u8 i = 0;
  u8 WriteCMD = MS5611_PROM_COEFF_1;
  u8 ReadC[6][2] = {0};

  IMU_CSB = 0;
  for(i=0; i<6; i++) {
    MS5611_WriteCMD(WriteCMD+i);
    MS5611_ReadData(ReadC[i], 2);
  }
  IMU_CSB = 1;

  COEFF.C[1] = (u16)((ReadC[0][0] << 8) | ReadC[0][1]);
  COEFF.C[2] = (u16)((ReadC[1][0] << 8) | ReadC[1][1]);
  COEFF.C[3] = (u16)((ReadC[2][0] << 8) | ReadC[2][1]);
  COEFF.C[4] = (u16)((ReadC[3][0] << 8) | ReadC[3][1]);
  COEFF.C[5] = (u16)((ReadC[4][0] << 8) | ReadC[4][1]);
  COEFF.C[6] = (u16)((ReadC[5][0] << 8) | ReadC[5][1]);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MS5611_ConvADC
**功能 : Convert ADC
**輸入 : ADC_ConvMode
**輸出 : None
**使用 : MS5611_ConvADC(MS5611_D1_OSR_4096);
**====================================================================================================*/
/*====================================================================================================*/
static void MS5611_ConvADC( u8 ADC_ConvMode )
{
  u8 WriteCMD = ADC_ConvMode;

  IMU_CSB = 0;
  MS5611_WriteCMD(WriteCMD);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MS5611_ReadADC
**功能 : Read ADC
**輸入 : COEFF, ADC_Sel
**輸出 : None
**使用 : MS5611_ReadADC(ADC_Sel);
**====================================================================================================*/
/*====================================================================================================*/
static void MS5611_ReadADC( u8 ADC_Sel )
{
  u8 ReadADC[4] = {0};
  u8 WriteCMD = MS5611_ADC;

  MS5611_WriteCMD(WriteCMD);
  MS5611_ReadData(ReadADC, 4);
  IMU_CSB = 1;

  COEFF.D[ADC_Sel] = (u32)(ReadADC[1]<<16 | ReadADC[2]<<8 | ReadADC[3]);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MS5611_Calculate
**功能 : MS5611 Calculate
**輸入 : None
**輸出 : None
**使用 : MS5611_Calculate();
**====================================================================================================*/
/*====================================================================================================*/
static void MS5611_Calculate( void )
{
  s32 dT = 0, TEMP = 0;
  s64 OFF = 0, SENS = 0;
  s64 T2 = 0, OFF2 = 0, SENS2 = 0;

  dT   = (s32)(COEFF.D[2] - (s64)(COEFF.C[5]) * 256);     // dT   = D2 - C5*(2^8)
  TEMP = (s32)(2000 + ((s64)dT * COEFF.C[6]) / 8388608);  // TEMP = 2000 + dT*C6/(2^23)

  OFF  = (s64)(((s64)(COEFF.C[2]) * 65536) + ((s64)(COEFF.C[4]) * dT) / 128);   // OFF  = C2*(2^16) + (C4*dT)/(2^7)
  SENS = (s64)(((s64)(COEFF.C[1]) * 32768) + ((s64)(COEFF.C[3]) * dT) / 256);   // SENS = C1*(2^15) + (C3*dT)/(2^8)

  if(TEMP < 2000) {
    T2    = ((s64)dT * dT) / 2147483648LL;              // T2    = (dT^2)/(2^31)
    OFF2  = 5 * ((s64)(TEMP-2000) * (TEMP-2000)) / 2;   // OFF2  = 5*((TEMP – 2000)^2)/(2^1)
    SENS2 = 5 * ((s64)(TEMP-2000) * (TEMP-2000)) / 4;   // SENS2 = 5*((TEMP – 2000)^2)/(2^2)
    if(TEMP < -1500) {
      OFF2  = OFF2 + 7 * (s64)(TEMP+1500)*(TEMP+1500);          // OFF2  = OFF2 + 7*((TEMP + 1500)^2)
      SENS2 = SENS2 + 11 * ((s64)(TEMP+1500)*(TEMP+1500)) / 2;  // SENS2 = SENS2 + 7*((TEMP + 1500)^2)/(2^1)
    }
    TEMP = TEMP - T2;     // TEMP = TEMP - T2
    OFF  = OFF - OFF2;    // OFF = OFF - OFF2
    SENS = SENS - SENS2;  // SENS = SENS - SENS2
  }

  COEFF.Temp  = (s32)TEMP;
  COEFF.Press = (s32)((((s64)(COEFF.D[1]) * SENS) / 2097152 - OFF) / 32768);  // Press = (D1*SENS/(2^21) - OFF)/(2^15)
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MS5611_Read
**功能 : MS5611 Read
**輸入 : Baro, ADC_ConvMode
**輸出 : None
**使用 : MS5611_Read(Baro_Buf, MS5611_D1_OSR_4096);
**====================================================================================================*/
/*====================================================================================================*/
void MS5611_Read( s32 *Baro_Buf, u8 ADC_ConvMode )
{
  static MS5611_STATUS ReadSta = MS5611_ConvInit;

  switch(ReadSta) {
    case MS5611_ConvInit:
      MS5611_ConvADC(ADC_ConvMode);
      ReadSta = MS5611_ConvD1;
      break;
    case MS5611_ConvD1:
      MS5611_ReadADC(MS5611_ADC_D1);        // Read D1
      MS5611_ConvADC(ADC_ConvMode | 0x10);  // Start Conv D2
      ReadSta = MS5611_ConvD2;
      break;
    case MS5611_ConvD2:
      MS5611_ReadADC(MS5611_ADC_D2);        // Read D2
      MS5611_ConvADC(ADC_ConvMode);         // Start Conv D1
      MS5611_Calculate();                   // Get Temp & Press
      Baro_Buf[0] = COEFF.Temp;
      Baro_Buf[1] = COEFF.Press;
      ReadSta = MS5611_ConvD1;
      break;
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
