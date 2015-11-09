/*
 * Generated by TKLGEN - VersionOct 27 2014
 * Copyright (C) 2013, Thiago Adams (thiago.adams@gmail.com)
 * www.thradams.com
 *
 */

#include "stdafx.h"
#include <assert.h>

#include "ModuleSample2Lex.h"
#include "ModuleSample2Parser.h"



typedef enum LanguageSample2_Tokens Token;

#define IFER(x) if (!(x)) return 0;

/*Context*/

int Context_Init(struct Context* p)
{
    p->lexeme[0] = 0;
    p->stream = 0;
    p->token = 0;
    return 1;
}

void OnError(struct Context* context, ErrorCode er)
{
}

static int ReadNextToken(struct Context* context)
{
    int r = LanguageSample2_NextToken(context->stream,
        context->lexeme,
        200,
        &context->token);
    if (r == 0)
    {
        OnError(context, ErrorCodeUnexpectedToken);
    }
    return r;
}

int Context_AttachStream(struct Context* p, struct sstream* ss)
{
    p->stream = ss;
    return ReadNextToken(p);
}

const wchar_t* Context_GetLexeme(struct Context* p)
{
    return p->lexeme;
}

enum LanguageSample2_Tokens Context_GetToken(struct Context* p)
{
    return p->token;
}

static int Match(struct Context* context, Token tk)
{
    if (tk != Context_GetToken(context))
    {
        OnError(context, ErrorCodeUnexpectedToken);
        return 0;
    }
    return ReadNextToken(context);
}

/*forward declarations*/ 
int Parse_Main(struct Context*);
int Parse_C(struct Context*);
int Parse_D(struct Context*);
int Parse_C2(struct Context*);
int Parse_E(struct Context*);
int Parse_D2(struct Context*);
int Parse_T(struct Context*);
int Parse_E2(struct Context*);
int Parse_F(struct Context*);
int Parse_T2(struct Context*);
int Parse_P(struct Context*);
int Parse_FOpt(struct Context*);


int Parse_Main(struct Context* ctx)
{
    Token token = Context_GetToken(ctx); 

    if (token == TKMinus ||
        token == TKOpen ||
        token == TKInteger ||
        token == TKIdentifier)
    {
        /*Main => C EndMark */
            IFER(Parse_C(ctx));
            IFER(Match(ctx, TKEndMark));
    }
    else
    {
        OnError(ctx, ErrorCodeUnexpectedToken);
        return 0;
    }

    return 1;
}

int Parse_C(struct Context* ctx)
{
    Token token = Context_GetToken(ctx); 

    if (token == TKMinus ||
        token == TKOpen ||
        token == TKInteger ||
        token == TKIdentifier)
    {
        /*C => D C2 */
            IFER(Parse_D(ctx));
            IFER(Parse_C2(ctx));
    }
    else
    {
        OnError(ctx, ErrorCodeUnexpectedToken);
        return 0;
    }

    return 1;
}

int Parse_D(struct Context* ctx)
{
    Token token = Context_GetToken(ctx); 

    if (token == TKMinus ||
        token == TKOpen ||
        token == TKInteger ||
        token == TKIdentifier)
    {
        /*D => E D2 */
            IFER(Parse_E(ctx));
            IFER(Parse_D2(ctx));
    }
    else
    {
        OnError(ctx, ErrorCodeUnexpectedToken);
        return 0;
    }

    return 1;
}

int Parse_C2(struct Context* ctx)
{
    Token token = Context_GetToken(ctx); 

    if (token == TKAnd)
    {
        /*C2 => And D C2 */
            IFER(Match(ctx, TKAnd));
            IFER(Parse_D(ctx));
            IFER(Action_PrintAnd(ctx));
            IFER(Parse_C2(ctx));
    }
    else if (token == TKOr)
    {
        /*C2 => Or D C2 */
            IFER(Match(ctx, TKOr));
            IFER(Parse_D(ctx));
            IFER(Action_PrintOr(ctx));
            IFER(Parse_C2(ctx));
    }
    else if (token == TKEndMark)
    {
        /*C2 => Epsilon */
    }
    else
    {
        OnError(ctx, ErrorCodeUnexpectedToken);
        return 0;
    }

    return 1;
}

