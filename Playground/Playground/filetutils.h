#pragma once
#include <wchar.h>
#include <stdio.h>

size_t get_file_size(FILE* hFile);
size_t ftowstr(FILE *hFile, wchar_t** ppszOut);
size_t string_to_file(char* pszFileName, wchar_t* psz, size_t len);
int file_to_string(const char* fileName, wchar_t** strBuffer);