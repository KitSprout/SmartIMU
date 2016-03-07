/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"

#include "algorithm_matrix.h"

#include <stdlib.h>
#include <string.h>
/*====================================================================================================*/
/*====================================================================================================*/
static void MATRIX_ERROR( void )
{
//  printf("\nERROR...\n");
  while(1);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_init
**功能 : Init Matrix
**輸入 : *pMatrix, *pArray, rows, cols
**輸出 : none
**使用 : Matrix_init(matrix, array, rows, cols);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_init( Matrix_TypeDef *pMatrix, double *pArray, uint16_t rows, uint16_t cols )
{
  pMatrix->rows  = rows;
  pMatrix->cols  = cols;
  pMatrix->arr   = pArray;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_clear
**功能 : Clear Matrix
**輸入 : *pMatrix
**輸出 : none
**使用 : Matrix_clear(matrix);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_clear( Matrix_TypeDef *pMatrix )
{
  for(uint32_t i = 0; i < pMatrix->rows * pMatrix->cols; i++)
    pMatrix->arr[i] = 0;
//  memset(pMatrix->arr, 0, sizeof(double) * pMatrix->total);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_create
**功能 : Create Matrix
**輸入 : rows, cols
**輸出 : *matrix
**使用 : matrix = Matrix_create(rows, cols);
**====================================================================================================*/
/*====================================================================================================*/
Matrix_TypeDef *Matrix_create( uint16_t rows, uint16_t cols )
{
  Matrix_TypeDef *matrix;

  matrix = (Matrix_TypeDef *)malloc(sizeof(Matrix_TypeDef));
  matrix->mState = MSTATE_MALLOC;
  matrix->rows   = rows;
  matrix->cols   = cols;
  matrix->arr    = (double *)malloc(sizeof(double) * rows * cols);
  Matrix_clear(matrix);

  return matrix;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_create
**功能 : Create Matrix
**輸入 : *pArray, rows, cols
**輸出 : *matrix
**使用 : matrix = Matrix_createPtr(array, rows, cols);
**====================================================================================================*/
/*====================================================================================================*/
Matrix_TypeDef *Matrix_createPtr( double *pArray, uint16_t rows, uint16_t cols )
{
  Matrix_TypeDef *matrix;

  matrix = (Matrix_TypeDef *)malloc(sizeof(Matrix_TypeDef));
  matrix->mState = MSTATE_POINTER;
  matrix->rows   = rows;
  matrix->cols   = cols;
  matrix->arr    = pArray;

  return matrix;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_delete
**功能 : Delete Matrix
**輸入 : *matrix
**輸出 : none
**使用 : Matrix_delete(matrix);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_delete( Matrix_TypeDef *matrix )
{
  if(matrix->mState == MSTATE_MALLOC)
    free(matrix->arr);
  free(matrix);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_createDiag
**功能 : Create Diagonal Matrix
**輸入 : cols, data
**輸出 : *matrix
**使用 : matrix = Matrix_createDiag(cols, data);
**====================================================================================================*/
/*====================================================================================================*/
Matrix_TypeDef *Matrix_createDiag( uint16_t cols, double data )
{
  Matrix_TypeDef *matrix = Matrix_create(cols, cols);

  for(uint16_t i = 0; i < cols; i++)
    matrix->arr[i * matrix->cols + i] = data;

  return matrix;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_copy
**功能 : Copy Matrix
**輸入 : *pMatrixC1, *pMatrixC2
**輸出 : none
**使用 : Matrix_copy(matrixC1, matrixC2);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_copy( Matrix_TypeDef *pMatrixC1, Matrix_TypeDef *pMatrixC2 )
{
  if((pMatrixC1->rows != pMatrixC2->rows) || (pMatrixC1->cols != pMatrixC2->cols))
    MATRIX_ERROR();

  for(uint32_t i = 0; i < pMatrixC1->rows * pMatrixC1->cols; i++)
    pMatrixC1->arr[i] = pMatrixC2->arr[i];
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_copyMatrix
**功能 : Copy Matrix
**輸入 : *pMatrix
**輸出 : *matrix
**使用 : matrix = Matrix_copyMatrix(matrixCpy);
**====================================================================================================*/
/*====================================================================================================*/
Matrix_TypeDef *Matrix_copyMatrix( Matrix_TypeDef *pMatrix )
{
  Matrix_TypeDef *matrix = Matrix_create(pMatrix->rows, pMatrix->cols);

  for(uint32_t i = 0; i < pMatrix->rows * pMatrix->cols; i++)
    matrix->arr[i] = pMatrix->arr[i];

  return matrix;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_copyArr
**功能 : Copy Array to Matrix
**輸入 : *pArray, rows, cols
**輸出 : *matrix
**使用 : matrix = Matrix_copyArray(array, row, col);
**====================================================================================================*/
/*====================================================================================================*/
Matrix_TypeDef *Matrix_copyArray( double *pArray, uint16_t rows, uint16_t cols )
{
  Matrix_TypeDef *matrix = Matrix_create(rows, cols);

  for(uint32_t i = 0; i < matrix->rows * matrix->cols; i++)
    matrix->arr[i] = pArray[i];

  return matrix;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_resize
**功能 : Resize Matrix
**輸入 : *pMatrix, rows, cols
**輸出 : none
**使用 : Matrix_resize(matrix, row, col);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_resize( Matrix_TypeDef *pMatrix, uint16_t rows, uint16_t cols )
{
/*
  Matrix_TypeDef *matrix = Matrix_create(rows, cols);

  for(uint32_t i = 0; i < matrix->total; i++)
    matrix->arr[i] = array[i];

  return matrix;
*/
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_setData
**功能 : Set Data
**輸入 : *pMatrix, rows, cols, data
**輸出 : None
**使用 : Matrix_setData(matrix, rows, cols, data);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_setData( Matrix_TypeDef *pMatrix, uint16_t rows, uint16_t cols, double data )
{
  if((rows < pMatrix->rows) && (cols < pMatrix->cols))
    MATRIX_ERROR();

  pMatrix->arr[rows * pMatrix->cols + cols] = data;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_getData
**功能 : Get Data
**輸入 : *pMatrix, rows, cols
**輸出 : data
**使用 : data = Matrix_getData(matrix, rows, cols);
**====================================================================================================*/
/*====================================================================================================*/
double Matrix_getData( Matrix_TypeDef *pMatrix, uint16_t rows, uint16_t cols )
{
  if((rows < pMatrix->rows) && (cols < pMatrix->cols))
    MATRIX_ERROR();

  return pMatrix->arr[rows * pMatrix->cols + cols];
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_setDiag
**功能 : Set Diagonal Matrix
**輸入 : *pMatrix, data
**輸出 : none
**使用 : Matrix_setDiag(matrix, data);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_setDiag( Matrix_TypeDef *pMatrix, double data )
{
  if(pMatrix->rows != pMatrix->cols)
    MATRIX_ERROR();

  for(uint16_t i = 0; i < pMatrix->cols; i++)
    pMatrix->arr[i * pMatrix->cols + i] = data;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_getDiag
**功能 : Get Diagonal
**輸入 : *pMatrix, pMatrixD
**輸出 : none
**使用 : Matrix_getDiag(matrix, matrixD);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_getDiag( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixD )
{
  if((pMatrix->rows != pMatrix->cols) || (pMatrixD->rows != pMatrixD->cols) || (pMatrix->rows != pMatrixD->cols))
    MATRIX_ERROR();

  if(pMatrix != pMatrixD) {
    Matrix_clear(pMatrix);
    for(uint16_t i = 0; i < pMatrix->rows; i++) {
      pMatrix->arr[i * pMatrix->cols + i] = pMatrixD->arr[i * pMatrixD->rows + i];
    }
  }
  else {
    Matrix_TypeDef *matrix = Matrix_copyMatrix(pMatrixD);
    for(uint16_t i = 0; i < pMatrix->rows; i++)
      pMatrix->arr[i * pMatrix->cols + i] = matrix->arr[i * matrix->rows + i];
    Matrix_delete(matrix);
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_setMatrix
**功能 : Set Matrix
**輸入 : *pMatrix, *pMatrixS, rows_pos, cols_pos
**輸出 : none
**使用 : Matrix_setMatrix(matrix, matrixS, rows_pos, cols_pos);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_setMatrix( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixS, uint16_t rows_pos, uint16_t cols_pos )
{
  if(((rows_pos + pMatrixS->rows) > pMatrix->rows) || ((cols_pos + pMatrixS->cols) > pMatrix->cols))
    MATRIX_ERROR();

  uint32_t cnt = 0;
  for(uint16_t i = rows_pos; i < rows_pos + pMatrixS->rows; i++) {
    for(uint16_t j = cols_pos; j < cols_pos + pMatrixS->cols; j++) {
      pMatrix->arr[i * pMatrix->cols + j] = pMatrixS->arr[cnt++];
    }
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_getMatrix
**功能 : Get Matrix
**輸入 : *pMatrixG, *pMatrix, rows_pos, cols_pos
**輸出 : none
**使用 : Matrix_getMatrix(matrixG, matrix, rows_pos, cols_pos);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_getMatrix( Matrix_TypeDef *pMatrixG, Matrix_TypeDef *pMatrix, uint16_t rows_pos, uint16_t cols_pos )
{
  if(((rows_pos + pMatrixG->rows) > pMatrix->rows) || ((cols_pos + pMatrixG->cols) > pMatrix->cols))
    MATRIX_ERROR();

  uint32_t cnt = 0;
  for(uint16_t i = rows_pos; i < rows_pos + pMatrixG->rows; i++) {
    for(uint16_t j = cols_pos; j < cols_pos + pMatrixG->cols; j++) {
      pMatrixG->arr[cnt++] = pMatrix->arr[i * pMatrix->cols + j];
    }
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_add
**功能 : Matrix Addition
**輸入 : *pMatrix, *pMatrixA1, pMatrixA2
**輸出 : none
**使用 : Matrix_add(matrix, matrixA1, matrixA2);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_add( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixA1, Matrix_TypeDef *pMatrixA2 )
{
  if((pMatrix->rows != pMatrixA1->rows) || (pMatrix->cols != pMatrixA1->cols) ||
     (pMatrix->rows != pMatrixA2->rows) || (pMatrix->cols != pMatrixA2->cols))
    MATRIX_ERROR();

  for(uint16_t i = 0; i < pMatrix->rows; i++)
    for(uint16_t j = 0; j < pMatrix->cols; j++)
      pMatrix->arr[i * pMatrix->cols + j] = pMatrixA1->arr[i * pMatrixA1->cols + j] + pMatrixA2->arr[i * pMatrixA2->cols + j];
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_sub
**功能 : Matrix Subtraction
**輸入 : *pMatrix, *pMatrixS1, *pMatrixS2
**輸出 : none
**使用 : Matrix_sub(matrix, matrixS1, matrixS2);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_sub( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixS1, Matrix_TypeDef *pMatrixS2 )
{
  if((pMatrix->rows != pMatrixS1->rows) || (pMatrix->cols != pMatrixS1->cols) ||
     (pMatrix->rows != pMatrixS2->rows) || (pMatrix->cols != pMatrixS2->cols))
    MATRIX_ERROR();

  for(uint16_t i = 0; i < pMatrix->rows; i++)
    for(uint16_t j = 0; j < pMatrix->cols; j++)
      pMatrix->arr[i * pMatrix->cols + j] = pMatrixS1->arr[i * pMatrixS1->cols + j] - pMatrixS2->arr[i * pMatrixS2->cols + j];
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_mul
**功能 : Matrix Multiplication
**輸入 : *pMatrix, *pMatrixM1, *pMatrixM2
**輸出 : none
**使用 : Matrix_mul(matrix, matrixM1, matrixM2);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_mul( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixM1, Matrix_TypeDef *pMatrixM2 )
{
  if((pMatrixM1->cols != pMatrixM2->rows) || (pMatrix->rows != pMatrixM1->rows) || (pMatrix->cols != pMatrixM2->cols))
    MATRIX_ERROR();

  if(pMatrix == pMatrixM1) {
    Matrix_TypeDef *matrix = Matrix_copyMatrix(pMatrixM1);
    Matrix_clear(pMatrix);
    for(uint16_t i = 0; i < matrix->rows; i++) {
      for(uint16_t j = 0; j < pMatrixM2->cols; j++) {
        for(uint16_t k = 0; k < matrix->cols; k++) {
          pMatrix->arr[i * pMatrix->cols + j] += matrix->arr[i * matrix->cols + k] * pMatrixM2->arr[k * pMatrixM2->cols + j];
        }
      }
    }
    Matrix_delete(matrix);
  }
  else if(pMatrix == pMatrixM2) {
    Matrix_TypeDef *matrix = Matrix_copyMatrix(pMatrixM2);
    Matrix_clear(pMatrix);
    for(uint16_t i = 0; i < pMatrixM1->rows; i++) {
      for(uint16_t j = 0; j < matrix->cols; j++) {
        for(uint16_t k = 0; k < pMatrixM1->cols; k++) {
          pMatrix->arr[i * pMatrix->cols + j] += pMatrixM1->arr[i * pMatrixM1->cols + k] * matrix->arr[k * matrix->cols + j];
        }
      }
    }
    Matrix_delete(matrix);
  }
  else {
    Matrix_clear(pMatrix);
    for(uint16_t i = 0; i < pMatrixM1->rows; i++) {
      for(uint16_t j = 0; j < pMatrixM2->cols; j++) {
        for(uint16_t k = 0; k < pMatrixM1->cols; k++) {
          pMatrix->arr[i * pMatrix->cols + j] += pMatrixM1->arr[i * pMatrixM1->cols + k] * pMatrixM2->arr[k * pMatrixM2->cols + j];
        }
      }
    }
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_mulNumb
**功能 : Matrix Multiplication
**輸入 : *pMatrix, *pMatrixM1, number
**輸出 : none
**使用 : Matrix_mulNumb(matrix, matrixM1, number);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_mulNumb( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixM1, double number )
{
  if((pMatrix->cols != pMatrixM1->cols) || (pMatrix->rows != pMatrixM1->rows))
    MATRIX_ERROR();

  for(uint32_t i = 0; i < pMatrix->rows * pMatrix->cols; i++) {
    pMatrix->arr[i] = pMatrixM1->arr[i] * number;
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_transpose
**功能 : Matrix Transpose
**輸入 : *pMatrix, *pMatrixT
**輸出 : none
**使用 : Matrix_transpose(matrix, matrixT);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_transpose( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixT )
{
  if(pMatrix != pMatrixT) {
    pMatrix->rows = pMatrixT->cols;
    pMatrix->cols = pMatrixT->rows;
    for(uint16_t i = 0; i < pMatrixT->rows; i++)
      for(uint16_t j = 0; j < pMatrixT->cols; j++)
        pMatrix->arr[j * pMatrix->cols + i] = pMatrixT->arr[i * pMatrixT->cols + j];
  }
  else {
    Matrix_TypeDef *matrix = Matrix_copyMatrix(pMatrixT);
    pMatrix->rows = matrix->cols;
    pMatrix->cols = matrix->rows;
    for(uint16_t i = 0; i < matrix->rows; i++)
      for(uint16_t j = 0; j < matrix->cols; j++)
        pMatrix->arr[j * pMatrix->cols + i] = matrix->arr[i * matrix->cols + j];
    Matrix_delete(matrix);
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_inv
**功能 : Inverse Matrix ( Gaussian Elimination )
**輸入 : *pMatrix, *pMatrixInv
**輸出 : none
**使用 : matrix = Matrix_inv(matrix, matrixInv);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_inv( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixInv )
{
  if((pMatrix->rows != pMatrix->cols) || (pMatrix->rows != pMatrixInv->rows))
    MATRIX_ERROR();

  /* check nonsingular */
  // if matrix is nonsingular
  // return ERROR;

  Matrix_TypeDef *matrix     = Matrix_create(pMatrixInv->rows, pMatrixInv->rows << 1);
  Matrix_TypeDef *matrixDiag = Matrix_createDiag(pMatrixInv->rows, 1.0);
  Matrix_setMatrix(matrix, pMatrixInv, 0, 0);
  Matrix_setMatrix(matrix, matrixDiag, 0, pMatrixInv->cols);
  Matrix_delete(matrixDiag);

  Matrix_gaussianElimination(matrix, matrix);

  Matrix_getMatrix(pMatrix, matrix, 0, pMatrixInv->cols);
  Matrix_delete(matrix);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_gaussianElimination
**功能 : Gaussian Elimination
**輸入 : *pMatrix, *pMatrixGE
**輸出 : none
**使用 : Matrix_gaussianElimination(matrix, matrixGE);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_gaussianElimination( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixGE )
{
  double tmp;

  if(pMatrix != pMatrixGE)
    Matrix_copy(pMatrix, pMatrixGE);

  /* left-down to zero */
  for(uint16_t i = 0; i < pMatrix->rows - 1; i++) {
    for(uint16_t j = i + 1; j < pMatrix->rows; j++) {
      tmp = pMatrix->arr[j * pMatrix->cols + i] / pMatrix->arr[i * pMatrix->cols + i];
      pMatrix->arr[j * pMatrix->cols + i] = 0;
      for(uint16_t k = i + 1; k < pMatrix->cols; k++)
        pMatrix->arr[j * pMatrix->cols + k] -= tmp * pMatrix->arr[i * pMatrix->cols + k];
    }
  }

  /* diagonal to one */
  for(uint16_t  i = 0; i < pMatrix->rows; i++) {
    tmp = pMatrix->arr[i * pMatrix->cols + i];
    pMatrix->arr[i * pMatrix->cols + i] = 1.0;
    for(uint16_t  j = i + 1; j < pMatrix->cols; j++)
      pMatrix->arr[i * pMatrix->cols + j] = pMatrix->arr[i * pMatrix->cols + j] / tmp;
  }

  /* right-up to zero */
  for(int16_t i = pMatrix->rows - 1; i > 0; i--) {
    for(int16_t j = i - 1; j >= 0; j--) {
      tmp = pMatrix->arr[j * pMatrix->cols + i];
      pMatrix->arr[j * pMatrix->cols + i] = 0;
      for(uint16_t  k = pMatrix->rows; k < pMatrix->cols; k++)
        pMatrix->arr[j * pMatrix->cols + k] -= tmp * pMatrix->arr[i * pMatrix->cols + k];
    }
  }

}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_det
**功能 : Determinant
**輸入 : *pMatrix
**輸出 : data
**使用 : data = Matrix_det(matrix);
**====================================================================================================*/
/*====================================================================================================*/
double Matrix_det( Matrix_TypeDef *pMatrix )
{
  double det = 0;
/*
  if(matrix->cols != matrix->rows)
    return 0;

*/
  return det;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_cholesky
**功能 : cholesky
**輸入 : *pMatrix, pMatrixC
**輸出 : none
**使用 : Matrix_cholesky(matrix, matrixC);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_cholesky( Matrix_TypeDef *pMatrix, Matrix_TypeDef *pMatrixC )
{
/*
  if(matrixA->cols != matrixA->rows)
    return;

  double tempSum = 0;
  for(uint16_t j = 0; j < matrix->cols; j++) {
    for(uint16_t i = j; i < matrix->rows; i++) {
      tempSum = 0;
      if(i == j) {
        for(uint16_t k = 0; k < i - 1; k++)
          tempSum = tempSum + sqrt_matrix(i, k)^2;
        matrix->arr[i * matrix->cols + j] = sqrt(pdMatrix[i * matrix->cols + j] - tempSum);
      }
      else {
        for(uint16_t k = 0; k < j - 1; k++)
          tempSum = tempSum + sqrt_matrix(i, k) * sqrt_matrix(j, k);
        sqrt_matrix(i, j) = (pdMatrix(i, j) - tempSum) / sqrt_matrix(j, j);
      }
    }
  }
*/
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Matrix_print
**功能 : Print Matrix
**輸入 : *pMatrix
**輸出 : none
**使用 : Matrix_print(matrix);
**====================================================================================================*/
/*====================================================================================================*/
void Matrix_print( Matrix_TypeDef *pMatrix )
{
  for(uint16_t i = 0; i < pMatrix->rows; i++) {
    printf(" ");
    for(uint16_t j = 0; j < pMatrix->cols; j++) {
      if(pMatrix->arr[i * pMatrix->cols + j] < 0)
        printf("%.5f\t", pMatrix->arr[i * pMatrix->cols + j]);
      else
        printf("+%.5f\t", pMatrix->arr[i * pMatrix->cols + j]);
    }
    printf("\r\n");
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
