#include "filetutils.h"
#include <stdlib.h>


size_t get_file_size(FILE* hFile)
{
  fseek(hFile, 0L, SEEK_SET);
  size_t sz = 0;
  wint_t er = 0;
  while ((er = fgetwc(hFile)) != WEOF)
  {
    sz++;
  }
  fseek(hFile, 0L, SEEK_SET);
  return sz;
}

size_t ftowstr(FILE *hFile, wchar_t** ppszOut)
{
  if (!ppszOut)
  {
    return 0;
  }

  size_t sz = get_file_size(hFile);
  *ppszOut = (wchar_t*)malloc(sizeof(wchar_t)* (sz + 1));

  if (!(*ppszOut))
  {
    return 0;
  }

  wint_t c;
  size_t n = 0;
  while ((c = fgetwc(hFile)) != WEOF)
  {
    (*ppszOut)[n++] = c;
  }
  (*ppszOut)[n] = 0;
  return n;
}

size_t string_to_file(char* pszFileName, wchar_t* psz, size_t len)
{
  FILE *hFile = fopen(pszFileName, "w, ccs=UTF-8");
  if (hFile)
  {
    fwrite(psz, sizeof(wchar_t), len, hFile);
    fclose(hFile);
    return len;
  }
  return 0;
}

int file_to_string(const char* fileName, wchar_t** strBuffer)
{
  int r = 1;
  FILE *hFile = fopen(fileName, "r, ccs=UTF-8");
  if (hFile)
  {
    size_t n = ftowstr(hFile, strBuffer);
    fclose(hFile);
    r = 0;
  }
  return r;
}
