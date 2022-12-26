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
%token ASSIGN QUOTED_STRING APOSTROPHED_STRING
%token SEMICOLON EXCL DOLLAR PLUS MINUS MULT DIV MOD QMARK
%token NEQ BOR ARG_A ARG_O ARG_N ARG_Z ARG_EQ ARG_NE ARG_GT ARG_GE ARG_LT ARG_LE

%type <strval> NEQ BOR ARG_A ARG_O ARG_N ARG_Z ARG_EQ ARG_NE ARG_GT ARG_GE ARG_LT ARG_LE ASSIGN CASE ESAC
%type <strval> SEMICOLON EXCL DOLLAR PLUS MINUS MULT DIV MOD QMARK IF ELIF ELSE TEST THEN FI
%type <strval> FOR WHILE UNTIL DO DONE IN RETURN EXIT ECHO_CALL READ DECLARE LOCAL INT STRING WORD EXPR
%type <strval> LBRACKET RBRACKET LPAREN RPAREN LBRACE RBRACE QUOTE APOSTROPHE
%type <strval> QUOTED_STRING APOSTROPHED_STRING
%type <strval> id operand concatenation plus_or_minus int operand_int sum_int mult_int test_block test_expr test_expr2 test_expr3 test_instruction operator1 addTmpValuesListTmp
%start program

%%
program : {log_debug("program : list_instructions")} list_instructions {log_debug("program : list_instructions")}
    ;

list_instructions : list_instructions SEMICOLON instructions {log_debug("program : list_instructions SEMICOLON instructions")}
    | instructions {log_debug("program : list_instructions -> instructions")}
    ;

instructions : id ASSIGN addTmpValuesListTmp concatenation {log_debug("instructions: (%s, %s, %s)", $1,$2,$3); assign(); }
    | id LBRACKET operand_int RBRACKET ASSIGN concatenation {log_debug("tab: (%s, %s, %s)", $1,$3,$6); }
    | DECLARE id LBRACKET int RBRACKET
    | { log_debug("entering if block"); } IF test_block THEN list_instructions else_part FI { log_debug("leaveing if block"); }
    | FOR id DO list_instructions DONE
    | FOR id IN list_operand DO list_instructions DONE
    | WHILE test_block DO list_instructions DONE
    | UNTIL test_block DO list_instructions DONE
    | CASE operand IN list_case ESAC
    | ECHO_CALL list_operand {echo();}
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
    | { log_debug("else_part empty"); }
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

concatenation : concatenation operand { log_debug("concat : %s %s", $1, $2); }
    | operand { log_debug("concatenation"); }
    ;

test_block : {log_debug("entering test_block");} TEST test_expr { log_debug("TEST %s", $2); }
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

operand : DOLLAR LBRACE id RBRACE { log_debug("DOLLAR LBRACE %s RBRACE", $3); }
    | DOLLAR LBRACE id LBRACKET operand_int RBRACKET RBRACE
    | WORD { asm_syscall(READ_CHAR); log_debug("operand : WORD (%s)", $1); addValueIntoListTmp($1);}
    | DOLLAR int
    | DOLLAR MULT
    | DOLLAR QMARK
    | QUOTED_STRING
    | APOSTROPHED_STRING
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

sum_int : sum_int plus_or_minus mult_int {doOperation();}
    | mult_int {log_debug("leaving sum_int => mult_int"); }
    ;

mult_int : mult_int mult_div_mod operand_int {log_debug("mult_int : mult_int mult_div_mod operand_int"); doOperation(); }
    | operand_int {log_debug("mult_int : operand_int"); }
    ;

operand_int : DOLLAR LBRACE id RBRACE
    | DOLLAR LBRACE id LBRACKET operand_int RBRACKET RBRACE
    | DOLLAR int
    | plus_or_minus DOLLAR LBRACE id RBRACE
    | plus_or_minus DOLLAR RBRACE id LBRACKET operand_int RBRACKET RBRACE
    | plus_or_minus DOLLAR int
    | int
    | plus_or_minus int { log_debug("leaving %s %s", $1, $2); }
    | LPAREN sum_int RPAREN
    ;

plus_or_minus : PLUS {currentOperation = PLUS_OPE;}
    | MINUS {currentOperation = MINUS_OPE;}
    ;

mult_div_mod : MULT {currentOperation = MULT_OPE;}
     | DIV {currentOperation = DIV_OPE;}
     | MOD {currentOperation = MOD_OPE;}
     ;

declare_fct : id LPAREN RPAREN LBRACE declare_loc list_instructions RBRACE
    ;

declare_loc : declare_loc LOCAL id ASSIGN concatenation SEMICOLON
    |
    ;

function_call : id list_operand
    | id
    ;

id : WORD { log_debug("id: WORD (%s)", $1); CHECK_TYPE(checkWordIsId($1)) addIdOrder($1); }
    ;

int : WORD { log_debug("int: WORD"); CHECK_TYPE(checkWordIsInt($1)); addValueIntoListTmp($1); setTypeOrder(INTEGER); }
    ;

addTmpValuesListTmp : {$$ = ""; addTmpValuesListTmp();};

%%

int yyerror (char * s)
{
    int i; char c;
    fprintf(stderr, "syntax error: %s\n", s);
    fprintf(stderr, "    at line %d, character '%s'\n", yylineno, yytext);
}