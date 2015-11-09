#pragma once
#include <ostream>
#include <string>


#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)

#define __WDATE__ WIDEN(__DATE__)

void FindReplace(std::wstring& in_this_string,
    const std::wstring& find,
    const std::wstring& replace);

std::wstring MakeUpper(const std::wstring& s);

inline void find_replace(std::wstring& in_this_string,
                         const std::wstring& find,
                         const std::wstring& replace)
{
    std::wstring::size_type pos = 0;
    while( std::wstring::npos != (pos = in_this_string.find(find, pos)) )
    {
        in_this_string.replace(pos, find.length(), replace);
        pos += replace.length();
    }
}

void PrintGeneratedFileHeader(std::wostream& os);
void PrintGeneratedFileLicense(std::wostream& os);

const wchar_t * TokenizerFile();
const wchar_t * StringStreamFile();
const wchar_t * FileStreamFile();


bool FileExists(const wchar_t* fileName);
void StringToNewFile(const wchar_t* psz, const wchar_t* fileName);