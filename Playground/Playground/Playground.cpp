// Playground.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PlaygroundModuleLex.h"


#include "PlaygroundModuleParser.h"
#include <stdio.h>
#include <stdlib.h>





int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("input text required");
        return 1;
    }
    
    PlaygroundLang_Context context;
    Result result = PlaygroundLang_Context_Init(&context,
                                                argv[1]);
    if (result == RESULT_OK)
    {
      PlaygroundLang_Parse(&context);
      
      PlaygroundLang_Context_Destroy(&context);
    }

	return 0;
}

