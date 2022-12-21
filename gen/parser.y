%{
#include "lexer.h"
%}
%right '='
%left '+' '-' '*' '/'

%token INTEGER CHAIN
%start start

%%


E : E '+' E {$$ = $1 + $3;}
    | E '*' E {$$ = $1 * $3;}
    | E '-' E {$$ = $1 - $3;}
    | E '/' E {$$ = $1 / $3;}
    | '(' E ')' {$$ = $2;}
    | INTEGER
    ;

start : start E '\n' {printf ("%d\n",$2);}
    |
    ;
%%
/*
int main (void)
{
    yyparse();
    return 0;
}
*/

int yyerror (char * s)
{
    printf(" Erreur : %s\n",s);
    return 0;
}
