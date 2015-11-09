#include "StdAfx.h"
#include "LLOutputC.h"
#include <iostream>
#include <sstream>
#include <set>
#include "Grammar.h"
#include <map>
#include "CommonOutput.h"

#define TAB_1    L"    "
#define TAB__2   L"        "
#define TAB___3  L"            "
#define TAB____4 L"                "

#define MM L"/*" << __LINE__ << L"*/"

static std::wostream& PrintProduction(std::wostream& os,
    const Production& production,
    Grammar& g,
    const std::wstring& tokenprefix,
    const std::wstring& tabs)
{
    int i = 0;
    int actionIndexAtProduction = 0;

    int actionIndex = production.GetAction(actionIndexAtProduction);

    if (actionIndex >= 0)
    {
        //os << tabs << L"ctx->actions->" << g.GetActionName(actionIndex) << L"(ctx);\n";
        os << tabs << L"IFER(ctx->on_action(" << g.GetActionName(actionIndex) << L", ctx));\n";
    }

    actionIndexAtProduction++;
    int numberOfItems = 0;
    for (; i < production.GetNumOfRightSymbols(); i++)
    {
        const GrammarSymbol* pS = production.GetRightSymbol(i);

        if (pS->IsTerminal())
        {
            if (pS != g.epsilon())
            {
                os << tabs << L"IFER(Match(ctx, " << tokenprefix << production.GetRightSymbol(i)->GetName() << L"));\n";
                numberOfItems++;
            }
            else
            {
                //os << L"ctx.Match(" << production.GetRightSymbol(i)->GetName() << L");\n";
            }
        }
        else
        {
            os << tabs << L"IFER(Parse_" << production.GetRightSymbol(i)->GetName() << L"(ctx));\n";
            numberOfItems++;
        }

        actionIndex = production.GetAction(actionIndexAtProduction);

        if (actionIndex >= 0)
        {
            //os << tabs << L"IFER(ctx->actions->" << g.GetActionName(actionIndex) << L"(ctx));\n";
            //os << tabs << L"IFER(" << g.GetActionName(actionIndex) << L"(ctx));\n";
            os << tabs << L"IFER(ctx->on_action(" << g.GetActionName(actionIndex) << L", ctx));\n";
        }

        actionIndexAtProduction++;
    }

    actionIndex = production.GetAction(actionIndexAtProduction);

    if (actionIndex >= 0)
    {
        //os << tabs << L"IFER(ctx->actions->" << g.GetActionName(actionIndex) << L"(ctx));\n";
        //os << tabs << L"IFER(" << g.GetActionName(actionIndex) << L"(ctx));\n";
        os << tabs << L"IFER(ctx->on_action(" << g.GetActionName(actionIndex) << L", ctx));\n";

    }

    if (numberOfItems == 0)
    {
        os << tabs << L"return ResultCodeEmpty; /*opt*/\n";
    }
    return os;
}


static std::wostream& PrintFowardDeclarations(std::wostream& os, Grammar& g, MMap& map)
{
    os << L"/*forward declarations*/ \n";
    int i = 0;
    int sub = 0;
    int currentRuleIndex = -1;

    for (auto it = map.begin(); it != map.end(); it++)
    {
        if (currentRuleIndex != it->m_pNotTerminal->GetIndex())
        {
            //mudou
            currentRuleIndex = it->m_pNotTerminal->GetIndex();
            os << L"ResultCode Parse_" << it->m_pNotTerminal->GetName() << L"( " << g.GetLanguageName() << L"_Context* ctx);\n";
            sub = 0;
        }

        sub++;
        i++;
    }

    os << L"\n\n";
    return os;
}


