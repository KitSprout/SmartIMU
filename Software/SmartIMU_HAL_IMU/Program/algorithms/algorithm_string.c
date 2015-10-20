/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"

#include "algorithm_string.h"
/*====================================================================================================*/
/*====================================================================================================*
**函數 : Str_NumToChar
**功能 : Number to string
**輸入 : Type, NumLen, *CharData, NumData
**輸出 : None
**使用 : Str_NumToChar(Type_O, 6, TrData, Data);
**====================================================================================================*/
/*====================================================================================================*/
void Str_NumToChar( StrType Type, uint8_t NumLen, char *CharData, int32_t NumData )
{
  uint8_t i = 0;
  uint32_t TmpStr[48] = {0};
  uint32_t TmpNum = 1;

  switch(Type) {

    case Type_B:
    case Type_O:
    case Type_D:
    case Type_H:
      for(i = 0; i < NumLen; i++) {
        TmpStr[i] = ((uint32_t)NumData) / TmpNum;
        TmpNum = TmpNum * Type;
      }
      for(i = 0; i < NumLen; i++) {
        CharData[NumLen-i-1] = TmpStr[i] - TmpStr[i+1] * Type;
        if(CharData[NumLen-i-1] > 9)
          CharData[NumLen-i-1] += 55;	// 65-10
        else
          CharData[NumLen-i-1] += 48;
      }
      CharData[NumLen] = '\0';
    break;

    case Type_I:
      if(NumData < 0) {
        CharData[0] = '-';
        NumData = (~NumData) + 1;
      }
      else {
        CharData[0] = '+';
      }
      for(i = 1; i < NumLen + 1; i++) {
        TmpStr[i-1] = ((uint32_t)NumData) / TmpNum;
        TmpNum = TmpNum * 10;
      }
      for(i = 1; i < NumLen + 1; i++) {
        CharData[NumLen-i+1] = TmpStr[i-1] - TmpStr[i] * 10;
        CharData[NumLen-i+1] += 48;
      }
      CharData[NumLen+1] = '\0';
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
**使用 : StrLen = Str_LenOfStr("Hello World!!");
**====================================================================================================*/
/*====================================================================================================*/
uint16_t Str_LenOfStr( char *inputStr )
{
  uint16_t StrLen = 0;

  if(inputStr == NULL)
    return StrLen;

  while(*(inputStr++))
    StrLen++;

  return StrLen;
}
/*====================================================================================================*/
/*====================================================================================================*/
