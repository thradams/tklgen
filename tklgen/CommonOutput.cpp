#include "stdafx.h"
#include "CommonOutput.h"
#include <string>
#include <fstream>
#include <codecvt>

// TODO: reference additional headers your program requires here
void FindReplace(std::wstring& in_this_string,
    const std::wstring& find,
    const std::wstring& replace)
{
    std::wstring::size_type pos = 0;

    while (std::wstring::npos != (pos = in_this_string.find(find, pos)))
    {
        in_this_string.replace(pos, find.length(), replace);
        pos += replace.length();
    }
}

std::wstring MakeUpper(const std::wstring& s)
{
    std::wstring r;
    r.reserve(s.size());

    std::wstring::const_iterator p = s.begin();

    while (p != s.end())
    {
        r += toupper(*p);
        ++p;
    }

    return r;
}

#define HEADERINFO \
L"/*\n"\
L" * Generated by TKLGEN - Version" WIDEN(__DATE__) L"\n"\
L" * Copyright (C) 2013, Thiago Adams (thiago.adams@gmail.com)\n"\
L" * www.thradams.com\n"\
L" *\n"\
L" */\n"

void PrintGeneratedFileHeader(std::wostream& os)
{
    os << HEADERINFO;
}

void PrintGeneratedFileLicense(std::wostream& os)
{
   
}

// http://www.thradams.com/codeblog/toliteral.htm

const wchar_t* TokenizerFile()
{
const wchar_t * psz =
    L"//////////////////////////////////////////////////////////////////////////////\n"
    L"// Generated by TKLGEN - Version {DATE}\n"
    L"// Copyright (C) 2013, Thiago Adams (thiago.adams@gmail.com)\n"
    L"// www.thradams.com\n"
    L"//\n"
    L"// Permission to copy, use, modify, sell and distribute this software\n"
    L"// is granted provided this copyright notice appears in all copies.\n"
    L"// This software is provided \"as is\" without express or implied\n"
    L"// warranty, and with no claim as to its suitability for any purpose.\n"
    L"//\n"
    L"//////////////////////////////////////////////////////////////////////////////\n"
    L"\n"
    L"#pragma once\n"
    L"\n"
    L"inline void Clear(std::wstring& ws)\n"
    L"{\n"
    L"    ws.clear();\n"
    L"}\n"
    L"\n"
    L"inline void Append(std::wstring& ws, wchar_t ch)\n"
    L"{\n"
    L"    ws.append(1, ch);\n"
    L"}\n"
    L"\n"
    L"\n"
    L"inline void Clear(std::string& ws)\n"
    L"{\n"
    L"    ws.clear();\n"
    L"}\n"
    L"\n"
    L"inline void Append(std::string& ws, char ch)\n"
    L"{\n"
    L"    ws.append(1, ch);\n"
    L"}\n"
    L"\n"
    L"template < class TDFA,\n"
    L"         class T,\n"
    L"         class TInputStream >\n"
    L"bool NextTokenNoInterleave(TInputStream& stream,\n"
    L"                           T& lexeme,\n"
    L"                           typename TDFA::TokenType& tk)\n"
    L"{\n"
    L"    Clear(lexeme);\n"
    L"    int lastGoodState = -1;\n"
    L"    int currentState = 0;\n"
    L"    wchar_t ch;\n"
    L"\n"
    L"    while (GetChar(stream, ch))\n"
    L"    {\n"
    L"        currentState = TDFA::GetNext(currentState, ch);\n"
    L"\n"
    L"        if (currentState == -1)\n"
    L"        {\n"
    L"            PutBack(stream, ch);\n"
    L"            break;\n"
    L"        }\n"
    L"\n"
    L"        typename TDFA::TokenType tk2;\n"
    L"\n"
    L"        if (TDFA::GetTokenFromState(currentState, tk2))\n"
    L"        {\n"
    L"            tk = tk2;\n"
    L"            lastGoodState = currentState;\n"
    L"        }\n"
    L"\n"
    L"        Append(lexeme, ch);\n"
    L"    }\n"
    L"\n"
    L"    return (lastGoodState != -1);\n"
    L"}\n"
    L"\n"
    L"template < class TDFA,\n"
    L"         class T,\n"
    L"         class TInputStream >\n"
    L"bool NextToken(TInputStream& stream,\n"
    L"               T& lexeme,\n"
    L"               typename TDFA::TokenType& tk)\n"
    L"{\n"
    L"    for (;;)\n"
    L"    {\n"
    L"        if (!NextTokenNoInterleave<TDFA>(stream, lexeme, tk))\n"
    L"        {\n"
    L"            return false;\n"
    L"        }\n"
    L"\n"
    L"        if (!TDFA::IsInterleave(tk))\n"
    L"        {\n"
    L"            return true;\n"
    L"        }\n"
    L"    }\n"
    L"}\n"
    L"\n";

    return psz;
}

