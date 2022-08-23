#pragma once
#include "transition.h"
//#include <multimap>
#include <map>
#include <vector>
#include <set>
#include <assert.h>
#include <memory>
#include <string>

class NfaState;


typedef std::set<Transition> TransitionSet;
class NfaState;

typedef std::vector<NfaState*> StateVector;


typedef std::set<NfaState*> StateSet;

typedef std::map<NfaState*, int> MapStateToInt;
typedef std::map< std::pair<int, wchar_t>, int > MapIntCharToInt;
typedef std::multimap<int, std::pair<wchar_t, int>> MapIntToCharInt;
typedef std::map< int, int > MapIntToInt;

class NfaState
{
  friend struct NfaStates;
  ~NfaState()
  {
  }

  typedef std::multimap<Transition, NfaState*> TransitionToStates;
  TransitionToStates m_TransitionToState;
  int m_Id;

public:
  typedef TransitionToStates::iterator Iterator;


  NfaState(int id) : m_Id(id)
  {
  }

  //uso de um multimap
  Iterator Begin()
  {
    return m_TransitionToState.begin();
  }

  //uso de um multimap
  Iterator End()
  {
    return m_TransitionToState.end();
  }

  //uso de um multimap
  Iterator Find(const Transition& t)
  {
    return m_TransitionToState.find(t);
  }

  void AddEpsilonTransition(NfaState* to)
  {
    m_TransitionToState.insert(std::make_pair(Transition(Transition::Epsilon), to));
  }

  void AddTransition(const Transition& ch, NfaState* to)
  {
    m_TransitionToState.insert(std::make_pair(ch, to));
  }
};


struct NfaStates
{
  StateVector m_v;
  NfaState* New()
  {
    NfaState* p = new NfaState(m_v.size());
    m_v.push_back(p);
    return p;
  }

  ~NfaStates()
  {
    for (size_t i = 0 ; i < m_v.size(); i++)
      delete m_v[i];
  }

};

inline void AddTransition(NfaState* from, const Transition& ch, NfaState* to)
{
  from->AddTransition(ch, to);
}

inline void AddTransition(NfaState* from, NfaState* to)
{
  from->AddEpsilonTransition(to);
}

enum ExpressionType
{
  ExpressionTypeOr,
  ExpressionTypeAnd,
  ExpressionTypeSingle,
  ExpressionTypeZeroOrMore,
  ExpressionTypeOneOrMore,
  ExpressionTypeOptional,
  ExpressionTypeRange
};

struct Expression
{
  Transition m_ch;
  std::unique_ptr<Expression> m_Left;
  std::unique_ptr<Expression> m_Right;

  //so oq precisaria na verdade eh o start e o end
  NfaState* m_StartState;
  NfaState* m_EndState;

  std::wstring m_LiteralString;

  Expression(const std::wstring& s,
             NfaStates& states,
             TransitionSet& symbols);

  Expression(const Transition& ch,
             NfaStates& states);

  Expression(std::unique_ptr<Expression> A,
             ExpressionType type,
             NfaStates& states);

  Expression(std::unique_ptr<Expression> A,
             std::unique_ptr<Expression> B,
             ExpressionType type,
             NfaStates& states,
             TransitionSet& symbols);
};

