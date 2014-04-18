/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "experiment_stm32f4.h"
/*====================================================================================================*/
/*====================================================================================================*/
int main( void )
{
  SystemInit();
  GPIO_Config();
  Other_Config();

  while(1) {
    PAO(0)  = !PAO(0);  PAO(1)  = !PAO(1);  PAO(3)  = !PAO(3);  PAO(4)  = !PAO(4);  PAO(5)  = !PAO(5);
    PAO(6)  = !PAO(6);  PAO(7)  = !PAO(7);  PAO(8)  = !PAO(8);  PAO(15) = !PAO(15);

//    PAO(9)  = !PAO(9);  PAO(10) = !PAO(10); PAO(11) = !PAO(11); PAO(12) = !PAO(12);

    PBO(3)  = !PBO(3);  PBO(4)  = !PBO(4);  PBO(5)  = !PBO(5);  PBO(6)  = !PBO(6);
    PBO(7)  = !PBO(7);  PBO(8)  = !PBO(8);  PBO(9)  = !PBO(9);  PBO(10) = !PBO(10);

    LED = !LED;
    Delay_10ms(5);
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
void Other_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Clk Init *************************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_3  | GPIO_Pin_4  | GPIO_Pin_5  |
                             GPIO_Pin_6  | GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
                             GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3  | GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_6  |
                             GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  PAO(0)  = 0;  PAO(1)  = 0;  PAO(3)  = 0;  PAO(4)  = 0;  PAO(5)  = 0;
  PAO(6)  = 0;  PAO(7)  = 0;  PAO(8)  = 0;  PAO(15) = 0;

//  PAO(9)  = 0;  PAO(10) = 0;  PAO(11) = 0;  PAO(12) = 0;

  PBO(3)  = 0;  PBO(4)  = 0;  PBO(5)  = 0;  PBO(6)  = 0;
  PBO(7)  = 0;  PBO(8)  = 0;  PBO(9)  = 0;  PBO(10) = 0;
}
/*====================================================================================================*/
/*====================================================================================================*/
