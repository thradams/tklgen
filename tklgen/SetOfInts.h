#pragma once
#include <set>

#include <iostream>
#include <algorithm>
#include <memory>
#include "expression.h"
#include <map>

typedef std::set<int> SetOfInts;
typedef std::shared_ptr<std::set<int>> SetOfIntsPtr;

//////////////////////////////////////////////
struct Dstates
{
  typedef std::pair<StateVector*, bool >  Pair;
  typedef std::vector< Pair >::iterator Iterator;

  std::vector< Pair > states;

  ~Dstates()
  {
    Iterator it = states.begin();

    for (; it != states.end(); it++)
    {
      delete it->first;
    }
  }

  void Add(StateVector* u)
  {
    states.push_back(std::make_pair(u, false));
    std::sort(states.back().first->begin(), states.back().first->end());
  }

  int Has(StateVector& u)
  {
    std::sort(u.begin(), u.end());
    int i = 0;
    Iterator it = states.begin();

    for (; it != states.end(); it++)
    {
      if (*it->first == u)
        return i;

      i++;
    }

    return -1;
  }

  int FindUnmarked()
  {
    int i = 0;
    Iterator it = states.begin();

    for (; it != states.end(); it++)
    {
      if (!it->second)
        return i;

      i++;
    }

    return -1;
  }
};


struct Map
{
  typedef std::map<SetOfIntsPtr, SetOfIntsPtr> TMap;
  TMap map;

  ~Map()
  {
    Clear();
  }

  void Set(const SetOfIntsPtr& p, int s)
  {
    TMap::iterator it = map.find(p);

    if (it == map.end())
    {
      SetOfIntsPtr st = std::make_shared<SetOfInts>();
      st->insert(s);
      map.insert(make_pair(p, st));
    }
    else
    {
      it->second->insert(s);
    }
  }

  void Clear()
  {
    map.clear();
  }

  SetOfIntsPtr Get(const SetOfIntsPtr& p)
  {
    TMap::iterator it = map.find(p);

    if (it == map.end())
      return nullptr;

    return it->second;
  }

  bool HasEmptyState()
  {
    if (GetSize() == 0)
      return false;

    TMap::iterator it = map.begin();
    return it->first == 0;
  }

  int GetSize() const
  {
    return map.size();
  }


  void PushInto(std::vector<SetOfIntsPtr>& arrGroup)
  {
    TMap::iterator it = map.begin();

    for (; it != map.end(); ++it)
    {
      arrGroup.push_back(it->second);
    }
  }

};



struct Output
{
  virtual void Last() {}
  virtual void EndState(int, int tokenid) {}
  virtual void Result(int from, const Transition& ch, int to)
  {
    //std::wcout << (wchar_t)(L'A' + from) << L"-" << ch.m_ch  << L"->" << (wchar_t)(L'A' + to) << std::endl;
    //std::wcout << from << L"-" << ch.m_ch << L"->" << to << std::endl;
  }
};


struct OutputMap : public Output
{
  MapIntCharToInt map;
  MapIntToCharInt map2;
  MapIntToInt map3;
  std::vector<int> tos;
  MapStateToInt& ends;

  OutputMap(MapStateToInt& e) : ends(e)
  {
  }

  virtual void EndState(int i, int tokenid)
  {
    map3.insert(std::make_pair(i, tokenid));
    //std::wcout << L"end:" << i << L" tokenid: " << tokenid << std::endl;
  }
  virtual void Result(int from, const Transition& ch, int to)
  {
    if (map2.find(to) == map2.end())
      tos.push_back(to);

    map.insert(std::make_pair(std::make_pair(from, ch.m_ch), to));
    map2.insert(std::make_pair(from, std::make_pair(ch.m_ch, to)));
    //std::wcout << (wchar_t)(L'A' + from) << L"-" << ch.m_ch  << L"->" << (wchar_t)(L'A' + to) << std::endl;
    //    std::wcout << from << L"-" << ch.m_ch << L"->" << to << std::endl;
  }

  void Last()
  {
    //o ultimo estado To pode ficar sem estar no mapa.
    //os outros algorimos vao precisar dele, e ele fica como "perdido" pois dele nao sai nada
    for (int k = 0; k < (int) tos.size(); k++)
    {
      if (map2.find(tos[k]) == map2.end())
      {
        //esta nos ends?
        // MapStateToInt::iterator it2 = ends.find(tos[k]);
        //if (it2 != ends.end())
        //{
        //map3.insert(make-pair(toks[k], it2->second))
        //}
        map2.insert(std::make_pair(tos[k], std::make_pair(EmptyTransition, tos[k])));
        map.insert(std::make_pair(std::make_pair(tos[k], EmptyTransition), tos[k]));
      }
    }
  }
};
