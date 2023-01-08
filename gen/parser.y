%define parse.trace

%{
#include "lexer.h"
#include "compilation.h"
#include "memory.h"
#include "boolExpr.h"
#include "marker.h"
%}

%union { char *strval; int intval; MemorySlot memval; MemorySlotList memlistval; boolExpr_t boolexprval; Marker markerval; }

%right ASSIGN
%left ARG_N ARG_Z ARG_EQ ARG_NE ARG_GT ARG_GE ARG_LT ARG_LE ARG_A ARG_O

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
%type <strval> marker marker_test marker_else marker_end_instruction marker_loop marker_done marker_if marker_until marker_for marker_do
%type <memval> operand operand_int int sum_int mult_int final_concatenation test_block test_expr test_expr2 test_expr3 test_instruction function_call
%type <memlistval> list_operand concatenation
%type <intval> plus_or_minus mult_div_mod table_int
%type <markerval> marker_fct_id marker_for_list marker_for_header
%type <boolexprval> operator1 operator2
%start program

%%
program : list_instructions {log_debug("program : list_instructions")}
    ;

list_instructions : list_instructions SEMICOLON instructions {log_debug("program : list_instructions SEMICOLON instructions")}
    | instructions {log_debug("program : list_instructions -> instructions")}
    ;

instructions : id ASSIGN final_concatenation {log_debug("instructions: (%s, %s, %s)", $1,$2,$3); assign($1, $3, 0); if(HAS_ERROR()) YYABORT ; }
    | id LBRACKET operand_int RBRACKET ASSIGN final_concatenation {log_debug("tab: (%s, %s, %s)", $1,$3,$6); assignArrayValue($1, $3, $6); if(HAS_ERROR()) YYABORT ; }
    | DECLARE id LBRACKET table_int RBRACKET { doDeclareStaticArray($2, $4); if(HAS_ERROR()) YYABORT ; }
    | IF marker_if test_block marker_test THEN list_instructions marker_end_instruction else_part FI { doMarkerFi(); if(HAS_ERROR()) YYABORT ; deleteBlock(); if(HAS_ERROR()) YYABORT ; }
    | FOR marker_for id DO list_instructions marker_done DONE
    | marker_for_header marker_for_list marker_do DO list_instructions marker_done DONE {doForIdAssign($1); if(HAS_ERROR()) YYABORT ; doMarkerEndLoop(); if(HAS_ERROR()) YYABORT ; doDeleteLocalOffset($2); if(HAS_ERROR()) YYABORT ; deleteBlock(); if(HAS_ERROR()) YYABORT ;}
    | WHILE marker_loop test_block marker_test DO list_instructions marker_done DONE { doMarkerEndLoop(); if(HAS_ERROR()) YYABORT ; deleteBlock(); if(HAS_ERROR()) YYABORT ; }
    | UNTIL marker_loop test_block marker_until marker_test DO list_instructions marker_done DONE { doMarkerEndLoop(); if(HAS_ERROR()) YYABORT ; deleteBlock(); if(HAS_ERROR()) YYABORT ; }
    | CASE operand IN list_case ESAC
    | ECHO_CALL list_operand { doEcho($2); if(HAS_ERROR()) YYABORT ; }
    | READ id { doStringRead($2); if(HAS_ERROR()) YYABORT ; }
    | READ id LBRACKET operand_int RBRACKET { doArrayRead($2,$4); if(HAS_ERROR()) YYABORT ; }
    | declare_fct
    | function_call
    | RETURN { doReturn(NULL); if(HAS_ERROR()) YYABORT ; }
    | RETURN operand_int { doReturn($2); if(HAS_ERROR()) YYABORT ; }
    | EXIT { doExit(NULL); if(HAS_ERROR()) YYABORT ; }
    | EXIT operand_int { doExit($2); if(HAS_ERROR()) YYABORT ; }
    ;

else_part : marker_else ELIF test_block marker_test THEN list_instructions marker_end_instruction else_part
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

list_operand : list_operand operand { $$ = appendMemorySlot($1, $2); if(HAS_ERROR()) YYABORT ; }
    | operand { $$ = newMemorySlotList($1); if(HAS_ERROR()) YYABORT ; }
    | DOLLAR LBRACE id LBRACKET MULT RBRACKET RBRACE
    ;

final_concatenation : concatenation { $$ = doConcatenation($1); if(HAS_ERROR()) YYABORT ; }
    ;

