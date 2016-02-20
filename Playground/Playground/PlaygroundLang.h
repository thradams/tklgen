#pragma once
#include "PlaygroundModuleLex.h"
#include "Result.h"
#include "StrBuilder.h"
#include "FStream.h"


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
    int/*PlaygroundLang_Actions*/ tk);
