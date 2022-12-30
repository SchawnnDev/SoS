#include <regex.h>
#include <limits.h>
#include "compilation.h"
#include "parser.h"
#include "lexer.h"
#include "asm.h"
#include "labels.h"
#include "header.asm.h"
#include "bufferWrite.asm.h"
#include "loadRegistersFromStack.asm.h"
#include "saveRegistersToStack.asm.h"
#include "bufferLen.asm.h"
#include "atoi.asm.h"

ListRangeVariable listRangeVariable;
ListIdentifierOrder listIdentifierOrder;
ListTmp listTmp;
ListInstruction listInstruction;
int currentOperation;
boolExpr_t currentBooleanExpression;

void setCurrentOperation(int operation)
{
    currentOperation = operation;
}

void setCurrentBooleanExpression(boolExpr_t expr)
{
    currentBooleanExpression = expr;
}

int errorType(const char *msg, ...)
{

    return (EXIT_FAILURE);
}

/*!
 * \fn void initStruct()
 * \brief Fonction qui initialise les structures
*/
void initStruct()
{
    log_trace("Started initStruct")
    listRangeVariable = initListRangeVariable();
    listIdentifierOrder = initListIdentifierOrder();
    listTmp = initListTmp();
    listInstruction = initListInstruction();
    currentOperation = UNSET;
}

/*!
 * \fn int compile(FILE *inputFile, FILE *outputFile)
 * \brief Fonction qui démarre la compilation
 *
 * \param inputFile : FILE*, le file descriptor du fichier à compiler
 * \param outputFile : FILE*, le file descriptor du fichier de sortie
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int compile(FILE *inputFile, FILE *outputFile)
{
    log_trace("Started compile (%d, %d)", inputFile, outputFile)
    initStruct();
    yyin = inputFile;
    // Generate all functions & headers
    asm_writeHeader();
    asm_code_printf("j _main")
    asm_code_printf("\n")
    asm_code_printf("\t# Functions library section\n")
    asm_code_printf("\n")
    asm_writeLoadRegistersFromStackFunction();
    asm_writeSaveRegistersToStackFunction();
    asm_writeBufferLenFunction();
    asm_writeBufferWriteFunction();
    asm_writeAtoiFunction();
    asm_code_printf("\n")
    asm_code_printf("\t# Start of main code section\n")
    asm_code_printf("\n")
    asm_code_printf("_main:")
    // Parse
    int result = yyparse();
    if (result != RETURN_SUCCESS) return result;
    return writeToFile(listInstruction,
                       outputFile == NULL ? stdout : outputFile);
}

/*!
 * \fn int assign()
 * \brief Fonction qui ajoute l'identifiant à la liste et transmet les données qui le compose
*/
int assign()
{
    log_trace("assign (void)")
    char* name = listIdentifierOrder->cursor->name;
    addIdentifier(listRangeVariable, listIdentifierOrder->cursor->name, TRUE);
    setValuesFromListTmp(listRangeVariable, listIdentifierOrder->cursor->name,
                         listTmp);

    if(getOffset(listRangeVariable, name, listTmp) == RETURN_FAILURE)
        return RETURN_FAILURE;

    char* stackOffset = listTmp->cursor->values[listTmp->cursor->numberValues-1];
    ///lw $a1, 0($t0)      # Load the address from the stack and store it in the given register
    asm_code_printf("\taddi $t0, $sp, %s\n", stackOffset)
    asm_loadLabelIntoRegister(ASM_VAR_OFFSET_NAME, "$t1");
    asm_code_printf("\tadd $t0, $t0, $t1\n")

    int values = listTmp->cursor->numberValues;

    for (int i = 0; i < values; ++i)
    {
        char *val = listTmp->cursor->values[i];
        // CHECKPOINTER(strcat(finalStr, val));
        if (listTmp->cursor->types[i] == TYPE_STACK)
        {
            int stack = atoi(val);

            // read sur le stack
            asm_readFromStack("$t0", stack);
            asm_useDisplayFromHeapFunction("$t0");
            continue;
        }

        asm_code_printf("\tla $a0, %s\n", val)

    }

    deleteListTmp(listTmp);
    deleteIdentifierOrder(listIdentifierOrder);
}

void assignArray()
{

}


/*!
 * \fn void addIdOrder(char * name)
 * \brief Fonction qui ajoute l'identifiant à la liste des ordes d'appartion des identificateurs
 *
 * \param name : char*, le nom de l'identificateur
*/
void addIdOrder(char *name)
{
    addIdentifierOrder(listIdentifierOrder, name);
}

