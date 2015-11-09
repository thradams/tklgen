#include "StdAfx.h"
#include "Algorithms.h"
#include "Production.h"
#include "Grammar.h"
#include <memory>
#include <set>
#include <map>
#include <stack>
using  std::wcout;
using  std::endl;



//Dado uma producao A -> x1 x2 ..xn
//retorna o first da sequencia da producao comecando em startindex
// por ex A - > x0 x1 x2
// se passar start = 1, vai retornar o first de FIRST(x1 x2)
std::set<const GrammarSymbol*> GetFirstSet(const FirstSets& first,
        const Grammar& g,
        const Production& prod,
        int startIndex = 0)
{
    std::set<const GrammarSymbol*> r;
    int epsilonCount = 0;

    for (int k = startIndex ; k < prod.GetNumOfRightSymbols(); k++)
    {
        const GrammarSymbol* pgs = prod.GetRightSymbol(k);

        // Put FIRST (Y1) - {epsilon} into FIRST (X)
        if (k == startIndex)
        {
            //Copia todos (-epsilon) de Y1first para Y1first(X)
            auto Y1first = first.Get(prod.GetRightSymbol(k));

            if (Y1first.find(g.epsilon()) != Y1first.end())
            {
                epsilonCount = 1;
            }

            for (auto it = Y1first.begin(); it != Y1first.end(); it++)
            {
                if (*it != g.epsilon())
                {
                    r.insert(*it);
                }
            }

            continue;
        }

        // O anterior tinha epsilon?
        auto YKfirst = first.Get(prod.GetRightSymbol(k - 1));

        if (YKfirst.find(g.epsilon()) != YKfirst.end())
        {
            epsilonCount++;
            //Copia todos (-epsilon) para first(X)
            auto Y1first = first.Get(prod.GetRightSymbol(k));

            for (auto it = Y1first.begin(); it != Y1first.end(); it++)
            {
                if (*it != g.epsilon())
                {
                    r.insert(*it);
                }
            }
        }
        else
        {
            //se o anterior nao tinha epsilon ja para
            break;
        }
    }

    // se todos eram epsilon entao adicionar epsilon TODO -1
    if (epsilonCount != 0 &&
            epsilonCount == (prod.GetNumOfRightSymbols() - startIndex))
    {
        r.insert(g.epsilon());
    }

    return r;
}


//controi todos os first(A) first(a) da gramatica g
FirstSets BuildFirstSets3(const Grammar& g)
{
    FirstSets  first;

    //if X is a terminal: FIRST (X) = X
    //X is epsilon: FIRST (X) = epsilon
    for (int k = 0 ; k < g.GetNumOfGrammarSymbols(); k++)
    {
        if (g.GetSymbol(k)->IsTerminal())
        {
            std::set<const GrammarSymbol*> fs;
            fs.insert(g.GetSymbol(k));
            first.AddSet(g.GetSymbol(k), std::move(fs));
        }
    }

    bool added = true;

    while (added)
    {
        added = false;

        for (int i = 0 ; i < g.GetNumOfProductions(); i++)
        {
            const Production& production  = g.GetProduction(i);
            const GrammarSymbol* X = production.GetLeftSymbol();
            int epsilonCount = 0;

            for (int k = 0 ; k < production.GetNumOfRightSymbols(); k++)
            {
                // Put FIRST (Y1) - {epsilon} into FIRST (X)
                if (k == 0)
                {
                    //Copia todos (-epsilon) de Y1first para Y1first(X)
                    auto Y1first = first.CreateGet(production.GetRightSymbol(k));

                    if (Y1first.find(g.epsilon()) != Y1first.end())
                    {
                        epsilonCount = 1;
                    }

                    for (auto it = Y1first.begin(); it != Y1first.end(); it++)
                    {
                        if (*it != g.epsilon())
                        {
                            const GrammarSymbol* ptemp = *it;
                            added = added || first.AddToSet(X, ptemp);
                        }
                    }

                    continue;
                }

                // O anterior tinha epsilon?
                auto YKfirst = first.CreateGet(production.GetRightSymbol(k - 1));

                if (YKfirst.find(g.epsilon()) != YKfirst.end())
                {
                    epsilonCount++;
                    //Copia todos (-epsilon) para first(X)
                    auto Y1first = first.CreateGet(production.GetRightSymbol(k));

                    for (auto it = Y1first.begin(); it != Y1first.end(); it++)
                    {
                        if (*it != g.epsilon())
                        {
                            const GrammarSymbol* ptemp = *it;
                            added = added || first.AddToSet(X,  ptemp);
                        }
                    }
                }
                else
                {
                    //se o anterior nao tinha epsilon ja pode parar
                    break;
                }
            }

            // se todos eram epsilon entao adicionar epsilon TODO -1
            if (epsilonCount != 0 &&
                    epsilonCount == production.GetNumOfRightSymbols())
            {
                added = added || first.AddToSet(X,  g.epsilon());
            }
        }
    }

    return first;
}

