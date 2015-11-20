#include "StdAfx.h"
#include "LexOutputC.h"
#include <sstream>
#include "console.h"
#include "CommonOutput.h"
using namespace std;
using namespace console;

#define MM L""
//L"/*" << __LINE__ << L"*/"


struct SwitchBuilderC
{
    int currentCase;
    typedef std::map<wchar_t, int> Map;
    Map m_map;
    int anyCharStateTo;
    std::wostream& m_stream;
    int m_identationVal;
    std::wstring m_TokenPrefix;

    MapIntToInt& m_finalTokenState;
    const std::vector<std::wstring>& m_TokenNames;

    SwitchBuilderC(std::wostream& s,
                  MapIntToInt& finalTokenState,
                  const std::vector<std::wstring>& tokensNames,
                  int identationVal,
                  const std::wstring& tokenPrefix) : m_stream(s),
        m_finalTokenState(finalTokenState),
        m_TokenNames(tokensNames),
        m_identationVal(identationVal),
        m_TokenPrefix(tokenPrefix)
    {
        anyCharStateTo = -1;
        currentCase = -1;
    }
    ~SwitchBuilderC()
    {
        Flush();
    }

    void Add(int s1, wchar_t ch, int s2)
    {


        if (currentCase == -1 ||
                currentCase == s1)
        {
            currentCase = s1;

            if (ch == Transition::AnyChar)
            {
                anyCharStateTo = s2;
                return;
            }
            else
                m_map[ch] = s2;
        }
        else if (currentCase != s1)
        {
            Flush();
            currentCase = s1;

            if (ch == Transition::AnyChar)
            {
                anyCharStateTo = s2;
                return;
            }
            else
                m_map[ch] = s2;
        }
    }

    void Flush()
    {
        if (m_map.empty() && anyCharStateTo == -1)
            return;

        Ident spaces(m_identationVal);

        int count = 0;
        m_stream << spaces  << spaces << L"case " << currentCase << L":" << endl;

        Map::iterator firstIt = m_map.end();
        Map::iterator lastIt = m_map.end();

        Map::iterator it = m_map.begin();

        for (; it != m_map.end(); ++it)
        {
            if (firstIt == m_map.end())
            {
                firstIt = it;
                lastIt = it;
            }
            else if (it->first == lastIt->first + 1 &&
                     it->second == lastIt->second)
            {
                lastIt = it;
            }
            else
            {
                if (firstIt->first < TokenCharStart)
                {
                    //outupt
                    if (firstIt == lastIt)
                    {

                        m_stream <<  spaces << spaces;

                        if (count > 0)
                            m_stream << L"else " << MM;

                        m_stream << L"if (ch == " << ToCharRepresentation(firstIt->first) << L")" << endl;

                        m_stream << spaces << spaces << spaces;
                        m_stream << L"return " << firstIt->second << L";" << MM << endl;

                        count++;
                    }
                    else
                    {
                        m_stream << spaces << spaces;
                        if (count > 0)
                            m_stream << L"else ";
                        

                        m_stream << L"if (ch >= " << ToCharRepresentation(firstIt->first) << L" && ch <= " << ToCharRepresentation(lastIt->first) << L")" << endl;
                        m_stream << spaces << spaces << spaces << L"return " << firstIt->second << L";" << MM << endl;
                        count++;
                    }

                    firstIt = it;
                    lastIt = it;
                }
                else
                {
                    if (firstIt->first  != EmptyTransition)
                    {
                        const int tokenIndex = firstIt->first  - TokenCharStart;
                        m_finalTokenState.insert(make_pair(currentCase , tokenIndex));
                        //stream << L"    return -1;//" << tokensNames.at(tokenIndex) <<  endl;//este eh o estado final
                        m_stream << spaces << L"/*" << m_TokenPrefix << m_TokenNames.at(tokenIndex) << L"*/" << MM << endl;//este eh o estado final
                        // m_stream << L"        //A:" << tokenIndex <<  endl;//este eh o estado final
                    }
                    else
                    {
                        m_stream << spaces << spaces;
                        if (count > 0)
                            m_stream << L"else";

                        m_stream << L" return -1;" << MM << endl;
                    }
                }
            }

        }

        if (firstIt != m_map.end())
        {
            if (firstIt->first < TokenCharStart)
            {
                if (firstIt == lastIt)
                {
                    m_stream << spaces << spaces;
                    if (count > 0)
                        L"else ";

                    m_stream << L"if (ch == " << ToCharRepresentation(firstIt->first) << L")" << endl;
                    m_stream << spaces << L"return " << firstIt->second << L";" << MM << endl;
                    count++;
                }
                else
                {
                    m_stream << spaces << spaces;
                    if (count > 0)
                        m_stream << L"else ";

                    m_stream << L"if (ch >= " << ToCharRepresentation(firstIt->first) << L" && ch <= " << ToCharRepresentation(lastIt->first) << L")" << endl;
                    m_stream << spaces << spaces << spaces << L"return " << firstIt->second << L";" << MM << endl;
                    count++;

                }
            }
            else
            {
                if (firstIt->first  != EmptyTransition)
                {
                    const int tokenIndex = firstIt->first  - TokenCharStart;
                    m_finalTokenState.insert(make_pair(currentCase , tokenIndex));
                    //  stream << L"    return -1;//" << tokensNames.at(tokenIndex) <<  endl;//este eh o estado final
                    m_stream << spaces << spaces << L"/* end state for " << m_TokenPrefix << (m_TokenNames.at(tokenIndex)) << L"*/" << MM << endl;//este eh o estado final
                    //m_stream << L"        //B: " << tokenIndex <<  endl;//este eh o estado final
                }
                else
                {
                    m_stream << spaces << spaces << spaces << L"return -1; " << MM << endl;
                }
            }

            if (anyCharStateTo != -1)
            {
                m_stream << spaces << L"else" << endl;
                m_stream << spaces << spaces<< L"return " << anyCharStateTo << L"; /*any*/" << endl;
                count++;
            }

        }
        else
        {
            if (anyCharStateTo != -1)
            {
                m_stream << spaces << L"return " << anyCharStateTo << L"; /*any*/" << MM << endl;
                count++;
            }
            else
            {
                m_stream << spaces << L"return -1;" << MM << endl;
            }

        }

        m_stream << spaces << spaces  << L"break;" << MM << endl;
        m_map.clear();
        anyCharStateTo = -1;
    }

};

