/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"

#include "algorithm_moveAve.h"
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MoveAve_SMA
**功能 : Simple Moving Average
**輸入 : newData, moveAveFIFO, fifoLens, windowLens
**輸出 : aveData
**使用 : MoveAve_SMA(newData, moveAveFIFO, fifoLens, windowLens);
**====================================================================================================*/
/*====================================================================================================*/
int16_t MoveAve_SMA( int16_t newData, int16_t *moveAveFIFO, uint16_t fifoLens, uint16_t windowLens )
{
  uint16_t p_front = moveAveFIFO[0];
  uint16_t p_rear  = ((p_front - windowLens + fifoLens) % fifoLens) + 1;

  int16_t aveData = 0;
  int32_t sumData = 0;

  moveAveFIFO[p_front] = newData;
  if(p_front > p_rear) {
    for(uint16_t i = p_front; i > p_rear - 1; i--)
      sumData = sumData + moveAveFIFO[i];
  }
  else {
    for(uint16_t i = p_front; i > 0; i--)
      sumData = sumData + moveAveFIFO[i];
    for(uint16_t i = fifoLens; i > p_rear - 1; i--)
      sumData = sumData + moveAveFIFO[i];
  }
  aveData = sumData / windowLens;
  moveAveFIFO[0] = (p_front % fifoLens) + 1;

  return aveData;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MoveAve_WMA
**功能 : Weighted Moving Average
**輸入 : newData, moveAveFIFO, weighted, fifoLens, windowLens
**輸出 : aveData
**使用 : MoveAve_WMA(newData, moveAveFIFO, weighted, fifoLens, windowLens);
**====================================================================================================*/
/*====================================================================================================*/
int16_t MoveAve_WMA( int16_t newData, int16_t *moveAveFIFO, int16_t *weighted, uint16_t fifoLens, uint16_t windowLens )
{
  uint8_t j = 0;
  uint16_t p_front = moveAveFIFO[0];
  uint16_t p_rear  = ((p_front - windowLens + fifoLens) % fifoLens) + 1;

  int16_t aveData = 0;
  int32_t sumData = 0;

  moveAveFIFO[p_front] = newData;
  if(p_front > p_rear) {
    for(uint16_t i = p_front; i > p_rear - 1; i--)
      sumData = sumData + moveAveFIFO[i] * weighted[++j];
  }
  else {
    for(uint16_t i = p_front; i > 0; i--)
      sumData = sumData + moveAveFIFO[i] * weighted[++j];
    for(uint16_t i = fifoLens; i > p_rear - 1; i--)
      sumData = sumData + moveAveFIFO[i] * weighted[++j];
  }
  aveData = sumData / weighted[j];
  moveAveFIFO[0] = (p_front % fifoLens) + 1;

  return aveData;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MoveAve_EMA
**功能 : Exponential Moving Average
**輸入 : NewData, MoveAve_FIFO, SampleLen
**輸出 : AveData
**使用 : MoveAve_EMA(NewData, MoveAve_FIFO, SampleLen);
**====================================================================================================*/
/*====================================================================================================*/

/*====================================================================================================*/
/*====================================================================================================*/