//controi todos os follow(A) da gramatica g
FollowSets BuildFolowSets3(Grammar& g,  const FirstSets& first)
{
    FollowSets follow;
    //Follow(start-symbol):= {$};
    follow.AddToSet(g.GetStartSymbol(), g.endmarker());

    //for all nonterminals A != start-symbol do Follow(A):={};
    for (int i = 0 ; i < g.GetNumOfGrammarSymbols(); i++)
    {
        const GrammarSymbol* pgs = g.GetSymbol(i);

        if (!pgs->IsTerminal() && pgs != g.GetStartSymbol())
        {
            follow.CreateGet(pgs);
        }
    }

    bool changed = true;

    while (changed)
    {
        changed = false;

        // Para cada producao A -> X1...Xn
        for (int k = 0 ; k < g.GetNumOfProductions(); k++)
        {
            const Production& A = g.GetProduction(k);
            Print(std::wcout, A);
            std::wcout << std::endl;

            for (int i = 0 ; i < A.GetNumOfRightSymbols(); i++)
            {
                //If there is a production A -> alfa B beta , then everything in FIRST(beta) except epsilon
                //is in FOLLOW(B).
                const GrammarSymbol* Xi = A.GetRightSymbol(i);

                //Para cada nao terminal Xi da producao
                if (!Xi->IsTerminal())
                {
                    //add First(Xi+1..Xn) - {epsilon} to Follow(Xi)
                    std::set<const GrammarSymbol*> firstXi1_Xn;
                    bool epsilon_is_in_firstXi1_Xn = false;

                    //se Xi não é o ultimo
                    if (i < (A.GetNumOfRightSymbols() - 1))
                    {
                        firstXi1_Xn = GetFirstSet(first, g, A, i + 1);

                        for (auto it = firstXi1_Xn.begin(); it != firstXi1_Xn.end(); ++it)
                        {
                            if (*it != g.epsilon())
                            {
                                if (follow.AddToSet(Xi, *it))
                                {
                                    changed = true;
                                }
                            }
                            else
                            {
                                epsilon_is_in_firstXi1_Xn = true;
                            }
                        }
                    }

                    // If there is a production A -> alfaB, or a production A -> alfa B beta , where
                    // FIRST(beta) contains epsilon, then everything in FOLLOW (A) is in FOLLOW (B) .
                    if (i == (A.GetNumOfRightSymbols() - 1) ||
                            epsilon_is_in_firstXi1_Xn)
                    {
                        //add Follow(A) to Follow(Xi)
                        auto FollowA = follow.Get(A.GetLeftSymbol());

                        for (auto it = FollowA.begin(); it != FollowA.end(); ++it)
                        {
                            if (follow.AddToSet(Xi, *it))
                            {
                                changed = true;
                            }
                        }
                    }
                }//para este Xi
            } //para cada Xi
        } //para cada producao
    } //enquando mudar...

    return follow;
}





std::wostream& PrintFollow(std::wostream& os, const FirstSets& s)
{
    auto it = s.cbegin();

    for (; it != s.cend(); it++)
    {
        os << L"Follow(" << it->first->GetName() << L") = ";
        auto it2 = it->second.cbegin();

        for (; it2 != it->second.cend(); it2++)
        {
            os << (*it2)->GetName() << L" ";
        }

        os <<  L"" << std::endl;
    }

    return os;
}


std::wostream& PrintFirst(std::wostream& os, const FirstSets& s)
{
    for (auto it = s.cbegin(); it != s.cend(); it++)
    {
        if (it->first->IsTerminal())
        {
            os << L"First(" << it->first->GetName() << L") = ";
            auto it2 = it->second.cbegin();

            for (; it2 != it->second.cend(); it2++)
            {
                os << (*it2)->GetName() << L" ";
            }

            os <<  L"" << std::endl;
        }
    }

    os <<  L"----" << std::endl;

    for (auto it = s.cbegin(); it != s.cend(); it++)
    {
        if (!it->first->IsTerminal())
        {
            os << L"First(" << it->first->GetName() << L") = ";
            auto it2 = it->second.cbegin();

            for (; it2 != it->second.cend(); it2++)
            {
                os << (*it2)->GetName() << L" ";
            }

            os <<  L"" << std::endl;
        }
    }

    return os;
}



