%option noyywrap yylineno

%{
#define YYERROR_VERBOSE 1

#include "parser.h"
#include "string.h"
#include "log.h"
%}

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
EXPR "expr"
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
QUOTED_STRING \"(\\.|[^"\\])*\"
APOSTROPHED_STRING \'(\\.|[^'\\])*\'

WORD [+-]?[0-9]+|[a-zA-Z][a-zA-Z0-9_]*

%%
{COMMENT} { log_debug("comment; ignoring"); }

"-o" { return ARG_O; }
"-n" { return ARG_N; }
"-z" { return ARG_Z; }
"-eq" { return ARG_EQ; }
"-ne" { return ARG_NE; }
"-gt" { log_debug("ARG_GT"); return ARG_GT; }
"-ge" { return ARG_GE; }
"-lt" { return ARG_LT; }
"-le" { return ARG_LE; }

{EXIT} {log_debug("EXIT : %s",yytext); return EXIT; }
{DECLARE} { return DECLARE; }
{LOCAL} { return LOCAL; }
{RETURN} { return RETURN; }
{ECHO_CALL} { return ECHO_CALL; }
{READ} { return READ; }
{FOR} { log_debug("FOR"); return FOR; }
{WHILE} { return WHILE; }
{UNTIL} { return UNTIL; }
{DO} { log_debug("DO"); return DO; }
{IN} { log_debug("IN"); return IN; }
{IF} { return IF; }
{ELIF} { return ELIF; }
{ELSE} { return ELSE; }
{TEST} { return TEST; }
{THEN} { return THEN; }
{FI} { return FI; }
{DONE} { log_debug("DONE"); return DONE; }
{CASE} { return CASE; }
{ESAC} { return ESAC; }
{EXPR} { return EXPR; }
{LBRACKET} { return LBRACKET; }
{RBRACKET} { return RBRACKET; }
{LPAREN} { return LPAREN; }
{RPAREN} { log_debug("RPAREN"); return RPAREN; }
{LBRACE} { log_debug("LBRACE"); return LBRACE; }
{RBRACE} { log_debug("RBRACE"); return RBRACE; }
{QUOTE} { return QUOTE; }
{APOSTROPHE} { return APOSTROPHE; }
{ASSIGN} { log_debug("ASSIGN"); return ASSIGN; }
{SEMICOLON} {log_debug("SEMICOLON : %s",yytext); return SEMICOLON; }
{EXCL} { return EXCL; }
{DOLLAR} { log_debug("DOLLAR"); return DOLLAR; }
{PLUS} { log_debug("PLUS"); return PLUS; }
{MINUS} { return MINUS; }
{MULT} { return MULT; }
{DIV} { return DIV; }
{MOD} { return MOD; }
{QMARK} { return QMARK; }
{NEQ} { return NEQ; }
{BOR} { return BOR; }
{QUOTED_STRING} { log_debug("QUOTED_STRING: %s", yytext); return QUOTED_STRING;}
{APOSTROPHED_STRING} { log_debug("APOSTROPHED_STRING: %s", yytext); return APOSTROPHED_STRING;}

{SPACE} { log_debug("space '%s'", yytext); }
{NEWLINE} { log_debug("NEWLINE"); }
{WORD} { yylval.strval = yytext; log_debug("WORD: %s", yytext); return WORD; }

%%