const wchar_t* StringStreamFile()
{
const wchar_t * psz =
    L"\n"
    L"#pragma once\n"
    L"#include <cassert>\n"
    L"\n"
    L"struct StringStream\n"
    L"{\n"
    L"  const wchar_t* m_pCharacteres;\n"
    L"  const wchar_t* m_pCurrentChar;\n"
    L"  size_t         m_CurrentLine;\n"
    L"  size_t         m_CurrentCol;\n"
    L"  bool           m_HasPutBack;\n"
    L"  wchar_t        m_PutBackCharacter;\n"
    L"  bool           m_EofSent;\n"
    L"\n"
    L"  bool GetCharCore(wchar_t& ch)\n"
    L"  {\n"
    L"    if (m_HasPutBack)\n"
    L"    {\n"
    L"      ch = m_PutBackCharacter;\n"
    L"      m_HasPutBack = false;\n"
    L"      m_PutBackCharacter = 0;\n"
    L"      return true;\n"
    L"    }\n"
    L"\n"
    L"    if (m_EofSent)\n"
    L"      return false;\n"
    L"\n"
    L"    ch = *m_pCurrentChar++;\n"
    L"\n"
    L"    if (ch == L\'\\0\')\n"
    L"    {\n"
    L"      ch = \'\\0\';\n"
    L"      m_EofSent = true;\n"
    L"    }\n"
    L"\n"
    L"    return true;\n"
    L"  }\n"
    L"\n"
    L"public:\n"
    L"\n"
    L"  StringStream(const wchar_t* psz)\n"
    L"    : m_pCharacteres(psz)\n"
    L"    , m_CurrentLine(1)\n"
    L"    , m_CurrentCol(1)\n"
    L"    , m_HasPutBack(false)\n"
    L"    , m_PutBackCharacter(L\'\\0\')\n"
    L"    , m_EofSent(false)\n"
    L"  {\n"
    L"    m_pCurrentChar = m_pCharacteres;\n"
    L"  }\n"
    L"\n"
    L"  friend void PutBack(StringStream& stream,  wchar_t ch)\n"
    L"  {\n"
    L"    assert(stream.m_HasPutBack == false);\n"
    L"\n"
    L"    if (ch == L\'\\n\')\n"
    L"    {\n"
    L"      if (stream.m_CurrentLine > 1)\n"
    L"      {\n"
    L"        stream.m_CurrentLine--;\n"
    L"      }\n"
    L"    }\n"
    L"    else\n"
    L"    {\n"
    L"      if (stream.m_CurrentCol > 1)\n"
    L"      {\n"
    L"        stream.m_CurrentCol--;\n"
    L"      }\n"
    L"    }\n"
    L"\n"
    L"    stream.m_PutBackCharacter = ch;\n"
    L"    stream.m_HasPutBack = true;\n"
    L"  }\n"
    L"\n"
    L"  friend bool GetChar(StringStream& stream, wchar_t& ch)\n"
    L"  {\n"
    L"    bool b = stream.GetCharCore(ch);\n"
    L"\n"
    L"    if (b && ch == L\'\\n\')\n"
    L"    {\n"
    L"      stream.m_CurrentLine++;\n"
    L"      stream.m_CurrentCol = 1;\n"
    L"    }\n"
    L"    else\n"
    L"    {\n"
    L"      stream.m_CurrentCol++;\n"
    L"    }\n"
    L"\n"
    L"    return b;\n"
    L"  }\n"
    L"\n"
    L"  size_t GetLine(const StringStream& stream)\n"
    L"  {\n"
    L"    return stream.m_CurrentLine;\n"
    L"  }\n"
    L"\n"
    L"  size_t GetCol(const StringStream& stream) \n"
    L"  {\n"
    L"    return stream.m_CurrentCol;\n"
    L"  }\n"
    L"};\n";

    return psz;
}