static void PrintActions(std::wostream& os, Grammar& g, bool virt)
{
#if 0
    os << L"/*Forward declarations for actions*/\n";
    os << L"struct Eval_Context;\n";
    os << L"\n";
    os << L" " << g.GetLanguageName() << L"_Actions{\n";
    const wchar_t * psz0 =
        L"    int (*{ACTION})( {LANG}_Context* ctx);\n";
    std::wstring ws0(psz0);
    find_replace(ws0, L"{LANG}", g.GetLanguageName());

    for (int i = 0; i < g.GetNumberOfActions(); i++)
    {
        std::wstring ws(ws0);
        find_replace(ws, L"{ACTION}", g.GetActionName(i));
        os << ws;
    }
    os << L"};\n";
#else

    os << L"enum " << g.GetLanguageName() << L"_Actions\n";
    os << L"{\n";

    for (int i = 0; i < g.GetNumberOfActions(); i++)
    {
        os << L"  " << g.GetActionName(i) << L",\n";
    }
    os << L"  OnError\n";
    os << L"};\n";


#endif
}


static void PrintActionsNames(std::wostream& os, Grammar& g, bool virt)
{
    os << L"//\n";

    os << L"const wchar_t* " << g.GetLanguageName() << L"_Actions_Text(";

    os << L"enum " << g.GetLanguageName() << L"_Actions e)\n";

    os << L"{\n";
    os << L"    switch(e)\n";
    os << L"    {\n";
    for (int i = 0; i < g.GetNumberOfActions(); i++)
    {
        os << L"        case  " << g.GetActionName(i) << L": return L\"" << g.GetActionName(i) << L"\";\n";
    }
    os << L"    }\n";
    os << L"    return L\"\";\n";
    os << L"};\n";


}
#define STR_HEADER \
    L"\n"\
    L"{ACTIONS}"\
    L"\n"\
    L"const wchar_t* {GRAMMAR}_Actions_Text(enum {GRAMMAR}_Actions e);\n"\
    L"typedef int (*{GRAMMAR}_OnAction)(enum {GRAMMAR}_Actions a, struct {GRAMMAR}_ContextT* ctx);\n"\
    L"\n"\
    L"#define LEXEME_MAX 400\n"\
    L"\n"\
    L"typedef struct {GRAMMAR}_ContextT\n"\
    L"{\n"\
    L"    wchar_t* lexeme;\n"\
    L"    int lexemeSize;\n"\
    L"    enum {GRAMMAR}_Tokens token;\n"\
L"    struct sstream* stream;\n"\
L"    {GRAMMAR}_OnAction   on_action;\n"\
L"} {GRAMMAR}_Context;\n"\
L"\n"\
L"\n"\
L"int {GRAMMAR}_Parse(struct sstream* ss, {GRAMMAR}_OnAction action, void* context);\n"\
L"int {GRAMMAR}_Parse_Str(const wchar_t* text, {GRAMMAR}_OnAction action, void* context);\n"\
L"\n"\
L"\n"


static void PrintOutputInterface(std::wostream& os, Grammar& g)
{
    std::wstring ws(STR_HEADER);

    std::wstring upperfile = MakeUpper(g.GetModuleName() + L".h");
    FindReplace(upperfile, L".", L"_");

    PrintGeneratedFileHeader(os);

    os << L"#ifndef " << upperfile << "" << std::endl;
    os << L"#define " << upperfile << "" << std::endl;
    os << std::endl;
    os << std::endl;

    find_replace(ws, L"{DATE}", __WDATE__);
    find_replace(ws, L"{GRAMMAR}", g.GetLanguageName());

    std::wostringstream ss;

    PrintActions(ss, g, false);
    find_replace(ws, L"{ACTIONS}", ss.str());

    os << ws;

    os << std::endl;
    os << L"#endif" << std::endl;
}

static int PreAnalise(MMap& map, MMap::iterator it, bool& allsame)
{
    int currentRuleIndex = it->m_pNotTerminal->GetIndex();
    int currentRule = it->m_RuleIndex;

    //Faz todos desta regra (até ela mudar)

    if (it == map.end())
    {
        return 0;
    }

    int sub = 0;
    allsame = true;

    while (it->m_pNotTerminal->GetIndex() == currentRuleIndex)
    {
        if (currentRule != it->m_RuleIndex)
        {
            allsame = false;
        }

        sub++;
        it++;

        if (it == map.end())
        {
            break;
        }
    }

    //retorna quantos tem destra regra
    return sub;
}


