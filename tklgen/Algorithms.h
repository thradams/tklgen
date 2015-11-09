#pragma once

#include "GrammarSymbol.h"
#include <vector>
#include <set>
#include <map>


class Grammar;
class GrammarSymbol;
class Production;
class Item;
class SetOfItems;
class CanonicalSets;


class MapSymbolToSet
{
  typedef std::map<const GrammarSymbol*, std::set<const GrammarSymbol*>> Map;
  Map m_map;

public:

  typedef std::map<const GrammarSymbol*, std::set<const GrammarSymbol*>> Map;
  typedef Map::const_iterator const_iterator;

  Map::const_iterator cbegin() const
  {
    return m_map.cbegin();
  }

  Map::const_iterator cend() const
  {
    return m_map.cend();
  }

  bool AddSet(const GrammarSymbol* pgs, std::set<const GrammarSymbol*> && s)
  {
    return m_map.insert(Map::value_type(pgs, std::move(s))).second;
  }

  bool AddToSet(const GrammarSymbol* pgs, const GrammarSymbol* pgs2)
  {
    return  m_map[pgs].insert(pgs2).second;
  }

  const std::set<const GrammarSymbol*>& Get(const GrammarSymbol* pgs) const
  {
    Map::const_iterator it = m_map.find(pgs);

    if (it == m_map.end())
    {
      //            assert(false);
      throw std::exception("null");
    }

    return it->second;
  }

  const std::set<const GrammarSymbol*>& CreateGet(const GrammarSymbol* pgs)
  {
    return m_map[pgs];
  }
};

typedef MapSymbolToSet FirstSets;
typedef MapSymbolToSet FollowSets;

std::wostream& PrintFirst(std::wostream& os, const FirstSets&);
std::wostream& PrintFollow(std::wostream& os, const FollowSets&);

FirstSets  BuildFirstSets3(const Grammar& g);
FollowSets BuildFolowSets3(Grammar& g,  const FirstSets&);

std::set<const GrammarSymbol*> Follow(const Grammar& g, const GrammarSymbol* pgs);

//Algoritmo 4.4 pag 85
void Print(std::wostream& os, MMap& map, Grammar& g);
MMap MakeLLTables(Grammar& g);