const wchar_t* FileStreamFile()
{
  const wchar_t * psz =
    L"\n"
    L"#pragma once\n"
    L"\n"
    L"#include <cstdio>\n"
    L"#include <cassert>\n"
    L"\n"
    L"struct FileStream\n"
    L"{\n"
    L"  typedef const wchar_t* BufferIterator;\n"
    L"\n"
    L"  wchar_t*     m_pCharacteres;\n"
    L"  const size_t m_MaxBufferSize;\n"
    L"  wchar_t*     m_pCurrentChar;\n"
    L"  FILE*        m_hFile;\n"
    L"  size_t       m_CurrentLine;\n"
    L"  size_t       m_CurrentCol;\n"
    L"\n"
    L"  bool         m_HasPutBack;\n"
    L"  wchar_t      m_PutBackCharacter;\n"
    L"\n"
    L"  bool         m_EofSent;\n"
    L"\n"
    L"  BufferIterator FirstBufferEnd() const\n"
    L"  {\n"
    L"    return m_pCharacteres + m_MaxBufferSize - 1;\n"
    L"  }\n"
    L"\n"
    L"  BufferIterator SecondBufferEnd() const\n"
    L"  {\n"
    L"    return m_pCharacteres + m_MaxBufferSize * 2 - 1;\n"
    L"  }\n"
    L"\n"
    L"  bool GetCharCore(wchar_t& ch)\n"
    L"  {\n"
    L"    if (m_HasPutBack)\n"
    L"    {\n"
    L"      ch = m_PutBackCharacter;\n"
    L"      m_HasPutBack = false;\n"
    L"      m_PutBackCharacter = 0;\n"
    L"      return true;\n"
    L"    }\n"
    L"\n"
    L"    if (m_EofSent)\n"
    L"      return false;\n"
    L"\n"
    L"    ch = *m_pCurrentChar++;\n"
    L"\n"
    L"    if (ch == L\'\\0\')\n"
    L"    {\n"
    L"      ch = \'\\0\';\n"
    L"      m_EofSent = true;\n"
    L"    }\n"
    L"    else\n"
    L"    {\n"
    L"      if (*m_pCurrentChar == L\'\\0\')\n"
    L"      {\n"
    L"        if (m_pCurrentChar == FirstBufferEnd())\n"
    L"        {\n"
    L"          wchar_t* pSecondBuffer =\n"
    L"            m_pCharacteres + m_MaxBufferSize;\n"
    L"\n"
    L"          size_t r = fread(pSecondBuffer,\n"
    L"                           sizeof(wchar_t),\n"
    L"                           m_MaxBufferSize - 1,\n"
    L"                           m_hFile);\n"
    L"          pSecondBuffer[r] = 0;\n"
    L"          m_pCurrentChar = pSecondBuffer;\n"
    L"        }\n"
    L"        else if (m_pCurrentChar == SecondBufferEnd())\n"
    L"        {\n"
    L"          wchar_t* pFirstBuffer = m_pCharacteres;\n"
    L"\n"
    L"          size_t r = fread(pFirstBuffer,\n"
    L"                           sizeof(wchar_t),\n"
    L"                           m_MaxBufferSize - 1,\n"
    L"                           m_hFile);\n"
    L"\n"
    L"          pFirstBuffer[r] = 0;\n"
    L"          m_pCurrentChar = pFirstBuffer;\n"
    L"        }\n"
    L"      }\n"
    L"    }\n"
    L"\n"
    L"    assert(m_pCharacteres[m_MaxBufferSize - 1] == 0);\n"
    L"    assert(m_pCharacteres[m_MaxBufferSize * 2 - 1] == 0);\n"
    L"    return true;\n"
    L"  }\n"
    L"\n"
    L"public:\n"
    L"\n"
    L"  FileStream(const wchar_t* fileName, size_t bufferSize = 4096)\n"
    L"    : m_MaxBufferSize(bufferSize)\n"
    L"    , m_CurrentLine(1)\n"
    L"    , m_CurrentCol(1)\n"
    L"    , m_HasPutBack(false)\n"
    L"    , m_PutBackCharacter(L\'\\0\')\n"
    L"    , m_EofSent(false)\n"
    L"  {\n"
    L"    assert(bufferSize >= 2);\n"
    L"\n"
    L"    //allocate 2 buffers\n"
    L"    m_pCharacteres = new wchar_t[m_MaxBufferSize * 2];\n"
    L"    m_pCharacteres[m_MaxBufferSize * 2 - 1] = 0;\n"
    L"    m_pCharacteres[m_MaxBufferSize - 1] = 0;\n"
    L"    m_pCurrentChar = m_pCharacteres;\n"
    L"\n"
    L"    errno_t err = _wfopen_s(&m_hFile, fileName, L\"r,ccs=UTF-8\");\n"
    L"\n"
    L"    if (err == 0)\n"
    L"    {\n"
    L"      size_t r = fread(m_pCharacteres,\n"
    L"                       sizeof(wchar_t),\n"
    L"                       m_MaxBufferSize - 1,\n"
    L"                       m_hFile);\n"
    L"\n"
    L"      m_pCharacteres[r] = L\'\\0\';\n"
    L"    }\n"
    L"  }\n"
    L"\n"
    L"  ~FileStream()\n"
    L"  {\n"
    L"    fclose(m_hFile);\n"
    L"    delete [] m_pCharacteres;\n"
    L"  }\n"
    L"\n"
    L"  friend void PutBack(FileStream& fileStream, wchar_t ch)\n"
    L"  {\n"
    L"    assert(fileStream.m_HasPutBack == false);\n"
    L"\n"
    L"    if (ch == L\'\\n\')\n"
    L"    {\n"
    L"      if (fileStream.m_CurrentLine > 1)\n"
    L"      {\n"
    L"        fileStream.m_CurrentLine--;\n"
    L"      }\n"
    L"    }\n"
    L"    else\n"
    L"    {\n"
    L"      if (fileStream.m_CurrentCol > 1)\n"
    L"      {\n"
    L"        fileStream.m_CurrentCol--;\n"
    L"      }\n"
    L"    }\n"
    L"\n"
    L"    fileStream.m_PutBackCharacter = ch;\n"
    L"    fileStream.m_HasPutBack = true;\n"
    L"  }\n"
    L"  \n"
    L"  friend bool GetChar(FileStream& fileStream, wchar_t& ch)\n"
    L"  {\n"
    L"    bool b = fileStream.GetCharCore(ch);\n"
    L"\n"
    L"    if (b && ch == L\'\\n\')\n"
    L"    {\n"
    L"      fileStream.m_CurrentLine++;\n"
    L"      fileStream.m_CurrentCol = 1;\n"
    L"    }\n"
    L"    else\n"
    L"    {\n"
    L"      fileStream.m_CurrentCol++;\n"
    L"    }\n"
    L"\n"
    L"    return b;\n"
    L"  }\n"
    L"\n"
    L"  friend size_t GetLine(const FileStream& fileStream)\n"
    L"  {\n"
    L"    return fileStream.m_CurrentLine;\n"
    L"  }\n"
    L"\n"
    L"  friend size_t GetCol(const FileStream& fileStream)\n"
    L"  {\n"
    L"    return fileStream.m_CurrentCol;\n"
    L"  }\n"
    L"};\n"
    L"\n";

    return psz;    
}

