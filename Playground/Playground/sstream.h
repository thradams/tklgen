#ifndef __STRINGSTREAM_H__
#define __STRINGSTREAM_H__

#include <wchar.h>

struct sstream
{
    const wchar_t* pCharacteres;
    const wchar_t* pCurrentChar;

    size_t   line;
    size_t   col;

    size_t   previousline;
    size_t   previouscol;

    int      bHasPutBack;
    wchar_t  putBackCharacter;
    int      bEofSent;
};

#define STRING_STREAM_INIT { 0, 0, 0, 0,0, 0, 0, 0,0,0}


int sstream_init(struct sstream* pThis, const wchar_t* psz);
void sstream_ungetwc(struct sstream* pThis, wchar_t ch);
int sstream_getwc(struct sstream* pThis, wchar_t* ch);


#endif
