#include "StdAfx.h"
#include "LexOutputCpp.h"
#include <sstream>
#include "console.h"
#include "CommonOutput.h"
using namespace std;
using namespace console;


struct SwitchBuilder
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

  SwitchBuilder(std::wostream& s,
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
  ~SwitchBuilder()
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
    m_stream << spaces << spaces << spaces << L"case " << currentCase << L":" << endl;

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

            m_stream << spaces << spaces << spaces;

            if (count > 0)
              m_stream << L"else ";

            m_stream << L"if (ch == " << ToCharRepresentation(firstIt->first) << L")" << endl;

            m_stream << spaces << spaces << spaces << spaces;
            m_stream << L"return " << firstIt->second << L";" << endl;

            count++;
          }
          else
          {
            if (count > 0)
              m_stream << L"        else ";
            else
              m_stream << L"        ";

            m_stream << L"if (ch >= " << ToCharRepresentation(firstIt->first) << L" && ch <= " << ToCharRepresentation(lastIt->first) << L")" << endl;
            m_stream << L"          return " << firstIt->second << L";" << endl;
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
            m_stream << L"        //" << m_TokenPrefix << m_TokenNames.at(tokenIndex) <<  endl;//este eh o estado final
            // m_stream << L"        //A:" << tokenIndex <<  endl;//este eh o estado final
          }
          else
          {
            if (count > 0)
              m_stream << L"        else ";

            m_stream << L"    return -1;" << endl;
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
          if (count > 0)
            m_stream << L"        else ";
          else
            m_stream << L"        ";

          m_stream << L"if (ch == " << ToCharRepresentation(firstIt->first) << L")" << endl;
          m_stream << L"          return " << firstIt->second << L";" << endl;
          count++;
        }
        else
        {
          if (count > 0)
            m_stream << L"        else ";
          else
            m_stream << L"       ";

          m_stream << L"if (ch >= " << ToCharRepresentation(firstIt->first) << L" && ch <= " << ToCharRepresentation(lastIt->first) << L")" << endl;
          m_stream << L"           return " << firstIt->second << L";" << endl;
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
          m_stream << L"        //" << m_TokenPrefix << (m_TokenNames.at(tokenIndex)) <<  endl;//este eh o estado final
          //m_stream << L"        //B: " << tokenIndex <<  endl;//este eh o estado final
        }
        else
        {
          m_stream << L"  return -1;" << endl;
        }
      }

      if (anyCharStateTo != -1)
      {
        m_stream << L"        else " << endl;
        m_stream << L"            return " << anyCharStateTo << L"; //any" << endl;
        count++;
      }

    }
    else
    {
      if (anyCharStateTo != -1)
      {
        m_stream << L"  return " << anyCharStateTo << L"; //any" << endl;
        count++;
      }
      else
      {
        m_stream << L"  return -1;" << endl;
      }

    }

    m_stream << L"    break; // " << endl;
    m_map.clear();
    anyCharStateTo = -1;
  }

};

void MakeGetCharIndex(const TransitionSet& symbols, std::wostream& stream, bool code)
{
  int index = 0;
  TransitionSet::const_iterator it = symbols.begin();

  if (code)
  {
    stream << L"int GetCharIndex(wchar_t ch)" << endl;
    stream << L"{" << endl;
    stream << L"    switch (ch)" << endl;
    stream << L"    {" << endl;
  }
  else
  {
    stream << L"//Symbols" << endl;
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
                  const std::wstring& tokenPrefix)
{
  console::Ident spaces(identationVal);

  SwitchBuilder builder(stream, finalTokenState, tokensNames, identationVal, tokenPrefix);
  MapIntToCharInt::const_iterator it = map.begin();


  stream << spaces << L"static int GetNext(int state, wchar_t ch)" << endl;
  stream << spaces <<  L"{" << endl;
  stream << spaces << spaces << L"switch (state)" << endl;
  stream << spaces << spaces << L"{" << endl;


  for (; it != map.end(); it++)
  {
    builder.Add(it->first, it->second.first, it->second.second);
  }

  builder.Flush();

  //nao faltou 1?-acho q nao
  // stream << L"      break;" << endl;
  stream << spaces << spaces << L"} //switch" << endl;
  stream << spaces << spaces << L"return -1;" << endl;
  stream << spaces << L"}" << endl << endl;

}

void MakeGetNext(const MapIntToCharInt& map,
                 std::wostream& stream,
                 bool code,
                 MapIntToInt& finalTokenState,
                 const std::vector<std::wstring>& tokensNames,
                 const std::wstring& tokenPrefix)
{
  MapIntToCharInt::const_iterator it = map.begin();
  int casen = -1;

  if (code)
  {
    stream << L"    static int GetNext(int state, wchar_t ch)" << endl;
    stream << L"    {" << endl;
    stream << L"      switch (state)" << endl;
    stream << L"      {" << endl;
  }
  else
  {
    stream << L"//State transition State" << endl;
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
          stream << L"        return " << anyCharState << L";//AnyChar" <<  endl;
        }
        else
        {
          stream << L"        else return " << anyCharState << L"; //AnyChar" <<  endl;
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
              stream << L"        //" << tokenPrefix << (tokensNames.at(tokenIndex)) <<  endl; //este eh o estado final
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
              stream << L"        return -1; //" << tokenPrefix << (tokensNames.at(tokenIndex)) <<  endl; //este eh o estado final
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
        stream << L"        return " << anyCharState << L";//AnyChar" <<  endl;
      }
      else
      {
        stream << L"        else return " << anyCharState << L"; //AnyChar" <<  endl;
      }
    }

    //nao faltou 1?-acho q nao
    stream << L"      break;" << endl;
    stream << L"      }" << endl;
    stream << L"      return -1;" << endl;
    stream << L"    }" << endl << endl;
  }
}
void MakeTokenEnum(const std::wstring& enumName,
                   std::wostream& stream,
                   const std::vector<std::wstring>& tokensNames,
                   bool code,
                   const std::wstring& tokenPrefix)
{
  if (code)
  {
    stream << L"enum " << enumName << endl;
    stream << L"{" << endl;
  }
  else
  {
    stream << L"//tokens" << endl;
  }

  for (int i = 0 ; i < (int)tokensNames.size(); i++)
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

  if (code)
  {
    stream << L"};" << endl << endl;
  }

  if (code)
  {
    stream << L"inline const wchar_t* " << enumName << "ToString(" << enumName << L" e)" << endl;
    stream << L"{" << endl;
    stream << L"    switch(e)" << endl;
    stream << L"    {" << endl;

    for (int i = 0 ; i < (int)tokensNames.size(); i++)
    {
      stream << L"    case " << tokenPrefix << (tokensNames[i]) << L": return L\"" << tokensNames[i] << L"\";" << endl;
    }

    stream << L"    default:break;" << endl;
    stream << L"    }" << endl;
    stream << L"    return L\"\";" << endl;
    stream << L"}" << endl << endl;
  }
}