void MakeGetCharIndex(const TransitionSet& symbols,
                      std::wostream& stream,
                      bool code,
                      const std::wstring& grammarName)
{
    int index = 0;
    TransitionSet::const_iterator it = symbols.begin();

    if (code)
    {

        stream << L"int " << grammarName << "_GetCharIndex(wchar_t ch)" << endl;



        stream << L"{" << endl;
        stream << L"    switch (ch)" << endl;
        stream << L"    {" << endl;
    }
    else
    {
        stream << L"/*Symbols*/" << endl;
    }

    for (; it != symbols.end(); ++it)
    {
        if (it->m_ch != EmptyTransition)
        {
            if (code)
            {
                stream << L"        case " << ToCharRepresentation(it->m_ch) << L": return " << index << L";" << endl;
            }
            else
            {
                stream << ToCharRepresentation(it->m_ch) << endl;
            }
        }

        index++;
    }

    if (code)
    {
        stream << L"    }" << endl;
        stream << L"}" << endl;
    }
}


void MakeGetNext2(const MapIntToCharInt& map,
                  std::wostream& stream,
                  MapIntToInt& finalTokenState,
                  const std::vector<std::wstring>& tokensNames,
                  int identationVal,
                  const std::wstring& tokenPrefix,
                  const std::wstring& grammarName)
{
    console::Ident spaces(identationVal);

    SwitchBuilderC builder(stream, finalTokenState, tokensNames, identationVal, tokenPrefix);
    MapIntToCharInt::const_iterator it = map.begin();


    stream <<  L"int " << grammarName << L"_GetNext(int state, wchar_t ch)" << endl;


    stream <<   L"{" << endl;
    stream <<  spaces << L"switch (state)" << endl;
    stream <<  spaces << L"{" << endl;


    for (; it != map.end(); it++)
    {
        builder.Add(it->first, it->second.first, it->second.second);
    }

    builder.Flush();

    //nao faltou 1?-acho q nao
    // stream << L"      break;" << endl;
    stream <<  spaces << L"} /*switch*/" << endl;
    stream <<  spaces << L"return -1;" << endl;
    stream <<  L"}" << endl << endl;

}

