#include "stdafx.h"
#include "GrammarParser.h"
#include <fstream>
#include "FileStream.h"
#include "Production.h"
#include "expression.h"
#include <memory>
#include <set>
#include "CommonOutput.h"
const wchar_t* GetCharName(wchar_t ch)
{
  switch (ch)
  {
  case 0x0000: return L"NULL";
  case 0x0001: return L"START_OF_HEADING";
  case 0x0002: return L"START_OF_TEXT";
  case 0x0003: return L"END_OF_TEXT";
  case 0x0004: return L"END_OF_TRANSMISSION";
  case 0x0005: return L"ENQUIRY";
  case 0x0006: return L"ACKNOWLEDGE";
  case 0x0007: return L"BELL";
  case 0x0008: return L"BACKSPACE";
  case 0x0009: return L"CHARACTER_TABULATION";
  case 0x000A: return L"LINE_FEED";
  case 0x000B: return L"LINE_TABULATION";
  case 0x000C: return L"FORM_FEED";
  case 0x000D: return L"CARRIAGE_RETURN";
  case 0x000E: return L"SHIFT_OUT";
  case 0x000F: return L"SHIFT_IN";
  case 0x0010: return L"DATA_LINK_ESCAPE";
  case 0x0011: return L"DEVICE_CONTROL_ONE";
  case 0x0012: return L"DEVICE_CONTROL_TWO";
  case 0x0013: return L"DEVICE_CONTROL_THREE";
  case 0x0014: return L"DEVICE_CONTROL_FOUR";
  case 0x0015: return L"NEGATIVE_ACKNOWLEDGE";
  case 0x0016: return L"SYNCHRONOUS_IDLE";
  case 0x0017: return L"END_OF_TRANSMISSION_BLOCK";
  case 0x0018: return L"CANCEL";
  case 0x0019: return L"END_OF_MEDIUM";
  case 0x001A: return L"SUBSTITUTE";
  case 0x001B: return L"ESCAPE";
  case 0x001C: return L"INFORMATION_SEPARATOR_FOUR";
  case 0x001D: return L"INFORMATION_SEPARATOR_THREE";
  case 0x001E: return L"INFORMATION_SEPARATOR_TWO";
  case 0x001F: return L"INFORMATION_SEPARATOR_ONE";
  case 0x0020: return L"SPACE";
  case 0x0021: return L"EXCLAMATION_MARK"; //!
  case 0x0022: return L"QUOTATION_MARK"; //"
  case 0x0023: return L"NUMBER_SIGN"; //#
  case 0x0024: return L"DOLLAR_SIGN"; //$
  case 0x0025: return L"PERCENT_SIGN"; //%
  case 0x0026: return L"AMPERSAND"; //&
  case 0x0027: return L"APOSTROPHE"; //'
  case 0x0028: return L"LEFT_PARENTHESIS"; //(
  case 0x0029: return L"RIGHT_PARENTHESIS"; //)
  case 0x002A: return L"ASTERISK"; //*
  case 0x002B: return L"PLUS_SIGN"; //+
  case 0x002C: return L"COMMA"; //,
  case 0x002D: return L"HYPHEN_MINUS"; //-
  case 0x002E: return L"FULL_STOP"; //.
  case 0x002F: return L"SOLIDUS"; ///
  case 0x0030: return L"DIGIT_ZERO"; //0
  case 0x0031: return L"DIGIT_ONE"; //1
  case 0x0032: return L"DIGIT_TWO"; //2
  case 0x0033: return L"DIGIT_THREE"; //3
  case 0x0034: return L"DIGIT_FOUR"; //4
  case 0x0035: return L"DIGIT_FIVE"; //5
  case 0x0036: return L"DIGIT_SIX"; //6
  case 0x0037: return L"DIGIT_SEVEN"; //7
  case 0x0038: return L"DIGIT_EIGHT"; //8
  case 0x0039: return L"DIGIT_NINE"; //9
  case 0x003A: return L"COLON"; //:
  case 0x003B: return L"SEMICOLON"; //;
  case 0x003C: return L"LESS_THAN_SIGN"; //<
  case 0x003D: return L"EQUALS_SIGN"; //=
  case 0x003E: return L"GREATER_THAN_SIGN"; //>
  case 0x003F: return L"QUESTION_MARK"; //?
  case 0x0040: return L"COMMERCIAL_AT"; //@
  case 0x0041: return L"LATIN_CAPITAL_LETTER_A"; //A
  case 0x0042: return L"LATIN_CAPITAL_LETTER_B"; //B
  case 0x0043: return L"LATIN_CAPITAL_LETTER_C"; //C
  case 0x0044: return L"LATIN_CAPITAL_LETTER_D"; //D
  case 0x0045: return L"LATIN_CAPITAL_LETTER_E"; //E
  case 0x0046: return L"LATIN_CAPITAL_LETTER_F"; //F
  case 0x0047: return L"LATIN_CAPITAL_LETTER_G"; //G
  case 0x0048: return L"LATIN_CAPITAL_LETTER_H"; //H
  case 0x0049: return L"LATIN_CAPITAL_LETTER_I"; //I
  case 0x004A: return L"LATIN_CAPITAL_LETTER_J"; //J
  case 0x004B: return L"LATIN_CAPITAL_LETTER_K"; //K
  case 0x004C: return L"LATIN_CAPITAL_LETTER_L"; //L
  case 0x004D: return L"LATIN_CAPITAL_LETTER_M"; //M
  case 0x004E: return L"LATIN_CAPITAL_LETTER_N"; //N
  case 0x004F: return L"LATIN_CAPITAL_LETTER_O"; //O
  case 0x0050: return L"LATIN_CAPITAL_LETTER_P"; //P
  case 0x0051: return L"LATIN_CAPITAL_LETTER_Q"; //Q
  case 0x0052: return L"LATIN_CAPITAL_LETTER_R"; //R
  case 0x0053: return L"LATIN_CAPITAL_LETTER_S"; //S
  case 0x0054: return L"LATIN_CAPITAL_LETTER_T"; //T
  case 0x0055: return L"LATIN_CAPITAL_LETTER_U"; //U
  case 0x0056: return L"LATIN_CAPITAL_LETTER_V"; //V
  case 0x0057: return L"LATIN_CAPITAL_LETTER_W"; //W
  case 0x0058: return L"LATIN_CAPITAL_LETTER_X"; //X
  case 0x0059: return L"LATIN_CAPITAL_LETTER_Y"; //Y
  case 0x005A: return L"LATIN_CAPITAL_LETTER_Z"; //Z
  case 0x005B: return L"LEFT_SQUARE_BRACKET"; //[
  case 0x005C: return L"REVERSE_SOLIDUS";
  case 0x005D: return L"RIGHT_SQUARE_BRACKET"; //]
  case 0x005E: return L"CIRCUMFLEX_ACCENT"; //^
  case 0x005F: return L"LOW_LINE"; //_
  case 0x0060: return L"GRAVE_ACCENT"; //`
  case 0x0061: return L"LATIN_SMALL_LETTER_A"; //a
  case 0x0062: return L"LATIN_SMALL_LETTER_B"; //b
  case 0x0063: return L"LATIN_SMALL_LETTER_C"; //c
  case 0x0064: return L"LATIN_SMALL_LETTER_D"; //d
  case 0x0065: return L"LATIN_SMALL_LETTER_E"; //e
  case 0x0066: return L"LATIN_SMALL_LETTER_F"; //f
  case 0x0067: return L"LATIN_SMALL_LETTER_G"; //g
  case 0x0068: return L"LATIN_SMALL_LETTER_H"; //h
  case 0x0069: return L"LATIN_SMALL_LETTER_I"; //i
  case 0x006A: return L"LATIN_SMALL_LETTER_J"; //j
  case 0x006B: return L"LATIN_SMALL_LETTER_K"; //k
  case 0x006C: return L"LATIN_SMALL_LETTER_L"; //l
  case 0x006D: return L"LATIN_SMALL_LETTER_M"; //m
  case 0x006E: return L"LATIN_SMALL_LETTER_N"; //n
  case 0x006F: return L"LATIN_SMALL_LETTER_O"; //o
  case 0x0070: return L"LATIN_SMALL_LETTER_P"; //p
  case 0x0071: return L"LATIN_SMALL_LETTER_Q"; //q
  case 0x0072: return L"LATIN_SMALL_LETTER_R"; //r
  case 0x0073: return L"LATIN_SMALL_LETTER_S"; //s
  case 0x0074: return L"LATIN_SMALL_LETTER_T"; //t
  case 0x0075: return L"LATIN_SMALL_LETTER_U"; //u
  case 0x0076: return L"LATIN_SMALL_LETTER_V"; //v
  case 0x0077: return L"LATIN_SMALL_LETTER_W"; //w
  case 0x0078: return L"LATIN_SMALL_LETTER_X"; //x
  case 0x0079: return L"LATIN_SMALL_LETTER_Y"; //y
  case 0x007A: return L"LATIN_SMALL_LETTER_Z"; //z
  case 0x007B: return L"LEFT_CURLY_BRACKET"; //{
  case 0x007C: return L"VERTICAL_LINE"; //|
  case 0x007D: return L"RIGHT_CURLY_BRACKET"; //}
  case 0x007E: return L"TILDE"; //~
  }
  return L"??";
}

