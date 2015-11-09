#ifndef __STRINGSTREAM_H__
#define __STRINGSTREAM_H__

#include <wchar.h>

struct sstream
{
    const wchar_t* m_pCharacteres;
    const wchar_t* m_pCurrentChar;
    size_t   m_CurrentLine;
    size_t   m_CurrentCol;
    int      m_HasPutBack;
    wchar_t  m_PutBackCharacter;
    int      m_EofSent;
};

#define STRING_STREAM_INIT { 0, 0, 0, 0, 0, 0, 0}

void sstream_init(struct sstream* pThis, const wchar_t* psz);
void sstream_ungetwc(struct sstream* pThis, wchar_t ch);
int sstream_getwc(struct sstream* pThis, wchar_t* ch);
size_t sstream_get_line_number(struct sstream* pThis);
size_t sstream_get_col_number(struct sstream* pThis);

#endif
