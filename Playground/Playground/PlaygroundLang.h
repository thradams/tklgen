#pragma once
#include "PlaygroundModuleLex.h"
#include "Result.h"
#include "StrBuilder.h"
#include "FStream.h"

typedef enum
{
PrintNewLine,
PrintPlus,
PrintMinus,
PrintMulti,
PrintDiv,
PrintPower,
PrintLexeme,
PrintNeg,
OnError
} PlaygroundLang_Actions;


typedef struct 
{
  Stream stream;
  PlaygroundLang_Tokens token;
  StrBuilder lexeme;
} PlaygroundLang_Context;


Result PlaygroundLang_Context_Init(PlaygroundLang_Context* p,
                                   const char* filename);

void PlaygroundLang_Context_Destroy(PlaygroundLang_Context* p);

Result PlaygroundLang_Match(PlaygroundLang_Context* ctx,
                            PlaygroundLang_Tokens tk);

Result PlaygroundLang_OnAction(PlaygroundLang_Context* ctx,
                               PlaygroundLang_Actions tk);
