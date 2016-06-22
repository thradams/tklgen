#include "stdafx.h"
#include "LexAlgorithms.h"
#include "Grammar.h"
#include "console.h"
#include <fstream>
#include "LexOutputJS.h"
#include "LexOutputC.h"
#include "LexOutputCpp.h"
#include "CommonOutput.h"
using namespace console;


////////////////////////////


void FindClosure(NfaState* start,
                 const Transition& ch,
                 StateVector& states)
{
  NfaState::Iterator it = start->Find(ch);

  while (it != start->End())
  {
    //assert(it->first == ch);
    states.push_back(it->second);
    FindClosure(it->second, ch, states);
    it++;
  }

}


//Set of NFA states reachable from NFA state s on epsilon-transitions alone.
void FindClosure0(NfaState* s,
                  const Transition& ch,
                  StateVector& states)
{
  states.clear();
  states.push_back(s);
  FindClosure(s, ch, states);
}


void E_ClosureT(const StateVector& T, StateVector& e_ClosureT)
{
  //push all states of T onto stack; //177
  StateVector stack = T;
  //initialize E_ClosureT to T;
  e_ClosureT = T;

  while (!stack.empty())
  {
    NfaState* t = stack.back();
    stack.pop_back();
    NfaState::Iterator it = t->Begin();

    for (; it != t->End(); it++)
    {
      if (it->first.IsEpsilon())
      {
        NfaState* u = it->second;

        if (std::find(e_ClosureT.begin(), e_ClosureT.end(), u) == e_ClosureT.end())
        {
          e_ClosureT.push_back(u);
          stack.push_back(u);
        }
      }
    }
  }
}


void ReduceDfa(MapIntCharToInt& map,//int out
               MapIntToCharInt& map2,//int out
               MapIntToInt& map3, //in out
               const TransitionSet& symbols
              )
{
  std::vector< SetOfIntsPtr > arrGroup;

  SetOfIntsPtr setAccepting = std::make_shared<SetOfInts>();
  SetOfIntsPtr setNonAccepting = std::make_shared<SetOfInts>();


  //PrintLn("Minimizing DFA 1/3...");
  ////////////////
  //divide em 2 grupos
  int casen = -1;

  for (MapIntToCharInt::iterator it = map2.begin();
       it != map2.end();
       it++)
  {
    if (casen != it->first)
    {
      casen = it->first;

      if (map3.find(casen) != map3.end())
      {
        setNonAccepting->insert(casen);
      }
      else
      {
        setAccepting->insert(casen);
      }
    }
  }

  //////////////
  arrGroup.push_back(setAccepting);
  arrGroup.push_back(setNonAccepting);

  Map map0;

  for (TransitionSet::const_iterator it = symbols.begin();
       it != symbols.end();
       it++)
  {
    const wchar_t inputSymbol = it->m_ch;
    int nPartionIndex = 0;

    while (nPartionIndex < (int)arrGroup.size())
    {
      //pega o grupo para particionar
      SetOfIntsPtr setToBePartitioned = arrGroup[nPartionIndex];
      nPartionIndex++;

      if (setToBePartitioned->size() < 2)
        continue; //cannot be particioned

      //para cada item na particao
      for (SetOfInts::iterator k = setToBePartitioned->begin();
           k != setToBePartitioned->end(); k++)
      {
        int state = *k;
        //tem transicao para esta letra?
        MapIntCharToInt::iterator itm = map.find(std::make_pair(state, inputSymbol));

        if (itm != map.end())
        {
          //o estado state vai para stateTransitionTo com o inputSymbol
          int stateTransionTo = itm->second;
          SetOfIntsPtr group = FindGroup(arrGroup, stateTransionTo);
          map0.Set(group, state);
        }
        else
        {
          map0.Set(nullptr, state);
        }
      }//each state

      if (map0.GetSize() > 1)  // means some states transition into different groups
      {
        std::vector< SetOfIntsPtr >::iterator itfind = std::find(arrGroup.begin(), arrGroup.end(), setToBePartitioned);
        arrGroup.erase(itfind);

        map0.PushInto(arrGroup);
        nPartionIndex = 0;  // we want to start from the begining again
        it = symbols.begin(); //iterInput.Reset();  // we want to start from the begining again
      }

      map0.Clear();
    }  // end of while..loop
  }

  //   PrintLn("Minimizing DFA 2/3...");
  for (int k = 0; k < (int)arrGroup.size(); k++)
  {
    SetOfIntsPtr s = arrGroup[k];

    if (s->size() > 1)
    {
      SetOfInts::iterator it = s->begin();
      int representative = *it;
      it++;

      for (; it != s->end(); it++)
      {
        int vr = *it;
        MapIntToInt::iterator it4 = map3.find(vr);
        //if (it4 == map3.end())
        {
          map2.erase(vr); //remover este estado do mapa

          //tinha nos ends?
          if (it4 != map3.end())
          {
            // assert(false);
            map3.insert(std::make_pair(representative, it4->second));
            map3.erase(vr);
          }

          MapIntToCharInt::iterator it2 = map2.begin();

          for (; it2 != map2.end(); ++it2)
          {
            //fazer todo mundo que aponta p aquele agora apontar p representative
            if (it2->second.second  == vr)
            {
              it2->second.second = representative;
            }
          }
        }
        // else
        //{
        //nao vou remover este
        //}
      }
    }
  }

  // PrintLn("Minimizing DFA 3/3...");
  ///////////////////////////////////////////////////////////////////////////////
  ////  normalizacao do numero do estado (para montar tabela por ex)
  ////
  //agora tinha que pegar e normalizar os numeros 0 1 2 3....
  MapIntToInt newids;
  MapIntToCharInt::iterator it2 = map2.begin();
  int lastid = -1;
  int idcount = 0;

  for (; it2 != map2.end(); ++it2)
  {
    //fazer todo mundo que aponta p aquele agora apontar p representative
    if (lastid != it2->first)
    {
      lastid = it2->first;
      newids.insert(std::make_pair(lastid, idcount));
      idcount++;
    }
  }

  // PrintLn("Minimizing DFA 4/3...");
  MapIntToCharInt map2New;
  it2 = map2.begin();

  for (; it2 != map2.end(); ++it2)
  {
    int from = newids[it2->first];
    int to = newids[it2->second.second];
    //if (it2->second.first != EmptyTransition)
    {
      map2New.insert(std::make_pair(from, std::make_pair(it2->second.first, to)));
    }
  }

  //  PrintLn("Minimizing DFA 5/3...");
  map2New.swap(map2);
  MapIntToInt map3New;
  MapIntToInt::iterator itm3 = map3.begin();

  for (; itm3 != map3.end(); itm3++)
  {
    map3New.insert(std::make_pair(newids[itm3->first], itm3->second));
  }

  map3New.swap(map3);


  //  PrintLn("Minimizing DFA 6/3...");

  //o map eh igual o map2 na verdade so que os pares sao diferente..
  //corrige o map agora para ficar igual (representar) o mesmo que map2
  {
    map.clear();
    MapIntToCharInt::iterator it2 = map2.begin();

    for (; it2 != map2.end(); ++it2)
    {
      map.insert(std::make_pair(std::make_pair(it2->first, it2->second.first), it2->second.second));
    }
  }

  // PrintLn("Minimizing DFA 6/6");
}



