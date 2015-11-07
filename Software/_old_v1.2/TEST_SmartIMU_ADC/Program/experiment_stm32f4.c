/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_adc.h"
#include "experiment_stm32f4.h"
#include "module_rs232.h"
#include "algorithm_string.h"
/*====================================================================================================*/
/*====================================================================================================*/
int main( void )
{
  u16 ADC_AVE = {0};

  SystemInit();
  GPIO_Config();
  ADC_Config();
  RS232_Config();

  while(1) {
    LED = !LED;
    Delay_100ms(1);
    ADC_Average(&ADC_AVE, 128);
    RS232_SendStr((u8*)"\f");
    RS232_SendStr((u8*)"ADC = ");
    RS232_SendNum(Type_D, 4, ADC_AVE);
    RS232_SendStr((u8*)"\r\n");
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
void GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* LED Clk Init *************************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  /* LED PC13 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  LED = 0;
}
/*====================================================================================================*/
/*====================================================================================================*/