const wchar_t* SampleFile()
{
   const wchar_t * psz =
    L"\n"
    L"#include \"stdafx.h\"\n"
    L"#include \"{GRAMMAR}Parser.h\"\n"
    L"#include <iostream>\n"
    L"\n"
    L"int _tmain(int argc, _TCHAR* argv[])\n"
    L"{\n"
    L"  if (argc == 1)\n"
    L"  {\n"
    L"    std::cout << \"missing input file \";\n"
    L"    return 1;\n"
    L"  }\n"
    L"\n"
    L"  try\n"
    L"  {\n"
    L"    FileStream ss(argv[1]);\n"
    L"    {GRAMMAR}::Context ctx(ss);\n"
    L"    {GRAMMAR}::Parse_Main(ctx);\n"
    L"  }\n"
    L"  catch (const {GRAMMAR}::ParserException& e)\n"
    L"  {\n"
    L"    std::cout << \"Error : line, col = \" << e.m_Line << \", \" << e.m_Col << std::endl;\n"
    L"  }\n"
    L"  catch (const std::exception& e)\n"
    L"  {\n"
    L"    std::cout << e.what() << std::endl;\n"
    L"  }\n"
    L"\n"
    L"  return 0;\n"
    L"}\n";

   return psz;
}
void StringToFile(const wchar_t* psz, const wchar_t* fileName)
{
    std::locale ulocale(std::locale(), new std::codecvt_utf8<wchar_t>);
    std::wofstream ofs(fileName);

    if (ofs.is_open())
    {
        ofs.imbue(ulocale);
        ofs << psz;
    }
}

bool FileExists(const wchar_t* fileName)
{
    std::wifstream ifs(fileName);
    return (ifs.is_open());
}

void StringToNewFile(const wchar_t* psz, const wchar_t* fileName)
{
    if (!FileExists(fileName))
    {
        StringToFile(psz, fileName);
    }
}

