/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    mathUnit.h
 *  @author  KitSprout
 *  @date    21-Apr-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __MATHUNIT_H
#define __MATHUNIT_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#if defined (NRF51)
  #include "core_cm0.h"
#else
  #include "core_cm4.h"
#endif
#include "arm_math.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
#define invSqrt( __iSq )    (1.0 / sqrt(__iSq))
#define invSqrtf( __iSq )   (1.0f / sqrtf(__iSq))
#define squa( __sq )        ((__sq) * (__sq))
#define squaf( __sq )       (((float32_t)(__sq)) * ((float32_t)(__sq)))
#define swap( __a, __b )    (__a) = (int32_t)(__a) ^ (int32_t)(__b); \
                            (__b) = (int32_t)(__a) ^ (int32_t)(__b); \
                            (__a) = (int32_t)(__a) ^ (int32_t)(__b)
#define swapf( __a, __b )   *(uint32_t*)(&(__a)) = *(uint32_t*)(&(__a)) ^ *(uint32_t*)(&(__b)); \
                            *(uint32_t*)(&(__b)) = *(uint32_t*)(&(__a)) ^ *(uint32_t*)(&(__b)); \
                            *(uint32_t*)(&(__a)) = *(uint32_t*)(&(__a)) ^ *(uint32_t*)(&(__b))
#define abs( __n )          (((__n) > 0) ? (__n) : -(__n))
#define toRad( __deg )      ((__deg) * 0.0174532925f)
#define toDeg( __rad )      ((__rad) * 57.2957795f)
#define arrayLens(__arr)    (sizeof(__arr) / sizeof((__arr)[0]))

/* Typedef ---------------------------------------------------------------------------------*/
typedef enum {
  KS_NTYPE = 0x00,  /* 8'b 0000 0000 */
  KS_I8    = 0x11,  /* 8'b 0001 0001 */
  KS_I16   = 0x12,  /* 8'b 0001 0010 */
  KS_I32   = 0x14,  /* 8'b 0001 0100 */
  KS_I64   = 0x18,  /* 8'b 0001 1000 */
  KS_U8    = 0x21,  /* 8'b 0010 0001 */
  KS_U16   = 0x22,  /* 8'b 0010 0010 */
  KS_U32   = 0x24,  /* 8'b 0010 0100 */
  KS_U64   = 0x28,  /* 8'b 0010 1000 */
  KS_F32   = 0x44,  /* 8'b 0100 0100 */
  KS_F64   = 0x48,  /* 8'b 0100 1000 */
} KSUnit_t;

typedef struct {
  float32_t x;
  float32_t y;
  float32_t z;
} __attribute__((aligned(4))) vector_t;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
float32_t invSqrtf_fast( const float32_t __iSq );
uint32_t  compareArray( KSUnit_t type, const void *pArray1, const void *pArray2, uint32_t lens );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
