
##TklGen

#Introduction

Tklgen is a scanner and parser generator.

The grammar used by tklgen is LL(1) and the generated parser is C++ top-down recursive-descendent.

The input file is a grammar LL(1) describing productions and terminals.

###Grammar syntax

```c
module tklgen
{
  language tklgen
  {
    //tokens (terminals)
    //syntax (productions)
  }
}
```

You can comment your grammar using line or block comments in the same way of C++.

The identifier after the **module** keyword is used to name the output files. 

The identifer after the **language** keyword is used as C++ namespaces to avoid name conflits.

Inside the language there are token declarations (terminals), and syntax declarations (productions).  

The **token** declaration is used to tell all the terminal symbols.

Sample:
```
  token Integer  = ('0'..'9')+;
```

Tokens operators

```
 ?         : Optional
 +         : One or more
 *         : Zero or more
 |         : Or operator
 'a'...'z' : Range
 "string"  : Sequence of characteres
 'c'       : One characteres

 Escape
 '\0' '\t' '\b' '\f' '\n' '\t'

 Unicode
 "\u0000"
 '\u0000'
```

Tklgen don't suport negation. "Any char except this one"
For this task I use this script [[excludechar.html|exclude char]]

The **interleave** is an especial token that is ignored by the scanner. 

Sample:
```
  interleave Blanks = (' ' | '\n' | '\r')+;
```


The **syntax** declaration declares the production. The "Main" production is obrigatory.

Sample:
```
  syntax Main = A;
  syntax A = Integer | empty;
```

The **empty** keyword declares an empty production.\\
The or "|" symbol is used to declare multiple productions.

A complete sample is the tklgen grammar used to describe itself:

[TKLGEN Grammar](tklgengrammar2.md)


###Parser components:

Generated files:

 * Parser (header and cpp)
 * DFA  (used by tokenizer)

Existing files:

 * InputStream: [[FileStream2.htm|FileStream]], [[StringStream2.htm|StringStream]]
 * Tokenizer: [[Tokenizertklgen2.htm|Tokenizer]]
 * StringStream + Tokenizer togeter [[StringStreamTokenizer.htm|StringStreamTokenizer]]

###Calculator

[Expression Sample](expressionsample2.md)

[JSON grammar sample](jsonsample.md)


###References

* "Compilers: Principles, Techniques, and Tools", Alfred V. Aho , Ravi Sethi , Jeffrey D. Ullman


