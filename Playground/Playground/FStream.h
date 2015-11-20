#pragma once
#include "Result.h"
#include "StringEx.h"

typedef struct
{
  FILE * pFile;
  StringC fileName;
} FStream;


void FStream_Destroy(FStream* pfStream);

Result FStream_Init(FStream* pfStream);

Result FStream_InitOpen(FStream* pfStream,
                        const char* fileName);

Result FStream_Open(FStream* pfStream,
                    const char* fileName);

Result FStream_Get(FStream* pfStream, wchar_t* ch);

Result FStream_Unget(FStream* pfStream, wchar_t ch);


typedef struct
{
  const char* pCharacteres;
  const char* pCurrentChar;
  wchar_t     putBackCharacter;
  StringC     name;
} SStream;


void SStream_Destroy(SStream* pfStream);


Result SStream_Init(SStream* pfStream,
                    const char* name,
                    const char* psz);


Result SStream_Get(SStream* pfStream, wchar_t* ch);

Result SStream_Unget(SStream* pfStream, wchar_t ch);



typedef struct
{
  int type;
  union
  {
    SStream sstream;
    FStream fstream;
  } streamObj;
} Stream;


void Stream_Destroy(Stream* pfStream);


Result Stream_InitOpen(Stream* pfStream,
                    const char* fileName);

Result Stream_InitStr(Stream* pfStream,
                      const char* name,
                       const char* psz);


const char* Stream_GetName(Stream* pfStream);

Result Stream_Get(Stream* pfStream, wchar_t* ch);

Result Stream_Unget(Stream* pfStream, wchar_t ch);

FStream* Stream_CastFStream(Stream* pfStream);