void FindEnds(const StateVector& u,
              int uindex,
              Output& out,
              const MapStateToInt& ends)
{
  StateVector::const_iterator it = u.begin();

  for (; it != u.end(); it++)
  {
    MapStateToInt::const_iterator it2 = ends.find(*it);

    if (it2 != ends.end())
    {
      //diz que uindex ‚ um estado final e que second eh o tokenn dele
      out.EndState(uindex, it2->second);
    }
  }
}

void MakeDStates(NfaState* start,
                 const TransitionSet& symbols,
                 Output& out,
                 const MapStateToInt& ends)
{
  Dstates d;
  {
    std::auto_ptr<StateVector> v(new StateVector());
    FindClosure0(start, Transition(Transition::Epsilon), *v.get());
    d.Add(v.get());
    v.release();
  }
  //verifica se o 1 estado eh um estado final
  FindEnds(*(d.states[0].first), 0, out, ends);

  StateVector r;
  r.reserve(1000);

  int index = d.FindUnmarked();

  while (index >= 0)
  {
    d.states[index].second = true;
    TransitionSet::const_iterator it = symbols.begin();

    for (; it != symbols.end(); ++it)
    {
      const Transition& ch = *it;

      r.clear();

      Move(*(d.states[index].first), ch, r);

      std::auto_ptr<StateVector> u(new StateVector());
      E_ClosureT(r, *u);

      int hasIndex = d.Has(*u);

      if (!u->empty() && hasIndex == -1)
      {
        FindEnds(*u, d.states.size(), out, ends);
        d.Add(u.get());
        u.release();
        out.Result(index, ch, d.states.size() - 1);
      }
      else if (hasIndex > 0)
      {
        //ja existe este estado
        out.Result(index, ch, hasIndex);
      }
    }

    index = d.FindUnmarked();
  }

  out.Last();
  //  FindEnds(d.states.back().first, d.states.size(), out, ends);
}


SetOfIntsPtr FindGroup(const std::vector<SetOfIntsPtr>& groups,
                       int state)
{
  for (int k = 0 ; k < (int)groups.size(); k++)
  {
    if (groups[k]->find(state) != groups[k]->end())
      return groups[k];
  }

  return nullptr;
}


void Move(const StateVector& T,
          const Transition& ch,
          StateVector& moveR)
{
  moveR.clear();

  for (int i = 0; i < (int)T.size(); i++)
  {
    NfaState* p = T[i];
    NfaState::Iterator it = p->Begin();

    for (; it != p->End(); it++)
    {
      if (it->first.Accept(ch))
      {
        moveR.push_back(it->second);
      }
    }
  }
}



