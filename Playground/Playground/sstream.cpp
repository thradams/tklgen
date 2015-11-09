
#include "sstream.h"
#include <assert.h>

typedef int ResultCode;
#define ErrorTypeSuccess  0
#define ErrorTypeError  1
#define ErrorTypeUnexpectedToken  2
#define ErrorTypeEmpty  3
#define ErrorTypeReadToken  4
#define ErrorTypeEof  5
#define ErrorTypeOutOfMem  6

int sstream_init(struct sstream* pThis, const wchar_t* psz)
{
    pThis->pCharacteres = psz;
    pThis->line = 1;
    pThis->col = 1;
    pThis->bHasPutBack = 0;
    pThis->putBackCharacter = L'\0';
    pThis->bEofSent = 0;
    pThis->pCurrentChar = psz;
    return ErrorTypeSuccess;
}

void sstream_ungetwc(struct sstream* pThis,
    wchar_t ch)
{
    assert(pThis->bHasPutBack == 0);
    pThis->line = pThis->previousline;
    pThis->col = pThis->previouscol;
    pThis->putBackCharacter = ch;
    pThis->bHasPutBack = 1;
  
}

ResultCode sstream_getwc(struct sstream* pThis,
    wchar_t* ch)
{
    if (pThis->bHasPutBack)
    {
        *ch = pThis->putBackCharacter;
        pThis->bHasPutBack = 0;
        pThis->putBackCharacter = 0;
        pThis->previousline = pThis->line;
        pThis->previouscol = pThis->col;

        if (*ch == L'\n')
        {
            pThis->line++;
            pThis->col = 0;
        }
        else
        {
            pThis->col++;
        }

        return ErrorTypeSuccess;
    }

    if (pThis->bEofSent)
    {
        return ErrorTypeEof;
    }

    *ch = *pThis->pCurrentChar++;

    if (*ch == L'\0')
    {
        *ch = '\0';
        pThis->bEofSent = 1;
    }

    pThis->previousline = pThis->line;
    pThis->previouscol = pThis->col;

    if (*ch == L'\n')
    {
        pThis->line++;
        pThis->col = 0;
    }
    else
    {
        pThis->col++;
    }

    return ErrorTypeSuccess;
}