int Parse_E(struct Context* ctx)
{
    Token token = Context_GetToken(ctx); 

    if (token == TKMinus ||
        token == TKOpen ||
        token == TKInteger ||
        token == TKIdentifier)
    {
        /*E => T E2 */
            IFER(Parse_T(ctx));
            IFER(Parse_E2(ctx));
    }
    else
    {
        OnError(ctx, ErrorCodeUnexpectedToken);
        return 0;
    }

    return 1;
}

int Parse_D2(struct Context* ctx)
{
    Token token = Context_GetToken(ctx); 

    if (token == TKGreaterThan)
    {
        /*D2 => GreaterThan E D2 */
            IFER(Match(ctx, TKGreaterThan));
            IFER(Parse_E(ctx));
            IFER(Action_PrintGreaterThan(ctx));
            IFER(Parse_D2(ctx));
    }
    else if (token == TKLessThan)
    {
        /*D2 => LessThan E D2 */
            IFER(Match(ctx, TKLessThan));
            IFER(Parse_E(ctx));
            IFER(Action_PrintLessThan(ctx));
            IFER(Parse_D2(ctx));
    }
    else if (token == TKLessEqualThan)
    {
        /*D2 => LessEqualThan E D2 */
            IFER(Match(ctx, TKLessEqualThan));
            IFER(Parse_E(ctx));
            IFER(Action_PrintLessEqualThan(ctx));
            IFER(Parse_D2(ctx));
    }
    else if (token == TKGreaterEqualThan)
    {
        /*D2 => GreaterEqualThan E D2 */
            IFER(Match(ctx, TKGreaterEqualThan));
            IFER(Parse_E(ctx));
            IFER(Action_PrintGreaterEqualThan(ctx));
            IFER(Parse_D2(ctx));
    }
    else if (token == TKEqual)
    {
        /*D2 => Equal E D2 */
            IFER(Match(ctx, TKEqual));
            IFER(Parse_E(ctx));
            IFER(Action_PrintEquals(ctx));
            IFER(Parse_D2(ctx));
    }
    else if (token == TKEndMark ||
        token == TKAnd ||
        token == TKOr)
    {
        /*D2 => Epsilon */
    }
    else
    {
        OnError(ctx, ErrorCodeUnexpectedToken);
        return 0;
    }

    return 1;
}

int Parse_T(struct Context* ctx)
{
    Token token = Context_GetToken(ctx); 

    if (token == TKMinus ||
        token == TKOpen ||
        token == TKInteger ||
        token == TKIdentifier)
    {
        /*T => F T2 */
            IFER(Parse_F(ctx));
            IFER(Parse_T2(ctx));
    }
    else
    {
        OnError(ctx, ErrorCodeUnexpectedToken);
        return 0;
    }

    return 1;
}

int Parse_E2(struct Context* ctx)
{
    Token token = Context_GetToken(ctx); 

    if (token == TKPlus)
    {
        /*E2 => Plus T E2 */
            IFER(Match(ctx, TKPlus));
            IFER(Parse_T(ctx));
            IFER(Action_PrintPlus(ctx));
            IFER(Parse_E2(ctx));
    }
    else if (token == TKMinus)
    {
        /*E2 => Minus T E2 */
            IFER(Match(ctx, TKMinus));
            IFER(Parse_T(ctx));
            IFER(Action_PrintMinus(ctx));
            IFER(Parse_E2(ctx));
    }
    else if (token == TKClose ||
        token == TKLessThan ||
        token == TKEndMark ||
        token == TKEqual ||
        token == TKGreaterThan ||
        token == TKAnd ||
        token == TKOr ||
        token == TKGreaterEqualThan ||
        token == TKLessEqualThan)
    {
        /*E2 => Epsilon */
    }
    else
    {
        OnError(ctx, ErrorCodeUnexpectedToken);
        return 0;
    }

    return 1;
}