void CreateMainNFA(NfaState * newStart, 
                   std::vector<std::unique_ptr<Expression>>& expr,
       NfaStates& states,
       TransitionSet& symbols,
       MapStateToInt& ends)
{
  
  wchar_t tokenchar = TokenCharStart;
  int k = 0;

  for (int j = 0; j < (int)expr.size(); j++)
  {
    Expression* expr1 = expr[j].get();

    AddTransition(newStart, expr1->m_StartState);

    NfaState* pNewEnd = states.New();
    const wchar_t charToEndStateToken = tokenchar++;
    symbols.insert(Transition(charToEndStateToken));
    AddTransition(expr1->m_EndState, Transition(charToEndStateToken), pNewEnd);
    expr1->m_EndState = pNewEnd;

    //mapeia como estado final
    ends.insert(std::make_pair(expr1->m_EndState, k));
    k++;

//    delete expr1;
  }

  expr.clear();
}


///////////////

void GenerateCpp(Grammar& grammar, const std::wstring& fileOutNameDfa,
  OutputMap& out)
{


  std::wofstream fileout(fileOutNameDfa.c_str());

  MapIntToInt finaltokens;
  MapIntCharToInt intCharToIntMapOut;

  int interleaveIndex = grammar.m_TokenInterleaveIndex;

  GenerateCode(fileout,
    fileOutNameDfa,
    grammar.GetLanguageName(),
    true,
    grammar.m_TokenNames,
    intCharToIntMapOut,
    finaltokens,
    out,
    grammar.m_TransitionSet,
    interleaveIndex,
    L"Tokens",
    L"DFA", //struct name
    L"TK");

}

void GenerateC(Grammar& grammar, const std::wstring& fileOutNameDfa,
  OutputMap& out)
{  
   std::wstring header = fileOutNameDfa;
  std::wstring source = fileOutNameDfa;
  find_replace(source, L".h", L".cpp");

  std::wofstream fileoutSource(source.c_str());

  std::wofstream fileoutHeader(header.c_str());


  MapIntToInt finaltokens;
  MapIntCharToInt intCharToIntMapOut;

  int interleaveIndex = grammar.m_TokenInterleaveIndex;

  GenerateCodeC(fileoutSource,
    fileOutNameDfa,
    grammar.GetLanguageName(),
    grammar.GetModuleName(),
    true,
    grammar.m_TokenNames,
    intCharToIntMapOut,
    finaltokens,
    out,
    grammar.m_TransitionSet,
    interleaveIndex,
    L"Tokens",
    L"TK");

  GenerateHeaderC(fileoutHeader,
      fileOutNameDfa,
      grammar.GetLanguageName(),
      grammar.GetModuleName(),
      true,
      grammar.m_TokenNames,
      intCharToIntMapOut,
      finaltokens,
      out,
      grammar.m_TransitionSet,
      interleaveIndex,
      L"Tokens",
      L"TK");

}


void GenerateJs(Grammar& grammar, const std::wstring& fileOutNameDfa,
  OutputMap& out)
{
  std::wstring header = fileOutNameDfa;
  std::wstring source = fileOutNameDfa;
  find_replace(source, L".h", L".cpp");

  std::wofstream fileoutSource(source.c_str());

  std::wofstream fileoutHeader(header.c_str());


  MapIntToInt finaltokens;
  MapIntCharToInt intCharToIntMapOut;

  int interleaveIndex = grammar.m_TokenInterleaveIndex;

  GenerateCodeC(fileoutSource,
    fileOutNameDfa,
    grammar.GetLanguageName(),
    grammar.GetModuleName(),
    true,
    grammar.m_TokenNames,
    intCharToIntMapOut,
    finaltokens,
    out,
    grammar.m_TransitionSet,
    interleaveIndex,
    L"Tokens",
    L"TK");

  GenerateHeaderC(fileoutHeader,
    fileOutNameDfa,
    grammar.GetLanguageName(),
    grammar.GetModuleName(),
    true,
    grammar.m_TokenNames,
    intCharToIntMapOut,
    finaltokens,
    out,
    grammar.m_TransitionSet,
    interleaveIndex,
    L"Tokens",
    L"TK");

}

void GenerateScanner(Grammar& grammar, 
                     const std::wstring& fileOutNameDfa,
                     OutputType type)
{
  ////////////////////////////
  //

  MapStateToInt ends;
  CreateMainNFA(grammar.m_pStartNfaState,
                grammar.m_Expressions,
                grammar.m_NfaStates,
                grammar.m_TransitionSet,
                ends);

  PrintLn(L"Converting NFA to DFA...");
  OutputMap out(ends);
  MakeDStates(grammar.m_pStartNfaState, grammar.m_TransitionSet, out, ends);
  PrintLn("Minimizing DFA...");

  ReduceDfa(out.map, out.map2, out.map3, grammar.m_TransitionSet);
  PrintLn("Minimizing DFA 7/6");

  PrintLn("Minimizing DFA end");
  
  if (type == OutputType_CPP)
  {
    GenerateCpp(grammar, fileOutNameDfa, out);
  }
  else if (type == OutputType_C)
  {
    GenerateC(grammar, fileOutNameDfa, out);
  }
  else if (type == OutputType_JS)
  {
    GenerateJs(grammar, fileOutNameDfa, out);
  }

  
}

