/*
 * Generated by TKLGEN - VersionOct 27 2014
 * Copyright (C) 2013, Thiago Adams (thiago.adams@gmail.com)
 * www.thradams.com
 *
 */

#include "ModuleSample2Lex.h"
#include <assert.h>

const wchar_t* LanguageSample2_Tokens_ToString(enum LanguageSample2_Tokens e)
{
    switch(e)
    {
    case TKPlus: return L"Plus";
    case TKMinus: return L"Minus";
    case TKMulti: return L"Multi";
    case TKDiv: return L"Div";
    case TKPower: return L"Power";
    case TKOpen: return L"Open";
    case TKClose: return L"Close";
    case TKEqual: return L"Equal";
    case TKGreaterThan: return L"GreaterThan";
    case TKLessThan: return L"LessThan";
    case TKGreaterEqualThan: return L"GreaterEqualThan";
    case TKLessEqualThan: return L"LessEqualThan";
    case TKEnd: return L"End";
    case TKAnd: return L"And";
    case TKOr: return L"Or";
    case TKEndMark: return L"EndMark";
    case TKInteger: return L"Integer";
    case TKIdentifier: return L"Identifier";
    case TKBlanks: return L"Blanks";
    default:break;
    }
    return L"";
}

static int LanguageSample2_GetNext(int state, wchar_t ch)
{
    switch (state)
    {
        case 0:
        if (ch == L'\0')
            return 1;
        else if (ch >= L'\t' && ch <= L'\n')
            return 2;
        else if (ch == L'\r')
            return 2;
        else if (ch == L' ')
            return 2;
        else if (ch == L'$')
            return 3;
        else if (ch == L'(')
            return 4;
        else if (ch == L')')
            return 5;
        else if (ch == L'*')
            return 6;
        else if (ch == L'+')
            return 7;
        else if (ch == L'-')
            return 8;
        else if (ch == L'/')
            return 9;
        else if (ch >= L'0' && ch <= L'9')
            return 10;
        else if (ch == L';')
            return 11;
        else if (ch == L'<')
            return 12;
        else if (ch == L'=')
            return 13;
        else if (ch == L'>')
            return 14;
        else if (ch >= L'A' && ch <= L'Z')
            return 3;
        else if (ch == L'^')
            return 15;
        else if (ch == L'_')
            return 3;
        else if (ch == L'a')
            return 16;
        else if (ch >= L'b' && ch <= L'n')
            return 3;
        else if (ch == L'o')
            return 17;
        else if (ch >= L'p' && ch <= L'z')
            return 3;
        break;
        case 1:
        /* end state for TKEndMark*/
        break;
        case 2:
        if (ch >= L'\t' && ch <= L'\n')
            return 2;
        else if (ch == L'\r')
            return 2;
        else if (ch == L' ')
            return 2;
        /* end state for TKBlanks*/
        break;
        case 3:
        if (ch == L'$')
            return 3;
        else if (ch >= L'0' && ch <= L'9')
            return 19;
        else if (ch >= L'A' && ch <= L'Z')
            return 3;
        else if (ch == L'_')
            return 3;
        else if (ch >= L'a' && ch <= L'z')
            return 3;
        /* end state for TKIdentifier*/
        break;
        case 4:
        /* end state for TKOpen*/
        break;
        case 5:
        /* end state for TKClose*/
        break;
        case 6:
        /* end state for TKMulti*/
        break;
        case 7:
        /* end state for TKPlus*/
        break;
        case 8:
        /* end state for TKMinus*/
        break;
        case 9:
        /* end state for TKDiv*/
        break;
        case 10:
        if (ch >= L'0' && ch <= L'9')
            return 10;
        /* end state for TKInteger*/
        break;
        case 11:
        /* end state for TKEnd*/
        break;
        case 12:
        if (ch == L'=')
            return 20;
        /* end state for TKLessThan*/
        break;
        case 13:
        /* end state for TKEqual*/
        break;
        case 14:
        if (ch == L'=')
            return 21;
        /* end state for TKGreaterThan*/
        break;
        case 15:
        /* end state for TKPower*/
        break;
        case 16:
        if (ch == L'$')
            return 3;
        else if (ch >= L'0' && ch <= L'9')
            return 19;
        else if (ch >= L'A' && ch <= L'Z')
            return 3;
        else if (ch == L'_')
            return 3;
        else if (ch >= L'a' && ch <= L'm')
            return 3;
        else if (ch == L'n')
            return 22;
        else if (ch >= L'o' && ch <= L'z')
            return 3;
        /* end state for TKIdentifier*/
        break;
        case 17:
        if (ch == L'$')
            return 3;
        else if (ch >= L'0' && ch <= L'9')
            return 19;
        else if (ch >= L'A' && ch <= L'Z')
            return 3;
        else if (ch == L'_')
            return 3;
        else if (ch >= L'a' && ch <= L'q')
            return 3;
        else if (ch == L'r')
            return 23;
        else if (ch >= L's' && ch <= L'z')
            return 3;
        /* end state for TKIdentifier*/
        break;
        case 18:
            return -1; 
        break;
        case 19:
        if (ch == L'$')
            return 24;
        else if (ch >= L'0' && ch <= L'9')
            return 19;
        else if (ch >= L'A' && ch <= L'Z')
            return 24;
        else if (ch == L'_')
            return 24;
        else if (ch >= L'a' && ch <= L'z')
            return 24;
        break;
        case 20:
        /* end state for TKLessEqualThan*/
        break;
        case 21:
        /* end state for TKGreaterEqualThan*/
        break;
        case 22:
        if (ch == L'$')
            return 3;
        else if (ch >= L'0' && ch <= L'9')
            return 19;
        else if (ch >= L'A' && ch <= L'Z')
            return 3;
        else if (ch == L'_')
            return 3;
        else if (ch >= L'a' && ch <= L'c')
            return 3;
        else if (ch == L'd')
            return 25;
        else if (ch >= L'e' && ch <= L'z')
            return 3;
        /* end state for TKIdentifier*/
        break;
        case 23:
        if (ch == L'$')
            return 3;
        else if (ch >= L'0' && ch <= L'9')
            return 19;
        else if (ch >= L'A' && ch <= L'Z')
            return 3;
        else if (ch == L'_')
            return 3;
        else if (ch >= L'a' && ch <= L'z')
            return 3;
    /*TKOr*/
        /* end state for TKOr*/
        break;
        case 24:
        if (ch == L'$')
            return 24;
        else if (ch >= L'A' && ch <= L'Z')
            return 24;
        else if (ch == L'_')
            return 24;
        else if (ch >= L'a' && ch <= L'z')
            return 24;
        /* end state for TKIdentifier*/
        break;
        case 25:
        if (ch == L'$')
            return 3;
        else if (ch >= L'0' && ch <= L'9')
            return 19;
        else if (ch >= L'A' && ch <= L'Z')
            return 3;
        else if (ch == L'_')
            return 3;
        else if (ch >= L'a' && ch <= L'z')
            return 3;
    /*TKAnd*/
        /* end state for TKAnd*/
        break;
    } /*switch*/
    return -1;
}

