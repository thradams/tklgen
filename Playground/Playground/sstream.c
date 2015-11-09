#include "sstream.h"
#include <assert.h>
int GetCharCore(struct sstream* pThis, wchar_t* ch)
{
    if (pThis->m_HasPutBack)
    {
        *ch = pThis->m_PutBackCharacter;
        pThis->m_HasPutBack = 0;
        pThis->m_PutBackCharacter = 0;
        return 1;
    }
    if (pThis->m_EofSent)
        return 0;
    *ch = *pThis->m_pCurrentChar++;
    if (ch == L'\0')
    {
        *ch = '\0';
        pThis->m_EofSent = 1;
    }
    return 1;
}
void sstream_init(struct sstream* pThis, const wchar_t* psz)
{
    pThis->m_pCharacteres = psz;
    pThis->m_CurrentLine = 1;
    pThis->m_CurrentCol = 1;
    pThis->m_HasPutBack = 0;
    pThis->m_PutBackCharacter = L'\0';
    pThis->m_EofSent = 0;
    pThis->m_pCurrentChar = psz;
}
void sstream_ungetwc(struct sstream* pThis, wchar_t ch)
{
    assert(pThis->m_HasPutBack == 0);
    if (ch == L'\n')
    {
        if (pThis->m_CurrentLine > 1)
        {
            pThis->m_CurrentLine--;
        }
    }
    else
    {
        if (pThis->m_CurrentCol > 1)
        {
            pThis->m_CurrentCol--;
        }
    }
    pThis->m_PutBackCharacter = ch;
    pThis->m_HasPutBack = 1;
}
int sstream_getwc(struct sstream* pThis, wchar_t* ch)
{
    int b = GetCharCore(pThis, ch);
    if (b && *ch == L'\n')
    {
        pThis->m_CurrentLine++;
        pThis->m_CurrentCol = 1;
    }
    else
    {
        pThis->m_CurrentCol++;
    }
    return b;
}
size_t sstream_get_line_number(struct sstream* pThis)
{
    return pThis->m_CurrentLine;
}
size_t sstream_get_col_number(struct sstream* pThis)
{
    return pThis->m_CurrentCol;
}