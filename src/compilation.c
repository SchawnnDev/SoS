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
#include "strcmp.asm.h"
#include "displayString.asm.h"
#include "memory.h"

ListRangeVariable listRangeVariable;
ListIdentifierOrder listIdentifierOrder;
ListTmp listTmp;
ListInstruction listInstruction;
int currentOperation;
boolExpr_t currentBooleanExpression;
int marker;

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
    asm_code_printf("j _main\n")
    asm_code_printf("\n")
    asm_code_printf("# Functions library section\n")
    asm_code_printf("\n")
    asm_writeLoadRegistersFromStackFunction();
    asm_writeSaveRegistersToStackFunction();
    asm_writeBufferLenFunction();
    asm_writeBufferWriteFunction();
    asm_writeAtoiFunction();
    asm_writeStrcmpFunction();
    asm_writeDisplayStringFunction();
    asm_code_printf("\n")
    asm_code_printf("# Start of main code section\n")
    asm_code_printf("\n")
    asm_code_printf("_main:\n")
    // Parse
    int result = yyparse();
    if (result != RETURN_SUCCESS) return result;
    destroyMemorySlot();
    return writeToFile(listInstruction,
                       outputFile == NULL ? stdout : outputFile);
}

int doConcatenation(const char* into, int skipOffset)
{
    log_trace("doConcatenation")
    CHECKPOINTER(listTmp)
    CHECKPOINTER(listTmp->cursor)

    // Do concatenation
    int values = listTmp->cursor->numberValues;

    asm_code_printf("\taddi $t0, $zero, 0 # Size counter\n") // $t0 will be the total size of concat
    const int max = skipOffset ? values - 1 : values;
    // This loop calculates the size of the concatenation
    for (int i = 0; i < max; ++i) // values - 1 : last element is the stack offset for the assigned value
    {
        char *val = listTmp->cursor->values[i];
        //
        if (listTmp->cursor->types[i] == TYPE_STACK)
        {
            // read from stack
            asm_code_printf("\t# Reading value from stack\n")
            asm_readFromStack("$t1", atoi(val));
            asm_useBufferLenFunction("$t1", "$t2");
            asm_code_printf("\tadd $t0, $t0, $t2 # stack value\n")
            asm_code_printf("\t# End of reading\n")
        } else {
            // Else is a label, do calculations:
            int len = countWithoutBackslash(val);
            if(len == 0) continue;
            asm_code_printf("\taddi $t0, $t0, %d # static value\n", len)
        }

    }

    // We now have the size in $t0
    asm_code_printf("\taddi $t0, $t0, 1\n") // Add 1 for the '\0' char
    // multiply by 4 to have words
    asm_code_printf("\tmul $t3, $t0, 4\n")
    // do a sbrk to get memory
    asm_code_printf("\tmove $a0, $t3\n")
    asm_syscall(SBRK);
    // start address of the heap memory is in $v0
    asm_code_printf("\tmove %s, $v0\n", into) // move start address to a0
    asm_code_printf("\tmove $t1, $v0\n") // moving it to $t1

    // let's write to the heap !
    for (int i = 0; i < max; ++i)
    {
        char *val = listTmp->cursor->values[i];
        //
        if (listTmp->cursor->types[i] == TYPE_STACK) {
            asm_readFromStack("$t0", val);
            asm_useBufferWriteFunction("$t0", "$t1", "$t1");
            //asm_code_printf("\taddi $t1, $t1, 4\n") // Move forward to write the next char
            continue;
        }

        unsigned int len = strlen(val);
        // Save chars to heap
        for (int j = 0; j < len; ++j) {
            asm_code_printf("\tli $t3, '%c", val[j])

            if(val[j] == '\\' && j + 1 < len)
            {
                j++;
                asm_code_printf("%c", val[j])
            }

            asm_code_printf("'\n")

            asm_code_printf("\tsw $t3, 0($t1)\n")
            asm_code_printf("\taddi $t1, $t1, 4\n") // Move forward to write the next char
        }

    }

    // At the end write $zero
    asm_code_printf("\tsw $zero, 0($t1)\n")

    return RETURN_SUCCESS;
}

