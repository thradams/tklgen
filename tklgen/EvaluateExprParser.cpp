/*
 * Generated by TKLGEN - VersionOct 27 2014
 * Copyright (C) 2013, Thiago Adams (thiago.adams@gmail.com)
 * www.thradams.com
 *
 */

#include "stdafx.h"
#include <cassert>
#include <string>
#include "tokenizer.h"
#include "EvaluateExprLex.h"
#include "EvaluateExprParser.h"


namespace EvaluateExpr
{
    //forward declarations 
    void Parse_Main(Context&);
    void Parse_A(Context&);
    void Parse_P(Context&);
    void Parse_A2(Context&);


    void Parse_Main(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKIdentifier ||
            token == TKNumber)
        {
            //Main => A 
            Parse_A(ctx);
        }
        else
        {
            OnError(ctx, ErrorCodeUnexpectedToken);
        }
    }

    void Parse_A(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKIdentifier ||
            token == TKNumber)
        {
            //A => P A2 
            Parse_P(ctx);
            Parse_A2(ctx);
        }
        else
        {
            OnError(ctx, ErrorCodeUnexpectedToken);
        }
    }

    void Parse_P(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKNumber)
        {
            //P => Number 
            Match(ctx, TKNumber);
        }
        else if (token == TKIdentifier)
        {
            //P => Identifier 
            Match(ctx, TKIdentifier);
        }
        else
        {
            OnError(ctx, ErrorCodeUnexpectedToken);
        }
    }

    void Parse_A2(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKGreater)
        {
            //A2 => Greater P A2 
            Match(ctx, TKGreater);
            Parse_P(ctx);
            Parse_A2(ctx);
        }
        else if (token == TKLess)
        {
            //A2 => Less P A2 
            Match(ctx, TKLess);
            Parse_P(ctx);
            Parse_A2(ctx);
        }
        else if (token == TKLessEqual)
        {
            //A2 => LessEqual P A2 
            Match(ctx, TKLessEqual);
            Parse_P(ctx);
            Parse_A2(ctx);
        }
        else if (token == TKGreaterEqual)
        {
            //A2 => GreaterEqual P A2 
            Match(ctx, TKGreaterEqual);
            Parse_P(ctx);
            Parse_A2(ctx);
        }
        else if (token == TKEqual)
        {
            //A2 => Equal P A2 
            Match(ctx, TKEqual);
            Parse_P(ctx);
            Parse_A2(ctx);
        }
        else if (token == TKEndMark)
        {
            //A2 => Epsilon 
        }
        else
        {
            OnError(ctx, ErrorCodeUnexpectedToken);
        }
    }

} //namespace EvaluateExpr
