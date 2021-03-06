//////////////////////////////////////////////////////////////////////////////
// Generated by TKLGEN - Version Mar  7 2013
// Copyright (C) 2013, Thiago Adams (thiago.adams@gmail.com)
// www.thradams.com
//
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cassert>
#include <string>
#include "tokenizer.h"
#include "Sample1DFA.h"


namespace Sample1
{

    class Context
    {
        typedef Tokenizer<Sample1::DFA, TokenizerStreamT> Scanner;
        Scanner m_scanner;
        
        Sample1::Tokens m_CurrentToken;
        std::wstring m_lexeme;
        
    public:
        Context(TokenizerStreamT& s) : m_scanner(s)
        {
            if (!m_scanner.NextToken(m_lexeme, m_CurrentToken))
            {
            }
        }

        //returns the current token
        Sample1::Tokens Token() const
        {
             return m_CurrentToken;
        }
        
        //checks if Token() == tk and then move the current token
        void Match(Sample1::Tokens tk)
        {
             if (tk != m_CurrentToken)
             {
                 throw std::runtime_error("token not expected");
             }
             if (!m_scanner.NextToken(m_lexeme, m_CurrentToken))
             {
             }
        }
        
        void Action_ModuleName() {}
        void Action_LanguageName() {}
        void Action_teste() {}
        void Action_TokenName() {}
        void Action_TokenEnd() {}
        void Action_SyntaxName() {}
        void Action_PrintAnd() {}
        void Action_PrintOr() {}
        void Action_PrintOptional() {}
        void Action_PrintZeroOrMore() {}
        void Action_PrintOneOrMore() {}
        void Action_PrintChar() {}
        void Action_PrintName() {}
        void Action_PrintAny() {}
        void Action_PrintString() {}
        void Action_PrintRange() {}
        void Action_CloseProduction() {}
        void Action_PrintEmptyProduction() {}
        void Action_PrintAction() {}
    };
    


    void Parse_Main(Context&);
} //namespace

