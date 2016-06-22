#pragma once
#include "expression.h"
#include "SetOfInts.h"



std::wstring ToCharRepresentation(wchar_t ch);

void GenerateCodeJs(std::wostream& fileout,
                  const std::wstring& fileOutName,
                  const std::wstring& grammarName,
                  const std::wstring& moduleName,
                  bool reduceswitch,
                  std::vector<std::wstring>& tokensNames,
                  MapIntCharToInt& intCharToIntMapOut,
                  MapIntToInt& finaltokens,
                  OutputMap& out,
                  const TransitionSet& symbols,
                  const int& interleaveIndex,
                  const std::wstring& enumName,
                  const std::wstring& tokenPrefix);


void GenerateHeaderJs(std::wostream& fileout,
  const std::wstring& fileOutName,
  const std::wstring& grammarName,
  const std::wstring& moduleName,
  bool reduceswitch,
  std::vector<std::wstring>& tokensNames,
  MapIntCharToInt& intCharToIntMapOut,
  MapIntToInt& finaltokens,
  OutputMap& out,
  const TransitionSet& symbols,
  const int& interleaveIndex,
  const std::wstring& enumName,
  const std::wstring& tokenPrefix);


