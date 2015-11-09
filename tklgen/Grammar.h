#pragma once
#include "Production.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>
#include "GrammarSymbol.h"
#include <map>
#include <algorithm>
#include "expression.h"

#define END_MARK_NAME L"EndMark"
#define EPSILON_MARK_NAME L"Epsilon"


struct Less
{
  bool operator()(const GrammarSymbol* a, const GrammarSymbol* b) const
  {
    if (a->IsTerminal() && !b->IsTerminal())
    {
      return true;
    }

    if (!a->IsTerminal() && b->IsTerminal())
    {
      return false;
    }

    return a->GetDeclarationIndex() < b->GetDeclarationIndex();
  }
};

class Symbols
{
  typedef std::vector<GrammarSymbol*> TSymbols;
  TSymbols m_Symbols;

  GrammarSymbol* m_pEndMarker;
  GrammarSymbol* m_pEpsilon;
  GrammarSymbol* m_pStart;
  typedef   std::map<std::wstring, std::wstring> AliasMap ;
  AliasMap m_alias;

  void SetIndexes()
  {
    int count = 0;
    TSymbols::iterator it = m_Symbols.begin();

    for (; it != m_Symbols.end(); it++)
    {
      (*it)->SetIndex(count++);
    }
  }

public:

  Symbols()
  {
    //m_TerminalCount = 0;
    m_pEndMarker = 0;
    m_pEpsilon = 0;
    m_pStart = 0;
    m_pEndMarker = new GrammarSymbol(END_MARK_NAME, true, m_Symbols.size(), 100000);
    m_pEpsilon = new GrammarSymbol(EPSILON_MARK_NAME, true, m_Symbols.size(), 100001);
    m_Symbols.push_back(m_pEpsilon);
    m_Symbols.push_back(m_pEndMarker);
    //Close();
  }



  ~Symbols()
  {
    TSymbols::const_iterator it = m_Symbols.begin();

    for (; it != m_Symbols.end(); it++)
    {
      delete *it;
    }
  }



  //const GrammarSymbol* StartSymbol() const
  //{
  //}

  const GrammarSymbol* epsilon() const
  {
    //assert(m_closed);
    return m_pEpsilon;
  }

  const GrammarSymbol* endmarker() const
  {
    //assert(m_closed);
    return m_pEndMarker;
  }

  GrammarSymbol* FindSymbolByAlias(const std::wstring& s) const
  {
    auto it2 = m_alias.find(s);

    if (it2 != m_alias.end())
    {
      return FindSymbol(it2->second);
    }

    return nullptr;
  }

  GrammarSymbol* FindSymbol(const std::wstring& s) const
  {
    TSymbols::const_iterator it = m_Symbols.begin();

    for (; it != m_Symbols.end(); it++)
    {
      if ((*it)->GetName() == s)
      {
        return (*it);
      }
    }

    return nullptr;
  }

  int GetNumOfTerminals() const
  {
    //assert(m_closed);
    int count = 0;
    TSymbols::const_iterator it = m_Symbols.begin();

    for (; it != m_Symbols.end(); it++)
    {
      if ((*it)->IsTerminal())
      {
        count++;
      }
    }

    return count;
  }

  int GetNumOfGrammarSymbols() const
  {
    //assert(m_closed);
    return m_Symbols.size();
  }

  const GrammarSymbol*  GetStartSymbol() const
  {
    //assert(m_closed);
    return m_pStart;
  }

  const GrammarSymbol* GetSymbol(int i) const
  {
    return m_Symbols[i];
  }

  const GrammarSymbol* GetSymbol(int i)
  {
    return m_Symbols[i];
  }

  void AddTokenAlias(const std::wstring& token, const std::wstring& alias)
  {
    m_alias[alias] = token;
  }

