#include "stdafx.h"
#include "GrammarParser.h"
#include <fstream>
#include "FileStream.h"
#include "Production.h"
#include "expression.h"
#include <memory>
#include <set>

int HexToInt(const wchar_t* value)
{
    struct CHexMap
    {
        TCHAR chr;
        int value;
    };
    const int HexMapL = 16;
    CHexMap HexMap[HexMapL] =
    {
        {'0', 0}, {'1', 1},
        {'2', 2}, {'3', 3},
        {'4', 4}, {'5', 5},
        {'6', 6}, {'7', 7},
        {'8', 8}, {'9', 9},
        {'A', 10}, {'B', 11},
        {'C', 12}, {'D', 13},
        {'E', 14}, {'F', 15}
    };
    //const wchar_t *mstr = value;
    const wchar_t* s = value;
    int result = 0;

    if (*s == '0' && *(s + 1) == 'X') s += 2;

    bool firsttime = true;

    while (*s != '\0')
    {
        bool found = false;

        for (int i = 0; i < HexMapL; i++)
        {
            if (*s == HexMap[i].chr)
            {
                if (!firsttime) result <<= 4;

                result |= HexMap[i].value;
                found = true;
                break;
            }
        }

        if (!found) break;

        s++;
        firsttime = false;
    }

    return result;
}


std::wstring LiteralToString(const std::wstring& ws)
{
    std::wstring result;
    result.reserve(ws.size());

    for (size_t i = 1; i < ws.size() - 1; i++)
    {
        wchar_t ch = ws[i];

        if (ch == '\\')
        {
            i++;
            ch = ws[i];

            switch (ch)
            {
                case L'0':
                    ch = L'\0';
                    break;

                case L'a':
                    ch = L'\a';
                    break;

                case L'b':
                    ch = L'\b';
                    break;

                case L'f':
                    ch = L'\f';
                    break;

                case L'n':
                    ch = L'\n';
                    break;

                case L'r':
                    ch = L'\r';
                    break;

                case L't':
                    ch = L'\t';
                    break;

                case L'\\':
                    ch = L'\\';
                    break;

                case L'\'':
                    ch = L'\'';
                    break;

                case L'"':
                    ch = L'"';
                    break;

                    //\uFFFE
                case L'u':
                {
                    i++; //skip u
                    wchar_t s[5];
                    s[0] = toupper(ws[i++]);
                    s[1] = toupper(ws[i++]);
                    s[2] = toupper(ws[i++]);
                    s[3] = toupper(ws[i]);
                    s[4] = 0;
                    ch = HexToInt(s);
                }
                break;
            }
        }//

        result += ch;
    }

    return result;
}


namespace tklgen
{

    const wchar_t* GetErrorString(ParserErrorCode e)
    {
        switch (e)
        {
            case ParserErrorCodeModule:
                return L"module expected";

            case ParserErrorCodeScannerError:
                return L"scanner error";

            case ParserErrorCodeTokenUnexpected:
                return L"invalid token";

            case ParserErrorCodeDeclarationExpexted:
                return L"Declaration expected";
        }

        return L"";
    }

    //forward declarations
    std::unique_ptr<Expression> Parse_TokenExpression(Context&);
    void Parse_SyntaxExpression(Context&, Production&);
    std::unique_ptr<Expression> Parse_T(Context&);
    std::unique_ptr<Expression> Parse_ZeroOrMoreT(Context&);
    std::unique_ptr<Expression> Parse_F(Context&);
    std::unique_ptr<Expression> Parse_ZeroOrMoreOrF(Context&);
    std::unique_ptr<Expression> Parse_OrF(Context&);
    std::unique_ptr<Expression> Parse_P(Context&);
    std::unique_ptr<Expression> Parse_OptOperator(Context&, std::unique_ptr<Expression> e1);
    std::unique_ptr<Expression> Parse_V(Context&);
    std::unique_ptr<Expression> Parse_RangeOpt(Context&, const std::wstring&);
    void Parse_SyntaxExpressionSingle(Context&, Production&);
    void Parse_SyntaxExpressionMoreSingle(Context& ctx, const std::wstring& productionName);
    void Parse_SyntaxExpressionItem(Context&, Production&);
    void Parse_SyntaxExpressionSingleFollow(Context&, Production&);
    void Parse_Main(Context&);
    void Parse_ModuleBody(Context&);
    void Parse_LanguageBody(Context&);
    void Parse_Declarations(Context&);
    void Parse_Declaration(Context&);
    void Parse_TokenDeclaration(Context&);
    void Parse_SyntaxDeclaration(Context&);
    void Parse_InterleaveDeclaration(Context&);
    void Parse_SyntaxExpressionAction(Context&, Production&);


