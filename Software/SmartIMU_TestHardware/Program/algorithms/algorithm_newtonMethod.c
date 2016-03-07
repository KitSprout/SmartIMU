/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"

#include "algorithm_newtonMethod.h"

#define row_X 9
#define col_X 1
/*====================================================================================================*/
/*====================================================================================================*/
void newtonMethod_G( const Matrix_TypeDef *matrixX, const double *pAccel, const uint16_t n, Matrix_TypeDef *matrixG, double refData );
/*====================================================================================================*/
/*====================================================================================================*
**函數 : newtonMethod
**功能 : gauss-newton method
**輸入 : *pMatrixX, *pMatrixA, alpha, n
**輸出 : none
**使用 : newtonMethod(matrixX, matrixA, alpha, refdata);
**====================================================================================================*/
/*====================================================================================================*/
void newtonMethod( Matrix_TypeDef *pMatrixX, Matrix_TypeDef *pMatrixA, const double alpha, double refData )
{
  Matrix_TypeDef *matrixG = Matrix_create(pMatrixX->rows, pMatrixX->cols);

//  uint8_t count = 0;
//  double tmpMatrixX[9] = {0};
//  double oldMatrixX[9] = {0};

  // gauss-newton method
  for(uint8_t i = 0; i < 100; i++) {
//  while(1) {
//    count = 0;
//    for(uint16_t j = 0; j < pMatrixX->rows; j++)
//      oldMatrixX[j] = pMatrixX->arr[j];

    newtonMethod_G(pMatrixX, pMatrixA->arr, pMatrixA->cols, matrixG, refData);
    Matrix_mulNumb(matrixG, matrixG, alpha);
    Matrix_sub(pMatrixX, pMatrixX, matrixG);

//    for(uint16_t j = 0; j < pMatrixX->rows; j++) {
//      tmpMatrixX[j] = (pMatrixX->arr[j] - oldMatrixX[j]) / ((pMatrixX->arr[j] + oldMatrixX[j]));
//      tmpMatrixX[j] = (tmpMatrixX[j] > 0.0) ? tmpMatrixX[j] : -tmpMatrixX[j];
//      if(tmpMatrixX[j] < 0.00000001)
//        count++;
//    }
//    if(count == pMatrixX->rows)
//      break;
  }

  Matrix_delete(matrixG);
}
/*====================================================================================================*/
/*====================================================================================================*/
void newtonMethod_e( const double *arrayX, const double *pAccel, double *pErr, double refData )
{
  double accel[3] = {0};
  double param[3] = {0};

  for(uint8_t i = 0; i < 3; i++)
    accel[i] = pAccel[i] - arrayX[6 + i];

  param[0] = arrayX[0] * accel[0] + arrayX[1] * accel[1] + arrayX[2] * accel[2];
  param[1] = arrayX[1] * accel[0] + arrayX[3] * accel[1] + arrayX[4] * accel[2];
  param[2] = arrayX[2] * accel[0] + arrayX[4] * accel[1] + arrayX[5] * accel[2];

  *pErr = param[0] * param[0] + param[1] * param[1] + param[2] * param[2] - refData * refData;
}
/*====================================================================================================*/
/*====================================================================================================*/
void newtonMethod_Ge( const double *arrayX, const double *pAccel, double *pGErr )
{
  double param_A = arrayX[0] * arrayX[0] + arrayX[1] * arrayX[1] + arrayX[2] * arrayX[2];
  double param_B = arrayX[1] * arrayX[1] + arrayX[3] * arrayX[3] + arrayX[4] * arrayX[4];
  double param_C = arrayX[2] * arrayX[2] + arrayX[4] * arrayX[4] + arrayX[5] * arrayX[5];
  double param_D = arrayX[0] * arrayX[1] + arrayX[1] * arrayX[3] + arrayX[2] * arrayX[4];
  double param_E = arrayX[1] * arrayX[2] + arrayX[3] * arrayX[4] + arrayX[4] * arrayX[5];
  double param_F = arrayX[0] * arrayX[2] + arrayX[1] * arrayX[4] + arrayX[2] * arrayX[5];

  double accel[3] = {0};
  for(uint8_t i = 0; i < 3; i++)
    accel[i] = pAccel[i] - arrayX[6 + i];

  double param_a1a1 = accel[0] * accel[0];
  double param_a1a2 = accel[0] * accel[1];
  double param_a1a3 = accel[0] * accel[2];
  double param_a2a2 = accel[1] * accel[1];
  double param_a2a3 = accel[1] * accel[2];
  double param_a3a3 = accel[2] * accel[2];

  pGErr[0] = arrayX[0] * param_a1a1 + arrayX[1] * param_a1a2 + arrayX[2] * param_a1a3;
  pGErr[1] = arrayX[1] * (param_a1a1 + param_a2a2) + (arrayX[0] + arrayX[3]) * param_a1a2 + arrayX[2] * param_a2a3 + arrayX[4] * param_a1a3;
  pGErr[2] = arrayX[2] * (param_a1a1 + param_a3a3) + arrayX[4] * param_a1a2 + arrayX[1] * param_a2a3 + (arrayX[0] + arrayX[5]) * param_a1a3;
  pGErr[3] = arrayX[1] * param_a1a2 + arrayX[3] * param_a2a2 + arrayX[4] * param_a2a3;
  pGErr[4] = arrayX[4] * (param_a2a2 + param_a3a3) + arrayX[2] * param_a1a2 + (arrayX[3] + arrayX[5]) * param_a2a3 + arrayX[1] * param_a1a3;
  pGErr[5] = arrayX[2] * param_a1a3 + arrayX[4] * param_a2a3 + arrayX[5] * param_a3a3;
  pGErr[6] = - param_A * accel[0] - param_D * accel[1] - param_F * accel[2];
  pGErr[7] = - param_D * accel[0] - param_B * accel[1] - param_E * accel[2];
  pGErr[8] = - param_F * accel[0] - param_E * accel[1] - param_C * accel[2];
}
/*====================================================================================================*/
/*====================================================================================================*/
void newtonMethod_G( const Matrix_TypeDef *matrixX, const double *pAccel, const uint16_t n, Matrix_TypeDef *matrixG, double refData )
{
  double accel[3] = {0};
  double pErr = 0;
  double pGErr[row_X * col_X] = {0};
  double arrayGE[row_X * col_X] = {0};
  double arrayG2E[row_X * row_X] = {0};

  for(uint16_t i = 0; i < n; i++) {
    accel[0] = pAccel[0 * n + i];
    accel[1] = pAccel[1 * n + i];
    accel[2] = pAccel[2 * n + i];
    newtonMethod_e(matrixX->arr, accel, &pErr, refData);
    newtonMethod_Ge(matrixX->arr, accel, pGErr);

    // G2E = sum(Ge^T * Ge)
    for(uint16_t j = 0; j < row_X; j++) {
      arrayGE[j] += pErr * pGErr[j];
      for(uint16_t k = 0; k < row_X; k++)
        arrayG2E[k * row_X + j] += pGErr[j] * pGErr[k];
    }
  }

  Matrix_TypeDef matrixGE  = { row_X, col_X, arrayGE,  MSTATE_NONE };
  Matrix_TypeDef matrixG2E = { row_X, row_X, arrayG2E, MSTATE_NONE };

  Matrix_inv(&matrixG2E, &matrixG2E);
  Matrix_mul(matrixG, &matrixG2E, &matrixGE);
}
/*====================================================================================================*/
/*====================================================================================================*/
