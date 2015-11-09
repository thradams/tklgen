#pragma once

#include "SetOfInts.h"
class Grammar;

//////////////////



void ReduceDfa(MapIntCharToInt& map,//int out
               MapIntToCharInt& map2,//int out
               MapIntToInt& map3, //in out
               const TransitionSet& symbols);

void FindClosure(NfaState* start,
                 const Transition& ch,
                 StateVector& states);

void FindClosure0(NfaState* s,
                  const Transition&  ch,
                  StateVector& states);

void E_ClosureT(const StateVector& T,
                StateVector& e_ClosureT);

SetOfIntsPtr FindGroup(const std::vector<SetOfIntsPtr>& groups,
                       int state);

void MakeDStates(NfaState* start,
                 const TransitionSet& symbols,
                 Output& out,
                 const MapStateToInt& ends);

void Move(const StateVector& T,
          const Transition& ch,
          StateVector& moveR);



void CreateMainNFA(NfaState* newStart,
                   std::vector<std::unique_ptr<Expression>>& expr,
                   NfaStates& states,
                   TransitionSet& symbols,
                   MapStateToInt& ends);


void GenerateScanner(Grammar& grammar,
  const std::wstring& fileOutNameDfa,
  bool bIsCpp);

////////////////