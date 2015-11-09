#include  "stdafx.h"
#include "expression.h"


Expression::Expression(const std::wstring& s, NfaStates& states, TransitionSet& symbols) : m_ch(0)
{
  m_LiteralString = s;

  //  m_Type = ExpressionTypeSingle;
  m_Left = nullptr;
  m_Right = nullptr;

  //(start) -a-> () -b-> ....() -c-> (end)
  //cria uma cadeia de "ands" com cada caracteres o start e end sao as extremidades

  m_StartState = states.New();
  m_EndState = 0;
  NfaState* state = m_StartState;

  for (size_t i = 0 ; i < s.size(); i++)
  {
    if (i == 0)
      m_ch = Transition(s[i]);

    NfaState* pNew = states.New();
    Transition ch(s[i]);
    symbols.insert(ch);
    AddTransition(state, ch, pNew);
    state = pNew;
  }

  m_EndState = state;
  //AddTransition(state, m_EndState);

}

Expression::Expression(const Transition& ch, NfaStates& states) : m_ch(ch)
{
  //   m_Type = ExpressionTypeSingle;
  m_Left = nullptr;
  m_Right = nullptr;
  /// e
  /// |----------------|
  /// |        e       |
  /// | ( ) -----> (O) |
  /// |----------------|
  /// A
  /// |----------------|
  /// |        A       |
  /// | ( ) -----> (O) |
  /// |----------------|
  m_StartState = states.New();
  m_EndState = states.New();
  AddTransition(m_StartState, ch, m_EndState);
}

Expression::Expression(std::unique_ptr<Expression> A,
                       ExpressionType type,
                       NfaStates& states)
{
  //   m_Type = type;
  m_Left = std::move(A);
  m_Right = nullptr;

  if (type == ExpressionTypeZeroOrMore)
  {
    m_StartState = states.New();
    m_EndState = states.New();
    AddTransition(m_StartState, m_Left->m_StartState);
    AddTransition(m_StartState, m_EndState);
    AddTransition(m_Left->m_EndState, m_Left->m_StartState);
    AddTransition(m_Left->m_EndState, m_EndState);
    /// A*
    ///                    e
    ///              |----------|
    ///              |          |
    ///           |--|----------|--|
    ///        e  |  V     A    |  |   e
    ///  ( )------->( ) -----> ( )-------(O)
    ///   |       |----------------|      ^
    ///   |                               |
    ///   |--------------------------------
  }
  else if (type == ExpressionTypeOptional)
  {
    m_StartState = states.New();
    m_EndState = states.New();
    AddTransition(m_StartState, m_Left->m_StartState);
    AddTransition(m_StartState, m_EndState);
    AddTransition(m_Left->m_EndState, m_EndState);
  }
  else if (type == ExpressionTypeOneOrMore)
  {
    m_StartState = states.New();
    m_EndState = states.New();
    AddTransition(m_StartState, m_Left->m_StartState);
    AddTransition(m_EndState, m_Left->m_StartState);
    AddTransition(m_Left->m_EndState, m_EndState);
    //revisar
  }
  else
  {
    assert(false);
  }
}

Expression::Expression(std::unique_ptr<Expression> A,
                       std::unique_ptr<Expression> B,
                       ExpressionType type,
                       NfaStates& states,
                       TransitionSet& symbols)
{
  //  m_Type = type;
  m_Left = std::move(A);
  m_Right = std::move(B);

  if (type == ExpressionTypeAnd)
  {
    /// AB
    /// |----------------|       |-----------------|
    /// |        A       |   e   |        B        |
    /// | ( ) -----> ( )---------->(  ) -----> (O) |
    /// |----------------|       |-----------------|
    AddTransition(m_Left->m_EndState, m_Right->m_StartState);
    m_StartState = m_Left->m_StartState;
    m_EndState = m_Right->m_EndState;
  }
  else if (type == ExpressionTypeOr)
  {
    /// A|B
    ///           |----------------|
    ///      e    |        A       |   e
    ///   --------->( ) -----> ( )---------
    ///   |       |----------------|      |
    ///   |                               v
    ///  ( )                             (O)
    ///   |                               ^
    ///   |       |----------------|      |
    ///   |       |        B       |      |
    ///   --------->( ) -----> ( )---------
    ///     e     |----------------|   e
    m_StartState = states.New();
    m_EndState = states.New();
    AddTransition(m_StartState, m_Left->m_StartState);
    AddTransition(m_StartState, m_Right->m_StartState);
    AddTransition(m_Left->m_EndState, m_EndState);
    AddTransition(m_Right->m_EndState, m_EndState);
  }
  else if (type == ExpressionTypeRange)
  {
    m_StartState = states.New();
    m_EndState = states.New();
    AddTransition(m_StartState, m_Left->m_StartState);
    AddTransition(m_StartState, m_Right->m_StartState);

    for (wchar_t ch = m_Left->m_ch.m_ch + 1 ; ch < m_Right->m_ch.m_ch; ch++)
    {
      symbols.insert(Transition(ch));
      std::unique_ptr<Expression> pNew(new Expression(Transition(ch), states));
      AddTransition(m_StartState, pNew->m_StartState);
      AddTransition(pNew->m_EndState, m_EndState);
    }

    AddTransition(m_Left->m_EndState, m_EndState);
    AddTransition(m_Right->m_EndState, m_EndState);
  }
  else
  {
    assert(false);
  }
}