void GenerateDescRecHeaderC(std::wostream& os,
    Grammar& g,
    MMap& map,
    const std::wstring& tokenPrefix)
{
    PrintOutputInterface(os, g);
}

#define SourceCode \
L"\n"\
L"#include <assert.h>\n"\
L"\n"\
L"#include <stdlib.h>\n"\
L"#include <stdio.h>\n"\
L"#include <wchar.h>\n"\
L"#include <stdbool.h>\n"\
L"#include \"errors.h\"\n"\
L"\n"\
L"inline bool IsError(ResultCode e)\n"\
L"{\n"\
L"    if ((e != ResultCodeSuccess &&\n"\
L"    e != ResultCodeEmpty))\n"\
L"    {\n"\
L"        return true;\n"\
L"    }\n"\
L"\n"\
L"    return false;\n"\
L"}\n"\
L"\n"\
L"#define IFER(x) if (IsError(x)) return ResultCodeError;\n"\
L"\n"\
L"static ResultCode resize_lexeme({GRAMMAR}_Context* p, size_t sz)\n"\
L"{\n"\
L"    wchar_t *newlexeme = (wchar_t*)malloc(sizeof(wchar_t) * sz);\n"\
L"    if (newlexeme == NULL)\n"\
L"    {\n"\
L"        return ResultCodeOutOfMem;\n"\
L"    }\n"\
L"\n"\
L"    if (p->lexeme)\n"\
L"    {\n"\
L"        free(p->lexeme);\n"\
L"        p->lexemeSize = 0;\n"\
L"    }\n"\
L"\n"\
L"    p->lexeme = newlexeme;\n"\
L"    p->lexemeSize = sz;\n"\
L"    p->lexeme[0] = L'\\0';\n"\
L"\n"\
L"    return ResultCodeSuccess;\n"\
L"}\n"\
L"\n"\
L"static ResultCode {GRAMMAR}_Context_Init({GRAMMAR}_Context* p, int initialLexemeSize)\n"\
L"{\n"\
L"    p->lexemeSize = 0;\n"\
L"    p->lexeme = 0;\n"\
L"\n"\
L"    ResultCode r = resize_lexeme(p, initialLexemeSize);\n"\
L"    if (r == ResultCodeSuccess)\n"\
L"    {\n"\
L"        p->lexeme[0] = L'\\0';\n"\
L"        p->stream = 0;\n"\
L"        p->token = (enum {GRAMMAR}_Tokens) 0;\n"\
L"    }\n"\
L"    \n"\
L"    return r;\n"\
L"}\n"\
L"\n"\
L"static void {GRAMMAR}_Context_destroy({GRAMMAR}_Context* p)\n"\
L"{\n"\
L"    p->lexemeSize = 0;\n"\
L"    free(p->lexeme);\n"\
L"}\n"\
L"\n"\
L"static ResultCode ReadNextToken({GRAMMAR}_Context* context)\n"\
L"{\n"\
L"    return {GRAMMAR}_NextToken(context->stream,\n"\
L"                          &context->lexeme,\n"\
L"                          &context->lexemeSize,\n"\
L"                          &context->token);\n"\
L"}\n"\
L"\n"\
L"static ResultCode Match({GRAMMAR}_Context* context, enum {GRAMMAR}_Tokens tk)\n"\
L"{\n"\
L"    if (tk != context->token)\n"\
L"    {\n"\
L"        return ResultCodeUnexpectedToken;\n"\
L"    }\n"\
L"\n"\
L"    ResultCode r = ReadNextToken(context);\n"\
L"\n"\
L"    if (r != ResultCodeSuccess &&\n"\
L"    r != ResultCodeEof)\n"\
L"    {\n"\
L"        return r;\n"\
L"    }\n"\
L"\n"\
L"    return ResultCodeSuccess;\n"\
L"}\n"\
L"\n"\
L"ResultCode Parse_Main({GRAMMAR}_Context* ctx);\n"\
L"ResultCode {GRAMMAR}_Parse(struct sstream* ss,\n"\
L"                      {GRAMMAR}_OnAction action, void* root)\n"\
L"{\n"\
L"    {GRAMMAR}_Context ctx;\n"\
L"    ResultCode r = {GRAMMAR}_Context_Init(&ctx, 200);\n"\
L"    if (r == ResultCodeSuccess)\n"\
L"    {\n"\
L"        ctx.stream = ss;\n"\
L"        ctx.on_action = action;\n"\
L"        r = ReadNextToken(&ctx);\n"\
L"\n"\
L"        if (r == ResultCodeSuccess)\n"\
L"        {\n"\
L"            r = Parse_Main(&ctx);\n"\
L"        }\n"\
L"    }\n"\
L"\n"\
L"    {GRAMMAR}_Context_destroy(&ctx);    \n"\
L"    return r;\n"\
L"}\n"\
L"\n"\
L"ResultCode {GRAMMAR}_Parse_Str(const wchar_t* text,\n"\
L"                              {GRAMMAR}_OnAction action,\n"\
L"                              void* root,\n"\
L"                              int* line,\n"\
L"                              int* col)\n"\
L"{\n"\
L"    struct sstream ss;\n"\
L"    sstream_init(&ss, text);\n"\
L"    ResultCode r = {GRAMMAR}_Parse(&ss, action, root);\n"\
L"    *line = ss.line;\n"\
L"    *col = ss.col;\n"\
L"    return r;\n"\
L"}\n"\
L"\n"