concatenation : concatenation operand { log_debug("concat : %s %s", $1, $2); $$ = appendMemorySlot($1, $2); if(HAS_ERROR()) YYABORT ; }
    | operand { $$ = newMemorySlotList($1); if(HAS_ERROR()) YYABORT ; }
    ;

test_block : TEST test_expr { $$ = $2; }
    ;

test_expr : test_expr ARG_O marker test_expr2 { $$ = doBoolExpression($1, L_OR, $4); if(HAS_ERROR()) YYABORT ; }
    | test_expr2
    ;

test_expr2 : test_expr2 ARG_A marker test_expr3 { $$ = doBoolExpression($1,L_AND,$4); if(HAS_ERROR()) YYABORT ; }
    | test_expr3
    ;

test_expr3 : LPAREN test_expr RPAREN { $$ = $2; }
    | EXCL LPAREN test_expr RPAREN { $$ = $3; doNegBoolExpression(); if(HAS_ERROR()) YYABORT ; }
    | test_instruction { log_debug("test_instruction"); if(HAS_ERROR()) YYABORT ; }
    | EXCL test_instruction { $$ = $2; doNegBoolExpression(); if(HAS_ERROR()) YYABORT ; }
    ;

test_instruction : final_concatenation ASSIGN final_concatenation { $$ = doBoolExpression($1, STR_EQ, $3); if(HAS_ERROR()) YYABORT ; }
    | final_concatenation NEQ final_concatenation { $$ = doBoolExpression($1, STR_NEQ, $3); if(HAS_ERROR()) YYABORT ; }
    | operator1 final_concatenation {$$ = doEmptyBoolExpression($1,$2); if(HAS_ERROR()) YYABORT ; }
    | operand operator2 operand { log_debug("operand %d operand", $2); $$ = doBoolExpression($1, $2, $3); if(HAS_ERROR()) YYABORT ; }
    ;

operand : DOLLAR LBRACE id RBRACE { log_debug("DOLLAR LBRACE %s RBRACE", $3); $$ = doGetVariableAddress($3, 0, 0); if(HAS_ERROR()) YYABORT ;}
    | DOLLAR LBRACE id LBRACKET operand_int RBRACKET RBRACE { $$ = doGetArrayAddress($3, $5, 0, 0); if(HAS_ERROR()) YYABORT ; }
    | WORD { log_debug("operand : WORD (%s)", $1); $$ = addWordToMemory($1); if(HAS_ERROR()) YYABORT ;}
    | DOLLAR int { $$ = doGetArgument($2, 0, 0); if(HAS_ERROR()) YYABORT ; /* TODO */}
    | DOLLAR MULT
    | DOLLAR QMARK
    | QUOTED_STRING { $$ = addStringToMemory($1); if(HAS_ERROR()) YYABORT ; }
    | APOSTROPHED_STRING { $$ = addStringToMemory($1); if(HAS_ERROR()) YYABORT ; }
    | DOLLAR LPAREN EXPR sum_int RPAREN { $$ = convertIntToString($4); if(HAS_ERROR()) YYABORT ; }
    | DOLLAR LPAREN function_call RPAREN { $$ = $3; }
    ;

operator1 : ARG_N { $$ = NOT_EMPTY; }
    | ARG_Z { $$ = EMPTY; }
    ;

operator2 : ARG_EQ { $$ = BOOL_EQ; }
    | ARG_NE { $$ = BOOL_NEQ; }
    | ARG_GT { $$ = BOOL_GT; }
    | ARG_GE { $$ = BOOL_GE; }
    | ARG_LT { $$ = BOOL_LT; }
    | ARG_LE { $$ = BOOL_LE; }
    ;

sum_int : sum_int plus_or_minus mult_int { log_debug("sum_int: CALCUL: %d | %d | %d", $1, $2, $3); $$ = doOperation($1, $2, $3); if(HAS_ERROR()) YYABORT ; }
    | mult_int {log_debug("leaving sum_int => mult_int"); }
    ;

mult_int : mult_int mult_div_mod operand_int { log_debug("mult_int: CALCUL: %s | %d | %s", $1, $2, $3); $$ = doOperation($1, $2, $3); if(HAS_ERROR()) YYABORT ; }
    | operand_int {log_debug("mult_int : operand_int"); }
    ;

