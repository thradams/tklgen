/*
 * Generated by TKLGEN - VersionNov  9 2015
 * Copyright (C) 2013, Thiago Adams (thiago.adams@gmail.com)
 * www.thradams.com
 *
 */
#ifndef PLAYGROUNDMODULE_H
#define PLAYGROUNDMODULE_H



enum PlaygroundLang_Actions
{
  PrintNewLine,
  PrintPlus,
  PrintMinus,
  PrintMulti,
  PrintDiv,
  PrintPower,
  PrintLexeme,
  PrintNeg,
  OnError
};

const wchar_t* PlaygroundLang_Actions_Text(enum PlaygroundLang_Actions e);
typedef int (*PlaygroundLang_OnAction)(enum PlaygroundLang_Actions a, struct PlaygroundLang_ContextT* ctx);

#define LEXEME_MAX 400

typedef struct PlaygroundLang_ContextT
{
    wchar_t* lexeme;
    int lexemeSize;
    enum PlaygroundLang_Tokens token;
    struct sstream* stream;
    PlaygroundLang_OnAction   on_action;
} PlaygroundLang_Context;


int PlaygroundLang_Parse(struct sstream* ss, PlaygroundLang_OnAction action, void* context);
int PlaygroundLang_Parse_Str(const wchar_t* text, PlaygroundLang_OnAction action, void* context);



#endif