/*
 * Generated by TKLGEN - VersionNov 18 2015
 * Copyright (C) 2013, Thiago Adams (thiago.adams@gmail.com)
 * www.thradams.com
 *
 */
#ifndef PLAYGROUNDMODULELEX_H
#define PLAYGROUNDMODULELEX_H

#include <wchar.h>


typedef enum 
{
    TKPreInclude,
    TKPreDefine,
    TKPreLine,
    TKPreUndef,
    TKPreError,
    TKPrePragma,
    TKPreIf,
    TKPreIfDef,
    TKPreIfNDef,
    TKPreElse,
    TKPreEndif,
    TKPreElif,
    TKEllipsis,
    TKRightAssign,
    TKLeftAssign,
    TKAddAssign,
    TKSubAssign,
    TKMultAssign,
    TKDivAssign,
    TKModAssign,
    TKAndAssign,
    TKXorAssign,
    TKOrAssign,
    TKRightOp,
    TKLeftOp,
    TKIncOp,
    TKDecOp,
    TKPtrOp,
    TKAndOp,
    TKOrOp,
    TKLeOp,
    TKGeOp,
    TKEqOp,
    TKNeOp,
    TKEXCLAMATION_MARK,
    TKQUOTATION_MARK,
    TKDOLLAR_SIGN,
    TKPERCENT_SIGN,
    TKAMPERSAND,
    TKAPOSTROPHE,
    TKLEFT_PARENTHESIS,
    TKRIGHT_PARENTHESIS,
    TKASTERISK,
    TKPLUS_SIGN,
    TKCOMMA,
    TKHYPHEN_MINUS,
    TKFULL_STOP,
    TKSOLIDUS,
    TKCOLON,
    TKSEMICOLON,
    TKLESS_THAN_SIGN,
    TKEQUALS_SIGN,
    TKGREATER_THAN_SIGN,
    TKQUESTION_MARK,
    TKCOMMERCIAL_AT,
    TKLEFT_SQUARE_BRACKET,
    TKREVERSE_SOLIDUS,
    TKRIGHT_SQUARE_BRACKET,
    TKCIRCUMFLEX_ACCENT,
    TKLOW_LINE,
    TKGRAVE_ACCENT,
    TKLEFT_CURLY_BRACKET,
    TKVERTICAL_LINE,
    TKRIGHT_CURLY_BRACKET,
    TKLINECOMMENT,
    TKCOMMENT,
    TKCHAR,
    TKSTRING,
    TKNUMBER,
    TKBlanks,
    TKBreakLine,
    TKNotBreakLine,
    TKEndMark,
    TKAuto,
    TKBreak,
    TKCase,
    TKChar,
    TKConst,
    TKContinue,
    TKDefault,
    TKDo,
    TKDouble,
    TKElse,
    TKEnum,
    TKExtern,
    TKFloat,
    TKFor,
    TKGoto,
    TKIf,
    TKInline,
    TKInt,
    TKLong,
    TKRegister,
    TKRestrict,
    TKReturn,
    TKShort,
    TKSigned,
    TKSizeOf,
    TKStatic,
    TKStruct,
    TKSwitch,
    TKTypedef,
    TKUnion,
    TKUnsigned,
    TKVoid,
    TKVolatile,
    TKWhile,
    TKAlignAs,
    TKAlignOf,
    TKAtomic,
    TKBool,
    TKComplex,
    TKGeneric,
    TKImaginary,
    TKNoReturn,
    TKStaticAssert,
    TKThreadLocal,
    TKFunc,
    TKNumber,
    TKIdentifier,
} PlaygroundLang_Tokens;

const wchar_t* PlaygroundLang_Tokens_ToString(PlaygroundLang_Tokens e);


int PlaygroundLang_GetTokenFromState(int state, PlaygroundLang_Tokens* tk);
int PlaygroundLang_GetNext(int state, wchar_t ch);
int PlaygroundLang_IsInterleave(PlaygroundLang_Tokens tk);

#endif
