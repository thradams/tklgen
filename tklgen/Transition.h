#pragma once


class Transition
{
public:
  wchar_t m_ch;

  
  //simbolo interno para representar transicao em "nada"
  static const wchar_t Epsilon = (wchar_t)(-1);

  //simbolo interno para representar transicao em qualquer coisa "any"
  static const wchar_t AnyChar = L'\uFFFE';//((wchar_t)(-1)) - 1; //reserved


  Transition() : m_ch(0) {}

  explicit Transition(wchar_t ch) : m_ch(ch) {}

  bool IsEpsilon() const
  {
    return m_ch == Epsilon;
  }

  bool Accept(const Transition& other) const
  {
    return m_ch == other.m_ch;
  }

  bool operator < (const Transition& other) const
  {
    return m_ch < other.m_ch;
  }
};

//constates
const wchar_t EmptyTransition = (wchar_t)(-1);

//const wchar_t AnyCharInputSymbol = L'.';
const wchar_t TokenCharStart = L'\u1000';
