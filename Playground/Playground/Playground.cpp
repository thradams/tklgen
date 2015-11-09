// Playground.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PlaygroundModuleLex.h"
#include "PlaygroundModuleParser.h"
#include <stdio.h>
#include "sstream.h"
#include "filetutils.h"
#include <stdlib.h>
#include "errors.h"
int OnAction(enum PlaygroundLang_Actions a,
             PlaygroundLang_Context* ctx)
{
    wprintf(L"%s %s %s\n",
        PlaygroundLang_Actions_Text(a),
        PlaygroundLang_Tokens_ToString(ctx->token),
        ctx->lexeme);
    return ResultCodeSuccess;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("input text required");
        return 1;
    }
    wchar_t* text;
    file_to_string(argv[1], &text);

    struct sstream ss;
    sstream_init(&ss, text);
    PlaygroundLang_Parse(&ss, OnAction, NULL);
    free(text);
	return 0;
}