void MakeGetNext(const MapIntToCharInt& map,
                 std::wostream& stream,
                 bool code,
                 MapIntToInt& finalTokenState,
                 const std::vector<std::wstring>& tokensNames,
                 const std::wstring& tokenPrefix,
                 const std::wstring& grammarName)
{
    MapIntToCharInt::const_iterator it = map.begin();
    int casen = -1;

    if (code)
    {
        stream << L"    int GetNext(int state, wchar_t ch)" << endl;
        stream << L"    {" << endl;
        stream << L"      switch (state)" << endl;
        stream << L"      {" << endl;
    }
    else
    {
        stream << L"/*State transition State*/" << endl;
    }

    int count = 0;
    int transitionscount = 0; //umero de ifs dentro do case
    bool hasAnyChar = false;
    int anyCharState = -1;

    for (; it != map.end(); it++)
    {
        if (casen != it->first)
        {
            if (hasAnyChar)
            {
                assert(anyCharState != -1);

                if (transitionscount == 0)
                {
                    stream << L"        return " << anyCharState << L";/*AnyChar*/" <<  endl;
                }
                else
                {
                    stream << L"        else return " << anyCharState << L"; /*AnyChar*/" <<  endl;
                }
            }

            anyCharState = -1;
            transitionscount = 0;
            hasAnyChar = false;

            if (code)
            {
                if (casen != -1)
                    stream << L"      break;" << endl;
            }

            count++;//numero de cases
            casen = it->first;

            if (code)
            {
                stream << L"      case " << it->first << L":" << endl;
            }
            else
            {
                stream << it->first << endl;
            }
        }

        if (it->second.first != EmptyTransition)
        {
            if (code)
            {
                if (it->second.first != Transition::AnyChar)
                {
                    transitionscount++;

                    if (transitionscount > 1)
                    {
                        if (it->second.first < TokenCharStart)
                        {
                            stream << L"        else if (ch == " << ToCharRepresentation(it->second.first) << L") return " << it->second.second << L";" <<  endl;
                        }
                        else
                        {
                            const int tokenIndex = it->second.first  - TokenCharStart;
                            finalTokenState.insert(make_pair(it->first , tokenIndex));
                            //stream << L"    return -1;//" << tokensNames.at(tokenIndex) <<  endl;//este eh o estado final
                            stream << L"        /*" << tokenPrefix << (tokensNames.at(tokenIndex)) << L"*/" << endl; //este eh o estado final
                        }

                        //stream << L"    else if (ch == " << ToCharRepresentation(it->second.first) << L") return " << it->second.second << L";" <<  endl;
                    }
                    else
                    {
                        if (it->second.first < TokenCharStart)
                        {
                            stream << L"        if (ch == " << ToCharRepresentation(it->second.first) << L") return " << it->second.second << L";" <<  endl;
                        }
                        else
                        {
                            const int tokenIndex = it->second.first  - TokenCharStart;
                            finalTokenState.insert(make_pair(it->first , int(it->second.first  - TokenCharStart)));
                            stream << L"        return -1; /*" << tokenPrefix << (tokensNames.at(tokenIndex)) <<  L"*/" << endl; //este eh o estado final
                        }
                    }
                }
                else
                {
                    hasAnyChar = true;
                    anyCharState = it->second.second;
                }
            }
            else
            {

                if (it->second.first < TokenCharStart)
                {
                    stream << ToCharRepresentation(it->second.first) << L" " << it->second.second <<  endl;
                }
                else
                {
                    const int tokenIndex = it->second.first  - TokenCharStart;
                    finalTokenState.insert(make_pair(it->first , int(it->second.first  - TokenCharStart)));
                    //stream << L"    return -1; //" << tokensNames.at(tokenIndex) <<  endl;//este eh o estado final
                }
            }
        }
        else
        {
            if (code)
            {
                stream << L"        return -1;" <<  endl;
            }
            else
            {
            }
        }
    }

    if (code)
    {
        if (hasAnyChar)
        {
            assert(anyCharState != -1);

            if (transitionscount == 0)
            {
                stream << L"        return " << anyCharState << L";/*AnyChar*/" <<  endl;
            }
            else
            {
                stream << L"        else return " << anyCharState << L"; /*AnyChar*/" <<  endl;
            }
        }

        //nao faltou 1?-acho q nao
        stream << L"      break;" << endl;
        stream << L"      }" << endl;
        stream << L"      return -1;" << endl;
        stream << L"    }" << endl << endl;
    }
}
void MakeTokenEnum(const std::wstring& enumName0,
                   std::wostream& stream,
                   const std::vector<std::wstring>& tokensNames,
                   bool code,
                   const std::wstring& tokenPrefix,
                   const std::wstring& grammarName,
                   bool bHeader)
{
    std::wstring enumName;
    enumName = grammarName + L"_" + enumName0;


    if (code)
    {
        if (bHeader)
        {
            stream << L"typedef enum " << endl;
            stream << L"{" << endl;
        }
    }
    else
    {
        stream << L"/*tokens*/" << endl;
    }

    if (bHeader)
    {
        for (int i = 0; i < (int)tokensNames.size(); i++)
        {
            if (code)
            {
                stream << L"    " << tokenPrefix << (tokensNames[i]) << L"," << endl;
            }
            else
            {
                stream << tokenPrefix << (tokensNames[i]) << endl;
            }
        }
    }

    if (code)
    {
        if (bHeader)
        {
          stream << L"} " << enumName << L";" << endl << endl;
        }
    }

    if (bHeader)
    {
        stream << L"const char* " << enumName << "_ToString(" << enumName << L" e);" << endl;
        stream << endl;
    }
    else
    {
        if (code)
        {
            stream << L"const char* " << enumName << "_ToString(" << enumName << L" e)" << endl;

            stream << L"{" << endl;
            stream << L"    switch(e)" << endl;
            stream << L"    {" << endl;

            for (int i = 0; i < (int)tokensNames.size(); i++)
            {
                stream << L"    case " << tokenPrefix << (tokensNames[i]) << L": return \"" << tokensNames[i] << L"\";" << endl;
            }

            stream << L"    default:break;" << endl;
            stream << L"    }" << endl;
            stream << L"    return \"\";" << endl;
            stream << L"}" << endl << endl;
        }
    }

}

