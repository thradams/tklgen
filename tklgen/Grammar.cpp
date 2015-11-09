#include "StdAfx.h"
#include "Grammar.h"
#include <string>
#include <map>
#include <iostream>


std::wostream& Print(std::wostream& os, const Grammar& g)
{
  os << L"Grammar (" << g.GetNumOfProductions() << L")" << std::endl;
  os << L"{" << std::endl;

  for (int i =0 ; i < g.GetNumOfProductions(); i++)
  {
	  const Production& production = g.GetProduction(i);
      os << L" ";
	  Print(os, production);
      os << std::endl;
  }
  os << L"}";
  os << std::endl;
  return os;
}


int CheckEmptyProduction(const Grammar& g)
{
    for (int k = 0 ; k < g.GetNumOfProductions(); k++)
    {
        const Production & production = g.GetProduction(k);
        if (production.GetNumOfRightSymbols() == 0)
            return k; 
    }
    return -1;
}

