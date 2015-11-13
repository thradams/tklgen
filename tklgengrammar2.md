
tklgen grammar

```

module tklgen2
{
  language tklgen
  {    
    token PlusCharacter                = '+';
    token LiteralChar                  = "'"((' '..'&' | '('..'[' | ']'..'~') |
	                                ('\\'('\'' | '\\' | '/' | '0' | 'b' | 'f' | 'n' | 'r' | 't' | ('u'('0'..'9' | 'A'..'F' | 'a'..'f')('0'..'9' | 'A'..'F' | 'a'..'f')('0'..'9' | 'A'..'F' | 'a'..'f')('0'..'9' | 'A'..'F' | 'a'..'f'))))
                                         )* "'";
    token RangeSeparator               = "..";
    token EqualsCharacter              = '=';
    token SemicolonCharacter           = ';';
    token CommaCharacter               = ',';
    
    token KeywordEmpty                 = "empty";
    token KeywordAny                   = "any";
    token KeywordModule                = "module";
    token KeywordLanguage              = "language";
    token KeywordInterleave            = "interleave";
    token KeywordSyntax                = "syntax";
    token KeywordToken                 = "token";

    token Identifier                   = ('a'..'z' | 'A'..'Z' | '_' | '$') +
                                         ('0'..'9' + ('a'..'z' | 'A'..'Z' | '_' | '$') +) ?;
        
    token QuestionMarkCharacter        = '?';
    token AsteriskCharacter            = '*';
    token LeftCurlyBracketCharacter    = '{';
    token RightCurlyBracketCharacter   = '}';
    token VerticalLineCharacter        = '|';
    token AmpersandCharacter           = '&';
    token LeftParenthesisCharacter     = '(';
    token RightParenthesisCharacter    = ')';
    token StringLiteral                = '"'((' '..'!' | '#'..'[' | ']'..'~') | 
                                         ('\\'('"' | '\\' | '0' | '/' | 'b' | 'f' | 'n' | 'r' | 't' | ('u'('0'..'9' | 'A'..'F' | 'a'..'f')('0'..'9' | 'A'..'F' | 'a'..'f')('0'..'9' | 'A'..'F' | 'a'..'f')('0'..'9' | 'A'..'F' | 'a'..'f'))))
                                         )* '"';
    
    interleave Blanks                  = ((" " | "\n" | "\t" | "\r") +) |
                                             ("//"(' '..'~') * ("\n" | "\r\n")) |
                                             ("/*"(((' '..')' | '+'..'~' | "\n" | "\r" | "\t")) |
                                             ("*"(' '..'.' | '0'..'~' | "\n" | "\r" | "\t")))* "*/");

    syntax Main = ModuleBody;
    syntax ModuleBody = "module" Identifier
                        '{'
                           LanguageBody 
                        '}';

    syntax LanguageBody = "language" Identifier
                          '{'
                             Declarations 
                          '}';

    syntax Declarations = Declaration Declarations | empty;

    syntax Declaration = TokenDeclaration |
                         SyntaxDeclaration |
                         InterleaveDeclaration;

    syntax TokenDeclaration = 
       "token" Identifier '=' TokenExpression ';';

    syntax InterleaveDeclaration = 
      "interleave" Identifier '=' TokenExpression ';';
    
    syntax SyntaxDeclaration = 
      "syntax" Identifier '=' SyntaxExpression ';';

    //tokens expressions-------------------------------------
    //Tip:
    //E --> T { T };
    //T --> F { "|" F };
    //F --> P ["?"];
    //P --> V | "(" E ")";
    //V --> v [".." v];
    
    syntax TokenExpression = T ZeroOrMoreT;
    syntax ZeroOrMoreT = T ZeroOrMoreT | empty;

    syntax T = F ZeroOrMoreOrF;
    syntax OrF = '|' F;
    syntax ZeroOrMoreOrF = OrF ZeroOrMoreOrF | empty;

    syntax F = P OptOperator;

    syntax OptOperator = QuestionMarkCharacter |
                         AsteriskCharacter     |
                         PlusCharacter         |
                         empty;

    syntax P = V | '(' TokenExpression ')';
    syntax V = LiteralChar RangeOpt |
               Identifier           |
               KeywordAny           |
               StringLiteral;

    syntax RangeOpt  = RangeSeparator LiteralChar | empty;
    
    //end token expressions---------------------------------------------------


    // Syntax expression----------------------------------------------------
    //Tip:
    //E --> T { | T};
    //T --> F { F };
    //T --> Empty;
    //F --> NAME | CHAR | STRING | OPENB Action CLOSEB;

    syntax SyntaxExpression = SyntaxExpressionSingle SyntaxExpressionMoreSingle;

    syntax SyntaxExpressionMoreSingle = 
      '|' SyntaxExpressionSingle SyntaxExpressionMoreSingle |
      empty;

    syntax SyntaxExpressionSingle = SyntaxExpressionItem SyntaxExpressionSingleFollow;
    syntax SyntaxExpressionSingle = KeywordEmpty;

    syntax SyntaxExpressionSingleFollow = SyntaxExpressionItem SyntaxExpressionSingleFollow |
                            empty;

    syntax SyntaxExpressionItem = Identifier |
                                  LiteralChar |
                                  StringLiteral |
                                  SyntaxExpressionAction;

    syntax SyntaxExpressionAction = '{' Identifier '}';

    //end syntax expression ---------------------------------------------------
  }
}


```
