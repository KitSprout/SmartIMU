/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "algorithm_string.h"
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Str_NumToChar
**功能 : 數值轉字串
**輸入 : Type, NumLen, *CharData, NumData
**輸出 : None
**使用 : Str_NumToChar(Type_O, 6, TrData, Data)
**====================================================================================================*/
/*====================================================================================================*/
void Str_NumToChar( u8 Type, u8 NumLen, u8 *CharData, s32 NumData )
{
  u8 i = 0;
  u32 Tmp[48] = {0};
  u32 NumTmp = 1;

  switch(Type) {

    case Type_B:
    case Type_O:
    case Type_D:
    case Type_H:
      for(i=0; i<NumLen; i++) {
        Tmp[i] = ((u32)NumData) / NumTmp;
        NumTmp = NumTmp * Type;
      }
      for(i=0; i<NumLen; i++) {
        CharData[NumLen-i-1] = Tmp[i] - Tmp[i+1]*Type;
        if(CharData[NumLen-i-1]>9)
          CharData[NumLen-i-1] += 55;	// 65-10
        else
          CharData[NumLen-i-1] += 48;
      }
      CharData[NumLen] = '\0';
    break;

    case Type_I:
      if(NumData<0) {
        CharData[0] = '-';
        NumData = (~NumData) + 1;
        for(i=1; i<NumLen+1; i++) {
          Tmp[i-1] = ((u32)NumData) / NumTmp;
          NumTmp = NumTmp * 10;
        }
        for(i=1; i<NumLen+1; i++) {
          CharData[NumLen-i+1] = Tmp[i-1] - Tmp[i]*10;
          CharData[NumLen-i+1] += 48;
        }
        CharData[NumLen+1] = '\0';
      }
      else {
        CharData[0] = '+';
        //NumData = (~NumData) + 1;
        for(i=1; i<NumLen+1; i++) {
          Tmp[i-1] = ((u32)NumData) / NumTmp;
          NumTmp = NumTmp*10;
        }
        for(i=1; i<NumLen+1; i++) {
          CharData[NumLen-i+1] = Tmp[i-1] - Tmp[i]*10;
          CharData[NumLen-i+1] += 48;
        }
        CharData[NumLen+1] = '\0';
      }
      break;

    case Type_F:
      break;
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Str_LenOfStr
**功能 : 數值轉字串
**輸入 : *inputStr
**輸出 : StrLen
**使用 : StrLen = Str_LenOfStr((u8*)"Hello World!!");
**====================================================================================================*/
/*====================================================================================================*/
u16 Str_LenOfStr( u8* inputStr )
{
  u16 StrLen = 0;

  if(inputStr == NULL)
    return StrLen;

  while(*(inputStr++))
    StrLen++;

  return StrLen;
}
/*====================================================================================================*/
/*====================================================================================================*/
