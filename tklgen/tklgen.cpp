#include "stdafx.h"
#include  "console.h"
#include <vector>
#include <fstream>
#include "GrammarParser.h"
#include  "Algorithms.h"
#include "LLOutputC.h"
#include "LLOutputJS.h"
#include "LLOutputCpp.h"
#include "LexOutputC.h"
#include "LexOutputCpp.h"
#include "console.h"
#include "LexAlgorithms.h"
#include "CommonOutput.h"


using namespace console;

void PrintProductInfo()
{
  using namespace std;
  auto oldColor = console::textcolor(console::WHITE);
  std::wcout << L"// TKLGEN - Version " << __DATE__ << std::endl;
  console::textcolor(oldColor);
  std::wcout << L"// Copyright (C) 2013, Thiago Adams (thiago.adams@gmail.com)" << std::endl;
  std::wcout << L"// www.thradams.com" << std::endl;
}



struct CommandLineOptions
{
  std::wstring m_InputFile;
  bool m_bAutoToken;
  bool m_bGenerateScanner;
  bool m_bGenerateParser;
  bool m_bShowHelp;
  bool m_bOverridesh;
  bool m_bOverridescpp;
  OutputType m_OutPutType;

  CommandLineOptions()
    : m_bGenerateScanner(true)
    , m_bGenerateParser(true)
    , m_bShowHelp(false)
    , m_bOverridesh(false)
    , m_bOverridescpp(false)
    , m_OutPutType(OutputType_C)
    , m_bAutoToken(false)
  {
  }
};


void PrintHelp()
{
  const wchar_t* psz =
    L"\n"
    L"Usage:\n"
    L"tklgen grammar.txt [Options]\n"
    L"\n"
    L"Options:\n"
    L"\n"
    L"  -h   Prints this help\n"
    L"  -at AutoToken [a-z] identifiers not found will be considered tokens\n"
    L"  -tk  Will not generate de scanner (Lex file)\n"
    L"  -ll  Will not generate the parser\n"
    L"  -oh  Overrides GrammarParser.h\n"
    L"  -op  Overrides GrammarParser.cpp\n"
    L"  -c   generates C code\n";

  std::wcout << psz;
}

void GetCommandLineOptions(int argc, _TCHAR* argv[],
  CommandLineOptions& commandLineOptions)
{
  if (argc == 1)
  {
    throw std::exception("Error: Specify a grammar file");
  }

  std::vector<std::wstring> args;

  commandLineOptions.m_InputFile = argv[1];

  for (int i = 0; i < argc; i++)
    args.push_back(argv[i]);

  for (size_t i = 0; i < args.size(); i++)
  {
    const auto& argument = args[i];

    if (argument == L"-tk")
    {
      commandLineOptions.m_bGenerateScanner = false;
    }
    else if (argument == L"-ll")
    {
      commandLineOptions.m_bGenerateParser = false;
    }
    else if (argument == L"-h")
    {
      commandLineOptions.m_bShowHelp = true;
    }
    else if (argument == L"-oh")
    {
      commandLineOptions.m_bOverridesh = true;
    }
    else if (argument == L"-op")
    {
      commandLineOptions.m_bOverridescpp = true;
    }
    else if (argument == L"-c")
    {
      commandLineOptions.m_OutPutType = OutputType_C;
    }
    else if (argument == L"-cpp")
    {
      commandLineOptions.m_OutPutType = OutputType_CPP;
    }
    else if (argument == L"-js")
    {
      commandLineOptions.m_OutPutType = OutputType_JS;
    }
    else if (argument == L"-at")
    {
      commandLineOptions.m_bAutoToken = true;
    }
  }
}



const wchar_t* IntToLiteral(int i)
{
  switch (i)
  {
  case '\0': return L"\\0";
  case '\r': return L"\\r";
  case '\n':return L"\\n";
  case '\t':return L"\\t";
  case '\'':return L"\\'";
  case '\"':return L"\\\"";
  case '\?':return L"\\?";
  case '\\':return L"\\\\";
  case '\a':return L"\\a";
  case '\b':return L"\\b";
  case '\f':return L"\\f";
  case '\v':return L"\\v";
  }

  static wchar_t ch[2] = { 0, 0 };
  ch[0] = wchar_t(i);
  return ch;
}

void PrintLiteral(const std::wstring& ws)
{
  if (ws.empty())
  {
    std::wcout << "'\\0'";
  }
  else
  {
    std::wcout << "\"";
    for (size_t i = 0; i < ws.size(); i++)
    {
      std::wcout << IntToLiteral(ws[i]);
    }
    std::wcout << "\"";
  }
}

template<class Language, class Stream>
void PrintTokens(Stream& stream)
{
  std::wstring lexeme;
  typename Language::TokenType token;

  while (NextToken<Language>(stream, lexeme, token))
  {
    std::wcout.width(25);
    std::wcout << TokensToString(token);
    std::wcout << L" : ";
    PrintLiteral(lexeme.c_str());
    std::wcout << std::endl;
  }
}
void PrintTokens(const wchar_t* fileName)
{
  try
  {
    FileStream ss(fileName);
    PrintTokens<tklgen::DFA>(ss);
  }
  catch (const std::exception& e)
  {
    std::cout << e.what() << std::endl;
  }

}