    std::unique_ptr<Expression> Parse_TokenExpression(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKKeywordAny ||
                token == TKLiteralChar ||
                token == TKIdentifier ||
                token == TKLeftParenthesisCharacter ||
                token == TKStringLiteral)
        {

            //TokenExpression => T ZeroOrMoreT
            auto e1 = Parse_T(ctx);
            auto e2 = Parse_ZeroOrMoreT(ctx);

            if (e1.get() != nullptr &&
                    e2.get() != nullptr)
            {
                std::unique_ptr<Expression> e3(new Expression(std::move(e1),
                                               std::move(e2),
                                               ExpressionTypeAnd,
                                               ctx.m_grammar.m_NfaStates,
                                               ctx.m_grammar.m_TransitionSet));
                return e3;
            }

            return e1;
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }

        return nullptr;
    }

    void Parse_SyntaxExpression(Context& ctx, Production& production)
    {
        auto token = GetToken(ctx);

        if (token == TKKeywordEmpty ||
                token == TKLiteralChar ||
                token == TKIdentifier ||
                token == TKLeftCurlyBracketCharacter ||
                token == TKStringLiteral)
        {
            //SyntaxExpression => SyntaxExpressionSingle SyntaxExpressionMoreSingle
            Parse_SyntaxExpressionSingle(ctx, production);
            Parse_SyntaxExpressionMoreSingle(ctx, production.GetLeftSymbol()->GetName());
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }
    }

    std::unique_ptr<Expression> Parse_T(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKKeywordAny ||
                token == TKLiteralChar ||
                token == TKIdentifier ||
                token == TKLeftParenthesisCharacter ||
                token == TKStringLiteral)
        {
            //T => F ZeroOrMoreOrF
            auto e1 = Parse_F(ctx);
            auto e2 = Parse_ZeroOrMoreOrF(ctx);

            if (e1.get() != nullptr &&
                    e2.get() != nullptr)
            {
                assert(e1.get() != nullptr);
                assert(e2.get() != nullptr);

                std::unique_ptr<Expression> e3(new Expression(std::move(e1),
                                               std::move(e2),
                                               ExpressionTypeOr,
                                               ctx.m_grammar.m_NfaStates,
                                               ctx.m_grammar.m_TransitionSet));


                return e3;
            }

            return e1;
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }

        return nullptr;
    }

    std::unique_ptr<Expression> Parse_ZeroOrMoreT(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKKeywordAny ||
                token == TKLiteralChar ||
                token == TKIdentifier ||
                token == TKLeftParenthesisCharacter ||
                token == TKStringLiteral)
        {
            //ZeroOrMoreT => T ZeroOrMoreT
            auto e1 = Parse_T(ctx);
            auto e2 = Parse_ZeroOrMoreT(ctx);

            if (e1.get() != nullptr &&
                    e2.get() != nullptr)
            {
                assert(e1.get() != nullptr);
                assert(e2.get() != nullptr);
                std::unique_ptr<Expression> e3(new Expression(std::move(e1),
                                               std::move(e2),
                                               ExpressionTypeAnd,
                                               ctx.m_grammar.m_NfaStates,
                                               ctx.m_grammar.m_TransitionSet));

                //assert(false);
                return e3;
            }

            return e1;
        }
        else if (token == TKSemicolonCharacter ||
                 token == TKRightParenthesisCharacter)
        {
            //ZeroOrMoreT => Epsilon
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }

        return nullptr;
    }

    std::unique_ptr<Expression> Parse_F(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKKeywordAny ||
                token == TKLiteralChar ||
                token == TKIdentifier ||
                token == TKLeftParenthesisCharacter ||
                token == TKStringLiteral)
        {
            //F => P OptOperator
            auto e1 = Parse_P(ctx);
            assert(e1.get() != nullptr);
            auto e2 = Parse_OptOperator(ctx, std::move(e1));
            return e2;
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);

        }

        return nullptr;
    }

    std::unique_ptr<Expression> Parse_ZeroOrMoreOrF(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKVerticalLineCharacter)
        {
            //ZeroOrMoreOrF => OrF ZeroOrMoreOrF
            auto e1 = Parse_OrF(ctx);
            auto e2 = Parse_ZeroOrMoreOrF(ctx);

            if (e2.get() != nullptr)
            {
                assert(e1.get() != nullptr);
                assert(e2.get() != nullptr);
                std::unique_ptr<Expression> e3(new Expression(std::move(e1),
                                               std::move(e2),
                                               ExpressionTypeOr,
                                               ctx.m_grammar.m_NfaStates,
                                               ctx.m_grammar.m_TransitionSet));
                return e3;
            }

            return e1;
        }
        else if (token == TKKeywordAny ||
                 token == TKLiteralChar ||
                 token == TKSemicolonCharacter ||
                 token == TKIdentifier ||
                 token == TKLeftParenthesisCharacter ||
                 token == TKRightParenthesisCharacter ||
                 token == TKStringLiteral)
        {
            //ZeroOrMoreOrF => Epsilon
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }

        return nullptr;
    }

    std::unique_ptr<Expression> Parse_OrF(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKVerticalLineCharacter)
        {
            //OrF => VerticalLineCharacter F
            Match(ctx, TKVerticalLineCharacter);
            return Parse_F(ctx);
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }

        return nullptr;
    }

    std::unique_ptr<Expression> Parse_P(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKKeywordAny ||
                token == TKLiteralChar ||
                token == TKIdentifier ||
                token == TKStringLiteral)
        {
            //P => V
            return Parse_V(ctx);
        }
        else if (token == TKLeftParenthesisCharacter)
        {
            //P => LeftParenthesisCharacter TokenExpression RightParenthesisCharacter
            Match(ctx, TKLeftParenthesisCharacter);
            auto e1 = Parse_TokenExpression(ctx);
            Match(ctx, TKRightParenthesisCharacter);
            return e1;
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }

        return nullptr;
    }

    std::unique_ptr<Expression> Parse_OptOperator(Context& ctx, std::unique_ptr<Expression> e1)
    {
        auto token = GetToken(ctx);

        if (token == TKQuestionMarkCharacter)
        {
            //OptOperator => QuestionMarkCharacter
            Match(ctx, TKQuestionMarkCharacter);
            assert(e1.get() != nullptr);

            std::unique_ptr<Expression> e3(new Expression(std::move(e1),
                                           ExpressionTypeOptional,
                                           ctx.m_grammar.m_NfaStates));
            return e3;
        }
        else if (token == TKAsteriskCharacter)
        {
            //OptOperator => AsteriskCharacter
            Match(ctx, TKAsteriskCharacter);
            assert(e1.get() != nullptr);

            std::unique_ptr<Expression> e3(new Expression(std::move(e1),
                                           ExpressionTypeZeroOrMore,
                                           ctx.m_grammar.m_NfaStates));
            return e3;
        }
        else if (token == TKPlusCharacter)
        {
            //OptOperator => PlusCharacter
            Match(ctx, TKPlusCharacter);
            assert(e1.get() != nullptr);


            std::unique_ptr<Expression> e3(new Expression(std::move(e1),
                                           ExpressionTypeOneOrMore,
                                           ctx.m_grammar.m_NfaStates));
            return e3;
        }
        else if (token == TKKeywordAny ||
                 token == TKLiteralChar ||
                 token == TKSemicolonCharacter ||
                 token == TKIdentifier ||
                 token == TKVerticalLineCharacter ||
                 token == TKLeftParenthesisCharacter ||
                 token == TKRightParenthesisCharacter ||
                 token == TKStringLiteral)
        {
            //OptOperator => Epsilon
            return std::move(e1);
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }

        return nullptr;
    }

    std::unique_ptr<Expression> Parse_V(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKLiteralChar)
        {
            auto ws = ctx.m_lexeme;
            //V => LiteralChar RangeOpt
            Match(ctx, TKLiteralChar);
            return Parse_RangeOpt(ctx, ws);
        }
        else if (token == TKIdentifier)
        {
            assert(false);
            //V => Identifier
            Match(ctx, TKIdentifier);
        }
        else if (token == TKKeywordAny)
        {
            //V => KeywordAny
            Match(ctx, TKKeywordAny);
            std::wstring wsnew = LiteralToString(L"'\\uFFFE'");


            std::unique_ptr<Expression> e1(new Expression(wsnew,
                                           ctx.m_grammar.m_NfaStates,
                                           ctx.m_grammar.m_TransitionSet));
            //ws = L"'\\uFFFE'"; //internal representation of any like a literal see //
            return e1;
        }
        else if (token == TKStringLiteral)
        {


            std::wstring wsnew = LiteralToString(ctx.m_lexeme);
            std::unique_ptr<Expression> e1(new Expression(wsnew,
                                           ctx.m_grammar.m_NfaStates,
                                           ctx.m_grammar.m_TransitionSet));

            //V => StringLiteral
            Match(ctx, TKStringLiteral);
            return e1;
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }

        return nullptr;
    }

    std::unique_ptr<Expression> Parse_RangeOpt(Context& ctx, const std::wstring& lexeme)
    {
        auto token = GetToken(ctx);

        if (token == TKRangeSeparator)
        {
            std::wstring wsnew = LiteralToString(lexeme);
            std::unique_ptr<Expression> e1(new Expression(wsnew,
                                           ctx.m_grammar.m_NfaStates,
                                           ctx.m_grammar.m_TransitionSet));

            //RangeOpt => RangeSeparator LiteralChar
            Match(ctx, TKRangeSeparator);

            wsnew = LiteralToString(ctx.m_lexeme);
            std::unique_ptr<Expression> e2(new Expression(wsnew,
                                           ctx.m_grammar.m_NfaStates,
                                           ctx.m_grammar.m_TransitionSet));

            Match(ctx, TKLiteralChar);



            std::unique_ptr<Expression> e3(new Expression(std::move(e1),
                                           std::move(e2),
                                           ExpressionTypeRange,
                                           ctx.m_grammar.m_NfaStates,
                                           ctx.m_grammar.m_TransitionSet));
            return e3;

        }
        else if (token == TKKeywordAny ||
                 token == TKLiteralChar ||
                 token == TKPlusCharacter ||
                 token == TKSemicolonCharacter ||
                 token == TKIdentifier ||
                 token == TKQuestionMarkCharacter ||
                 token == TKAsteriskCharacter ||
                 token == TKVerticalLineCharacter ||
                 token == TKLeftParenthesisCharacter ||
                 token == TKRightParenthesisCharacter ||
                 token == TKStringLiteral)
        {
            //RangeOpt => Epsilon
            std::wstring wsnew = LiteralToString(lexeme);
            std::unique_ptr<Expression> e1(new Expression(wsnew,
                                           ctx.m_grammar.m_NfaStates,
                                           ctx.m_grammar.m_TransitionSet));
            return e1;
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }

        assert(false);
        return nullptr;
    }

    void Parse_SyntaxExpressionSingle(Context& ctx, Production& production)
    {
        auto token = GetToken(ctx);

        if (token == TKLiteralChar ||
                token == TKIdentifier ||
                token == TKLeftCurlyBracketCharacter ||
                token == TKStringLiteral)
        {
            //SyntaxExpressionSingle => SyntaxExpressionItem SyntaxExpressionSingleFollow
            Parse_SyntaxExpressionItem(ctx, production);
            Parse_SyntaxExpressionSingleFollow(ctx, production);
        }
        else if (token == TKKeywordEmpty)
        {
            //SyntaxExpressionSingle => KeywordEmpty
            Match(ctx, TKKeywordEmpty);
            production.AddNextSymbol(ctx.m_grammar.epsilon());
        }
        else
        {
            assert(false);
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }
    }

    void Parse_SyntaxExpressionMoreSingle(Context& ctx, const std::wstring& productionName)
    {
        auto token = GetToken(ctx);

        if (token == TKVerticalLineCharacter)
        {
            //SyntaxExpressionMoreSingle => VerticalLineCharacter SyntaxExpressionSingle SyntaxExpressionMoreSingle
            Match(ctx, TKVerticalLineCharacter);

            //auto ws = production.GetLeftSymbol()->GetName();
            Production& newProduction = ctx.m_grammar.AddProduction(productionName);
            Parse_SyntaxExpressionSingle(ctx, newProduction);
            Parse_SyntaxExpressionMoreSingle(ctx, productionName);
        }
        else if (token == TKSemicolonCharacter)
        {
            //SyntaxExpressionMoreSingle => Epsilon
        }
        else
        {
            assert(false);
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }
    }

    void Parse_SyntaxExpressionItem(Context& ctx, Production& production)
    {
        auto token = GetToken(ctx);

        if (token == TKIdentifier)
        {
            //SyntaxExpressionItem => Identifier
            auto ws = ctx.m_lexeme;
            const GrammarSymbol* pSymbol = ctx.m_grammar.FindSymbol(ws);

            if (pSymbol == nullptr)
            {
                pSymbol = ctx.m_grammar.AddSymbol(ctx.m_lexeme, false);
            }

            assert(pSymbol != nullptr);
            production.AddNextSymbol(pSymbol);

            Match(ctx, TKIdentifier);

        }
        else if (token == TKLiteralChar)
        {

            //SyntaxExpressionItem => StringLiteral
            std::wstring ws = ctx.m_lexeme.substr(1, ctx.m_lexeme.size() - 2);
            auto pSymbom = ctx.m_grammar.FindSymbolByAlias(ws);

            if (pSymbom != nullptr)
            {
                assert(pSymbom->IsTerminal());
                production.AddNextSymbol(pSymbom);
            }
            else
            {
                assert(false);
            }

            Match(ctx, TKLiteralChar);
        }
        else if (token == TKStringLiteral)
        {

            std::wstring ws = ctx.m_lexeme.substr(1, ctx.m_lexeme.size() - 2);
            auto pSymbom = ctx.m_grammar.FindSymbolByAlias(ws);

            if (pSymbom != nullptr)
            {
                assert(pSymbom->IsTerminal());
                production.AddNextSymbol(pSymbom);
            }
            else
            {
              std::wcout << L"symbol " << ws << L" not found";
              assert(false);
            }

            //SyntaxExpressionItem => StringLiteral

            Match(ctx, TKStringLiteral);
        }
        else if (token == TKLeftCurlyBracketCharacter)
        {
            //SyntaxExpressionItem => SyntaxExpressionAction
            Parse_SyntaxExpressionAction(ctx, production);
        }
        else
        {
            assert(false);
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }
    }

    void Parse_SyntaxExpressionSingleFollow(Context& ctx, Production& production)
    {
        auto token = GetToken(ctx);

        if (token == TKLiteralChar ||
                token == TKIdentifier ||
                token == TKLeftCurlyBracketCharacter ||
                token == TKStringLiteral)
        {
            //SyntaxExpressionSingleFollow => SyntaxExpressionItem SyntaxExpressionSingleFollow
            Parse_SyntaxExpressionItem(ctx, production);
            Parse_SyntaxExpressionSingleFollow(ctx, production);
        }
        else if (token == TKSemicolonCharacter ||
                 token == TKVerticalLineCharacter)
        {
            //SyntaxExpressionSingleFollow => Epsilon
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }
    }

    void Parse_Main(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKKeywordModule)
        {
            //Main => ModuleBody
            Parse_ModuleBody(ctx);
        }
        else
        {
            assert(false);
            OnError(ctx, ParserErrorCodeModule);
        }
    }

    void Parse_ModuleBody(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKKeywordModule)
        {
            //ModuleBody => KeywordModule Identifier LeftCurlyBracketCharacter LanguageBody RightCurlyBracketCharacter
            Match(ctx, TKKeywordModule);
            ctx.m_grammar.SetModuleName(ctx.m_lexeme);
            Match(ctx, TKIdentifier);
            Match(ctx, TKLeftCurlyBracketCharacter);
            Parse_LanguageBody(ctx);
            Match(ctx, TKRightCurlyBracketCharacter);
        }
        else
        {
            assert(false);
            OnError(ctx, ParserErrorCodeModule);
        }
    }

    void Parse_LanguageBody(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKKeywordLanguage)
        {
            //LanguageBody => KeywordLanguage Identifier LeftCurlyBracketCharacter Declarations RightCurlyBracketCharacter
            Match(ctx, TKKeywordLanguage);
            ctx.m_grammar.SetLanguageName(ctx.m_lexeme);
            Match(ctx, TKIdentifier);
            Match(ctx, TKLeftCurlyBracketCharacter);
            Parse_Declarations(ctx);
            Match(ctx, TKRightCurlyBracketCharacter);
        }
        else
        {
            assert(false);
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }
    }

    void Parse_Declarations(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKKeywordInterleave ||
                token == TKKeywordSyntax ||
                token == TKKeywordToken)
        {
            //Declarations => Declaration Declarations
            Parse_Declaration(ctx);
            Parse_Declarations(ctx);
        }
        else if (token == TKRightCurlyBracketCharacter)
        {
            //Declarations => Epsilon
        }
        else
        {
            OnError(ctx, ParserErrorCodeDeclarationExpexted);
        }
    }

    void Parse_Declaration(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKKeywordToken)
        {
            //Declaration => TokenDeclaration
            Parse_TokenDeclaration(ctx);
        }
        else if (token == TKKeywordSyntax)
        {
            //Declaration => SyntaxDeclaration
            Parse_SyntaxDeclaration(ctx);
        }
        else if (token == TKKeywordInterleave)
        {
            //Declaration => InterleaveDeclaration
            Parse_InterleaveDeclaration(ctx);
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }
    }

    void Parse_TokenDeclaration(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKKeywordToken)
        {
            //TokenDeclaration => KeywordToken Identifier EqualsCharacter TokenExpression SemicolonCharacter
            Match(ctx, TKKeywordToken);
            auto tokenName = ctx.m_lexeme;
            ctx.m_grammar.m_TokenNames.push_back(tokenName);
            ctx.m_grammar.AddSymbol(ctx.m_lexeme, true);
            Match(ctx, TKIdentifier);
            Match(ctx, TKEqualsCharacter);
            auto e1 = Parse_TokenExpression(ctx);

            if (e1.get() != nullptr)
            {
                if (!e1->m_LiteralString.empty())
                {
                    //Quando achar esta string no syntax pode usar como sendo este token
                    //e1->m_LiteralString.clear();
                    ctx.m_grammar.AddTokenAlias(tokenName, e1->m_LiteralString);
                }
            }

            ctx.m_grammar.m_Expressions.push_back(std::move(e1));
            Match(ctx, TKSemicolonCharacter);
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }
    }

    void Parse_SyntaxDeclaration(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKKeywordSyntax)
        {
            //SyntaxDeclaration => KeywordSyntax Identifier EqualsCharacter SyntaxExpression SemicolonCharacter
            Match(ctx, TKKeywordSyntax);
            auto syntaxName = ctx.m_lexeme;
            Production& production = ctx.m_grammar.AddProduction(syntaxName);
            Match(ctx, TKIdentifier);
            Match(ctx, TKEqualsCharacter);
            Parse_SyntaxExpression(ctx, production);
            Match(ctx, TKSemicolonCharacter);
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }
    }

    void Parse_InterleaveDeclaration(Context& ctx)
    {
        auto token = GetToken(ctx);

        if (token == TKKeywordInterleave)
        {
            //InterleaveDeclaration => KeywordInterleave Identifier EqualsCharacter TokenExpression SemicolonCharacter
            Match(ctx, TKKeywordInterleave);
            auto tokenName = ctx.m_lexeme;
            ctx.m_grammar.m_TokenNames.push_back(tokenName);
            // ctx.m_grammar.AddSymbol(ctx.m_lexeme, true);
            Match(ctx, TKIdentifier);
            Match(ctx, TKEqualsCharacter);
            auto e1 = Parse_TokenExpression(ctx);

            if (e1.get() != nullptr)
            {
                if (!e1->m_LiteralString.empty())
                {
                    //Quando achar esta string no syntax pode usar como sendo este token
                    //e1->m_LiteralString.clear();
                    //   ctx.m_grammar.AddTokenAlias(tokenName, e1->m_LiteralString);
                }
            }

            ctx.m_grammar.m_TokenInterleaveIndex = ctx.m_grammar.m_TokenNames.size() - 1;
            ctx.m_grammar.m_Expressions.push_back(std::move(e1));
            Match(ctx, TKSemicolonCharacter);
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }
    }

    void Parse_SyntaxExpressionAction(Context& ctx, Production& production)
    {
        auto token = GetToken(ctx);

        if (token == TKLeftCurlyBracketCharacter)
        {
            //SyntaxExpressionAction => LeftCurlyBracketCharacter Identifier RightCurlyBracketCharacter
            Match(ctx, TKLeftCurlyBracketCharacter);
            production.AddNextAction(ctx.m_lexeme);
            Match(ctx, TKIdentifier);
            Match(ctx, TKRightCurlyBracketCharacter);
        }
        else
        {
            OnError(ctx, ParserErrorCodeTokenUnexpected);
        }
    }

    void Parse(const wchar_t* file, Grammar& grammar)
    {
        //std::wifstream ss(file);
        FileStream fileStream(file, 4);
        tklgen::Context context(grammar, fileStream);
        tklgen::Parse_Main(context);
    }
} //namespace