void GenerateDescRecC(std::wostream& os,
    Grammar& g,
    MMap& map,
    const std::wstring& tokenPrefix,
    const std::wstring& parserFileSuffix)
{
    if (map.empty())
    {
        return;
    }

    PrintGeneratedFileHeader(os);
    PrintGeneratedFileLicense(os);


    os << L"\n";
    os << L"#include \"stdafx.h\"\n";
    os << L"#include <assert.h>\n";
    os << L"\n";
    os << L"#include \"" << g.GetModuleName() << L"Lex.h\"\n";
    os << L"#include \"" << g.GetModuleName() << parserFileSuffix << L".h\"\n";
    os << L"\n";
    os << L"\n";
    os << L"#include \"sstream.h\"\n";
    os << L"#include \"errors.h\"\n";
    os << L"\n";
    os << L"\n";
    PrintActionsNames(os, g, false);
    //PrintActions(os, g, false);
    os << L"\n";

    std::wstring ws(SourceCode);
    find_replace(ws, L"{GRAMMAR}", g.GetLanguageName());
    find_replace(ws, L"{MODULE}", g.GetLanguageName());

    os << ws;

    PrintFowardDeclarations(os, g, map);


    int i = 0;
    int currentRuleIndex = -1;
    auto it = map.begin();
    int rulecount = 0;

    for (; it != map.end();)
    {
        int currentRuleIndex = it->m_pNotTerminal->GetIndex();
        //Faz todos desta regra (até ela mudar)
        os << L"ResultCode " << "Parse_" << it->m_pNotTerminal->GetName() << L"( " << g.GetLanguageName() + L"_Context* ctx)\n";
        os << L"{\n";

        os << TAB_1 << L"enum " << g.GetLanguageName() << L"_Tokens token = ctx->token; \n";
        os << L"\n";

        int sub = 0;
        rulecount = 0;

        while (it->m_pNotTerminal->GetIndex() == currentRuleIndex)
        {
            int currentResultIndex = it->m_RuleIndex;
            //faz todos que resultam na mesma producao
            int count = 0;

            while (currentResultIndex == it->m_RuleIndex)
            {
                if (count == 0)
                {
                    os << TAB_1;

                    if (rulecount > 0)
                    {
                        os << L"else ";
                    }

                    os << L"if (token == " << tokenPrefix << it->m_pTerminal->GetName();
                }
                else
                {
                    os << L" ||\n";
                    os << TAB_1 << L"    token == " << tokenPrefix << it->m_pTerminal->GetName();
                }

                auto itcopy = it;
                it++;
                count++;

                if (it == map.end() ||
                    currentResultIndex != it->m_RuleIndex)
                {
                    os << L")\n"; //fecha if
                    const Production& production = g.GetProduction(itcopy->m_RuleIndex);
                    os << TAB_1 << L"{\n";
                    os << TAB__2;
                    os << L"/*";
                    Print(os, production);
                    os << L"*/\n";
                    PrintProduction(os, production, g, tokenPrefix, TAB__2);
                    os << TAB_1 << L"}\n";
                    break;
                }

                if (it == map.end())
                {
                    break;
                }
            }

            /*   if (it != map.end())
               {
               it++;
               }
               */
            if (it == map.end())
            {
                break;
            }

            rulecount++;
        } //regra

        os << TAB_1 << L"else\n";
        os << TAB_1 << L"{\n";
        os << TAB__2 << L"ctx->on_action(OnError, ctx);\n";
        os << TAB__2 << L"return ResultCodeError;\n";
        os << TAB_1 << L"}\n";
        os << L"\n";
        os << TAB_1 << L"return ResultCodeSuccess;\n";
        os << L"}\n\n";
    }

}


