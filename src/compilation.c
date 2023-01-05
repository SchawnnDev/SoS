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
#include "intToString.asm.h"

ListRangeVariable listRangeVariable;
ListInstruction listInstruction;
int marker;


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
    listInstruction = initListInstruction();
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
    asm_writeIntToStringFunction();
    asm_code_printf("\n")
    asm_code_printf("# Start of main code section\n")
    asm_code_printf("\n")
    asm_code_printf("_main:\n")
    // Parse
    int result = yyparse();
    if (result != RETURN_SUCCESS) return result;
    destroyMemorySlot();
    return writeToFile(listInstruction, outputFile == NULL ? stdout : outputFile);
}

MemorySlot doConcatenation(MemorySlotList slotList)
{
    log_trace("doConcatenation")
    asm_code_printf("\t# start of concatenation\n")

    if (slotList == NULL) {
        log_error("Cant concatenate an empty list")
        return NULL;
    }

    MemorySlot memorySlot = reserveMemorySlot();
    asm_code_printf("\taddi $t0, $zero, 1 # Size counter\n") // $t0 will be the total size of concat (start at 1 for \0)
    MemorySlotList first = firstMemorySlotList(slotList);
    MemorySlotList temp = first;

    // Calculate size
    while(temp != NULL)
    {
        // slot should not be null
        if(temp->slot == NULL) {
            log_error("concatenation: slot in slotlist is null")
            temp = temp->next;
            continue;
        }
        asm_readFromStack("$t1", getMipsOffset(temp->slot));
        asm_useBufferLenFunction("$t1", "$t2");
        asm_code_printf("\tadd $t0, $t0, $t2\n")
        temp = temp->next;
    }

    asm_code_printf("\tmove $a0, $t0\n")
    asm_syscall(SBRK);
    asm_code_printf("\tmove $t0, $v0\n") // move start address to v0
    // write heap address to stack
    asm_getStackAddress("$t1", getMipsOffset(memorySlot));
    asm_code_printf("\tsw $t0, 0($t1)\n")
    // reset pointer to first
    temp = first;

    while (temp != NULL) {
        // slot should not be null
        if(temp->slot == NULL) {
            log_error("concatenation: slot in slotlist is null")
            temp = temp->next;
            continue;
        }
        asm_readFromStack("$t1", getMipsOffset(temp->slot));
        asm_useBufferWriteFunction("$t1", "$t0", "$t0");

        // Free memory
        if (temp->slot->temp)
            temp->slot->used = false;

        temp = temp->next;
    }

    // At the end write $zero
    asm_code_printf("\tsb $zero, 0($t0)\n")

    asm_code_printf("\t# end of concatenation\n")

    return memorySlot;
}

/*!
 * \fn int assign()
 * \brief Fonction qui ajoute l'identifiant à la liste et transmet les données qui le compose
*/
MemorySlot assign(char *name, MemorySlot memorySlot)
{
    log_trace("assign (void)")
    asm_code_printf("\n\t# assign of %s\n", name)
    MemorySlot slot = getOrCreateMemorySlot(name);
    if (slot == NULL) return slot;

    asm_readFromStack("$t0", getMipsOffset(memorySlot));

    asm_getStackAddress("$t1", getMipsOffset(slot));
    asm_code_printf("\tsw $t0, 0($t1)\n")

    if(memorySlot->temp) freeMemory(memorySlot);

    return slot;
}

void assignArray()
{

}

int doExit(MemorySlot slot)
{
    if(slot == NULL)
    {
        asm_syscall(SYS_EXIT);
        return RETURN_SUCCESS;
    }

    asm_readFromStack("$a0", getMipsOffset(slot));

    return asm_syscall(SYS_EXIT2);
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
            asm_code_printf("\tdiv $t0, $t1\n")
            asm_code_printf("\tmflo $t0\n")
            break;
        case MOD_OPE:
            asm_code_printf("\tdiv $t0, $t1\n")
            asm_code_printf("\tmfhi $t0\n")
            break;
        default:
            // ERROR: unknown operation
            break;
    }

    if (right->temp) freeMemory(right);
    if (!left->temp) left = reserveMemorySlot();

    asm_getStackAddress("$t1", getMipsOffset(left));
    asm_code_printf("\tsw $t0, 0($t1)\n")

    asm_code_printf("\n\t#End of operation code\n\n")

    return left;
}