/*!
 * \fn int assign()
 * \brief Fonction qui ajoute l'identifiant à la liste et transmet les données qui le compose
*/
MemorySlot assign()
{
    log_trace("assign (void)")
    char* name = listIdentifierOrder->cursor->name;
    addIdentifier(listRangeVariable, listIdentifierOrder->cursor->name, TRUE);
    setValuesFromListTmp(listRangeVariable, listIdentifierOrder->cursor->name,
                         listTmp);

    // Get offset of stack for the assigned value
    //if (getOffset(listRangeVariable, name, listTmp) == RETURN_FAILURE)
      //  return RETURN_FAILURE;

    asm_code_printf("\n\t# Assignation of var %s\n", name)

    if(doConcatenation("$a2", TRUE) == RETURN_FAILURE)
        return NULL;

    // save start address to the assigned value stack
    asm_allocateMemoryOnStack(1); // TODO: check if memory address is right (_offset...)
    asm_code_printf("\tsw $a2, 0($sp)\n")

    asm_code_printf("\n\t# End of assignation of var %s\n", name)

    deleteListTmp(listTmp);
    deleteIdentifierOrder(listIdentifierOrder);
    return RETURN_SUCCESS;
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

MemorySlot doOperation(MemorySlot left, int operation, MemorySlot right)
{
    asm_code_printf("\n\t#Start of operation code\n\n")

    asm_readFromStack("$t0", getMipsOffset(left));
    asm_readFromStack("$t1", getMipsOffset(right));

    switch (operation) {
        case PLUS_OPE:
            asm_code_printf("\tadd $t0, $t0, $t1\n")
            break;
        case MINUS_OPE:
            asm_code_printf("\tsub $t0, $t0, $t1\n")
            break;
        case MULT_OPE:
            asm_code_printf("\tmul $t0, $t0, $t1\n")
            break;
        case DIV_OPE:
            asm_code_printf("\tmul $t0, $t1\n")
            asm_code_printf("\tmflo $t0\n")
            break;
        case MOD_OPE:
            asm_code_printf("\tmul $t0, $t1\n")
            asm_code_printf("\tmfhi $t0\n")
            break;
        default:
            // ERROR: unknown operation
            break;
    }

    freeMemory(right);
    asm_getStackAddress("$t1", getMipsOffset(left));
    asm_code_printf("\tsw $t0, 0($t1)\n")

    asm_code_printf("\n\t#End of operation code\n\n")

    return left;
}

int doOperationAddInt()
{
    return RETURN_SUCCESS;
}

int getValues()
{
    return getValuesFromIdentifier(listRangeVariable,
                                   listIdentifierOrder->cursor->name, listTmp);
}

int doEcho()
{
    log_trace("doEcho")
    asm_code_printf("\n\t# Do echo section\n\n")

    log_trace("doConcatenation")
    CHECKPOINTER(listTmp)
    CHECKPOINTER(listTmp->cursor)

    // Do concatenation
    int values = listTmp->cursor->numberValues;

    // This loop calculates the size of the concatenation
    for (int i = 0; i < values; ++i) // values - 1 : last element is the stack offset for the assigned value
    {
        char *val = listTmp->cursor->values[i];
        //
        if (listTmp->cursor->types[i] == TYPE_STACK)
        {
            // read from stack
            asm_readFromStack("$a0", val);
            asm_jal(ASM_DISPLAY_STRING_FUNCTION_NAME);
        } else {
            // Else is a label, do calculations:
            if(strlen(val) == 0) continue;
            const char* lbl = createNewLabel();
            asm_data_printf("\t%s: .asciiz \"%s\"\n", lbl, val)
            asm_code_printf("\tla $a0, %s\n", lbl)
            asm_syscall(PRINT_STRING);
        }

    }

    asm_code_printf("\n\t# End do echo section\n\n")

    return RETURN_SUCCESS;
}

int setMarker(){
    marker = listInstruction->cursorCode->numberCode;
    asm_code_printf("\t%s : \n",createNewLabel())

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

    asm_code_printf("\n\t# Start of Test block of ope %d\n", currentBooleanExpression)

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
                asm_code_printf("\tli $t2, %s\n", listTmp->cursor->values[index])
            }
        }
    }

    /*
    int reg1 = atoi(listTmp->cursor->values[listTmp->cursor->numberValues - 2]);
    int reg2 = atoi(listTmp->cursor->values[listTmp->cursor->numberValues - 1]);
    deleteListTmp(listTmp);
    addListTmp(listTmp, initTmpValues(listTmp->cursor));
    */
    char* else_lab;
    switch (currentBooleanExpression)
    {
        case BOOL_EQ:
            addIntoTrueList(listInstruction,"\tbeq $t1, $t2,");
            addIntoFalseList(listInstruction,"\n\tj");
            break;
        case BOOL_NEQ:
            addIntoTrueList(listInstruction,"\tbne $t1, $t2,");
            addIntoFalseList(listInstruction,"\n\tj");
            break;
        case BOOL_GT:
            addIntoTrueList(listInstruction,"\tbgt $t1, $t2,");
            addIntoFalseList(listInstruction,"\n\tj");
            break;
        case BOOL_GE:
            addIntoTrueList(listInstruction,"\tbge $t1, $t2,");
            addIntoFalseList(listInstruction,"\n\tj");
            break;
        case BOOL_LT:
            addIntoTrueList(listInstruction,"\tblt $t1, $t2,");
            addIntoFalseList(listInstruction,"\n\tj");
            break;
        case BOOL_LE:
            addIntoTrueList(listInstruction,"\tble $t1, $t2,");
            addIntoFalseList(listInstruction,"\n\tj");
            break;
        case L_AND:
            asm_code_printf("%s", "ligne ET\n")
            //else_lab = createNewLabel();

            break;
        case L_OR:
            asm_code_printf("%s", "ligne OU\n")
            else_lab = createNewLabel();
            completeTrueList(listInstruction,else_lab);
            completeTrueList(listInstruction,else_lab);
            //completeFalseList(listInstruction, );
            completeFalseList(listInstruction, listInstruction->cursorCode->lineCode[marker]);
            break;
    }

    asm_code_printf("\n\t# End of Test block of ope %d\n", currentBooleanExpression)
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

