%option noyywrap

%{
    #include "parser.h"
    #include "string.h"
    extern int yylval;
//if then for do done in while until case esac echo read return exit local elif else  declare
//test expr
%}


COMMENT ([#].+NEWLINE)
NEWLINE [\n]

DIGIT [0-9]
INTEGER {DIGIT}{DIGIT}*
STRING
WORD
ID [[:alpha:]][[:alnum:]]*

DECLARE "declare"
LOCAL "local"
RETURN "return"
EXIT "exit"
ECHO "echo"
READ "read"
FOR "for"
WHILE "while"
UNTIL "until"
DO "do"
DONE "done"
IN "in"
IF "if"
ELIF "elif"
ELSE "else"
TEST "test"
THEN "then"
FI "fi"
DONE "done"
CASE "case"
ESAC "esac"
LBRACKET "["
RBRACKET "]"
LPAREN "("
RPAREN ")"
LBRACE "{"
RBRACE "}"
QUOTE /"
APOSTROPHE '
ASSIGN "="
COMMA ";"
EXCL "!"
DOLLAR "$"
PLUS "+"
MINUS "-"
MULT "*"
DIV "/"
MOD "%"
QMARK "?"
NEQ "!="
BOR "|"

%%
"-o" { return ARG_O; }
"-n" { return ARG_N; }
"-z" { return ARG_Z; }
"-eq" { return ARG_EQ; }
"-ne" { return ARG_NE; }
"-gt" { return ARG_Gt; }
"-ge" { return ARG_GE; }
"-lt" { return ARG_LT; }
"-le" { return ARG_LE; }

{DECLARE} { return DECLARE; }
{LOCAL} { return LOCAL; }
{RETURN} { return RETURN; }
{EXIT} { return EXIT; }
{ECHO} { return ECHO; }
{READ} { return READ; }
{FOR} { return FOR; }
{WHILE} { return WHILE; }
{UNTIL} { return UNTIL; }
{DO} { return DO; }
{DONE} { return DONE; }
{IN} { return IN; }
{IF} { return IF; }
{ELIF} { return ELIF; }
{ELSE} { return ELSE; }
{TEST} { return TEST; }
{THEN} { return THEN; }
{FI} { return FI; }
{DONE} { return DONE; }
{CASE} { return CASE; }
{ESAC} { return ESAC; }
{LBRACKET} { return LBRACKET; }
{RBRACKET} { return RBRACKET; }
{LPAREN} { return LPAREN; }
{RPAREN} { return RPAREN; }
{LBRACE} { return LBRACE; }
{RBRACE} { return RBRACE; }
{QUOTE} { return QUOTE; }
{APOSTROPHE} { return APOSTROPHE; }
{ASSIGN} { return ASSIGN; }
{COMMA} { return COMMA; }
{EXCL} { return EXCL; }
{DOLLAR} { return DOLLAR; }
{PLUS} { return PLUS; }
{MINUS} { return MINUS; }
{MULT} { return MULT; }
{DIV} { return DIV; }
{MOD} { return MOD; }
{QMARK} { return QMARK; }
{NEQ} { return NEQ; }
{BOR} { return BOR; }

[ ] { return SPACE; }
{NEWLINE} { return NEWLINE; }
. { return UNKNOWN; }

%%
