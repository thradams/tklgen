//////////////////////////////////////////////////////////////////////////////
// Generated by TKLGEN - Version Mar  8 2013
// Copyright (C) 2013, Thiago Adams (thiago.adams@gmail.com)
// www.thradams.com
//
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include "tokenizer.h"
#include "Sample1DFA.h"
#include "Sample1Parser.h"


namespace Sample1
{
    //forward declarations 
    void Parse_Main(Context&);
    void Parse_ModuleBody(Context&);
    void Parse_LanguageBody(Context&);
    void Parse_Declarations(Context&);
    void Parse_Declaration(Context&);
    void Parse_TokenDeclaration(Context&);
    void Parse_SyntaxDeclaration(Context&);
    void Parse_InterleaveDeclaration(Context&);
    void Parse_TokenExpression(Context&);
    void Parse_SyntaxExpression(Context&);
    void Parse_T(Context&);
    void Parse_ZeroOrMoreT(Context&);
    void Parse_F(Context&);
    void Parse_OrF(Context&);
    void Parse_ZeroOrMoreOrF(Context&);
    void Parse_P(Context&);
    void Parse_OptOperator(Context&);
    void Parse_V(Context&);
    void Parse_RangeOpt(Context&);
    void Parse_SyntaxExpressionMoreSingle(Context&);
    void Parse_SyntaxExpressionSingle(Context&);
    void Parse_SyntaxExpressionSingleFollow(Context&);
    void Parse_SyntaxExpressionItem(Context&);
    void Parse_SyntaxExpressionAction(Context&);