static std::wstring GetFileExtension(OutputType type)
{
  switch (type)
  {
  case OutputType_C: return L".cpp";
  case OutputType_CPP: return L".cpp";
  case OutputType_JS: return L".ts";
  }
  return L".c";
}

int _tmain(int argc, _TCHAR* argv[])
{
  PrintProductInfo();

  if (argc == 1)
  {
    PrintHelp();
    return 0;
  }

  //
  //Debugar tokens
  //PrintTokens(argv[1]);
  //

  try
  {
    CommandLineOptions commandLineOptions;
    GetCommandLineOptions(argc, argv, commandLineOptions);

    if (commandLineOptions.m_bShowHelp)
    {
      PrintHelp();
      return 0;
    }

    Grammar grammar;
    grammar.m_bAutoToken = commandLineOptions.m_bAutoToken;

    tklgen::Parse(commandLineOptions.m_InputFile.c_str(), grammar);
    Print(std::wcout, grammar);

    if (commandLineOptions.m_bGenerateScanner)
    {
      std::wstring fileOutNameDfa = grammar.GetModuleName() + L"Lex.h";
      GenerateScanner(grammar, fileOutNameDfa, commandLineOptions.m_OutPutType);
      std::wcout << fileOutNameDfa << L" created" << std::endl;
    }

    //LLGEN-----------------------------------------------------

    if (commandLineOptions.m_bGenerateParser)
    {
      MMap mmap = MakeLLTables(grammar);

      std::wstring fileExtension = GetFileExtension(commandLineOptions.m_OutPutType);
      std::wstring fileParserCppName = grammar.GetModuleName() + L"Parser" + fileExtension;

      if (!FileExists(fileParserCppName.c_str()) ||
        commandLineOptions.m_bOverridescpp)
      {
        std::wofstream fileParserCpp(fileParserCppName);

        if (!fileParserCpp.is_open())
          throw std::exception("fileOutName");

        if (commandLineOptions.m_OutPutType == OutputType_CPP)
        {
          GenerateDescRec(fileParserCpp, grammar, mmap, L"TK", L"Parser");
        }
        else if (commandLineOptions.m_OutPutType == OutputType_C)
        {
          GenerateDescRecC(fileParserCpp, grammar, mmap, L"TK", L"Parser");
        }
        else if (commandLineOptions.m_OutPutType == OutputType_JS)
        {
          GenerateDescRecJs(fileParserCpp, grammar, mmap, L"Tokens.TK_", L"Parser");
        }

        std::wcout << fileParserCppName << L" created" << std::endl;
      }
      else
      {
        std::wcout << L"The File " << fileParserCppName << L" already exists;\n";
        std::wcout << L"Delete this file before run tklgen or use the ";
        std::wcout << L"option -oh to override this file\n";
      }

      std::wstring fileParserHeaderName = grammar.GetModuleName() + L"Parser.h";

      if (!FileExists(fileParserHeaderName.c_str()) ||
        commandLineOptions.m_bOverridesh)
      {
        std::wofstream fileParserHeader(fileParserHeaderName);

        if (!fileParserHeader.is_open())
          throw std::exception("fileOutNameH");
        if (commandLineOptions.m_OutPutType == OutputType_CPP)
        {
          GenerateDescRecHeader(fileParserHeader, grammar, mmap, L"TK");
        }
        else if (commandLineOptions.m_OutPutType == OutputType_C)
        {
          GenerateDescRecHeaderC(fileParserHeader, grammar, mmap, L"TK");
        }
        else if (commandLineOptions.m_OutPutType == OutputType_JS)
        {
          GenerateDescRecHeaderJs(fileParserHeader, grammar, mmap, L"TK");
        }

        std::wcout << fileParserHeaderName << L" created" << std::endl;
      }
      else
      {
        std::wcout << L"The File " << fileParserHeaderName << L" already exists;\n";
        std::wcout << L"Delete this file before run tklgen or use the ";
        std::wcout << L"option -op to override this file\n";
      }
    }



    std::wstring ws = SampleFile();
    find_replace(ws, L"{GRAMMAR}", grammar.GetModuleName());
    StringToNewFile(ws.c_str(), L"SampleProgram.txt");

    auto oldColor = console::textcolor(console::GREEN);
    std::wcout << L"SUCCEEDED" << std::endl;
    console::textcolor(oldColor);
    //-------------------------------------------------------------
  }
  catch (const tklgen::ParserErrorException& e)
  {
    auto oldColor = console::textcolor(console::LIGHTRED);
    std::cout << e.m_Line << ", " << e.m_Col << std::endl;
    std::wcout << tklgen::GetErrorString(e.m_Error) << std::endl;
    console::textcolor(oldColor);
  }
  catch (std::exception& e)
  {
    auto oldColor = console::textcolor(console::LIGHTRED);
    std::cout << e.what() << std::endl;
    console::textcolor(oldColor);
  }


  return 0;
}

