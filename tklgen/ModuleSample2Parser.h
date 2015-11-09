/*
 * Generated by TKLGEN - VersionOct 27 2014
 * Copyright (C) 2013, Thiago Adams (thiago.adams@gmail.com)
 * www.thradams.com
 *
 */
#ifndef MODULESAMPLE2_H
#define MODULESAMPLE2_H


#include "sstream.h"


typedef int ErrorCode;

enum LanguageSample2_Error
{
    ErrorCodeUnexpectedToken,
};

struct Context
{
    wchar_t lexeme[200];
    enum LanguageSample2_Tokens token;
    struct sstream* stream;
};

int Context_Init(struct Context*);
int Context_AttachStream(struct Context* p, struct sstream* ss);
const wchar_t* Context_GetLexeme(struct Context* p);
enum LanguageSample2_Tokens Context_GetToken(struct Context* p);

int Parse_Main(struct Context*);

/*Forward declarations for actions*/
int Action_PrintAnd(struct Context* /*ctx*/);
int Action_PrintOr(struct Context* /*ctx*/);
int Action_PrintGreaterThan(struct Context* /*ctx*/);
int Action_PrintLessThan(struct Context* /*ctx*/);
int Action_PrintLessEqualThan(struct Context* /*ctx*/);
int Action_PrintGreaterEqualThan(struct Context* /*ctx*/);
int Action_PrintEquals(struct Context* /*ctx*/);
int Action_PrintPlus(struct Context* /*ctx*/);
int Action_PrintMinus(struct Context* /*ctx*/);
int Action_PrintMulti(struct Context* /*ctx*/);
int Action_PrintDiv(struct Context* /*ctx*/);
int Action_PrintPower(struct Context* /*ctx*/);
int Action_PrintLexeme(struct Context* /*ctx*/);
int Action_PrintNeg(struct Context* /*ctx*/);

#endif