int Parse_F(struct Context* ctx)
{
    Token token = Context_GetToken(ctx); 

    if (token == TKMinus ||
        token == TKOpen ||
        token == TKInteger ||
        token == TKIdentifier)
    {
        /*F => P FOpt */
            IFER(Parse_P(ctx));
            IFER(Parse_FOpt(ctx));
    }
    else
    {
        OnError(ctx, ErrorCodeUnexpectedToken);
        return 0;
    }

    return 1;
}

int Parse_T2(struct Context* ctx)
{
    Token token = Context_GetToken(ctx); 

    if (token == TKMulti)
    {
        /*T2 => Multi F T2 */
            IFER(Match(ctx, TKMulti));
            IFER(Parse_F(ctx));
            IFER(Action_PrintMulti(ctx));
            IFER(Parse_T2(ctx));
    }
    else if (token == TKDiv)
    {
        /*T2 => Div F T2 */
            IFER(Match(ctx, TKDiv));
            IFER(Parse_F(ctx));
            IFER(Action_PrintDiv(ctx));
            IFER(Parse_T2(ctx));
    }
    else if (token == TKPlus ||
        token == TKMulti ||
        token == TKMinus ||
        token == TKDiv ||
        token == TKPower ||
        token == TKClose ||
        token == TKLessThan ||
        token == TKEndMark ||
        token == TKEqual ||
        token == TKGreaterThan ||
        token == TKAnd ||
        token == TKOr ||
        token == TKGreaterEqualThan ||
        token == TKLessEqualThan)
    {
        /*T2 => Epsilon */
    }
    else
    {
        OnError(ctx, ErrorCodeUnexpectedToken);
        return 0;
    }

    return 1;
}

int Parse_P(struct Context* ctx)
{
    Token token = Context_GetToken(ctx); 

    if (token == TKInteger)
    {
        /*P => Integer */
            Action_PrintLexeme(ctx);
            IFER(Match(ctx, TKInteger));
    }
    else if (token == TKIdentifier)
    {
        /*P => Identifier */
            Action_PrintLexeme(ctx);
            IFER(Match(ctx, TKIdentifier));
    }
    else if (token == TKOpen)
    {
        /*P => Open E Close */
            IFER(Match(ctx, TKOpen));
            IFER(Parse_E(ctx));
            IFER(Match(ctx, TKClose));
    }
    else if (token == TKMinus)
    {
        /*P => Minus T */
            IFER(Match(ctx, TKMinus));
            IFER(Parse_T(ctx));
            IFER(Action_PrintNeg(ctx));
    }
    else
    {
        OnError(ctx, ErrorCodeUnexpectedToken);
        return 0;
    }

    return 1;
}

int Parse_FOpt(struct Context* ctx)
{
    Token token = Context_GetToken(ctx); 

    if (token == TKPower)
    {
        /*FOpt => Power F */
            IFER(Match(ctx, TKPower));
            IFER(Parse_F(ctx));
            IFER(Action_PrintPower(ctx));
    }
    else if (token == TKPlus ||
        token == TKMulti ||
        token == TKMinus ||
        token == TKDiv ||
        token == TKPower ||
        token == TKClose ||
        token == TKLessThan ||
        token == TKEndMark ||
        token == TKEqual ||
        token == TKGreaterThan ||
        token == TKAnd ||
        token == TKOr ||
        token == TKGreaterEqualThan ||
        token == TKLessEqualThan)
    {
        /*FOpt => Epsilon */
    }
    else
    {
        OnError(ctx, ErrorCodeUnexpectedToken);
        return 0;
    }

    return 1;
}

