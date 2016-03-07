/* #include "algorithm_newtonMethod.h" */

#ifndef __ALGORITHM_NEWTONMETHOD_H
#define __ALGORITHM_NEWTONMETHOD_H

#include "stm32f4xx_hal.h"
#include "algorithm_matrix.h"
/*====================================================================================================*/
/*====================================================================================================*/
void newtonMethod( Matrix_TypeDef *pMatrixX, Matrix_TypeDef *pMatrixA, const double alpha, double refData );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