/*!
 * \fn void setTypeOrder(int type)
 * \brief Fonction qui modfifie le type de l'identifiant contenu dans la liste des ordes d'appartion des identificateurs
 *
 * \param name : char*, le nom de l'identificateur
*/
void setTypeOrder(int type)
{
    log_trace("setTypeOrder (int %d)", type);
    setTypeIdentifierOrder(listIdentifierOrder, type);
}

/*!
 * \fn void addTmpValuesListTmp
 * \brief Fonction qui ajoute une structure de valeur temporaire
*/
void addTmpValuesListTmp()
{
    addListTmp(listTmp, initTmpValues(listTmp->cursor));
}

/*!
 * \fn void setTypeOrder(int type)
 * \brief Fonction qui remplie la liste temporaire
 *
 * \param value : char*, le contenue
*/
void addValueIntoListTmp(char *value)
{
    addIntoListTmp(listTmp, value);
}

/* ToDo : version 1 : aide au debug*/
void echo()
{
    printIdentifierFromListRange(listRangeVariable,
                                 listIdentifierOrder->cursor->name);
    deleteIdentifierOrder(listIdentifierOrder);
}

void doOperation()
{
    operationListTmp(listTmp, currentOperation);
    currentOperation = UNSET;
}

int checkRegex(const char *pattern, const char *string)
{
    log_trace("checkRegex(%s, %s)", pattern, string)
    //char *pattern = "^[a-zA-Z_][a-zA-Z0-9_]*";
    //char *string = "hello";
    regex_t regex;
    int ret;

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0)
    {
        log_error("Error compiling regular expression")
        return -1;
    }

    ret = regexec(&regex, string, 0, NULL, 0);
    if (ret == 0)
    {
        log_trace("String matches regular expression");
        regfree(&regex);
        return 1;
    }

    if (ret == REG_NOMATCH)
    {
        log_error("String does not match regular expression");
        regfree(&regex);
        return 0;
    }

    log_error("Error executing regular expression");
    return -1;
}

int checkWordIsId(const char *word)
{
    return checkRegex("^[a-zA-Z_][a-zA-Z0-9_]*", word);
}

int checkWordIsInt(const char *word)
{
    return checkRegex("^[+-]?[0-9]+", word);
}

int getValues()
{
    return getValuesFromIdentifier(listRangeVariable,
                                   listIdentifierOrder->cursor->name, listTmp);
}

int parseInt32(const char *word)
{
    char *endptr;
    long int parsed = strtol(word, &endptr, 10);

    if (endptr == word || *endptr != '\0')
    {
        log_error("Invalid number for parseInt32(%s)", word);
        return -1;
    }

    if (parsed < INT_MIN || parsed > INT_MAX)
    {
        // The number is not within the range of a 32-bit integer
        log_error("Number is not 32-bit for parseInt32(%s)", word);
        return -1;
    }

    return (int) parsed;
}

int doEcho()
{
    log_trace("doEcho")
    CHECKPOINTER(listTmp)
    CHECKPOINTER(listTmp->cursor)
    int values = listTmp->cursor->numberValues;

    for (int i = 0; i < values; ++i)
    {
        char *val = listTmp->cursor->values[i];
        // CHECKPOINTER(strcat(finalStr, val));
        if (listTmp->cursor->types[i] == TYPE_STACK)
        {
            int stack = atoi(val);

            // read sur le stack
            asm_readFromStack("$t0", stack);
            asm_useDisplayFromHeapFunction("$t0");
            continue;
        }

        asm_code_printf("\tla $a0, %s\n", val)
        asm_syscall(PRINT_STRING);

    }

    return RETURN_SUCCESS;
}

int doBoolExpression()
{
    log_trace("doBoolExpression (ListTmp %p, int %d)", listTmp,
              currentBooleanExpression)
    CHECKPOINTER(listTmp);
    CHECKPOINTER(listTmp->cursor);

    if (listTmp->cursor->numberValues < 2)
    {
        log_error("numberValues : %d", listTmp->cursor->numberValues)
        perror("doBoolExpression : you must have two values.");
        return RETURN_FAILURE;
    }

    int index;
    for (index = listTmp->cursor->numberValues - 2;
         index < listTmp->cursor->numberValues; index++){

        if (currentBooleanExpression == BOOL_EQ ||
            currentBooleanExpression == BOOL_NEQ
            || currentBooleanExpression == BOOL_GT ||
            currentBooleanExpression == BOOL_GE
            || currentBooleanExpression == BOOL_LT ||
            currentBooleanExpression == BOOL_LE)
        {

            if (listTmp->cursor->types[index] == TYPE_STACK)
            {
                asm_readFromStack("$t1", listTmp->cursor->values[index]);
                // ToDo Call Mips ATOI
            } else
            {
                asm_code_printf("li $t2, %s\n", listTmp->cursor->values[index])
            }
        }
    }

    /*
    int reg1 = atoi(listTmp->cursor->values[listTmp->cursor->numberValues - 2]);
    int reg2 = atoi(listTmp->cursor->values[listTmp->cursor->numberValues - 1]);
    deleteListTmp(listTmp);
    addListTmp(listTmp, initTmpValues(listTmp->cursor));
    */
    char* else_lab = createNewLabel();

    switch (currentBooleanExpression)
    {
        case BOOL_EQ:
            asm_code_printf("beq $t1, $t2, %s",else_lab)
            break;
        case BOOL_NEQ:
            asm_code_printf("bne $t1, $t2, %s", else_lab)
            break;
        case BOOL_GT:
            asm_code_printf("bgt $t1, $t2, %s", else_lab)
            break;
        case BOOL_GE:
            asm_code_printf("bge $t1, $t2, %s", else_lab)
            break;
        case BOOL_LT:
            asm_code_printf("blt $t1, $t2, %s", else_lab)
            break;
        case BOOL_LE:
            asm_code_printf("ble $t1, $t2, %s", else_lab)
            break;
        case L_AND:

            break;
        case L_OR:
            asm_code_printf("%s", "ligne OU")
            break;
    }


    return RETURN_SUCCESS;
}