operand_int : DOLLAR LBRACE id RBRACE { $$ = doGetVariableAddress($3, 0, 1); if(HAS_ERROR()) YYABORT ; }
    | DOLLAR LBRACE id LBRACKET operand_int RBRACKET RBRACE { $$ = doGetArrayAddress($3, $5, 0, 1); if(HAS_ERROR()) YYABORT ; }
    | DOLLAR int { $$ = doGetArgument($2, 0, 1); if(HAS_ERROR()) YYABORT ; }
    | plus_or_minus DOLLAR LBRACE id RBRACE { log_debug("doGetVariableAddress(%d, %s)", $1, $4); $$ = doGetVariableAddress($4, $1 == MINUS_OPE, 1); if(HAS_ERROR()) YYABORT ; }
    | plus_or_minus DOLLAR RBRACE id LBRACKET operand_int RBRACKET RBRACE { $$ = doGetArrayAddress($4, $6, $1 == MINUS_OPE, 1); if(HAS_ERROR()) YYABORT ; }
    | plus_or_minus DOLLAR int { $$ = doGetArgument($2, $1 == MINUS_OPE, 1); if(HAS_ERROR()) YYABORT ; }
    | int
    | plus_or_minus int { $$ = doUnaryCheck($2, $1 == MINUS_OPE); if(HAS_ERROR()) YYABORT ; }
    | LPAREN sum_int RPAREN { $$ = $2; }
    ;

plus_or_minus : PLUS { $$ = PLUS_OPE; }
    | MINUS { $$ = MINUS_OPE; }
    ;

mult_div_mod : MULT { $$ = MULT_OPE; }
     | DIV { $$ = DIV_OPE; }
     | MOD { $$ = MOD_OPE;}
     ;

declare_fct : marker_fct_id declare_loc list_instructions RBRACE { doDeclareFunction($1); if(HAS_ERROR()) YYABORT ; }
    ;

marker_fct_id: id LPAREN RPAREN LBRACE { $$ = doFunctionStartMarker($1); if(HAS_ERROR()) YYABORT ; }
    ;

declare_loc : declare_loc LOCAL id ASSIGN final_concatenation SEMICOLON
    |
    ;

function_call : id list_operand { $$ = doFunctionCall($1, $2); if(HAS_ERROR()) YYABORT ; }
    | id { $$ = doFunctionCall($1, NULL); if(HAS_ERROR()) YYABORT ; }
    ;

id : WORD { log_debug("id: WORD (%s)", $1); CHECK_TYPE(checkWordIsId($1)); char* destination;
            CHECKPOINTER(destination = malloc(strlen($1) + 1)); if(HAS_ERROR()) YYABORT ;
            CHECKPOINTER(strcpy(destination, $1)); if(HAS_ERROR()) YYABORT ; $$ = destination; }
    ;

int : WORD { log_debug("int: WORD"); CHECK_TYPE(checkWordIsInt($1)); $$ = doWriteInt($1); if(HAS_ERROR()) YYABORT ; }
    ;

table_int : WORD { $$ = doParseTableInt($1); if(HAS_ERROR()) YYABORT ; }

marker : {$$ = ""; setMarker(); if(HAS_ERROR()) YYABORT ; }

marker_test : {$$ = ""; doMarkerTest(); if(HAS_ERROR()) YYABORT ; }

marker_else : {$$ = ""; doMarkerElse(); if(HAS_ERROR()) YYABORT ; }

marker_end_instruction : {$$ = ""; doMarkerEndInstruction(); if(HAS_ERROR()) YYABORT ; }

marker_loop : {$$ = ""; addBlock(BLOCK_WHILE_UNTIL); if(HAS_ERROR()) YYABORT ; doMarkerLoop(BLOCK_WHILE_UNTIL); if(HAS_ERROR()) YYABORT ; }

marker_for : {$$ = ""; addBlock(BLOCK_FOR); if(HAS_ERROR()) YYABORT ; }

marker_do : {$$ = ""; doMarkerLoop(BLOCK_FOR); if(HAS_ERROR()) YYABORT ; doMarkerTestFor(); if(HAS_ERROR()) YYABORT ; }

marker_done : {$$ = ""; doMarkerDone(); if(HAS_ERROR()) YYABORT ; }

marker_if : { $$ = ""; addBlock(BLOCK_IF); if(HAS_ERROR()) YYABORT ; }

marker_until : { $$ = ""; doNegBoolExpression(); if(HAS_ERROR()) YYABORT ; }

marker_for_list : list_operand { $$ = doMarkerForList($1); if(HAS_ERROR()) YYABORT ; }

marker_for_header : FOR marker_for id IN { $$ = getOrCreateForIdMarker($3); if(HAS_ERROR()) YYABORT ; }
;

%%

int yyerror (char * s)
{
    int i; char c;
    fprintf(stderr, "syntax error: %s\n", s);
    fprintf(stderr, "    at line %d, character '%s'\n", yylineno, yytext);
}