int doOperationAddInt()
{
    return RETURN_SUCCESS;
}

int doEcho(MemorySlotList list)
{
    log_trace("doEcho")
    asm_code_printf("\n\t# Do echo section\n\n")

    log_trace("doConcatenation")

    list = firstMemorySlotList(list);
    if(list == NULL) { log_error("list == NULL;") return RETURN_FAILURE; }
    MemorySlotList first = list;

    do {
        if(list->slot == NULL) {
            log_error("list->slot == NULL;")
            destroyMemoryList(first);
            return RETURN_FAILURE;
        }

        asm_readFromStack("$a0", getMipsOffset(list->slot));
        asm_jal(ASM_DISPLAY_STRING_FUNCTION_NAME);

        if(list->slot->temp) // Free memory
         list->slot->used = false;

        list = list->next;
    } while(list != NULL);

    destroyMemoryList(first);

    asm_code_printf("\n\t# End do echo section\n\n")

    return RETURN_SUCCESS;
}

int setMarker(){
    marker = listInstruction->cursorCode->numberCode;
    asm_code_printf("\t%s : \n",createNewLabel())

    return RETURN_SUCCESS;
}

MemorySlot doBoolExpression(MemorySlot left, boolExpr_t boolExpr, MemorySlot right)
{


    switch (op)
    {
        case BOOL_EQ:
            asm_useStrCmpFunction("$t0", "$t1", "$t0");
            break;
        case BOOL_NEQ:
            asm_useStrCmpFunction("$t0", "$t1", "$t0");
            break;
    }


    asm_code_printf("\tsw $t0, 0($t1)\n")




    log_trace("doBoolExpression (int %d)", boolExpr)

    asm_code_printf("\n\t# Start of Test block of ope %d\n", boolExpr)

    if (left == NULL || right == NULL) {
        log_error("Cant do bool expr on null")
        return NULL;
    }

    asm_getStackAddress("$t0", getMipsOffset(left));
    asm_getStackAddress("$t1", getMipsOffset(right));

    if (boolExpr == BOOL_EQ || boolExpr == BOOL_NEQ || boolExpr == BOOL_GT ||
        boolExpr == BOOL_GE || boolExpr == BOOL_LT || boolExpr == BOOL_LE)
    {

    }

    char* else_lab;
    switch (boolExpr)
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
            else_lab = (char*)createNewLabel();
            completeTrueList(listInstruction,else_lab);
            completeTrueList(listInstruction,else_lab);
            //completeFalseList(listInstruction, );
            completeFalseList(listInstruction, listInstruction->cursorCode->lineCode[marker]);
            break;
    }

    if (right->temp) freeMemory(right);
    if (!left->temp) left = reserveMemorySlot();

    asm_getStackAddress("$t1", getMipsOffset(left));

    asm_code_printf("\n\t# End of Test block of ope %d\n", boolExpr)
    return left;
}

MemorySlot getOrCreateMemorySlot(char* id)
{
    addIdentifier(listRangeVariable, id);
    VariablePosition pos = searchIdentifierPosition(listRangeVariable, id);

    if (pos->indexIdentifier == NOTFOUND)
    {
        log_error("Identifier not found.")
        return NULL;
    }

    MemorySlot slot = getOffsetOfIdentifier(pos->rangePosition->listIdentifier,
                                            pos->indexIdentifier);

    if (slot == NULL)
    {
        log_error("MemorySlot of identifier not found")
        return NULL;
    }

    return slot;
}

