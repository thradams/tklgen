#pragma once
#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <cassert>
#include "GrammarSymbol.h"
// Exemplo:
// A=>XYZ

class Grammar;

class Production
{
  Grammar* m_pGrammar;
  const GrammarSymbol* m_pLeftSymbol;
  std::vector<const GrammarSymbol*> m_RightSymbol;

  std::vector<int> m_Actions;

  bool m_IsInitial;
  Production();//not
public:
  //mutable int m_NumVisits;

  Production(Grammar* pGrammar,
             const GrammarSymbol* pLeftSymbol,
             const std::wstring& s) :
    m_pLeftSymbol(pLeftSymbol),
    m_IsInitial(false),
    m_pGrammar(pGrammar)
  {
    //m_NumVisits = 0;
    if (pLeftSymbol == nullptr)
    {
      throw std::invalid_argument("");
    }
  }

  void SetIsInitial(bool b)
  {
    m_IsInitial = b;
  }

  bool IsInitial() const
  {
    //o primeiro symbolo eh reservado para o inicio
    return m_IsInitial;
  }

  Production& AddNextAction(const std::wstring& s);
  // Production& AddNextSymbol(const std::wstring& s, bool isTerminal = false);
  Production& AddNextSymbol(const GrammarSymbol* pRightSymbol);

  //retorna o numero de simbolos terminais
  int GetNumOfRightSymbolsTerminals() const
  {
    int count = 0;

    for (int i = 0 ; i < GetNumOfRightSymbols(); i++)
    {
      if (GetRightSymbol(i)->IsTerminal())
        count++;
    }

    return count;
  }

  int GetNumOfRightSymbols() const
  {
    return m_RightSymbol.size();
  }

  int GetAction(int i) const
  {
    if (m_Actions.empty() ||
        i >= (int)m_Actions.size())
    {
      return -1;
    }
   // assert(m_Actions.size() == GetNumOfRightSymbols() + 1);
    return m_Actions[i];
  }

  const GrammarSymbol* GetRightSymbol(int i) const
  {
    assert(m_RightSymbol[i] != nullptr);
    return m_RightSymbol[i];
  }

  const GrammarSymbol* GetLeftSymbol() const
  {
    assert(m_pLeftSymbol != nullptr);
    return m_pLeftSymbol;
  }

};


std::wostream& Print(std::wostream& os,
                     const Production& production,
                     int dot = -1);