void MakeGetTokenFromState(const std::wstring& enumName,
                           const MapIntToInt& map,
                           std::wostream& stream,
                           const std::vector<std::wstring>& tokensNames,
                           bool code,
                           int identationVal,
                           int tokenInterleaveIndex,
                           const std::wstring& tokenPrefix)
{
  Ident spaces(identationVal);
  MapIntToInt::const_iterator it = map.begin();

  if (code)
  {
    stream  << spaces << L"static bool IsInterleave(" << enumName << " tk)" << endl;
    stream  << spaces << L"{" << endl;

    if (tokenInterleaveIndex < 0)
    {
      stream  << spaces << spaces << L"return false; //todo add you blanks token here" << endl;
    }
    else
    {
      stream  << spaces << spaces << L"return tk == " << tokenPrefix << (tokensNames[tokenInterleaveIndex]) << L";" << endl;
    }

    stream  << spaces << L"}" << endl;
    stream  << endl;

    stream  << spaces << L"static bool GetTokenFromState(int state, " << enumName << L"& tk)" << endl;
    stream  << spaces << L"{" << endl;

    stream  << spaces << spaces << L"switch(state)" << endl;
    stream  << spaces << spaces << L"{" << endl;
  }
  else
  {
    stream << L"//State TokenID" << endl;
  }

  for (; it != map.end(); ++it)
  {
    if (code)
    {
      stream  << spaces << spaces << spaces << L"case " << it->first << L": tk = " << tokenPrefix << (tokensNames.at(it->second)) << L"; break;" << endl;
    }
    else
    {
      stream  << it->first << L" " << tokenPrefix << (tokensNames.at(it->second)) << endl;
    }
  }

  if (code)
  {
    stream  << spaces << spaces << spaces  << L"default:" << endl;
    stream  << spaces << spaces << spaces << spaces << L"return false;" << endl;
    stream  << spaces << spaces << L"}" << endl;
    stream  << spaces << spaces << L"return true;" << endl;
    stream  << spaces << L"}" << endl;
  }
}



static std::wstring ToCharRepresentation(wchar_t ch)
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






void GenerateCode(std::wostream& fileout,
                  const std::wstring& fileOutName,
                  const std::wstring& grammarName,
                  bool reduceswitch,
                  std::vector<std::wstring>& tokensNames,
                  MapIntCharToInt& intCharToIntMapOut,
                  MapIntToInt& finaltokens,
                  OutputMap& out,
                  const TransitionSet& symbols,
                  const int& interleaveIndex,
                  const std::wstring& enumName,
                  const std::wstring& className,
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
    PrintGeneratedFileLicense(os);
    
    std::wstring upperfile = MakeUpper(fileOutName);
    FindReplace(upperfile, L".", L"_");

    os << L"#ifndef __" << upperfile << "__" << endl;
    os << L"#define __" << upperfile << "__" << endl;
    os << endl;
  }



  os << L"namespace " << grammarName << endl;
  os << L"{" << endl << endl;


  MakeTokenEnum(enumName, os, tokensNames, makecode, tokenPrefix);

  if (!makecode)
  {
    MakeGetCharIndex(symbols, os, makecode);
  }
  else
  {

    os << L"struct " <<  className << std::endl;
    os << L"{" << endl;
    os << L"    typedef " << enumName << L" TokenType;" << endl << endl;
  }

  const int identation = 4;

  if (reduceswitch)
    MakeGetNext2(out.map2, os, finaltokens, tokensNames, identation, tokenPrefix);
  else
    MakeGetNext(out.map2, os, makecode, finaltokens, tokensNames, tokenPrefix);


  MakeGetTokenFromState(enumName,
                        finaltokens,
                        os,
                        tokensNames,
                        makecode,
                        identation,
                        interleaveIndex,
                        tokenPrefix);

  if (makecode)
  {
    os << L"};" << endl;
    os << L"} //namespace" << endl;
    os << L"#endif //header" << endl;
  }
}