int doDeclareStaticArray(char *id, int size)
{
    log_trace("doDeclareStaticArray(%s, %d)", id, size)

    MemorySlot slot = getOrCreateMemorySlot(id);
    if(slot == NULL) return RETURN_FAILURE;

    const char *label = createNewLabel();
    asm_writeStaticArray(label, size);
    asm_getStackAddress("$t0", getMipsOffset(slot));
    asm_loadLabelIntoRegister(label, "$t1");
    asm_code_printf("\tsw $t1, 0($t0)\n")
    free((char*)label);
    return RETURN_SUCCESS;
}

MemorySlot addStringToMemory(const char *str) {
    char *copy;
    unsigned int len = strlen(str) - 1;
    CHECKPOINTER(copy = malloc(len))
    CHECKPOINTER(strncpy(copy, str + 1, len - 1));
    copy[len - 1] = '\0'; // add nul char
    MemorySlot slot = reserveMemorySlot();
    const char* label = createNewLabel();
    asm_data_printf("\t%s: .asciiz \"%s\"\n", label, copy)
    asm_loadLabelIntoRegister(label, "$t0");
    //asm_allocateOnHeap("$t1", (int)len - 1);
    //asm_useBufferWriteFunction("$t0", "$t1", "$t1");
    asm_getStackAddress("$t1", getMipsOffset(slot));
    asm_code_printf("\tsw $t0, 0($t1)\n")
    free(copy);
    return slot;
}

MemorySlot addWordToMemory(const char *str) {
    MemorySlot slot = reserveMemorySlot();
    const char* label = createNewLabel();
    asm_data_printf("\t%s: .asciiz \"%s\"\n", label, str)
    asm_loadLabelIntoRegister(label, "$t0");
    //asm_allocateOnHeap("$t1", (int)len - 1);
    //asm_useBufferWriteFunction("$t0", "$t1", "$t1");
    asm_getStackAddress("$t1", getMipsOffset(slot));
    asm_code_printf("\tsw $t0, 0($t1)\n")
    return slot;
}

int doArrayRead()
{
    log_trace("doArrayRead")

    return RETURN_SUCCESS;
}

MemorySlot doGetVariableAddress(char *id, bool negative)
{
    log_trace("doGetVariableAddress")

    VariablePosition pos = searchIdentifierPosition(listRangeVariable, id);

    if(pos->indexIdentifier == NOTFOUND) {
        log_error("The variable was not found.")
        return NULL;
    }

    MemorySlot slot = getOffsetOfIdentifier(pos->rangePosition->listIdentifier, pos->indexIdentifier);

    // No modification if positive
    if(!negative) return slot;
    asm_readFromStack("$t0", getMipsOffset(slot));
    if(!slot->temp) slot = reserveMemorySlot();

    asm_code_printf("\tli $t1, -1\n")
    asm_code_printf("\tmul $t0, $t0, $t1\n")

    asm_getStackAddress("$t2", getMipsOffset(slot));
    asm_code_printf("\tsw $t0, 0($t2)\n")

    return slot;
}

