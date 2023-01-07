%define parse.trace

%{
#include "lexer.h"
#include "compilation.h"
#include "memory.h"
#include "boolExpr.h"
%}

%union { char *strval; int intval; MemorySlot memval; MemorySlotList memlistval; boolExpr_t boolexprval; }
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

%type <strval> ASSIGN CASE ESAC
%type <strval> EXCL DOLLAR PLUS MINUS MULT DIV MOD QMARK IF ELIF ELSE TEST THEN FI
%type <strval> FOR WHILE UNTIL DO DONE IN RETURN EXIT ECHO_CALL READ DECLARE LOCAL INT STRING WORD EXPR
%type <strval> LBRACKET RBRACKET LPAREN RPAREN LBRACE RBRACE QUOTE APOSTROPHE
%type <strval> QUOTED_STRING APOSTROPHED_STRING
%type <strval> id
%type <strval> marker marker_then marker_else marker_end_instruction
%type <memval> operand operand_int int sum_int mult_int final_concatenation test_block test_expr test_expr2 test_expr3 test_instruction
%type <memlistval> list_operand concatenation
%type <intval> plus_or_minus mult_div_mod table_int
%type <boolexprval> operator1 operator2
%start program

%%
program : list_instructions {log_debug("program : list_instructions")}
    ;

list_instructions : list_instructions SEMICOLON instructions {log_debug("program : list_instructions SEMICOLON instructions")}
    | instructions {log_debug("program : list_instructions -> instructions")}
    ;

instructions : id ASSIGN final_concatenation {log_debug("instructions: (%s, %s, %s)", $1,$2,$3); assign($1, $3, 0); }
    | id LBRACKET operand_int RBRACKET ASSIGN final_concatenation {log_debug("tab: (%s, %s, %s)", $1,$3,$6); assignArrayValue($1, $3, $6); }
    | DECLARE id LBRACKET table_int RBRACKET { doDeclareStaticArray($2, $4); }
    | IF test_block marker_then THEN list_instructions marker_end_instruction else_part FI { doMarkerFi();}
    | FOR id DO list_instructions DONE
    | FOR id IN list_operand DO list_instructions DONE
    | WHILE test_block DO list_instructions DONE
    | UNTIL test_block DO list_instructions DONE
    | CASE operand IN list_case ESAC
    | ECHO_CALL list_operand { doEcho($2); }
    | READ id { doStringRead($2); }
    | READ id LBRACKET operand_int RBRACKET { doArrayRead($2,$4); }
    | declare_fct
    | function_call
    | RETURN
    | RETURN operand_int
    | EXIT { doExit(NULL); }
    | EXIT operand_int { doExit($2); }
    ;

else_part : marker_else ELIF test_block marker_then THEN list_instructions marker_end_instruction else_part
    | marker_else ELSE list_instructions marker_end_instruction
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

list_operand : list_operand operand { $$ = appendMemorySlot($1, $2); }
    | operand { $$ = newMemorySlotList($1); }
    | DOLLAR LBRACE id LBRACKET MULT RBRACKET RBRACE
    ;

final_concatenation : concatenation { $$ = doConcatenation($1); }
    ;

concatenation : concatenation operand { log_debug("concat : %s %s", $1, $2); $$ = appendMemorySlot($1, $2); }
    | operand { $$ = newMemorySlotList($1); }
    ;

test_block : TEST test_expr { $$ = $2; }
    ;

test_expr : test_expr ARG_O marker test_expr2 { $$ = doBoolExpression($1, L_OR, $4);}
    | test_expr2
    ;

test_expr2 : test_expr2 ARG_A marker test_expr3 { $$ = doBoolExpression($1,L_AND,$4);}
    | test_expr3
    ;

test_expr3 : LPAREN test_expr RPAREN { $$ = $2; }
    | EXCL LPAREN test_expr RPAREN { $$ = $3; }
    | test_instruction { log_debug("test_instruction"); }
    | EXCL test_instruction { $$ = $2; }
    ;

test_instruction : final_concatenation ASSIGN final_concatenation { $$ = doBoolExpression($1, STR_EQ, $3); }
    | final_concatenation NEQ final_concatenation { $$ = doBoolExpression($1, STR_NEQ, $3); }
    | operator1 final_concatenation {$$ = doEmptyBoolExpression($1,$2);}
    | operand operator2 operand { log_debug("operand %d operand", $2); $$ = doBoolExpression($1, $2, $3); }
    ;