#define SSTREAMSOURCE \
L"#include \"sstream.h\"\n"\
L"#include <assert.h>\n"\
L"int GetCharCore(struct sstream* pThis, wchar_t* ch)\n"\
L"{\n"\
L"    if (pThis->m_HasPutBack)\n"\
L"    {\n"\
L"        *ch = pThis->m_PutBackCharacter;\n"\
L"        pThis->m_HasPutBack = 0;\n"\
L"        pThis->m_PutBackCharacter = 0;\n"\
L"        return 1;\n"\
L"    }\n"\
L"    if (pThis->m_EofSent)\n"\
L"        return 0;\n"\
L"    *ch = *pThis->m_pCurrentChar++;\n"\
L"    if (ch == L'\\0')\n"\
L"    {\n"\
L"        *ch = '\\0';\n"\
L"        pThis->m_EofSent = 1;\n"\
L"    }\n"\
L"    return 1;\n"\
L"}\n"\
L"void sstream_init(struct sstream* pThis, const wchar_t* psz)\n"\
L"{\n"\
L"    pThis->m_pCharacteres = psz;\n"\
L"    pThis->m_CurrentLine = 1;\n"\
L"    pThis->m_CurrentCol = 1;\n"\
L"    pThis->m_HasPutBack = 0;\n"\
L"    pThis->m_PutBackCharacter = L'\\0';\n"\
L"    pThis->m_EofSent = 0;\n"\
L"    pThis->m_pCurrentChar = psz;\n"\
L"}\n"\
L"void sstream_ungetwc(struct sstream* pThis, wchar_t ch)\n"\
L"{\n"\
L"    assert(pThis->m_HasPutBack == 0);\n"\
L"    if (ch == L'\\n')\n"\
L"    {\n"\
L"        if (pThis->m_CurrentLine > 1)\n"\
L"        {\n"\
L"            pThis->m_CurrentLine--;\n"\
L"        }\n"\
L"    }\n"\
L"    else\n"\
L"    {\n"\
L"        if (pThis->m_CurrentCol > 1)\n"\
L"        {\n"\
L"            pThis->m_CurrentCol--;\n"\
L"        }\n"\
L"    }\n"\
L"    pThis->m_PutBackCharacter = ch;\n"\
L"    pThis->m_HasPutBack = 1;\n"\
L"}\n"\
L"int sstream_getwc(struct sstream* pThis, wchar_t* ch)\n"\
L"{\n"\
L"    int b = GetCharCore(pThis, ch);\n"\
L"    if (b && *ch == L'\\n')\n"\
L"    {\n"\
L"        pThis->m_CurrentLine++;\n"\
L"        pThis->m_CurrentCol = 1;\n"\
L"    }\n"\
L"    else\n"\
L"    {\n"\
L"        pThis->m_CurrentCol++;\n"\
L"    }\n"\
L"    return b;\n"\
L"}\n"\
L"size_t sstream_get_line_number(struct sstream* pThis)\n"\
L"{\n"\
L"    return pThis->m_CurrentLine;\n"\
L"}\n"\
L"size_t sstream_get_col_number(struct sstream* pThis)\n"\
L"{\n"\
L"    return pThis->m_CurrentCol;\n"\
L"}"

#define STREAMHEADER \
L"#ifndef __STRINGSTREAM_H__\n"\
L"#define __STRINGSTREAM_H__\n"\
L"#include <wchar.h>\n"\
L"struct sstream\n"\
L"{\n"\
L"    const wchar_t* m_pCharacteres;\n"\
L"    const wchar_t* m_pCurrentChar;\n"\
L"    size_t   m_CurrentLine;\n"\
L"    size_t   m_CurrentCol;\n"\
L"    int      m_HasPutBack;\n"\
L"    wchar_t  m_PutBackCharacter;\n"\
L"    int      m_EofSent;\n"\
L"};\n"\
L"#define STRING_STREAM_INIT { 0, 0, 0, 0, 0, 0, 0}\n"\
L"void sstream_init(struct sstream* pThis, const wchar_t* psz);\n"\
L"void sstream_ungetwc(struct sstream* pThis, wchar_t ch);\n"\
L"int sstream_getwc(struct sstream* pThis, wchar_t* ch);\n"\
L"size_t sstream_get_line_number(struct sstream* pThis);\n"\
L"size_t sstream_get_col_number(struct sstream* pThis);\n"\
L"#endif"
