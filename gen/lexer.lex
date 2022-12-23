%option noyywrap yylineno

%{
#include "parser.h"
#include "string.h"

#include "log.h"
%}
/*
COMMENT [#].*

DIGIT [0-9]
INTEGER {DIGIT}{DIGIT}*
STRING "string"
WORD "10"
ID [[:alpha:]][[:alnum:]] *[a-z]*  [[:alpha:]][[:alnum:]]* */

DIGIT [0-9]
INTEGER {DIGIT}{DIGIT}*
ID [a-zA-Z]([a-zA-Z]|[0-9])*

NEWLINE \n
DECLARE "declare"
LOCAL "local"
RETURN "return"
EXIT "exit"
ECHO_CALL "echo"
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
CASE "case"
ESAC "esac"
LBRACKET "["
RBRACKET "]"
LPAREN "("
RPAREN ")"
LBRACE "{"
RBRACE "}"
QUOTE ["]
APOSTROPHE '
ASSIGN "="
SEMICOLON ";"
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
SPACE [ ]
COMMENT #.*
WORD [^\n=;+-]*
/* ([^NEWLINE]|[^COMMENT]|[^ASSIGN])* */

%%
{COMMENT} { log_trace("comment; ignoring"); }

"-o" { return ARG_O; }
"-n" { return ARG_N; }
"-z" { return ARG_Z; }
"-eq" { return ARG_EQ; }
"-ne" { return ARG_NE; }
"-gt" { return ARG_GT; }
"-ge" { return ARG_GE; }
"-lt" { return ARG_LT; }
"-le" { return ARG_LE; }

{EXIT} {log_trace("EXIT : %s",yytext); return EXIT; }
{DECLARE} { return DECLARE; }
{LOCAL} { return LOCAL; }
{RETURN} { return RETURN; }
{ECHO_CALL} { return ECHO_CALL; }
{READ} { return READ; }
{FOR} { return FOR; }
{WHILE} { return WHILE; }
{UNTIL} { return UNTIL; }
{DO} { return DO; }
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
{ASSIGN} { log_trace("ASSIGN"); return ASSIGN; }
{SEMICOLON} {log_trace("SEMICOLON : %s",yytext); return SEMICOLON; }
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

[ ] { log_trace("space '%s'", yytext); }
{NEWLINE} { log_trace("NEWLINE"); }
{WORD} { yylval.strval = yytext; log_trace("WORD: %s", yytext); return WORD; }

%%
