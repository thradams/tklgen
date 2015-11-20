#include "PlaygroundLang.h"
#include "PlaygroundModuleParser.h"
#include "assert.h"
#include <stdio.h>

Result PlaygroundLang_NextTokenNoInterleave(Stream* stream,
                                            StrBuilder* strBuilder,
                                            PlaygroundLang_Tokens* tk)
{
  int lastGoodState = -2;
  int currentState = 0;
  wchar_t ch;
  int index = 0;

  Result r = Stream_Get(stream, &ch);

  while (r == RESULT_OK)
  {
    currentState = PlaygroundLang_GetNext(currentState, ch);

    if (lastGoodState == -2 &&
        currentState == -1)
    {
      ASSERT(0);
      return RESULT_FAIL;
    }

    if (currentState == -1)
    {
      Stream_Unget(stream, ch);      
      break;
    }

    PlaygroundLang_Tokens tk2;

    if (PlaygroundLang_GetTokenFromState(currentState, &tk2))
    {
      *tk = tk2;
      lastGoodState = currentState;
    }

    StrBuilder_AppendWChar(strBuilder, ch);    
    r = Stream_Get(stream, &ch);
  }

  if (r == RESULT_EOF)
  {
    if (lastGoodState != -2)
    {
      r = RESULT_OK;
    }
  }
  return r;
}


Result PlaygroundLang_NextToken(Stream* stream,
                                StrBuilder* lexeme,
                                PlaygroundLang_Tokens* tk)
{
  Result r = RESULT_FAIL;

  for (;;)
  {
    r = PlaygroundLang_NextTokenNoInterleave(stream, lexeme, tk);

    if (r == RESULT_OK)
    {
      if (!PlaygroundLang_IsInterleave(*tk))
      {
        break;
      }
    }
    else
    {
      break;
    }
  }

  return r;
}


Result PlaygroundLang_Context_Init(PlaygroundLang_Context* p,
                                   const char* filename)
{
  Result result;
  do
  {
    result = StrBuilder_Init(&p->lexeme, 100);
    if (result == RESULT_OK)
    {
      result = Stream_InitOpen(&p->stream, filename);
      if (result == RESULT_OK)
      {
        result = PlaygroundLang_NextToken(&p->stream,
                                            &p->lexeme,
                                            &p->token);
        if (result == RESULT_OK)
          break;

        Stream_Destroy(&p->stream);
      }

      StrBuilder_Destroy(&p->lexeme);
    }
  } while (0);
  
  return result;
}

void PlaygroundLang_Context_Destroy(PlaygroundLang_Context* p)
{
  StrBuilder_Destroy(&p->lexeme);
  Stream_Destroy(&p->stream);
}

Result PlaygroundLang_Match(PlaygroundLang_Context* context,
                            PlaygroundLang_Tokens tk)
{
  if (tk != context->token)
  {
    return RESULT_FAIL;
  }

  StrBuilder_Clear(&context->lexeme);
  Result r = PlaygroundLang_NextToken(&context->stream,
                                      &context->lexeme,
                                      &context->token);

  return r;
}

Result PlaygroundLang_OnAction(PlaygroundLang_Context* ctx,
                               PlaygroundLang_Actions a)
{
  printf("%s %s %s\n",
         PlaygroundLang_Actions_Text(a),
         PlaygroundLang_Tokens_ToString(ctx->token),
         ctx->lexeme.c_str);


  return RESULT_OK;
}




