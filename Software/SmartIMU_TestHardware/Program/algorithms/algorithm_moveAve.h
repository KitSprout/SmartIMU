/* #include "algorithm_moveAve.h" */

#ifndef __ALGORITHM_MOVEAVE_H
#define __ALGORITHM_MOVEAVE_H

#include "stm32f4xx_hal.h"
/*====================================================================================================*/
/*====================================================================================================*/
int16_t MoveAve_SMA( int16_t newData, int16_t *moveAveFIFO, uint16_t fifoLens, uint16_t windowLens );
int16_t MoveAve_WMA( int16_t newData, int16_t *moveAveFIFO, int16_t *weighted, uint16_t fifoLens, uint16_t windowLens );
/*====================================================================================================*/
/*====================================================================================================*/
#endif	 
