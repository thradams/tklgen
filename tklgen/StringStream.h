//////////////////////////////////////////////////////////////////////////////
// TKLGEN - Version Mar  8 2013
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

struct StringStream
{
  const wchar_t* m_pCharacteres;
  const wchar_t* m_pCurrentChar;
  size_t         m_CurrentLine;
  size_t         m_CurrentCol;
  bool           m_HasPutBack;
  wchar_t        m_PutBackCharacter;
  bool           m_EofSent;

  bool NextCharCore(wchar_t& ch)
  {
    if (m_HasPutBack)
    {
      ch = m_PutBackCharacter;
      m_HasPutBack = false;
      m_PutBackCharacter = 0;
      return true;
    }

    if (m_EofSent)
      return false;

    ch = *m_pCurrentChar++;

    if (ch == L'\0')
    {
      ch = '\0';
      m_EofSent = true;
    }

    return true;
  }

public:

  StringStream(const wchar_t* psz)
    : m_pCharacteres(psz)
    , m_CurrentLine(1)
    , m_CurrentCol(1)
    , m_HasPutBack(false)
    , m_PutBackCharacter(L'\0')
    , m_EofSent(false)
  {
    m_pCurrentChar = m_pCharacteres;
  }

  void PutBack(wchar_t ch)
  {
    assert(m_HasPutBack == false);

    if (ch == L'\n')
    {
      if (m_CurrentLine > 1)
      {
        m_CurrentLine--;
      }
    }
    else
    {
      if (m_CurrentCol > 1)
      {
        m_CurrentCol--;
      }
    }

    m_PutBackCharacter = ch;
    m_HasPutBack = true;
  }

  bool NextChar(wchar_t& ch)
  {
    bool b = NextCharCore(ch);

    if (b && ch == L'\n')
    {
      m_CurrentLine++;
      m_CurrentCol = 1;
    }
    else
    {
      m_CurrentCol++;
    }

    return b;
  }

  size_t GetLine() const
  {
    return m_CurrentLine;
  }

  size_t GetCol() const
  {
    return m_CurrentCol;
  }
};