void Print(std::wostream& os, const MKey& k)
{
    os << L"[" << k.m_pNotTerminal->GetName() << L", "
       << k.m_pTerminal->GetName()
       << L"]";
}


void Print(std::wostream& os, MMap& map, Grammar& g)
{
    for (auto it = map.begin(); it != map.end(); it++)
    {
       os << L"[" << it->m_pNotTerminal->GetName() << L", "
       << it->m_pTerminal->GetName()
       << L"]";

        //Print(os, it->m_pTerminal);
        os << L" : " ;
        const Production& production = g.GetProduction(it->m_RuleIndex);
        Print(os , production);
        os << std::endl;
    }
}


MMap BuildMTable(const FirstSets& first,
                 const FollowSets& follow,
                 Grammar& g)
{
    MMap M;
    std::wcout << L"\n Building M table \n";

    for (int k = 0; k < g.GetNumOfProductions(); k++)
    {
        //Para cada producao da gramatica A = alfa
        const Production& production = g.GetProduction(k);
        Print(std::wcout, production);
        //Aqui o first tem que ser em relacao a "alfa" e não a "A"
        // pois o A engloba todos os "firsts" e aqui se quer o first especifico
        // desta producao
        // Entao o FirstSets& first é o "pior caso" o first de qualquer "A"
        // No aho novo parece que tem um defeito de escrita e que o first deveria
        // ser first alfa no dois (esta so no segundo)
        //Nao testei se o follow teria que ter algo assim
        std::set<const GrammarSymbol*> f = GetFirstSet(first, g, production);

        //Regra 1
        //auto f = first.Get(production.GetLeftSymbol());
        for (auto it = f.begin(); it != f.end(); ++it)
        {
            // Para cada terminal a em primeiro(A)
            const GrammarSymbol* pgs = (*it);

            if (pgs->IsTerminal() && pgs != g.epsilon())
            {
                //M[A, a] = alfa
                std::wcout << L"[" << production.GetLeftSymbol()->GetName() << L"," <<
                           pgs->GetName() << L"] = " ;
                Print(std::wcout, production);

                /*if (M.find(MKey(production.GetLeftSymbol(), pgs)) != M.end())
                {
                    std::wcout << L"<-- duplicated" << std::endl;
                    throw std::exception("multiple entries");
                }*/
                
                if (M.find(MKey(production.GetLeftSymbol(), pgs, k)) != M.end())
                {
                    std::wcout << L"<-- duplicated" << std::endl;
                    throw std::exception("multiple entries");
                }

                std::wcout << std::endl;
                M.insert(MKey(production.GetLeftSymbol(), pgs, k));
                //M[MKey(production.GetLeftSymbol(), pgs)] = k;
            }
            else if (pgs == g.epsilon())
            {
                // Nao existe epsilon o input stream
                // entao vou fazer para cada follow
                auto fo = follow.Get(production.GetLeftSymbol());
                //se esta em folow

                for (auto it = fo.begin(); it != fo.end(); ++it)
                {
                    const GrammarSymbol* b = (*it);

                    if (b->IsTerminal() && b != g.epsilon())  //ou $ que da no mesmo
                    {
                        std::wcout << L"[" << production.GetLeftSymbol()->GetName() << L"," <<
                                   b->GetName() << L"] = " ;
                        Print(std::wcout, production);


                        if (M.find(MKey(production.GetLeftSymbol(), b, k)) != M.end())
                        {
                            std::wcout << L"<-- duplicated" << std::endl;
                            throw std::exception("multiple entries");
                        }

                        std::wcout << std::endl;
                        M.insert(MKey(production.GetLeftSymbol(), b, k));
                        //M[MKey(production.GetLeftSymbol(), b)] = k;
                    }
                }
            }
        }
    }

    return M;
}

MMap MakeLLTables(Grammar& g)
{
    const GrammarSymbol* pMain =  g.FindSymbol(L"Main");

    if (pMain == nullptr)
    {
        throw std::exception("Main syntax must be defined!");
    }

    if (g.GetStartSymbol() == nullptr)
    {
        throw std::exception("Main syntax must be defined! (start)");
    }

    const FirstSets first = BuildFirstSets3(g);
    const FollowSets follow = BuildFolowSets3(g, first);
    PrintFirst(std::wcout, first);
    std::wcout << std::endl;
    PrintFollow(std::wcout, follow);
    MMap M = BuildMTable(first, follow, g);
    std::wcout << L"M table" << std::endl;
    Print(std::wcout, M, g);
    return M;
}

//http://smlweb.cpsc.ucalgary.ca/