operand : DOLLAR LBRACE id RBRACE { log_debug("DOLLAR LBRACE %s RBRACE", $3); $$ = doGetVariableAddress($3, 0, 0); }
    | DOLLAR LBRACE id LBRACKET operand_int RBRACKET RBRACE { $$ = doGetArrayAddress($3, $5, 0, 0); }
    | WORD { log_debug("operand : WORD (%s)", $1); $$ = addWordToMemory($1); }
    | DOLLAR int { $$ = $2; /* TODO */}
    | DOLLAR MULT
    | DOLLAR QMARK
    | QUOTED_STRING { $$ = addStringToMemory($1); }
    | APOSTROPHED_STRING { $$ = addStringToMemory($1); }
    | DOLLAR LPAREN EXPR sum_int RPAREN { $$ = convertIntToString($4); }
    | DOLLAR LPAREN function_call RPAREN
    ;

operator1 : ARG_N { $$ = NOT_EMPTY; }
    | ARG_Z { $$ = EMPTY; }
    ;

operator2 : ARG_EQ { $$ = BOOL_EQ; if(HAS_ERROR()) YYABORT ;}
    | ARG_NE { $$ = BOOL_NEQ; }
    | ARG_GT { $$ = BOOL_GT; }
    | ARG_GE { $$ = BOOL_GE; }
    | ARG_LT { $$ = BOOL_LT; }
    | ARG_LE { $$ = BOOL_LE; }
    ;

sum_int : sum_int plus_or_minus mult_int { log_debug("sum_int: CALCUL: %d | %d | %d", $1, $2, $3); $$ = doOperation($1, $2, $3); }
    | mult_int {log_debug("leaving sum_int => mult_int"); }
    ;

mult_int : mult_int mult_div_mod operand_int { log_debug("mult_int: CALCUL: %s | %d | %s", $1, $2, $3); $$ = doOperation($1, $2, $3); }
    | operand_int {log_debug("mult_int : operand_int"); }
    ;

operand_int : DOLLAR LBRACE id RBRACE { $$ = doGetVariableAddress($3, 0, 1); }
    | DOLLAR LBRACE id LBRACKET operand_int RBRACKET RBRACE { $$ = doGetArrayAddress($3, $5, 0, 1); }
    | DOLLAR int
    | plus_or_minus DOLLAR LBRACE id RBRACE { log_debug("doGetVariableAddress(%d, %s)", $1, $4); $$ = doGetVariableAddress($4, $1 == MINUS_OPE, 1); }
    | plus_or_minus DOLLAR RBRACE id LBRACKET operand_int RBRACKET RBRACE { $$ = doGetArrayAddress($4, $6, $1 == MINUS_OPE, 1); }
    | plus_or_minus DOLLAR int
    | int
    | plus_or_minus int { $$ = doUnaryCheck($2, $1 == MINUS_OPE); }
    | LPAREN sum_int RPAREN { $$ = $2; }
    ;

plus_or_minus : PLUS { $$ = PLUS_OPE; }
    | MINUS { $$ = MINUS_OPE; }
    ;

mult_div_mod : MULT { $$ = MULT_OPE; }
     | DIV { $$ = DIV_OPE; }
     | MOD { $$ = MOD_OPE;}
     ;

declare_fct : id LPAREN RPAREN LBRACE declare_loc list_instructions RBRACE
    ;

declare_loc : declare_loc LOCAL id ASSIGN final_concatenation SEMICOLON
    |
    ;

function_call : id list_operand
    | id
    ;

id : WORD { log_debug("id: WORD (%s)", $1); CHECK_TYPE(checkWordIsId($1)); char* destination;
            CHECKPOINTER(destination = malloc(strlen($1) + 1));
            CHECKPOINTER(strcpy(destination, $1)); $$ = destination; }
    ;

int : WORD { log_debug("int: WORD"); CHECK_TYPE(checkWordIsInt($1)); $$ = doWriteInt($1); }
    ;

table_int : WORD { $$ = doParseTableInt($1); }

marker : {$$ = ""; setMarker();}

marker_then : {$$ = ""; doMarkerThen();}

marker_else : {$$ = ""; doMarkerElse();}

marker_end_instruction : {$$ = ""; doMarkerEndInstruction();}

%%

int yyerror (char * s)
{
    int i; char c;
    fprintf(stderr, "syntax error: %s\n", s);
    fprintf(stderr, "    at line %d, character '%s'\n", yylineno, yytext);
}