
#include "stdafx.h"


#include "StringEx.h"
#include "dmalloc.h"
#include <stdlib.h>
#include <string.h>
#include <stdlib.h> //malloc
#include "assert.h"


void String_Reset(StringC* pString)
{
  String_Destroy(pString);
  String_Init(pString, NULL);
}

void String_Destroy(StringC* pString)
{
  Free(*pString);
  *pString = NULL;
}

void String_Attach(StringC* pString, char* psz)
{
  Free(*pString);
  *pString = psz;
}

void String_Swap(StringC* pA, StringC* pB)
{
  StringC temp = *pA;
  *pA = *pB;
  *pB = temp;
}

Result String_Change(StringC *pS1,
                     const char* psz1)
{
  Result result;

  StringC s1;
  result = String_Init(&s1, psz1);
  if (result == RESULT_OK)
  {    
      String_Swap(&s1, pS1);
      String_Destroy(&s1);
  }
  return result;
}

Result String_ChangeTwo(StringC *pS1,
                        const char* psz1,
                        StringC *pS2,                        
                        const char* psz2)
{
  Result result;

  StringC s1;
  result = String_Init(&s1, psz1);
  if (result == RESULT_OK)
  {
    StringC s2;
    result = String_Init(&s2, psz2);
    if (result == RESULT_OK)
    {
      String_Swap(&s1, pS1);
      String_Swap(&s2, pS2);
      String_Destroy(&s2);
    }
    String_Destroy(&s1);
  }

  return result;
}

Result String_InitWithSize(StringC* pString, size_t sz)
{
  Result result = RESULT_OK;
  if (sz > 0)
  {
    char*snew = (char*)Malloc(sizeof(char) * sz);
    if (snew != NULL)
    {
      snew[0] = '\0';
      *pString = (char*)snew;
    }
    else
    {
      result = RESULT_OUT_OF_MEM;
    }    
  }
  else
  {
    *pString = NULL;    
  }
  return result;
}

Result String_Init(StringC* pString, const char* sourceOpt)
{
  if (sourceOpt != NULL)
  {
    size_t len = strlen(sourceOpt) + 1;
    void *snew = Malloc(sizeof(char) * len);

    if (snew == NULL)
    {
      return RESULT_OUT_OF_MEM;
    }

    memcpy(snew, sourceOpt, len);

    *pString = (char*)snew;
  }
  else
  {
    *pString = NULL;
  }

  return RESULT_OK;
}


bool IsStrEmpty(const char* psz)
{
  return psz == NULL || psz[0] == 0;
}



Result IntToStr(int v, char* dest, size_t destlen)
{
#ifndef ARDUINO

  if (_itoa_s(v, dest, destlen, 10) != 0)
  {
    return RESULT_FAIL;
  }

#else
  itoa(v, dest, 10);
#endif
  return RESULT_OK;
}

Result StrNew(char** dest, size_t size)
{
  ASSERT(size > 0);
  ASSERT(*dest == NULL);
  char* temp = (char*)Malloc(sizeof(char) * size);

  if (temp == NULL)
  {
    return RESULT_OUT_OF_MEM;
  }

  *dest = temp;
  return RESULT_OK;
}



Result StringCopy(char* dest,
                  size_t destsize,
                  const char* source)
{
  ASSERT(dest != NULL);

  if (source == NULL)
  {
    *dest = '\0';
    return RESULT_OK;
  }

  for (size_t i = 0; i < destsize - 1; i++)
  {
    if (*source == '\0')
      break;

    *dest = *source;
    source++;
    dest++;
  }

  *dest = '\0';

  if (*source != 0)
  {
    //nao coube
    return RESULT_FAIL;
  }

  return RESULT_OK;
}
