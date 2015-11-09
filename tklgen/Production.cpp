#include "StdAfx.h"
#include "Production.h"
#include "Grammar.h"
#include "console.h"

Production& Production::AddNextAction(const std::wstring & s)
{
    for (size_t i = m_Actions.size(); i < m_RightSymbol.size(); i++)
    {
        m_Actions.push_back(-1);//nada
    }

    int k = -1;
    if (!s.empty())
    {        
     k = m_pGrammar->AddAction(s);
    }

    m_Actions.push_back(k);//nada

    return *this;
}

//Production& Production::AddNextSymbol(const std::wstring & s, bool isTerminal)
//{
  //  const GrammarSymbol* pRightSymbol = m_pGrammar->AddSymbol(s, isTerminal);
   // return AddNextSymbol(pRightSymbol);    
//}

Production& Production::AddNextSymbol(const GrammarSymbol* pRightSymbol)
{
    m_RightSymbol.push_back(pRightSymbol);  
    return *this;
}


std::wostream& Print(std::wostream& os,
	                 const Production& production, 
					 int dot /*= -1*/)
{
	os << production.GetLeftSymbol()->GetName() << L" => ";

	int i= 0;
	for ( ; i < production.GetNumOfRightSymbols(); i++)
	{
	  if (dot == i)
		  os << L'.';

    if (production.GetRightSymbol(i)->IsTerminal())
    {
      auto old = console::textcolor(console::YELLOW);
      os << production.GetRightSymbol(i)->GetName();	
      console::textcolor(old);
    }
    else
    {
      os << production.GetRightSymbol(i)->GetName();	
    }
	  

	  if (dot != i + 1)
		  os << L' ';
	}
	if (dot == i)
		  os << L'.';
	
	return os;
}


