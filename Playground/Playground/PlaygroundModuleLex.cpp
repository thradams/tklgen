/*
 * Generated by TKLGEN - VersionNov 18 2015
 * Copyright (C) 2013, Thiago Adams (thiago.adams@gmail.com)
 * www.thradams.com
 *
 */

#include "PlaygroundModuleLex.h"
const char* PlaygroundLang_Tokens_ToString(PlaygroundLang_Tokens e)
{
    switch(e)
    {
    case TKPlus: return "Plus";
    case TKMinus: return "Minus";
    case TKMulti: return "Multi";
    case TKDiv: return "Div";
    case TKPower: return "Power";
    case TKOpen: return "Open";
    case TKClose: return "Close";
    case TKEnd: return "End";
    case TKEndMark: return "EndMark";
    case TKInteger: return "Integer";
    case TKIdentifier: return "Identifier";
    case TKBlanks: return "Blanks";
    default:break;
    }
    return "";
}

int PlaygroundLang_GetNext(int state, wchar_t ch)
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
        else if (ch >= L'A' && ch <= L'Z')
            return 3;
        else if (ch == L'^')
            return 12;
        else if (ch == L'_')
            return 3;
        else if (ch >= L'a' && ch <= L'z')
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
            return 14;
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
        /* end state for TKPower*/
        break;
        case 13:
            return -1; 
        break;
        case 14:
        if (ch == L'$')
            return 15;
        else if (ch >= L'0' && ch <= L'9')
            return 14;
        else if (ch >= L'A' && ch <= L'Z')
            return 15;
        else if (ch == L'_')
            return 15;
        else if (ch >= L'a' && ch <= L'z')
            return 15;
        break;
        case 15:
        if (ch == L'$')
            return 15;
        else if (ch >= L'A' && ch <= L'Z')
            return 15;
        else if (ch == L'_')
            return 15;
        else if (ch >= L'a' && ch <= L'z')
            return 15;
        /* end state for TKIdentifier*/
        break;
    } /*switch*/
    return -1;
}

int PlaygroundLang_IsInterleave(PlaygroundLang_Tokens tk)
{
    return tk == TKBlanks;
}

int PlaygroundLang_GetTokenFromState(int state, PlaygroundLang_Tokens* tk)
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
        case 12: *tk = TKPower; break;
        case 15: *tk = TKIdentifier; break;
        default:
            return 0;
    }
    return 1;
}
