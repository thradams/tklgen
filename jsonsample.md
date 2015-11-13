 

JSON Grammar sample:

See : [http://www.json.org/|http://www.json.org/]

```
module Sample2
{
  language Sample2
  {
    token EndMark = '\0';    

    token LEFT_PARENTHESIS = '(';
    token RIGHT_PARENTHESIS = ')';    
    token COMMA = ',';
    
    token COLON = ':';
    token SEMICOLON = ';';
    
    token LEFT_SQUARE_BRACKET = '[';
    token RIGHT_SQUARE_BRACKET = ']';
    
    token LEFT_CURLY_BRACKET = '{';    
    token RIGHT_CURLY_BRACKET = '}';


    interleave Blanks = (" " | "\n" | "\t" | "\r") +;
    token True = "true";
    token False = "false";
    token Null = "null";
    token Number = ('+' | '-') ? ('0'..'9') + ('.' '0'..'9') ? (('E' | 'e')('+' | '-') ? '0'..'9') ?;

    token Identifier = ('a'..'z' | 'A'..'Z' | '_') + ('_' | '0'..'9' | 'a'..'z' | 'A'..'Z')* ;
    token String = '"'(

      (' '..'!' | '#'..'[' | ']'..'\u00FF') |

      ('\\'('"' | '\\' | '/' | 'b' | 'f' | 'n' | 'r' | 't' | ('u'('0'..'9' | 'A'..'F' | 'a'..'f')('0'..'9' | 'A'..'F' | 'a'..'f')('0'..'9' | 'A'..'F' | 'a'..'f')('0'..'9' | 'A'..'F' | 'a'..'f'))))

    ) *

    '"';

    syntax Value = String |
                   Number |
                   Object |
                   Array |
                   "true" |
                   "false" |
                   "null";

    syntax Object = '{' Members '}';
    syntax Array = '[' Elements ']';

    syntax Elements = Value ElementsFollow | empty;
    syntax ElementsFollow = ',' Value ElementsFollow | empty;

    syntax Members = Pair MembersFollow | empty;
    syntax MembersFollow = ',' Pair MembersFollow | empty;

    syntax Pair = String ':' Value;
    syntax Main = Object EndMark | Array EndMark;
  }
}


```
