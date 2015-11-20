
#pragma once
#include "Result.h"


typedef char * StringC;
#define STRINGC_INIT NULL

Result String_InitWithSize(StringC* pString, size_t sz);
Result String_Init(StringC* pString, const char* source);
void String_Destroy(StringC* pString);
void String_Reset(StringC* pString);
void String_Attach(StringC* pString, char* psz);
void String_Swap(StringC* pA, StringC* pB);

Result String_Change(StringC *pS1,
                     const char* psz1);

Result String_ChangeTwo(StringC *pS1,                        
                        const char* psz1,
                        StringC *pS2,
                        const char* psz2);

Result IntToStr(int v, char* dest, size_t destlen);
bool IsStrEmpty(const char* psz);


typedef char StringS;


#define StringS_Init(s) ((s)[0] = '\0')
#define StringS_Destroy(s) 
#define StringS_Change(dest, source) StringCopy(dest, SIZE_OF_ARRAY((dest)), source)

#define StringS_Size(s) SIZE_OF_ARRAY((s))

Result StringCopy(char* dest,
                  size_t destsize,
                  const char* source);