void MakeGetTokenFromState(const std::wstring& enumName0,
                           const MapIntToInt& map,
                           std::wostream& stream,
                           const std::vector<std::wstring>& tokensNames,
                           bool code,
                           int identationVal,
                           int tokenInterleaveIndex,
                           const std::wstring& tokenPrefix,
                           const std::wstring& grammarName)
{
    std::wstring enumName;
    enumName = grammarName + L"_" + enumName0;

    Ident spaces(identationVal);
    MapIntToInt::const_iterator it = map.begin();

    if (code)
    {
        stream <<  L"int " << grammarName << L"_IsInterleave(" << enumName << " tk)" << endl;
        stream  <<  L"{" << endl;

        if (tokenInterleaveIndex < 0)
        {
            stream  << spaces << L"return false; /*todo add you blanks token here*/" << endl;
        }
        else
        {
            stream  <<  spaces << L"return tk == " << tokenPrefix << (tokensNames[tokenInterleaveIndex]) << L";" << endl;
        }

        stream  <<  L"}" << endl;
        stream  << endl;


        stream <<  L"int " << grammarName  << L"_GetTokenFromState(int state, " << enumName << L"* tk)" << endl;


        stream  <<  L"{" << endl;

        stream  <<  spaces << L"switch (state)" << endl;
        stream  <<  spaces << L"{" << endl;
    }
    else
    {
        stream << L"/*State TokenID*/" << endl;
    }

    for (; it != map.end(); ++it)
    {
        if (code)
        {

            stream <<  spaces << spaces << L"case " << it->first << L": *tk = " << tokenPrefix << (tokensNames.at(it->second)) << L"; break;" << endl;

        }
        else
        {
            stream  << it->first << L" " << tokenPrefix << (tokensNames.at(it->second)) << endl;
        }
    }

    if (code)
    {
        stream  <<  spaces << spaces  << L"default:" << endl;


        stream <<  spaces << spaces << spaces << L"return 0;" << endl;


        stream  <<  spaces << L"}" << endl;


        stream <<  spaces << L"return 1;" << endl;



        stream  <<  L"}" << endl;
    }
}



std::wstring ToCharRepresentation(wchar_t ch)
{
    std::wstring s;
    s = L"L'";


    //if (ch == Transition::AnyChar)
    //  {
    //    s += L"\\uFFFE";
    //}
    //else
    if (ch == L'\n')
    {
        s += L"\\n";
    }
    else if (ch == L'\0')
    {
        s += L"\\0";
    }
    else if (ch == L'\t')
    {
        s += L"\\t";
    }
    else if (ch == L'\r')
    {
        s += L"\\r";
    }
    else if (ch == L'\'')
    {
        s += L"\\'";
    }
    else if (ch == L'\\')
    {
        s += L"\\\\";
    }
    else if (ch >= L' ' && ch <= L'~')
    {
        s += ch; //direct
    }
    else
    {
        //u0000 format
        std::wostringstream ss;
        ss << std::hex << (unsigned int)(ch);
        s += L"\\u";

        for (int k = 0 ; k < (4 - (int)ss.str().size()); k++)
        {
            s += L"0";
        }

        s += ss.str();
    }

    s += L"'";
    return s;
}



