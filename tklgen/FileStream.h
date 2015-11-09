
#pragma once

#include <cstdio>
#include <cassert>

struct FileStream
{
  typedef const wchar_t* BufferIterator;

  wchar_t*     m_pCharacteres;
  const size_t m_MaxBufferSize;
  wchar_t*     m_pCurrentChar;
  FILE*        m_hFile;
  size_t       m_CurrentLine;
  size_t       m_CurrentCol;

  bool         m_HasPutBack;
  wchar_t      m_PutBackCharacter;

  bool         m_EofSent;

  BufferIterator FirstBufferEnd() const
  {
    return m_pCharacteres + m_MaxBufferSize - 1;
  }

  BufferIterator SecondBufferEnd() const
  {
    return m_pCharacteres + m_MaxBufferSize * 2 - 1;
  }

  bool GetCharCore(wchar_t& ch)
  {
    if (m_HasPutBack)
    {
      ch = m_PutBackCharacter;
      m_HasPutBack = false;
      m_PutBackCharacter = 0;
      return true;
    }

    if (m_EofSent)
      return false;

    ch = *m_pCurrentChar++;

    if (ch == L'\0')
    {
      ch = '\0';
      m_EofSent = true;
    }
    else
    {
      if (*m_pCurrentChar == L'\0')
      {
        if (m_pCurrentChar == FirstBufferEnd())
        {
          wchar_t* pSecondBuffer =
            m_pCharacteres + m_MaxBufferSize;

          size_t r = fread(pSecondBuffer,
                           sizeof(wchar_t),
                           m_MaxBufferSize - 1,
                           m_hFile);
          pSecondBuffer[r] = 0;
          m_pCurrentChar = pSecondBuffer;
        }
        else if (m_pCurrentChar == SecondBufferEnd())
        {
          wchar_t* pFirstBuffer = m_pCharacteres;

          size_t r = fread(pFirstBuffer,
                           sizeof(wchar_t),
                           m_MaxBufferSize - 1,
                           m_hFile);

          pFirstBuffer[r] = 0;
          m_pCurrentChar = pFirstBuffer;
        }
      }
    }

    assert(m_pCharacteres[m_MaxBufferSize - 1] == 0);
    assert(m_pCharacteres[m_MaxBufferSize * 2 - 1] == 0);
    return true;
  }

public:

  FileStream(const wchar_t* fileName, size_t bufferSize = 4096)
    : m_MaxBufferSize(bufferSize)
    , m_CurrentLine(1)
    , m_CurrentCol(1)
    , m_HasPutBack(false)
    , m_PutBackCharacter(L'\0')
    , m_EofSent(false)
  {
    assert(bufferSize >= 2);

    //allocate 2 buffers
    m_pCharacteres = new wchar_t[m_MaxBufferSize * 2];
    m_pCharacteres[m_MaxBufferSize * 2 - 1] = 0;
    m_pCharacteres[m_MaxBufferSize - 1] = 0;
    m_pCurrentChar = m_pCharacteres;

    errno_t err = _wfopen_s(&m_hFile, fileName, L"r,ccs=UTF-8");

    if (err == 0)
    {
      size_t r = fread(m_pCharacteres,
                       sizeof(wchar_t),
                       m_MaxBufferSize - 1,
                       m_hFile);

      m_pCharacteres[r] = L'\0';
    }
  }

  ~FileStream()
  {
    fclose(m_hFile);
    delete [] m_pCharacteres;
  }

  friend void PutBack(FileStream& fileStream, wchar_t ch)
  {
    assert(fileStream.m_HasPutBack == false);

    if (ch == L'\n')
    {
      if (fileStream.m_CurrentLine > 1)
      {
        fileStream.m_CurrentLine--;
      }
    }
    else
    {
      if (fileStream.m_CurrentCol > 1)
      {
        fileStream.m_CurrentCol--;
      }
    }

    fileStream.m_PutBackCharacter = ch;
    fileStream.m_HasPutBack = true;
  }
  
  friend bool GetChar(FileStream& fileStream, wchar_t& ch)
  {
    bool b = fileStream.GetCharCore(ch);

    if (b && ch == L'\n')
    {
      fileStream.m_CurrentLine++;
      fileStream.m_CurrentCol = 1;
    }
    else
    {
      fileStream.m_CurrentCol++;
    }

    return b;
  }

  friend size_t GetLine(const FileStream& fileStream)
  {
    return fileStream.m_CurrentLine;
  }

  friend size_t GetCol(const FileStream& fileStream)
  {
    return fileStream.m_CurrentCol;
  }
};

