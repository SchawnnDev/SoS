%{
#include "lexer.h"
%}
%right ASSIGN
%left ARG_A ARG_O ARG_N ARG_Z ARG_EQ ARG_NE ARG_GT ARG_GE ARG_LT ARG_LE

%token INTEGER STRING WORD EXPR
%token ID DECLARE LOCAL
%token RETURN EXIT ECHO_CALL READ
%token FOR WHILE UNTIL DO DONE IN
%token IF ELIF ELSE TEST THEN FI
%token CASE ESAC
%token LBRACKET RBRACKET LPAREN RPAREN LBRACE RBRACE QUOTE APOSTROPHE
%token ASSIGN
%token COMMA EXCL DOLLAR PLUS MINUS MULT DIV MOD QMARK
%token NEQ BOR ARG_A ARG_O ARG_N ARG_Z ARG_EQ ARG_NE ARG_GT ARG_GE ARG_LT ARG_LE
%token NEWLINE UNKNOWN
%start program

%%
program : list_instructions
    ;

list_instructions : list_instructions COMMA instructions
    | instructions
    ;

instructions : ID ASSIGN concatenation
    | ID LBRACKET operand_integer RBRACKET ASSIGN concatenation
    | DECLARE ID LBRACKET INTEGER RBRACKET
    | IF test_block THEN list_instructions else_part FI
    | FOR ID DO list_instructions DONE
    | FOR ID IN list_instructions DO list_instructions DONE
    | WHILE test_block DO list_instructions DONE
    | UNTIL test_block DO list_instructions DONE
    | CASE operand IN list_case ESAC
    | ECHO_CALL list_operand
    | READ ID
    | READ ID LBRACKET operand_integer RBRACKET
    | declare_fct
    | function_call
    | RETURN
    | RETURN operand_integer
    | EXIT
    | EXIT operand_integer
    ;

else_part : ELIF test_block THEN list_instructions else_part
    | ELSE list_instructions
    |
    ;

list_case : list_case filter RPAREN list_instructions COMMA COMMA
    | filter RPAREN list_instructions COMMA COMMA
    ;

filter : WORD
	| QUOTE STRING QUOTE
	| APOSTROPHE STRING APOSTROPHE
    | filter BOR WORD
    | filter BOR QUOTE STRING QUOTE
    | filter BOR APOSTROPHE STRING APOSTROPHE
    | MULT
    ;

list_operand : list_operand operand
    | operand
    | DOLLAR LBRACE ID LBRACKET MULT RBRACKET RBRACE
    ;

concatenation : concatenation operand
    | operand
    ;

test_block : TEST test_expr
    ;

test_expr : test_expr ARG_O test_expr2
    | test_expr2
    ;

test_expr2 : test_expr2 ARG_A test_expr3
    | test_expr3
    ;

test_expr3 : LPAREN test_expr RPAREN
    | EXCL LPAREN test_expr RPAREN
    | test_instruction
    | EXCL test_instruction
    ;

test_instruction : concatenation ASSIGN concatenation
    | concatenation NEQ concatenation
    | operator1 concatenation
    | operand operator2 operand
    ;

operand : DOLLAR LBRACE ID RBRACE
	| DOLLAR LBRACE ID LBRACKET operand_integer RBRACKET RBRACE
    | WORD
    | DOLLAR INTEGER
    | DOLLAR MULT
    | DOLLAR QMARK
    | QUOTE STRING QUOTE
    | APOSTROPHE STRING APOSTROPHE
    | DOLLAR LPAREN EXPR sum_integer RPAREN
    | DOLLAR LPAREN function_call RPAREN
    ;

operator1 : ARG_N
    | ARG_Z
    ;

operator2 : ARG_EQ
    | ARG_NE
    | ARG_GT
    | ARG_GE
    | ARG_LT
    | ARG_LE
    ;

sum_integer : sum_integer plus_or_minus mult_integer
    | mult_integer
    ;

mult_integer : mult_integer mult_div_mod operand_integer
    | operand_integer
    ;

operand_integer : DOLLAR LBRACE ID RBRACE
	| DOLLAR LBRACE ID LBRACKET operand_integer RBRACKET RBRACE
	| DOLLAR INTEGER
    | plus_or_minus DOLLAR LBRACE ID RBRACE
    | plus_or_minus DOLLAR RBRACE ID LBRACKET operand_integer RBRACKET RBRACE
    | plus_or_minus DOLLAR  INTEGER
    | INTEGER
    | plus_or_minus INTEGER
    | LPAREN sum_integer RPAREN
    ;

plus_or_minus : PLUS
    | MINUS
    ;

mult_div_mod : MULT
     | DIV
     | MOD
     ;

declare_fct : ID LPAREN RPAREN LBRACE declare_loc list_instructions RBRACE
    ;

declare_loc : declare_loc LOCAL ID ASSIGN concatenation COMMA
    |
    ;

function_call : ID list_operand
    | ID
    ;
%%
/*
int main (voID)
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
