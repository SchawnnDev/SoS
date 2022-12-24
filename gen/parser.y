%define parse.trace

%{
#include "lexer.h"
#include "compilation.h"
%}

%union { char *strval; }
%right ASSIGN
%left ARG_A ARG_O ARG_N ARG_Z ARG_EQ ARG_NE ARG_GT ARG_GE ARG_LT ARG_LE

%token INT STRING WORD EXPR
%token DECLARE LOCAL
%token RETURN EXIT ECHO_CALL READ
%token FOR WHILE UNTIL DO DONE IN
%token IF ELIF ELSE TEST THEN FI
%token CASE ESAC
%token LBRACKET RBRACKET LPAREN RPAREN LBRACE RBRACE QUOTE APOSTROPHE
%token ASSIGN
%token SEMICOLON EXCL DOLLAR PLUS MINUS MULT DIV MOD QMARK
%token NEQ BOR ARG_A ARG_O ARG_N ARG_Z ARG_EQ ARG_NE ARG_GT ARG_GE ARG_LT ARG_LE

%type <strval> WORD id concatenation operand DOLLAR LBRACE RBRACE QUOTE STRING APOSTROPHE ASSIGN
%start program

%%
program : {log_trace("program : list_instructions")} list_instructions {log_trace("program : list_instructions")}
    ;

list_instructions : list_instructions SEMICOLON instructions {log_trace("program : list_instructions SEMICOLON instructions")}
    | instructions {log_trace("program : list_instructions -> instructions")}
    ;

instructions : id ASSIGN concatenation {log_trace("instructions: (%s, %s, %s)", $1,$2,$3); assign(); }
    | id LBRACKET operand_int RBRACKET ASSIGN concatenation
    | DECLARE id LBRACKET int RBRACKET
    | IF test_block THEN list_instructions else_part FI
    | FOR id DO list_instructions DONE
    | FOR id IN list_instructions DO list_instructions DONE
    | WHILE test_block DO list_instructions DONE
    | UNTIL test_block DO list_instructions DONE
    | CASE operand IN list_case ESAC
    | ECHO_CALL list_operand
    | READ id
    | READ id LBRACKET operand_int RBRACKET
    | declare_fct
    | function_call
    | RETURN
    | RETURN operand_int
    | EXIT
    | EXIT operand_int
    ;

else_part : ELIF test_block THEN list_instructions else_part
    | ELSE list_instructions
    |
    ;

list_case : list_case filter RPAREN list_instructions SEMICOLON SEMICOLON
    | filter RPAREN list_instructions SEMICOLON SEMICOLON
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
    | DOLLAR LBRACE id LBRACKET MULT RBRACKET RBRACE
    ;

concatenation : concatenation operand { log_trace("concat : %s %s", $1, $2); }
    | operand { log_trace("concatenation"); }
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

operand : DOLLAR LBRACE id RBRACE
	| DOLLAR LBRACE id LBRACKET operand_int RBRACKET RBRACE
    | WORD {log_trace("operand : WORD (%s)", $1);}
    | DOLLAR int
    | DOLLAR MULT
    | DOLLAR QMARK
    | QUOTE STRING QUOTE
    | APOSTROPHE STRING APOSTROPHE
    | DOLLAR LPAREN EXPR sum_int RPAREN
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

sum_int : sum_int plus_or_minus mult_int
    | mult_int
    ;

mult_int : mult_int mult_div_mod operand_int
    | operand_int
    ;

operand_int : DOLLAR LBRACE id RBRACE
	| DOLLAR LBRACE id LBRACKET operand_int RBRACKET RBRACE
	| DOLLAR int
    | plus_or_minus DOLLAR LBRACE id RBRACE
    | plus_or_minus DOLLAR RBRACE id LBRACKET operand_int RBRACKET RBRACE
    | plus_or_minus DOLLAR  int
    | int
    | plus_or_minus int
    | LPAREN sum_int RPAREN
    ;

plus_or_minus : PLUS
    | MINUS
    ;

mult_div_mod : MULT
     | DIV
     | MOD
     ;

declare_fct : id LPAREN RPAREN LBRACE declare_loc list_instructions RBRACE
    ;

declare_loc : declare_loc LOCAL id ASSIGN concatenation SEMICOLON
    |
    ;

function_call : id list_operand
    | id
    ;

id : WORD { log_trace("id: WORD (%s)", $1); CHECK_TYPE(checkWordIsId($1)) addIdOrder($1); }
    ;

int : WORD { log_trace("int: WORD"); CHECK_TYPE(checkWordIsInt($1)); addValueIntoListTmp($1); setTypeOrder(INTEGER);}
    ;

%%

int yyerror (char * s)
{
    fprintf(stderr, "%s at line %d, column\n", s, yylineno);
    return 0;
}