// Utils
// TODO: CONVERT TO HANDLE +/-
int parseInt32(const char *word, int *err)
{
    char *endptr;
    long int parsed = strtol(word, &endptr, 10);

    if (endptr == word || *endptr != '\0')
    {
        log_error("Invalid number for parseInt32(%s)", word);
        *err = 1;
        return RETURN_FAILURE;
    }

    if (parsed < INT_MIN || parsed > INT_MAX)
    {
        // The number is not within the range of a 32-bit integer
        log_error("Number is not 32-bit for parseInt32(%s)", word);
        *err = 1;
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

    return ret;
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
    log_trace("doWriteInt(%s)", val)
    CHECK_TYPE(checkWordIsInt(val))
    MemorySlot mem = reserveMemorySlot();
    asm_getStackAddress("$t0", getMipsOffset(mem));
    int parsed;
    int err = 0;
    if ((parsed = parseInt32(val, &err)) == RETURN_FAILURE && err)
        return NULL;
    asm_code_printf("\tli $t1, %d\n", parsed)
    asm_code_printf("\tsw $t1, 0($t0)\n")
    return mem;
}

int doParseTableInt(const char *val)
{
    log_trace("doParseTableInt(%s)", val)

    if (!checkWordIsInt(val))
    {
        log_error("%s should be int type.", val)
        return RETURN_FAILURE;
    }

    int parsedSize;
    if ((parsedSize = parseInt32(val, NULL)) == RETURN_FAILURE)
    {
        log_error("Cant parse int32(%s)", val)
        return parsedSize;
    }

    if(parsedSize <= 0)
    {
        log_error("Table size should be > 0 (actually: %d)", parsedSize)
        return RETURN_FAILURE;
    }

    return parsedSize;
}

int doStringRead(const char *id)
{
    log_trace("doStringRead(%s)", id)
    MemorySlot slot = getOrCreateMemorySlot((char*)id);
    if(slot == NULL) return RETURN_FAILURE;

    asm_code_printf("\tla $a0, %s\n", ASM_VAR_GLOBAL_READ_BUFFER_NAME)
    asm_code_printf("\tla $a1, %d\n", ASM_VAR_GLOBAL_READ_BUFFER_SIZE)
    asm_syscall(READ_STRING);

    // string is in ASM_VAR_GLOBAL_READ_BUFFER_NAME
    // init a size counter
    asm_loadLabelIntoRegister(ASM_VAR_GLOBAL_READ_BUFFER_NAME, "$t0");
    asm_useBufferLenFunction("$t0", "$t1");
    asm_code_printf("\taddi $t1, $t1, 1 # Add NUL char\n")

    asm_code_printf("\tmove $a0, $t1\n")
    asm_syscall(SBRK);
    asm_code_printf("\tmove $t1, $v0\n") // move start address to v0
    // write heap address to stack
    asm_getStackAddress("$t2", getMipsOffset(slot));
    asm_code_printf("\tsw $t1, 0($t2)\n")
    asm_useBufferWriteFunction("$t0", "$t1", "$t1");

    // At the end write $zero
    asm_code_printf("\tsb $zero, 0($t1)\n")

    return RETURN_SUCCESS;
}

MemorySlot convertIntToString(MemorySlot slot)
{
    log_trace("convertIntToString(%s)", slot)
    // /!\ slot not reallocated because it should always be temp

    asm_getStackAddress("$t0", getMipsOffset(slot));
    asm_code_printf("\tla $t1, 0($t0)\n")

    asm_useIntToStringFunction("$t1", "$t1");

    // rewrite stack to new address
    asm_code_printf("\tsw $t1, 0($t0)\n")

    return slot;
}

MemorySlot doUnaryCheck(MemorySlot slot, bool negative)
{
    log_trace("doUnaryCheck(%d, %s)", slot, negative ? "-" : "+")
    if(!negative) return slot;
    // /!\ slot not reallocated because it should always be temp
    asm_getStackAddress("$t0", getMipsOffset(slot));
    asm_code_printf("\tlw $t1, 0($t0)\n")
    asm_code_printf("\tli $t2, -1\n")
    asm_code_printf("\tmul $t1, $t1, $t2\n")
    asm_code_printf("\tsw $t1, 0($t0)\n")
    return slot;
}

MemorySlot doConcatBoolExpr(MemorySlot left, boolExpr_t op, MemorySlot right)
{
    log_trace("doConcatenation")
    asm_code_printf("\t# start of concatenation\n")

    if (left == NULL || right == NULL) {
        log_error("Cant do bool expr on null")
        return NULL;
    }

    asm_getStackAddress("$t0", getMipsOffset(left));
    asm_getStackAddress("$t1", getMipsOffset(right));

    switch (op)
    {
        case BOOL_EQ:
            asm_useStrCmpFunction("$t0", "$t1", "$t0");
            break;
        case BOOL_NEQ:
            asm_useStrCmpFunction("$t0", "$t1", "$t0");
            break;
    }

    if (right->temp) freeMemory(right);
    if (!left->temp) left = reserveMemorySlot();

    asm_getStackAddress("$t1", getMipsOffset(left));
    asm_code_printf("\tsw $t0, 0($t1)\n")

    return left;
}