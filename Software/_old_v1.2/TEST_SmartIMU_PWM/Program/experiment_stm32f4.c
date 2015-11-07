/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "experiment_stm32f4.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define PWM_MOTOR_MIN 800
#define PWM_MOTOR_MED 1400
#define PWM_MOTOR_MAX 2000  // 32~80%

#define PWM1  TIM2->CCR1
#define PWM2  TIM2->CCR2
#define PWM3  TIM4->CCR4
#define PWM4  TIM4->CCR3
/*====================================================================================================*/
/*====================================================================================================*/
int main( void )
{
  u8 State = 0;
  u32 i = PWM_MOTOR_MIN;

  SystemInit();
  GPIO_Config();
  PWM_Config();
  
  PWM1 = PWM_MOTOR_MIN;

  while(1) {
    LED = !LED;

    PWM2 = i;
    PWM3 = i;
    PWM4 = i;

    i = (State) ? (i-1) : (i+1);
    if(i==PWM_MOTOR_MAX)  State = 1;
    if(i==PWM_MOTOR_MIN)  State = 0;

    Delay_10ms(1);
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
void PWM_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_OCInitTypeDef TIM_OCInitStruct;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM4, ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);

  /* TIM2 PWM1 PA0 */ /* TIM2 PWM2 PA1 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* TIM4 PWM3 PB9 */ /* TIM4 PWM4 PB8 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  TIM_DeInit(TIM2);
  TIM_DeInit(TIM4);

  /************************** PWM Output **************************************/
  /* TIM2 TIM4 Time Base */
  TIM_TimeBaseStruct.TIM_Period = (u16)(2500-1);              // Period = 2.5ms => Freq = 400Hz
  TIM_TimeBaseStruct.TIM_Prescaler = (u16)(84-1);             // fCK_PSC = APB1*2 = 42*2 = 84, fCK_PSC /84 = 1M ( 1us )
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;    // Count Up
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct);

  /* TIM2 TIM4 OC */
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;              // Set PWM1 Mode
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;  // Enable OC
  TIM_OCInitStruct.TIM_Pulse = PWM_MOTOR_MAX;                 // Set Pulse
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;      // when Count < PWM_MOTOR_MIN => High
  TIM_OC1Init(TIM2, &TIM_OCInitStruct);                       // Init TIM2 OC1
  TIM_OC2Init(TIM2, &TIM_OCInitStruct);                       // Init TIM2 OC2
  TIM_OC3Init(TIM4, &TIM_OCInitStruct);                       // Init TIM4 OC3
  TIM_OC4Init(TIM4, &TIM_OCInitStruct);                       // Init TIM4 OC4
  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);           // Enable TIM2 OC1 Preload
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);           // Enable TIM2 OC2 Preload
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);           // Enable TIM4 OC3 Preload
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);           // Enable TIM4 OC4 Preload

  /* Enable */
  TIM_ARRPreloadConfig(TIM2, ENABLE);                         // Enable TIM2 ARR Preload
  TIM_ARRPreloadConfig(TIM4, ENABLE);                         // Enable TIM4 ARR Preload
  TIM_Cmd(TIM2, ENABLE);                                      // Start TIM2
  TIM_Cmd(TIM4, ENABLE);                                      // Start TIM4

  TIM2->CCR1 = PWM_MOTOR_MIN;
  TIM2->CCR2 = PWM_MOTOR_MIN;
  TIM4->CCR1 = PWM_MOTOR_MIN;
  TIM4->CCR2 = PWM_MOTOR_MIN;
}
/*====================================================================================================*/
/*====================================================================================================*/