  const GrammarSymbol* AddSymbol(const std::wstring& s, bool isTerminal)
  {
    GrammarSymbol* pFind = FindSymbol(s);
    if (pFind != nullptr)
    {
      assert(pFind == epsilon() || pFind == endmarker());
      return nullptr;
    }

    std::auto_ptr<GrammarSymbol> up(new GrammarSymbol(s, isTerminal, m_Symbols.size(), m_Symbols.size()));
    GrammarSymbol* pr = up.get();

    if (s == L"Main")
    {
      m_pStart = up.get();
    }

    /*if (m_Symbols.size() ==2)
    {
    m_pStart = up.get();
    }*/
    m_Symbols.push_back(pr);
    up.release();
    //se faco o sort nao consigo verificar...
    //os cannocal sets mudam de numero e consequente goto e action
    //mesmo usando a ordem de declaracao
    std::sort(m_Symbols.begin(), m_Symbols.end(), Less());
    SetIndexes();
    return pr;
  }
};


class Grammar : public Symbols
{
  std::vector<std::unique_ptr<Production>> m_Productions;

  typedef std::vector<std::unique_ptr<Production>>::const_iterator Iterator;

  std::vector<std::wstring> m_Actions; //usado no LL

  //opcional
  std::wstring m_ModuleName;
  std::wstring m_LanguageName;

public:

  NfaState* m_pStartNfaState;
  NfaStates m_NfaStates;
  TransitionSet m_TransitionSet;
  std::vector<std::wstring> m_TokenNames;
  std::vector<std::unique_ptr<Expression>> m_Expressions;
  int m_TokenInterleaveIndex;

  Grammar()
  {
    m_TokenInterleaveIndex = -1;
    m_pStartNfaState = m_NfaStates.New();
  }

  Production& GetProduction(int i)
  {
    return *m_Productions.at(i);
  }

  int GetNumberOfActions() const
  {
    return (int)m_Actions.size();
  };

  const std::wstring& GetActionName(int i) const
  {
    return m_Actions.at(i);
  };

  int AddAction(const std::wstring& s)
  {
    std::vector<std::wstring>::iterator it = std::find(m_Actions.begin(), m_Actions.end(), s);

    if (it != m_Actions.end())
    {
      return it - m_Actions.begin();
    }

    m_Actions.push_back(s);
    return m_Actions.size() - 1;
  }

  const Production& GetProduction(int i) const
  {
    return *m_Productions.at(i);
  }

  //  void AddNextSymbol(Production& production, const std::wstring& s, bool isTerminal)
  // {
  //    const GrammarSymbol* p = AddSymbol(s, isTerminal);
  //   production.AddNextSymbol(p);
  // }

  Production& AddProduction(const std::wstring& s)
  {
    const GrammarSymbol* p = FindSymbol(s);

    if (p == nullptr)
    {
      p = AddSymbol(s, false);
    }

    assert(!p->IsTerminal());

    std::unique_ptr<Production> up(new Production(this, p, s));
    m_Productions.push_back(std::move(up));
    return *m_Productions.back().get();
  }

  int FindProductionIndex(const Production& production) const
  {
    for (int i = 0; i < GetNumOfProductions(); i++)
    {
      if (&production == &GetProduction(i))
      {
        return i;
      }
    }

    return -1;
  }

  const int GetNumOfProductions() const
  {
    return (int)m_Productions.size();
  }


  const std::wstring& GetModuleName() const
  {
    return m_ModuleName;
  }

  const std::wstring& GetLanguageName() const
  {
    return m_LanguageName;
  }


  void SetModuleName(const std::wstring& s)
  {
    m_ModuleName = s;
  }

  void SetLanguageName(const std::wstring& s)
  {
    m_LanguageName = s;
  }

  /*bool IsTerminal(int symbolIndex) const
  {
  const SymbolItem & s = GetSymbol(symbolIndex);
  return s.IsTerminal();
  }*/
};

std::wostream& Print(std::wostream& os, const Grammar& g);

int CheckEmptyProduction(const Grammar& g);