static void GenerateDescRec3(std::wostream& os,
    Grammar& g,
    MMap& map,
    const std::wstring& tokenPrefix)
{
    if (map.empty())
    {
        return;
    }

    PrintGeneratedFileHeader(os);
    PrintGeneratedFileLicense(os);

    os << L"#pragma once\n";
    os << L"\n";
    os << L"#include <string.h>\n";

    os << L"#include \"" << g.GetModuleName() << L"Lex.h\"\n";
    os << L"\n";
    os << L"\n";

    //PrintOutputClass(os, g);
    PrintFowardDeclarations(os, g, map);
    int i = 0;
    int sub = 0;
    int currentRuleIndex = -1;
    auto it = map.begin();

    for (; it != map.end();)
    {
        int currentRuleIndex = it->m_pNotTerminal->GetIndex();
        //Faz todos desta regra (até ela mudar)
        os << TAB_1 << L"int " << "Parse_" << it->m_pNotTerminal->GetName() << L"( " << g.GetLanguageName() << L"_Context* ctx)\n";
        os << TAB_1 << L"{\n";
        int sub = 0;
        bool allsame = false;
        bool firstPrinted = false;

        if (PreAnalise(map, it, allsame) == 1)
        {
            //se so tem um nao precisa testar pelo token pois sempre haver um teste
            //a seguir de qualquer forma
            const Production& production = g.GetProduction(it->m_RuleIndex);
            os << TAB__2 << L"//";
            Print(os, production);
            os << L"\n";
            PrintProduction(os, production, g, tokenPrefix, TAB__2);
            it++;
        }
        else
        {
            while (it->m_pNotTerminal->GetIndex() == currentRuleIndex)
            {
                //se todos forem iguais nao testa pelo token
                //e so imprimi o primeiro que eh igual aos outros
                if (!allsame)
                {
                    if (sub == 0)
                    {
                        os << TAB__2 << L"if (IsToken(ctx, " << tokenPrefix << it->m_pTerminal->GetName() << L"))\n";
                    }
                    else
                    {
                        os << TAB__2 << L"else if (IsToken(ctx, " << tokenPrefix << it->m_pTerminal->GetName() << L"))\n";
                    }
                }

                const Production& production = g.GetProduction(it->m_RuleIndex);

                if (!allsame)
                {
                    os << TAB__2 << L"{\n";
                }

                if (!allsame || (allsame && !firstPrinted))
                {
                    os << (!allsame ? TAB___3 : TAB__2);
                    os << L"/*";
                    Print(os, production);
                    os << L"*/\n";
                    PrintProduction(os, production, g, tokenPrefix, !allsame ? TAB___3 : TAB__2);

                    if (!allsame)
                    {
                        os << TAB__2 << L"}\n";
                    }
                }

                sub++;
                it++;

                if (it == map.end())
                {
                    break;
                }

                firstPrinted = true;
            }
        }

        os << TAB_1 << L"}\n\n";
    }


}