    void Parse_Main(Context& ctx)
    {
        if (ctx.Token() == TKKeywordModule)
        {
            //Main => ModuleBody 
            Parse_ModuleBody(ctx);
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_ModuleBody(Context& ctx)
    {
        if (ctx.Token() == TKKeywordModule)
        {
            //ModuleBody => KeywordModule Identifier LeftCurlyBracketCharacter LanguageBody RightCurlyBracketCharacter 
            ctx.Match(TKKeywordModule);
            ctx.Match(TKIdentifier);
            ctx.Action_ModuleName();
            ctx.Match(TKLeftCurlyBracketCharacter);
            Parse_LanguageBody(ctx);
            ctx.Match(TKRightCurlyBracketCharacter);
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_LanguageBody(Context& ctx)
    {
        if (ctx.Token() == TKKeywordLanguage)
        {
            //LanguageBody => KeywordLanguage Identifier LeftCurlyBracketCharacter Declarations RightCurlyBracketCharacter 
            ctx.Match(TKKeywordLanguage);
            ctx.Match(TKIdentifier);
            ctx.Action_LanguageName();
            ctx.Match(TKLeftCurlyBracketCharacter);
            Parse_Declarations(ctx);
            ctx.Match(TKRightCurlyBracketCharacter);
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_Declarations(Context& ctx)
    {
        if (ctx.Token() == TKKeywordInterleave ||
            ctx.Token() == TKKeywordToken ||
            ctx.Token() == TKKeywordSyntax)
        {
            //Declarations => Declaration Declarations 
            Parse_Declaration(ctx);
            Parse_Declarations(ctx);
        }
        else if (ctx.Token() == TKRightCurlyBracketCharacter)
        {
            //Declarations => Epsilon 
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_Declaration(Context& ctx)
    {
        if (ctx.Token() == TKKeywordToken)
        {
            //Declaration => TokenDeclaration 
            Parse_TokenDeclaration(ctx);
        }
        else if (ctx.Token() == TKKeywordSyntax)
        {
            //Declaration => SyntaxDeclaration 
            Parse_SyntaxDeclaration(ctx);
        }
        else if (ctx.Token() == TKKeywordInterleave)
        {
            //Declaration => InterleaveDeclaration 
            Parse_InterleaveDeclaration(ctx);
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_TokenDeclaration(Context& ctx)
    {
        if (ctx.Token() == TKKeywordToken)
        {
            //TokenDeclaration => KeywordToken Identifier EqualsCharacter TokenExpression SemicolonCharacter 
            ctx.Action_teste();
            ctx.Match(TKKeywordToken);
            ctx.Match(TKIdentifier);
            ctx.Action_TokenName();
            ctx.Match(TKEqualsCharacter);
            Parse_TokenExpression(ctx);
            ctx.Match(TKSemicolonCharacter);
            ctx.Action_TokenEnd();
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_SyntaxDeclaration(Context& ctx)
    {
        if (ctx.Token() == TKKeywordSyntax)
        {
            //SyntaxDeclaration => KeywordSyntax Identifier EqualsCharacter SyntaxExpression SemicolonCharacter 
            ctx.Match(TKKeywordSyntax);
            ctx.Match(TKIdentifier);
            ctx.Action_SyntaxName();
            ctx.Match(TKEqualsCharacter);
            Parse_SyntaxExpression(ctx);
            ctx.Match(TKSemicolonCharacter);
            ctx.Action_TokenEnd();
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_InterleaveDeclaration(Context& ctx)
    {
        if (ctx.Token() == TKKeywordInterleave)
        {
            //InterleaveDeclaration => KeywordInterleave Identifier EqualsCharacter TokenExpression SemicolonCharacter 
            ctx.Match(TKKeywordInterleave);
            ctx.Match(TKIdentifier);
            ctx.Match(TKEqualsCharacter);
            Parse_TokenExpression(ctx);
            ctx.Match(TKSemicolonCharacter);
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_TokenExpression(Context& ctx)
    {
        if (ctx.Token() == TKKeywordAny ||
            ctx.Token() == TKLiteralChar ||
            ctx.Token() == TKIdentifier ||
            ctx.Token() == TKLeftParenthesisCharacter ||
            ctx.Token() == TKStringLiteral)
        {
            //TokenExpression => T ZeroOrMoreT 
            Parse_T(ctx);
            Parse_ZeroOrMoreT(ctx);
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_SyntaxExpression(Context& ctx)
    {
        if (ctx.Token() == TKKeywordEmpty ||
            ctx.Token() == TKLiteralChar ||
            ctx.Token() == TKIdentifier ||
            ctx.Token() == TKLeftCurlyBracketCharacter ||
            ctx.Token() == TKStringLiteral)
        {
            //SyntaxExpression => SyntaxExpressionSingle SyntaxExpressionMoreSingle 
            Parse_SyntaxExpressionSingle(ctx);
            Parse_SyntaxExpressionMoreSingle(ctx);
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_T(Context& ctx)
    {
        if (ctx.Token() == TKKeywordAny ||
            ctx.Token() == TKLiteralChar ||
            ctx.Token() == TKIdentifier ||
            ctx.Token() == TKLeftParenthesisCharacter ||
            ctx.Token() == TKStringLiteral)
        {
            //T => F ZeroOrMoreOrF 
            Parse_F(ctx);
            Parse_ZeroOrMoreOrF(ctx);
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_ZeroOrMoreT(Context& ctx)
    {
        if (ctx.Token() == TKKeywordAny ||
            ctx.Token() == TKLiteralChar ||
            ctx.Token() == TKIdentifier ||
            ctx.Token() == TKLeftParenthesisCharacter ||
            ctx.Token() == TKStringLiteral)
        {
            //ZeroOrMoreT => T ZeroOrMoreT 
            Parse_T(ctx);
            ctx.Action_PrintAnd();
            Parse_ZeroOrMoreT(ctx);
        }
        else if (ctx.Token() == TKSemicolonCharacter ||
            ctx.Token() == TKRightParenthesisCharacter)
        {
            //ZeroOrMoreT => Epsilon 
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_F(Context& ctx)
    {
        if (ctx.Token() == TKKeywordAny ||
            ctx.Token() == TKLiteralChar ||
            ctx.Token() == TKIdentifier ||
            ctx.Token() == TKLeftParenthesisCharacter ||
            ctx.Token() == TKStringLiteral)
        {
            //F => P OptOperator 
            Parse_P(ctx);
            Parse_OptOperator(ctx);
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_OrF(Context& ctx)
    {
        if (ctx.Token() == TKVerticalLineCharacter)
        {
            //OrF => VerticalLineCharacter F 
            ctx.Match(TKVerticalLineCharacter);
            Parse_F(ctx);
            ctx.Action_PrintOr();
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_ZeroOrMoreOrF(Context& ctx)
    {
        if (ctx.Token() == TKVerticalLineCharacter)
        {
            //ZeroOrMoreOrF => OrF ZeroOrMoreOrF 
            Parse_OrF(ctx);
            Parse_ZeroOrMoreOrF(ctx);
        }
        else if (ctx.Token() == TKKeywordAny ||
            ctx.Token() == TKLiteralChar ||
            ctx.Token() == TKIdentifier ||
            ctx.Token() == TKSemicolonCharacter ||
            ctx.Token() == TKLeftParenthesisCharacter ||
            ctx.Token() == TKRightParenthesisCharacter ||
            ctx.Token() == TKStringLiteral)
        {
            //ZeroOrMoreOrF => Epsilon 
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_P(Context& ctx)
    {
        if (ctx.Token() == TKKeywordAny ||
            ctx.Token() == TKLiteralChar ||
            ctx.Token() == TKIdentifier ||
            ctx.Token() == TKStringLiteral)
        {
            //P => V 
            Parse_V(ctx);
        }
        else if (ctx.Token() == TKLeftParenthesisCharacter)
        {
            //P => LeftParenthesisCharacter TokenExpression RightParenthesisCharacter 
            ctx.Match(TKLeftParenthesisCharacter);
            Parse_TokenExpression(ctx);
            ctx.Match(TKRightParenthesisCharacter);
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_OptOperator(Context& ctx)
    {
        if (ctx.Token() == TKQuestionMarkCharacter)
        {
            //OptOperator => QuestionMarkCharacter 
            ctx.Match(TKQuestionMarkCharacter);
            ctx.Action_PrintOptional();
        }
        else if (ctx.Token() == TKAsteriskCharacter)
        {
            //OptOperator => AsteriskCharacter 
            ctx.Match(TKAsteriskCharacter);
            ctx.Action_PrintZeroOrMore();
        }
        else if (ctx.Token() == TKPlusCharacter)
        {
            //OptOperator => PlusCharacter 
            ctx.Match(TKPlusCharacter);
            ctx.Action_PrintOneOrMore();
        }
        else if (ctx.Token() == TKKeywordAny ||
            ctx.Token() == TKLiteralChar ||
            ctx.Token() == TKIdentifier ||
            ctx.Token() == TKSemicolonCharacter ||
            ctx.Token() == TKLeftParenthesisCharacter ||
            ctx.Token() == TKVerticalLineCharacter ||
            ctx.Token() == TKRightParenthesisCharacter ||
            ctx.Token() == TKStringLiteral)
        {
            //OptOperator => Epsilon 
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_V(Context& ctx)
    {
        if (ctx.Token() == TKLiteralChar)
        {
            //V => LiteralChar RangeOpt 
            ctx.Match(TKLiteralChar);
            ctx.Action_PrintChar();
            Parse_RangeOpt(ctx);
        }
        else if (ctx.Token() == TKIdentifier)
        {
            //V => Identifier 
            ctx.Match(TKIdentifier);
            ctx.Action_PrintName();
        }
        else if (ctx.Token() == TKKeywordAny)
        {
            //V => KeywordAny 
            ctx.Match(TKKeywordAny);
            ctx.Action_PrintAny();
        }
        else if (ctx.Token() == TKStringLiteral)
        {
            //V => StringLiteral 
            ctx.Match(TKStringLiteral);
            ctx.Action_PrintString();
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_RangeOpt(Context& ctx)
    {
        if (ctx.Token() == TKRangeSeparator)
        {
            //RangeOpt => RangeSeparator LiteralChar 
            ctx.Match(TKRangeSeparator);
            ctx.Match(TKLiteralChar);
            ctx.Action_PrintRange();
        }
        else if (ctx.Token() == TKKeywordAny ||
            ctx.Token() == TKPlusCharacter ||
            ctx.Token() == TKLiteralChar ||
            ctx.Token() == TKIdentifier ||
            ctx.Token() == TKSemicolonCharacter ||
            ctx.Token() == TKLeftParenthesisCharacter ||
            ctx.Token() == TKQuestionMarkCharacter ||
            ctx.Token() == TKAsteriskCharacter ||
            ctx.Token() == TKVerticalLineCharacter ||
            ctx.Token() == TKRightParenthesisCharacter ||
            ctx.Token() == TKStringLiteral)
        {
            //RangeOpt => Epsilon 
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_SyntaxExpressionMoreSingle(Context& ctx)
    {
        if (ctx.Token() == TKVerticalLineCharacter)
        {
            //SyntaxExpressionMoreSingle => VerticalLineCharacter SyntaxExpressionSingle SyntaxExpressionMoreSingle 
            ctx.Match(TKVerticalLineCharacter);
            ctx.Action_CloseProduction();
            Parse_SyntaxExpressionSingle(ctx);
            ctx.Action_CloseProduction();
            Parse_SyntaxExpressionMoreSingle(ctx);
        }
        else if (ctx.Token() == TKSemicolonCharacter)
        {
            //SyntaxExpressionMoreSingle => Epsilon 
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_SyntaxExpressionSingle(Context& ctx)
    {
        if (ctx.Token() == TKLiteralChar ||
            ctx.Token() == TKIdentifier ||
            ctx.Token() == TKLeftCurlyBracketCharacter ||
            ctx.Token() == TKStringLiteral)
        {
            //SyntaxExpressionSingle => SyntaxExpressionItem SyntaxExpressionSingleFollow 
            Parse_SyntaxExpressionItem(ctx);
            Parse_SyntaxExpressionSingleFollow(ctx);
        }
        else if (ctx.Token() == TKKeywordEmpty)
        {
            //SyntaxExpressionSingle => KeywordEmpty 
            ctx.Match(TKKeywordEmpty);
            ctx.Action_PrintEmptyProduction();
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_SyntaxExpressionSingleFollow(Context& ctx)
    {
        if (ctx.Token() == TKLiteralChar ||
            ctx.Token() == TKIdentifier ||
            ctx.Token() == TKLeftCurlyBracketCharacter ||
            ctx.Token() == TKStringLiteral)
        {
            //SyntaxExpressionSingleFollow => SyntaxExpressionItem SyntaxExpressionSingleFollow 
            Parse_SyntaxExpressionItem(ctx);
            Parse_SyntaxExpressionSingleFollow(ctx);
        }
        else if (ctx.Token() == TKSemicolonCharacter ||
            ctx.Token() == TKVerticalLineCharacter)
        {
            //SyntaxExpressionSingleFollow => Epsilon 
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_SyntaxExpressionItem(Context& ctx)
    {
        if (ctx.Token() == TKIdentifier)
        {
            //SyntaxExpressionItem => Identifier 
            ctx.Match(TKIdentifier);
            ctx.Action_PrintName();
        }
        else if (ctx.Token() == TKLiteralChar)
        {
            //SyntaxExpressionItem => LiteralChar 
            ctx.Match(TKLiteralChar);
            ctx.Action_PrintChar();
        }
        else if (ctx.Token() == TKStringLiteral)
        {
            //SyntaxExpressionItem => StringLiteral 
            ctx.Match(TKStringLiteral);
            ctx.Action_PrintString();
        }
        else if (ctx.Token() == TKLeftCurlyBracketCharacter)
        {
            //SyntaxExpressionItem => SyntaxExpressionAction 
            Parse_SyntaxExpressionAction(ctx);
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

    void Parse_SyntaxExpressionAction(Context& ctx)
    {
        if (ctx.Token() == TKLeftCurlyBracketCharacter)
        {
            //SyntaxExpressionAction => LeftCurlyBracketCharacter Identifier RightCurlyBracketCharacter 
            ctx.Match(TKLeftCurlyBracketCharacter);
            ctx.Match(TKIdentifier);
            ctx.Action_PrintAction();
            ctx.Match(TKRightCurlyBracketCharacter);
        }
        else
        {
            throw ParserException(ctx.GetLine(), ctx.GetCol(), ErrorCodeUnexpectedToken);
        }
    }

} //namespace Sample1
