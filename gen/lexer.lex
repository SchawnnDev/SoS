%option noyywrap

%{
    #include "parser.h"
    #include "string.h"
    extern int yylval;
//if then for do done in while until case esac echo read return exit local elif else  declare
//test expr
%}

NEWLINE [\n]
DIGIT [0-9]
LBRACKET "["
RBRACKET "]"
QUOTE 'COMMA ('["]"
       "APOSTROPHE '[']"]

COMMENT ([#].+NEWLINE)

INTEGER
STRING
WORD
ID
DECLARE
LOCAL
RETURN
EXIT
ECHO
READ
FOR
WHILE
UNTIL
DO
DONE
IN
IF
ELIF
ELSE
TEST
THEN
FI
DONE
CASE
ESAC
LBRACKET
RBRACKET
LPAREN
RPAREN
LBRACE
RBRACE
QUOTE
APOSTROPHE
ASSIGN
COMMA
EXCL
DOLLAR
PLUS
MINUS
MULT
DIV
MOD
QMARK
NEQ
BOR

%%
"+"|"-"|"*"|"/"|"("|")" {return yytext[0];}
{DIGIT}+ {yylval=atoi(yytext); return INTEGER;}

ARG_O ARG_N ARG_Z ARG_EQ ARG_NE ARG_GT ARG_GE ARG_LT ARG_LE




"\n" {return yytext[0];}

%%