int doDeclareStaticArray()
{
    log_trace("doDeclareStaticArray")
    CHECKPOINTER(listTmp)
    CHECKPOINTER(listTmp->cursor)
    CHECKPOINTER(listIdentifierOrder)

    if (listIdentifierOrder->cursor->index == 0)
    {
        perror("doDeclareStaticArray : you must have one identifier.");
        return RETURN_FAILURE;
    }

    if (listTmp->cursor->numberValues == 0)
    {
        log_error("numberValues : %d", listTmp->cursor->numberValues)
        perror("doDeclareStaticArray : you must have one value.");
        return RETURN_FAILURE;
    }

    const int size = atoi(listTmp->cursor->values[0]);

    if (size <= 0)
    {
        log_error(
                "doDeclareStaticArray: size should be greater than 0 (actual: %d)",
                size)
        return RETURN_FAILURE;
    }

    char *name = listIdentifierOrder->cursor->name;
    char *id = idToLabel(name);

    if (asm_writeStaticArray(id, size) == RETURN_FAILURE)
        return RETURN_FAILURE;

    free(id);
    setArraySize(listRangeVariable, name, size);
    deleteListTmp(listTmp);
    addListTmp(listTmp, initTmpValues(listTmp->cursor));
    addIdentifier(listRangeVariable, name, FALSE);
    deleteIdentifierOrder(listIdentifierOrder);

    return RETURN_SUCCESS;
}

int writeWord(const char *str, int addQuotes)
{
    const char *label = createNewLabel();
    int result = asm_writeAsciiz(label, str, addQuotes);

    if (result == RETURN_FAILURE)
        return RETURN_FAILURE;

    // Don't forget to add label to tmp list
    addValueIntoListTmp(label);
    return result;
}

int writeApostrophedString(const char *str)
{
    char *copy;
    unsigned int len = strlen(str);
    CHECKPOINTER(copy = malloc(len + 1))
    // Replace ' char by " char
    CHECKPOINTER(strcpy(copy, str))
    copy[0] = '"';
    copy[len - 1] = '"';
    const char *label = createNewLabel();
    int result = asm_writeAsciiz(label, copy, TRUE);

    // Don't forget to add label to tmp list
    if (result == RETURN_SUCCESS)
        addValueIntoListTmp(label);

    free(copy);
    return result;
}

int doArrayRead()
{
    log_trace("doArrayRead")
    CHECK_IDENTIFIER_NOT_ZERO("doArrayRead")
    CHECK_VALUE_NOT_ZERO("doArrayRead")

    char *id = listIdentifierOrder->cursor->name;
    const int index = listIdentifierOrder->cursor->index;


    //const int size = atoi(listTmp->cursor->values[0]);

    const int type = listTmp->cursor->types[listTmp->cursor->numberValues - 1];

    if (type == TYPE_STACK)
    {
        // read from stack
    } else
    {
        // load li
    }


    // read tableau[1];
    // read tableau[$1];
    // read tableau[${a}];

/*    if (size <= 0)
    {
        log_error(
                "doDeclareStaticArray: size should be greater than 0 (actual: %d)",
                size)
        return RETURN_FAILURE;
    }*/


    return RETURN_SUCCESS;
}


int doGetVariableAddress()
{
    log_trace("doGetVariableAddress")
    CHECK_IDENTIFIER_NOT_ZERO("doGetVariableAddress")

    char *name = listIdentifierOrder->cursor->name;

    if (getOffset(listRangeVariable, name, listTmp) == RETURN_FAILURE)
        return RETURN_FAILURE;

    return RETURN_SUCCESS;
}