MemorySlot addStringToMemory(const char *str) {
    char *copy;
    unsigned int len = strlen(str) - 1;
    CHECKPOINTER(copy = malloc(len))
    CHECKPOINTER(strncpy(copy, str + 1, len - 1));
    copy[len - 1] = '\0'; // add nul char
    // Don't forget to add label to tmp list
    // addValueIntoListTmp(copy);
    MemorySlot slot = reserveMemorySlot();
    const char* label = createNewLabel();
    asm_data_printf("\t%s: .asciiz \"%s\"\n", label, copy)
    asm_loadLabelIntoRegister(label, "$t0");
    asm_getStackAddress("$t1", getMipsOffset(slot));
    asm_code_printf("\tsw $t0, 0($t1)\n")
    free(copy);
    return slot;
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


    return RETURN_SUCCESS;
}

MemorySlot doGetVariableAddress()
{
    log_trace("doGetVariableAddress")
    CHECK_IDENTIFIER_NOT_ZERO("doGetVariableAddress")

    char *name = listIdentifierOrder->cursor->name;
    // TODO ICI ???
    int a = searchIdentifierPosition(listRangeVariable, name)->indexIdentifier;
    //if (getOffset(listRangeVariable, name, listTmp) == RETURN_FAILURE)
      //  return RETURN_FAILURE;
    deleteIdentifierOrder(listIdentifierOrder);
    return getOffsetOfIdentifier(listRangeVariable->cursor->listIdentifier, a);
}

// Utils
// TODO: CONVERT TO HANDLE +/-
int parseInt32(const char *word)
{
    char *endptr;
    long int parsed = strtol(word, &endptr, 10);

    if (endptr == word || *endptr != '\0')
    {
        log_error("Invalid number for parseInt32(%s)", word);
        return RETURN_FAILURE;
    }

    if (parsed < INT_MIN || parsed > INT_MAX)
    {
        // The number is not within the range of a 32-bit integer
        log_error("Number is not 32-bit for parseInt32(%s)", word);
        return RETURN_FAILURE;
    }

    return (int) parsed;
}

int checkRegex(const char *pattern, const char *string)
{
    log_trace("checkRegex(%s, %s)", pattern, string)
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

MemorySlot doWriteInt(const char *val)
{
    MemorySlot mem = reserveMemorySlot();
    setTypeOrder(INTEGER);
    asm_getStackAddress("$t0", getMipsOffset(mem));
    int parsed;
    if((parsed = parseInt32(val))== RETURN_FAILURE) return NULL;
    asm_code_printf("\tli $t1, %d\n", parsed)
    asm_code_printf("\tsw $t1, 0($t0)\n")
    return mem;
}