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

COMMENT ([#].+NEWLINE)
%%
"+"|"-"|"*"|"/"|"("|")" {return yytext[0];}
{DIGIT}+ {yylval=atoi(yytext); return INTEGER;}






"\n" {return yytext[0];}

%%
