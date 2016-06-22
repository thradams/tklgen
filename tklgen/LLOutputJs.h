#pragma once
#include <iostream>
#include <string>
#include <map>
#include "Grammar.h"

//Gera a saida como um parser de funcoes recursivas sem tabela
void GenerateDescRecJs(std::wostream& ,
                     Grammar& g,
                     MMap& mmap,
                     const std::wstring& tokenPrefix,
                     const std::wstring& parserFileSuffix);

//Gera a saida como um parser de funcoes recursivas sem tabela
void GenerateDescRecHeaderJs(std::wostream& ,
                           Grammar& g,
                           MMap& mmap,
                           const std::wstring& tokenPrefix);
const wchar_t* SampleFileJs();