std::wstring GetLiteralTokenName(const std::wstring& str)
{
  if (str.size() == 1)
    return GetCharName(str[0]);

  return str;
}

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
            //assert(false);
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
              if (ws.size() > 0)
              {
                
                if (ctx.m_grammar.m_bAutoToken &&
                    (ws[0] >= 'a' && ws[0] <= 'z') || 
                     ws[0] == '_' ||
                     ws.size() == 1)
                {
                 // ws = MakeUpper(ws);
                  //considera como token
                  ctx.m_grammar.m_TokenNames.push_back(ws);
                  pSymbol = ctx.m_grammar.AddSymbol(ws, true);
                }
                else
                {
                  //considera como outro sytax 
                  pSymbol = ctx.m_grammar.AddSymbol(ctx.m_lexeme, false);
                }
              }
            }

            assert(pSymbol != nullptr);
            production.AddNextSymbol(pSymbol);

            Match(ctx, TKIdentifier);

        }
        else if (token == TKLiteralChar ||
                 token == TKStringLiteral)
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
              if (ctx.m_grammar.m_bAutoToken)
              {
                //ws = MakeUpper(ws);
                std::wstring charname = GetLiteralTokenName(ws);

                ctx.m_grammar.AddTokenAlias(charname, ws);

                //considera como token
                ctx.m_grammar.m_TokenNames.push_back(charname);
                auto pSymbom = ctx.m_grammar.AddSymbol(charname, true);
                production.AddNextSymbol(pSymbom);
              }
              else
              {
                assert(false);
              }
            }

            Match(ctx, TKLiteralChar);
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