static std::wstring MakeUpper(const std::wstring& s)
{
    std::wstring r;
    r.reserve(s.size());

    std::wstring::const_iterator p  = s.begin();

    while (p != s.end())
    {
        r += toupper(*p);
        ++p;
    }

    return r;
}




void GenerateCodeC(std::wostream& fileout,
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
                   const std::wstring& tokenPrefix)
{
    PrintLn("Generating code...");


    if (!fileout)
        throw std::exception("output file cannot be opened. Is the file path correct?");

    std::wostream& os = fileout;

    bool makecode = true;

    if (makecode)
    {
        PrintGeneratedFileHeader(os);
        //PrintGeneratedFileLicense(os);

        std::wstring upperfile = MakeUpper(fileOutName);
        FindReplace(upperfile, L".", L"_");
    }
    os << "\n";
    os << "#include \"" << moduleName << L"Lex.h\"\n";
    //os << "#include <assert.h>\n\n";
    //os << L"#include \"errors.h\"\n";
    //os << L"#include <stdlib.h>\n";

    MakeTokenEnum(enumName,
                  os,
                  tokensNames,
                  makecode,
                  tokenPrefix,
                  grammarName,
                  false);


    if (!makecode)
    {
        MakeGetCharIndex(symbols, os, makecode, grammarName);
    }
    else
    {
    }

    const int identation = 4;

    if (reduceswitch)
    {
        MakeGetNext2(out.map2, os, finaltokens, tokensNames, identation, tokenPrefix, grammarName);
    }
    else
    {
        MakeGetNext(out.map2, os, makecode, finaltokens, tokensNames, tokenPrefix, grammarName);
    }

    MakeGetTokenFromState(enumName,
                          finaltokens,
                          os,
                          tokensNames,
                          makecode,
                          identation,
                          interleaveIndex,
                          tokenPrefix,
                          grammarName);

//    GenerateScanner(os, grammarName);
}




void GenerateHeaderC(std::wostream& fileout,
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
                     const std::wstring& tokenPrefix)
{
    PrintLn("Generating code...");

    if (!fileout)
        throw std::exception("output file cannot be opened. Is the file path correct?");

    std::wostream& os = fileout;

    bool makecode = true;

    if (makecode)
    {
        PrintGeneratedFileHeader(os);
        std::wstring upperfile = MakeUpper(fileOutName);
        FindReplace(upperfile, L".", L"_");

        os << L"#ifndef " << upperfile << "" << endl;
        os << L"#define " << upperfile << "" << endl;
        os << endl;
    }

    //os << "#include <wchar.h>" << endl;
    //os << "#include \"sstream.h\"" << endl;
    os << endl << endl;

    MakeTokenEnum(enumName,
                  os,
                  tokensNames,
                  makecode,
                  tokenPrefix,
                  grammarName,
                  true);

    if (!makecode)
    {
        os << L"int " << grammarName << "_GetCharIndex(wchar_t ch);" << endl;
    }

    const int identation = 4;

    if (reduceswitch)
    {
        //os << L"int " << grammarName << L"_GetNext(int state, wchar_t ch);" << endl;
    }
    else
    {
        MakeGetNext(out.map2, os, makecode, finaltokens, tokensNames, tokenPrefix, grammarName);
    }

    os << endl;

    os << L"int " << grammarName << "_GetNext(int state, wchar_t ch);" << endl;
    os << L"int " << grammarName << "_GetTokenFromState(int state, " << grammarName << L"_Tokens* tk);" << endl;
    os << L"int " << grammarName << "_IsInterleave(" << grammarName << L"_Tokens tk);" << endl;
   // os << L"const char* " << grammarName << L"_Tokens_ToString(" << grammarName << L"_Tokens e);" << endl;

    //os << "int " << grammarName << "_NextTokenNoInterleave(struct sstream* stream," << endl;
    //os << "  wchar_t** buffer," << endl;
    //os << "  int* bufferSize," << endl;
    //os << L" enum " << grammarName << L"_Tokens* tk);" << endl;

    //os << endl;

    //os << "int " << grammarName << "_NextToken(struct sstream* stream," << endl;
    //os << "   wchar_t** buffer," << endl;
    //os << "   int* bufferSize," << endl;
    //os << L"  enum " << grammarName << L"_Tokens* tk);" << endl;


    if (makecode)
    {
        os << endl;
        os << L"#endif" << endl;
    }
}

