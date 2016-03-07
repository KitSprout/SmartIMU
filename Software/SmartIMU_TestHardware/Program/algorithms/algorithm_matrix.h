/* #include "algorithm_matrix.h" */

#ifndef __ALGORITHM_MATRIX_H
#define __ALGORITHM_MATRIX_H

#include "stm32f4xx_hal.h"
/*====================================================================================================*/
/*====================================================================================================*/
typedef enum {
  MSTATE_MALLOC = 0,
  MSTATE_POINTER,
  MSTATE_NONE,
} Matrix_mState_TypeDef;

typedef struct {
  uint16_t rows;
  uint16_t cols;
  double   *arr;
  Matrix_mState_TypeDef mState;
} Matrix_TypeDef;
/*====================================================================================================*/
/*====================================================================================================*/
void            Matrix_init( Matrix_TypeDef *pMatrix, double *pArray, uint16_t rows, uint16_t cols );
void            Matrix_clear( Matrix_TypeDef *pMatrix );
Matrix_TypeDef *Matrix_create( uint16_t rows, uint16_t cols );
Matrix_TypeDef *Matrix_createPtr( double *pArray, uint16_t rows, uint16_t cols );
void            Matrix_delete( Matrix_TypeDef *matrix );
Matrix_TypeDef *Matrix_createDiag( uint16_t cols, double data );
void            Matrix_copy( Matrix_TypeDef *pMatrixC1, Matrix_TypeDef *pMatrixC2 );
Matrix_TypeDef *Matrix_copyMatrix( Matrix_TypeDef *pMatrix );
Matrix_TypeDef *Matrix_copyArray( double *pArray, uint16_t rows, uint16_t cols );
void            Matrix_resize( Matrix_TypeDef *pMatrix, uint16_t rows, uint16_t cols );
void            Matrix_setData( Matrix_TypeDef *pMatrix, uint16_t rows, uint16_t cols, double data );
double          Matrix_getData( Matrix_TypeDef *pMatrix, uint16_t rows, uint16_t cols );
void            Matrix_setDiag( Matrix_TypeDef *pMatrix, double data );
void            Matrix_getDiag( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixD );
void            Matrix_setMatrix( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixS, uint16_t rows_pos, uint16_t cols_pos );
void            Matrix_getMatrix( Matrix_TypeDef *pMatrixG, Matrix_TypeDef *pMatrix, uint16_t rows_pos, uint16_t cols_pos );
void            Matrix_add( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixA1, Matrix_TypeDef *pMatrixA2 );
void            Matrix_sub( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixS1, Matrix_TypeDef *pMatrixS2 );
void            Matrix_mul( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixM1, Matrix_TypeDef *pMatrixM2 );
void            Matrix_mulNumb( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixM1, double number );
void            Matrix_transpose( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixT );
void            Matrix_inv( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixInv );
void            Matrix_gaussianElimination( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixGE );
double          Matrix_det( Matrix_TypeDef *pMatrix );
void            Matrix_cholesky( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixC );
void            Matrix_print( Matrix_TypeDef *pMatrix );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
