#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <crtdbg.h>
#ifdef assert
#undef assert
#define assert(x) _ASSERTE(x)
#else
#define assert(x) _ASSERTE(x)
#endif

class GrammarSymbol
{
    std::wstring m_Name;
    bool m_IsTerminal;
    int m_Index;
    int m_DeclarationIndex;
public:


    GrammarSymbol(const std::wstring& name,
        bool isTerminal,
        int  i,
        int idecl) : m_Name(name), m_IsTerminal(isTerminal)
    {
        m_Index = i;
        m_DeclarationIndex = idecl;
    }

    ~GrammarSymbol(void)
    {
    }

    void SetIndex(int i)
    {
        m_Index = i;
    }

    int GetIndex() const
    {
        return m_Index;
    }

    int GetDeclarationIndex() const
    {
        return m_DeclarationIndex;
    }

    bool IsTerminal() const
    {
        //  if (m_Name == L"Epsilon")
        //        {
        //     assert(false);
        //        return false;
        //      }
        return m_IsTerminal;
    }



    const std::wstring& GetName() const
    {
        return m_Name;
    }
private:
    //bool operator ==(const GrammarSymbol& other) const;
    //{
    //   return GetName()==other.GetName() &&
    //           IsTerminal() == other.IsTerminal();
    //}
    //bool operator !=(const GrammarSymbol& other) const;
    //{
    //        return !operator ==(other);
    //  }
};

class GrammarSymbols
{
    std::vector<const GrammarSymbol*> m_items;
public:

    void Add(const GrammarSymbol* p)
    {
        m_items.push_back(p);
    }

    bool Has(const GrammarSymbol* pgs)
    {
        for (int i = 0; i < Count(); i++)
        {
            if (Get(i) == pgs)
            {
                return true;
            }
        }

        return false;
    }

    const GrammarSymbol* Get(int i) const
    {
        return m_items.at(i);
    }

    int Count() const
    {
        return (int) m_items.size();
    }
};





struct MKey
{
    const GrammarSymbol* m_pNotTerminal;
    const GrammarSymbol* m_pTerminal;
    int m_RuleIndex;
    MKey()
    {
        m_RuleIndex = -1;
        m_pNotTerminal = 0;
        m_pTerminal = 0;
    }

    MKey(const GrammarSymbol* pNotTerminal,
         const GrammarSymbol* pTerminal, 
         int ruleIndex)
    {
        m_pNotTerminal = pNotTerminal;
        m_pTerminal = pTerminal;
        m_RuleIndex = ruleIndex;
    }

    bool operator < (const MKey& other) const
    {
        if (m_pNotTerminal != other.m_pNotTerminal)
        {
            return m_pNotTerminal < other.m_pNotTerminal;
        }

        if (m_RuleIndex != other.m_RuleIndex)
        {
         return m_RuleIndex < other.m_RuleIndex;
        }
        
          return m_pTerminal < other.m_pTerminal;
    }
};

//A tabela "M" conforme chamada no aho
//associa :
 
//  Nao terminal  Terminal
//                |   a         |   b
//----------------------------------
//  X             |  regra prod |  regra
//  Y             |             |

typedef std::set<MKey> MMap;