static int LanguageSample2_IsInterleave(enum LanguageSample2_Tokens tk)
{
    return tk == TKBlanks;
}

static int LanguageSample2_GetTokenFromState(int state, enum LanguageSample2_Tokens* tk)
{
    switch (state)
    {
        case 1: *tk = TKEndMark; break;
        case 2: *tk = TKBlanks; break;
        case 3: *tk = TKIdentifier; break;
        case 4: *tk = TKOpen; break;
        case 5: *tk = TKClose; break;
        case 6: *tk = TKMulti; break;
        case 7: *tk = TKPlus; break;
        case 8: *tk = TKMinus; break;
        case 9: *tk = TKDiv; break;
        case 10: *tk = TKInteger; break;
        case 11: *tk = TKEnd; break;
        case 12: *tk = TKLessThan; break;
        case 13: *tk = TKEqual; break;
        case 14: *tk = TKGreaterThan; break;
        case 15: *tk = TKPower; break;
        case 16: *tk = TKIdentifier; break;
        case 17: *tk = TKIdentifier; break;
        case 20: *tk = TKLessEqualThan; break;
        case 21: *tk = TKGreaterEqualThan; break;
        case 22: *tk = TKIdentifier; break;
        case 23: *tk = TKOr; break;
        case 24: *tk = TKIdentifier; break;
        case 25: *tk = TKAnd; break;
        default:
            return 0;
    }
    return 1;
}

int LanguageSample2_NextTokenNoInterleave(struct sstream* stream,
    wchar_t* buffer,
    int bufferSize,
    enum LanguageSample2_Tokens* tk)
{
    if (bufferSize <= 0)
    {
        assert(0);
        return 0;
    }
    buffer[0] = 0;
    int lastGoodState = -1;
    int currentState = 0;
    wchar_t ch;
    int index = 0;
    while (sstream_getwc(stream, &ch))
    {
        currentState = LanguageSample2_GetNext(currentState, ch);
        if (currentState == -1)
        {
            sstream_ungetwc(stream, ch);
            break;
        }
        enum LanguageSample2_Tokens tk2;
        if (LanguageSample2_GetTokenFromState(currentState, &tk2))
        {
            *tk = tk2;
            lastGoodState = currentState;
        }
        if (index < bufferSize)
        {
            buffer[index++] = ch;
        }
        else
        {
            assert(0);
            return 0;
        }
    }
    buffer[index] = 0;
    return (lastGoodState != -1);
}

int LanguageSample2_NextToken(struct sstream* stream,
    wchar_t* buffer,
    int bufferSize,
    enum LanguageSample2_Tokens* tk)
{
    for (;;)
    {
        if (LanguageSample2_NextTokenNoInterleave(stream, buffer, bufferSize, tk) == 0)
        {
            return 0;
        }
        if (!LanguageSample2_IsInterleave(*tk))
        {
            return 1;
        }
    }
}