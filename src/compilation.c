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
#include "errorManager.asm.h"

ListRangeVariable listRangeVariable;
ListInstruction listInstruction;
int marker;

/*!
 * \fn void initStruct()
 * \brief Fonction qui initialise les structures
*/
void initStruct()
{
    log_trace("Started initStruct")
    error = FALSE;
    listRangeVariable = initListRangeVariable();
    listInstruction = initListInstruction();
}

/*!
 * \fn void freeStruct()
 * \brief Fonction qui libère les structures
*/
void freeStruct()
{
    log_trace("Started freeStruct")
    cleanListRangeVariable(listRangeVariable);
    cleanListInstruction(listInstruction);
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
    CHECK_ERROR_RETURN(RETURN_FAILURE)
    yyin = inputFile;

    // Generate all functions & headers
    asm_writeHeader();
    asm_code_printf("\t# Args handling\n")
    asm_writeArgsToStack();
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
    asm_writeErrorManagerFunctions();
    asm_code_printf("\n")
    asm_code_printf("# Start of main code section\n")
    asm_code_printf("\n")
    asm_code_printf("_main:\n")
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    // Parse
    int result = yyparse();
    if (result != RETURN_SUCCESS)
    {
        freeStruct();
        return RETURN_FAILURE;
    }

    result = writeToFile(listInstruction, outputFile == NULL ? stdout : outputFile);
    if (result != RETURN_SUCCESS)
    {
        freeStruct();
        return RETURN_FAILURE;
    }

    freeStruct();
    return RETURN_SUCCESS;
}

MemorySlot doConcatenation(MemorySlotList slotList)
{
    log_trace("doConcatenation")
    asm_code_printf("\t# start of concatenation\n")

    if (slotList == NULL) {
        log_error("Cant concatenate an empty list")
        setErrorFailure();
        return NULL;
    }
    
    MemorySlot memorySlot = reserveBlockMemorySlot(listRangeVariable);
    CHECK_ERROR_RETURN(NULL)

    asm_code_printf("\taddi $t0, $zero, 1 # Size counter\n") // $t0 will be the total size of concat (start at 1 for \0)
    MemorySlotList first = firstMemorySlotList(slotList);
    MemorySlotList temp = first;
    CHECK_ERROR_RETURN(NULL)

    // Calculate size
    while(temp != NULL)
    {
        // slot should not be null
        if(temp->slot == NULL) {
            log_error("concatenation: slot in slotlist is null")
            temp = temp->next;
            continue;
        }

        if(temp->slot->label == NULL)
        {
            asm_readFromStack("$t1", CALCULATE_OFFSET(temp->slot));
        } else {
            asm_loadLabelIntoRegister(temp->slot->label, "$t1");
        }

        asm_useBufferLenFunction("$t1", "$t2");
        asm_code_printf("\tadd $t0, $t0, $t2\n")
        temp = temp->next;
    }

    asm_code_printf("\tmove $a0, $t0\n")
    asm_syscall(SBRK);
    asm_code_printf("\tmove $t0, $v0\n") // move start address to v0
    // write heap address to stack
    if(memorySlot->label == NULL)
    {
        asm_getStackAddress("$t1", CALCULATE_OFFSET(memorySlot));
    } else {
        asm_loadLabelAddressIntoRegister(memorySlot->label, "$t1");
    }
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

        if(temp->slot->label == NULL)
        {
            asm_readFromStack("$t1", CALCULATE_OFFSET(temp->slot));
            temp->slot->used = false;
        } else {
            asm_loadLabelIntoRegister(temp->slot->label, "$t1");
        }

        asm_useBufferWriteFunction("$t1", "$t0", "$t0");
        temp = temp->next;
    }

    // At the end write $zero
    asm_code_printf("\tsb $zero, 0($t0)\n")
    asm_code_printf("\t# end of concatenation\n")

    CHECK_ERROR_RETURN(NULL)
    return memorySlot;
}

/*!
 * \fn int assign()
 * \brief Fonction qui ajoute l'identifiant à la liste et transmet les données qui le compose
*/
MemorySlot assign(char *name, MemorySlot memorySlot, bool local)
{
    log_trace("assign (void)")

    asm_code_printf("\n\t# assign of %s\n", name)
    Identifier iden = getIdentifier(name, true, false);
    CHECK_ERROR_RETURN(NULL)
    MemorySlot slot = iden->memory;
    if (slot == NULL) return slot;

    if(memorySlot->label == NULL)
    {
        asm_readFromStack("$t0", CALCULATE_OFFSET(memorySlot));
        memorySlot->used = false;
    } else {
        asm_loadLabelIntoRegister(memorySlot->label, "$t0");
    }

    if(slot->label == NULL)
    {
        asm_getStackAddress("$t1", CALCULATE_OFFSET(slot));
        slot->used = false;
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t1");
    }

    asm_code_printf("\tsw $t0, 0($t1)\n")
    CHECK_ERROR_RETURN(NULL)
    return slot;
}

int assignArrayValue(char *name, MemorySlot offset, MemorySlot concat)
{
    log_trace("assignArrayValue(%s)", name)
    Identifier iden = getIdentifier(name, false, false);
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    if (iden == NULL)
    {
        log_error("Array %s was not set, cant assign.", name)
        setErrorFailure();
        return RETURN_FAILURE;
    }

    if(iden->type != ARRAY)
    {
        log_error("Can't access to a non array variable (%s).", name)
        setErrorFailure();
        return RETURN_FAILURE;
    }

    if(offset == NULL || concat == NULL) {
        setErrorFailure();
        return RETURN_FAILURE;
    }

    // slot -> address of table
    MemorySlot slot = iden->memory;
    // offset -> address of offset
    if(offset->label == NULL)
    {
        asm_readFromStack("$t0", CALCULATE_OFFSET(offset));
        freeMemory(offset);
    } else {
        asm_loadLabelIntoRegister(offset->label, "$t0");
    }

    // check if array out of bounds
    asm_code_printf("\tli $t1, %d\n", iden->size)
    // error management
    asm_code_printf("\tbge $t0, $t1, %s\n", ASM_OUT_OF_BOUNDS_ERROR_FUNCTION_NAME)
    asm_code_printf("\tblt $t0, $zero, %s\n", ASM_OUT_OF_BOUNDS_ERROR_FUNCTION_NAME)

    // load address of table
    if(slot->label == NULL)
    {
        asm_readFromStack("$t1", CALCULATE_OFFSET(slot));
    } else {
        asm_loadLabelIntoRegister(slot->label, "$t1");
    }
    asm_code_printf("\tmul $t2, $t0, %d\n", ASM_INTEGER_SIZE)
    // access to $t1[$t0] address
    asm_code_printf("\tadd $t1, $t1, $t2\n")

    // read start address from concatenation from stack into $t5
    if(concat->label == NULL)
    {
        asm_readFromStack("$t5", CALCULATE_OFFSET(concat));
        freeMemory(concat);
    } else {
        asm_loadLabelIntoRegister(concat->label, "$t5");
    }
    asm_code_printf("\tsw $t5, 0($t1)\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int doExit(MemorySlot slot)
{
    if(slot == NULL) return asm_syscall(SYS_EXIT);

    if(slot->label == NULL)
    {
        asm_readFromStack("$a0", CALCULATE_OFFSET(slot));
        freeMemory(slot);
    } else {
        asm_loadLabelIntoRegister(slot->label, "$a0");
    }

    return asm_syscall(SYS_EXIT2);
}

MemorySlot doOperation(MemorySlot left, int operation, MemorySlot right)
{
    asm_code_printf("\n\t#Start of operation code\n\n")

    if(left->label == NULL)
    {
        asm_readFromStack("$t0", CALCULATE_OFFSET(left));
    } else {
        asm_loadLabelIntoRegister(left->label, "$t0");
        // identifier so find a new memory slot
        left = reserveBlockMemorySlot(listRangeVariable);
        CHECK_ERROR_RETURN(NULL)
    }

    if(right->label == NULL)
    {
        asm_readFromStack("$t1", CALCULATE_OFFSET(right));
        freeMemory(right);
    } else {
        asm_loadLabelIntoRegister(right->label, "$t1");
    }

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

    // no check for label because 'left' can only be a temp value
    asm_getStackAddress("$t1", CALCULATE_OFFSET(left));
    asm_code_printf("\tsw $t0, 0($t1)\n")

    asm_code_printf("\n\t#End of operation code\n\n")

    CHECK_ERROR_RETURN(NULL)
    return left;
}

int doEcho(MemorySlotList list)
{
    log_trace("doEcho")
    asm_code_printf("\n\t# Do echo section\n\n")

    list = firstMemorySlotList(list);
    if(list == NULL)
    {
        log_error("list == NULL;")
        setErrorFailure();
        return RETURN_FAILURE;
    }
    MemorySlotList first = list;

    do {
        if(list->slot == NULL) {
            log_error("list->slot == NULL;")
            setErrorFailure();
            destroyMemoryList(first);
            return RETURN_FAILURE;
        }

        if(list->slot->label == NULL)
        {
            asm_readFromStack("$a0", CALCULATE_OFFSET(list->slot));
            freeMemory(list->slot);
        } else {
            asm_loadLabelIntoRegister(list->slot->label, "$a0");
        }

        asm_jal(ASM_DISPLAY_STRING_FUNCTION_NAME);

        list = list->next;
    } while(list != NULL);

    destroyMemoryList(first);

    asm_code_printf("\n\t# End do echo section\n\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int setMarker(){
    marker = listInstruction->cursorCode->numberCode;
    asm_code_printf("\t%s : \n",createNewLabel())

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int doMarkerTest()
{
    char* then = (char*)createNewLabel();
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    asm_code_printf("\t%s:\n",then)
    completeOneUnDefineGoto(listInstruction, "");
    completeTrueList(listInstruction,then);

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int doMarkerElse()
{
    char* then = (char*)createNewLabel();
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    asm_code_printf("\t%s:\n",then)
    completeFalseList(listInstruction,then);

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int doMarkerFi()
{
    char* then = (char*)createNewLabel();
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    asm_code_printf("\t%s:\n",then)
    completeFalseList(listInstruction,then);
    completeUnDefineGoto(listInstruction,then);

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int doMarkerEndInstruction()
{
    addIntoUnDefineGoto(listInstruction,"\tj");
    asm_code_printf("\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int doMarkerLoop()
{
    asm_code_printf("\n\t# Start of Test block of LOOP\n")
    addIntoUnDefineGoto(listInstruction,"\t");
    asm_code_printf("\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int doMarkerTestFor()
{
    addIntoTrueList(listInstruction,"\tblt $t0, $t1,");
    addIntoFalseList(listInstruction,"\n\tj");
    asm_code_printf("\n")
    addIntoUnDefineGoto(listInstruction,"\t");

    asm_code_printf("\n\tj %s\n",createNewForLabel())
    asm_code_printf("\n\t %s_:\n",getForLabel())

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int doMarkerFor()
{
    asm_code_printf("\n\tli $t0, $zero\n")
    asm_readFromStack("$t1", 0);

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int doForIdAssign(char *name)
{
    asm_code_printf("\n\t %s:\n",getForLabel())

    asm_code_printf("\n\t# assign of %s\n", name)
    Identifier iden = getIdentifier(name, true, false);
    CHECK_ERROR_RETURN(RETURN_FAILURE)
    MemorySlot slot = iden->memory;
    if (slot == NULL) return RETURN_FAILURE;

    if(slot->label == NULL)
    {
        asm_getStackAddress("$t2", CALCULATE_OFFSET(slot));
        slot->used = false;
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t2");
    }
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    asm_code_printf("\tmul $t3, $t0, %d\n", ASM_INTEGER_SIZE)
    asm_code_printf("\tlw $t4, 0($t3)\n")
    asm_code_printf("\tsw $t4, 0($t2)\n")

    asm_code_printf("\n\tj %s_\n",createNewForLabel())

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int doMarkerEndLoop()
{
    char* then = (char*)createNewLabel();
    asm_code_printf("\t%s:\n",then)
    completeFalseList(listInstruction,then);
    completeUnDefineGoto(listInstruction,then);

    asm_code_printf("\n\t# End of Test block of LOOP\n\n")
    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int doMarkerDone()
{
    char* then = (char*)createNewLabel();
    completeUnDefineGoto(listInstruction,then);
    asm_code_printf("\n\tj %s\n",then)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

Marker doMarkerForList(MemorySlotList list)
{
    log_trace("doMarkerForList")
    asm_code_printf("\n\t# Do marker for list section\n\n")

    list = firstMemorySlotList(list);
    if(list == NULL)
    {
        log_error("list == NULL;")
        setErrorFailure();
        return NULL;
    }
    MemorySlotList first = list;

    // asm_writeRegistersToStack();

    int count = 1;

    do
    {
        if (list->slot == NULL)
        {
            log_error("list->slot == NULL;")
            destroyMemoryList(first);
            setErrorFailure();
            return NULL;
        }

        count++;

        list = list->next;
    } while (list != NULL);

    asm_code_printf("\taddi $sp, $sp, -%d\n", count * ASM_INTEGER_SIZE)
    asm_appendInternalOffset(count);

    list = first;

    asm_code_printf("\tli $t0, %d\n", count)
    asm_code_printf("\tsw $t0, 0($sp)\n")

    Marker mark = newMarker();
    CHECK_ERROR_RETURN(NULL)
    mark->index = count;

    count = 0;

    do {

        count += ASM_INTEGER_SIZE;

        if(list->slot->label == NULL)
        {
            asm_readFromStack("$a0", CALCULATE_OFFSET(list->slot));
            freeMemory(list->slot);
        } else {
            asm_loadLabelIntoRegister(list->slot->label, "$a0");
        }

        asm_code_printf("\tsw $a0, %d($sp)\n", count)

        list = list->next;
    } while(list != NULL);

    destroyMemoryList(first);

    asm_code_printf("\tli $s7, 0\n")
    asm_code_printf("\n\t# End do marker for list section\n\n")

    CHECK_ERROR_RETURN(NULL)
    return mark;
}

int doDeleteLocalOffset(Marker mark)
{
    // mark contains only int for number of elements on stack
    asm_subtractInternalOffset(mark->index);
    destroyMarker(mark);
    asm_loadRegistersFromStack();
    return RETURN_SUCCESS;
}

int addBlock(int blockType)
{
    int returnValue;
    returnValue = addRangeVariable(listRangeVariable, blockType);
    CHECK_ERROR_RETURN(RETURN_FAILURE)
    returnValue += addStructListGoTo(listInstruction);
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    return returnValue;
}

int deleteBlock()
{
    int returnValue;
    returnValue = deleteRangeVariable(listRangeVariable);
    CHECK_ERROR_RETURN(RETURN_FAILURE)
    returnValue += deleteStructListGoTo(listInstruction);
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    return returnValue;
}

MemorySlot doBoolExpression(MemorySlot left, boolExpr_t boolExpr, MemorySlot right)
{
    log_trace("doBoolExpression (int %d)", boolExpr)

    asm_code_printf("\n\t# Start of Test block of ope %d\n\n", boolExpr)

    if((boolExpr != L_AND) && (boolExpr != L_OR)){

        if (left == NULL || right == NULL) {
            log_error("Cant do bool expr on null")
            setErrorFailure();
            return NULL;
        }

        if(left->label == NULL)
        {
            asm_readFromStack("$t0", CALCULATE_OFFSET(left));
            freeMemory(left);
        } else {
            asm_loadLabelIntoRegister(left->label, "$t0");
        }

        if(right->label == NULL)
        {
            asm_readFromStack("$t1", CALCULATE_OFFSET(right));
            freeMemory(right);
        } else {
            asm_loadLabelIntoRegister(right->label, "$t1");
        }
        CHECK_ERROR_RETURN(NULL)
    }

    if (boolExpr == BOOL_EQ || boolExpr == BOOL_NEQ || boolExpr == BOOL_GT ||
        boolExpr == BOOL_GE || boolExpr == BOOL_LT || boolExpr == BOOL_LE)
    {
        asm_useAtoiFunction("$t0","$t0");
        asm_useAtoiFunction("$t1","$t1");
        CHECK_ERROR_RETURN(NULL)
    }

    char* block;
    char* block1;
    switch (boolExpr)
    {
        case STR_EQ:
            asm_useStrCmpFunction("$t0", "$t1", "$t0");
            addIntoTrueList(listInstruction,"\tbeq $t0, 1,");
            addIntoFalseList(listInstruction,"\n\tj");
            asm_code_printf("\n")
            addIntoUnDefineGoto(listInstruction,"\t");
            break;
        case STR_NEQ:
            asm_useStrCmpFunction("$t0", "$t1", "$t0");
            addIntoTrueList(listInstruction,"\tbeq $t0, $zero,");
            addIntoFalseList(listInstruction,"\n\tj");
            asm_code_printf("\n")
            addIntoUnDefineGoto(listInstruction,"\t");
            break;
        case BOOL_EQ:
            addIntoTrueList(listInstruction,"\tbeq $t0, $t1,");
            addIntoFalseList(listInstruction,"\n\tj");
            asm_code_printf("\n")
            addIntoUnDefineGoto(listInstruction,"\t");
            break;
        case BOOL_NEQ:
            addIntoTrueList(listInstruction,"\tbne $t0, $t1,");
            addIntoFalseList(listInstruction,"\n\tj");
            asm_code_printf("\n")
            addIntoUnDefineGoto(listInstruction,"\t");
            break;
        case BOOL_GT:
            addIntoTrueList(listInstruction,"\tbgt $t0, $t1,");
            addIntoFalseList(listInstruction,"\n\tj");
            asm_code_printf("\n")
            addIntoUnDefineGoto(listInstruction,"\t");
            break;
        case BOOL_GE:
            addIntoTrueList(listInstruction,"\tbge $t0, $t1,");
            addIntoFalseList(listInstruction,"\n\tj");
            asm_code_printf("\n")
            addIntoUnDefineGoto(listInstruction,"\t");
            break;
        case BOOL_LT:
            addIntoTrueList(listInstruction,"\tblt $t0, $t1,");
            addIntoFalseList(listInstruction,"\n\tj");
            asm_code_printf("\n")
            addIntoUnDefineGoto(listInstruction,"\t");
            break;
        case BOOL_LE:
            addIntoTrueList(listInstruction,"\tble $t0, $t1,");
            addIntoFalseList(listInstruction,"\n\tj");
            asm_code_printf("\n")
            addIntoUnDefineGoto(listInstruction,"\t");
            break;
        case L_AND:
            asm_code_printf("\n\t# Start of Test block of AND\n")

            block = (char*)createNewLabel();
            CHECK_ERROR_RETURN(NULL)
            completeOneUnDefineGoto(listInstruction,block);
            completeTrueList(listInstruction,block);

            block = (char*)createNewLabel();
            CHECK_ERROR_RETURN(NULL)
            completeOneUnDefineGoto(listInstruction,block);
            completeTrueList(listInstruction,block);

            block = (char*)createNewLabel();
            CHECK_ERROR_RETURN(NULL)
            asm_code_printf("\t%s:\n",block)
            addIntoTrueList(listInstruction,"\tj");
            asm_code_printf("\n")

            block = (char*)createNewLabel();
            CHECK_ERROR_RETURN(NULL)
            asm_code_printf("\t%s:\n",block)
            completeFalseList(listInstruction, block);
            completeFalseList(listInstruction, block);
            addIntoFalseList(listInstruction,"\tj");
            asm_code_printf("\n")

            addIntoUnDefineGoto(listInstruction,"\t");

            asm_code_printf("\n\t# End of Test block of AND\n")
            break;
        case L_OR:
            asm_code_printf("\n\t# Start of Test block of OR\n")

            block = (char*)createNewLabel();
            block1 = (char*)createNewLabel();
            CHECK_ERROR_RETURN(NULL)
            asm_code_printf("\t%s:\n",block)
            completeOneUnDefineGoto(listInstruction,block1);
            completeTrueList(listInstruction,block);
            completeTrueList(listInstruction,block);

            addIntoTrueList(listInstruction,"\tj");
            asm_code_printf("\n")

            block = (char*)createNewLabel();
            block1 = (char*)createNewLabel();
            CHECK_ERROR_RETURN(NULL)
            asm_code_printf("\t%s:\n",block)
            completeOneUnDefineGoto(listInstruction,block1);
            completeFalseList(listInstruction, block);
            completeFalseList(listInstruction, "");

            addIntoFalseList(listInstruction,"\tj");
            asm_code_printf("\n")

            addIntoUnDefineGoto(listInstruction,"\t");
            asm_code_printf("\n\t# End of Test block of OR\n")
            break;
        default:
            log_error("Operation not allow %d",boolExpr)
            break;
    }
    asm_code_printf("\n")

    asm_code_printf("\n\t# End of Test block of ope %d\n\n", boolExpr)
    CHECK_ERROR_RETURN(NULL)
    return NULL;
}

MemorySlot doEmptyBoolExpression( boolExpr_t boolExpr, MemorySlot right)
{
    log_trace("doEmptyBoolExpression (int %d)", boolExpr)

    asm_code_printf("\n\t# Start of Test block of ope %d\n", boolExpr)

    if ( right == NULL) {
        log_error("Cant do bool expr on null")
        setErrorFailure();
        return NULL;
    }

    if(right->label == NULL)
    {
        asm_readFromStack("$t1", CALCULATE_OFFSET(right));
        freeMemory(right);
    } else {
        asm_loadLabelIntoRegister(right->label, "$t1");
    }

    switch (boolExpr)
    {
        case EMPTY:
            asm_code_printf("\tlb $t0, 0($t1)");
            addIntoTrueList(listInstruction,"\n\tbeq $t0, $zero,");
            addIntoFalseList(listInstruction,"\n\tj");
            asm_code_printf("\n")
            addIntoUnDefineGoto(listInstruction,"\t");
            break;
        case NOT_EMPTY:
            asm_code_printf("\tlb $t0, 0($t1)");
            addIntoTrueList(listInstruction,"\n\tbne $t0, $zero,");
            addIntoFalseList(listInstruction,"\n\tj");
            asm_code_printf("\n")
            addIntoUnDefineGoto(listInstruction,"\t");
            break;
        default:
            log_error("Operation not allow %d",boolExpr)
            break;
    }
    asm_code_printf("\n")

    asm_code_printf("\n\t# End of Test block of ope %d\n", boolExpr)
    return NULL;
}

int doNegBoolExpression()
{
    return switchTrueFalseList(listInstruction);
}

Identifier getIdentifier(char *id, bool create, bool local)
{

    if(create)
    {
        addIdentifier(listRangeVariable, id);
        CHECK_ERROR_RETURN(NULL)
    }

    VariablePosition pos = searchIdentifierPosition(listRangeVariable, id);
    CHECK_ERROR_RETURN(NULL)

    if (pos->indexIdentifier == NOTFOUND)
    {
        log_debug("Identifier not found.")
        return NULL;
    }

    int position = pos->indexIdentifier;
    ListIdentifier listIdentifier = pos->rangePosition->listIdentifier;

    if (position <= NOTFOUND || position >= listIdentifier->numberIdentifiers)
    {
        log_debug("Position out of range : position : %d", position)
        return NULL;
    }

    Identifier iden = listIdentifier->Identifiers[position];

    if (iden == NULL)
    {
        log_error("Identifier not found in listIdentifiers")
        setErrorFailure();
        return NULL;
    }

    return iden;
}

int doDeclareStaticArray(char *id, int size)
{
    log_trace("doDeclareStaticArray(%s, %d)", id, size)

    Identifier iden = getIdentifier(id, false, false);
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    if(iden != NULL)
    {
        log_error("Array %s was already declared.", id)
        setErrorFailure();
        return RETURN_FAILURE;
    }
    asm_code_printf("\t# Start of declaration of table %s : %d\n", id, size)

    // Now create identifier
    iden = getIdentifier(id, true, false);
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    // add array size & type of identifier
    iden->size = size;
    iden->type = ARRAY;

    MemorySlot slot = iden->memory;
    if(slot == NULL)
    {
        setErrorFailure();
        return RETURN_FAILURE;
    }

    const char *label = createNewLabel();
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    asm_writeStaticArray(label, size);
    if(slot->label == NULL)
    {
        asm_getStackAddress("$t0", CALCULATE_OFFSET(slot));
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t0");
    }
    asm_loadLabelAddressIntoRegister(label, "$t1");
    asm_code_printf("\tsw $t1, 0($t0)\n")
    free((char*)label);
    asm_code_printf("\t# end of declaration of table %s\n", id)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

MemorySlot addStringToMemory(const char *str) {
    char *copy;
    unsigned int len = strlen(str) - 1;

    CHECKPOINTER(copy = malloc(len))
    CHECK_ERROR_RETURN(NULL)
    CHECKPOINTER(strncpy(copy, str + 1, len - 1));
    CHECK_ERROR_RETURN(NULL)

    copy[len - 1] = '\0'; // add nul char
    MemorySlot slot = reserveBlockMemorySlot(listRangeVariable);
    const char* label = createNewLabel();
    CHECK_ERROR_RETURN(NULL)

    asm_data_printf("\t%s: .asciiz \"%s\"\n", label, copy)
    asm_loadLabelAddressIntoRegister(label, "$t0");
    //asm_allocateOnHeap("$t1", (int)len - 1);
    //asm_useBufferWriteFunction("$t0", "$t1", "$t1");
    if(slot->label == NULL)
    {
        asm_getStackAddress("$t1", CALCULATE_OFFSET(slot));
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t1");
    }
    asm_code_printf("\tsw $t0, 0($t1)\n")

    free(copy);
    CHECK_ERROR_RETURN(NULL)
    return slot;
}

MemorySlot addWordToMemory(const char *str) {
    MemorySlot slot = reserveBlockMemorySlot(listRangeVariable);
    const char* label = createNewLabel();
    CHECK_ERROR_RETURN(NULL)

    asm_data_printf("\t%s: .asciiz \"%s\"\n", label, str)
    asm_loadLabelAddressIntoRegister(label, "$t0");
    //asm_allocateOnHeap("$t1", (int)len - 1);
    //asm_useBufferWriteFunction("$t0", "$t1", "$t1");
    if(slot->label == NULL)
    {
        asm_getStackAddress("$t1", CALCULATE_OFFSET(slot));
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t1");
    }
    asm_code_printf("\tsw $t0, 0($t1)\n")

    CHECK_ERROR_RETURN(NULL)
    return slot;
}

int doArrayRead(char *id, MemorySlot offset)
{
    log_trace("doStringRead(%s)", id)
    Identifier iden = getIdentifier(id, false, false);
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    if (iden == NULL)
    {
        log_error("Array %s was not set, cant assign.", id)
        setErrorFailure();
        return RETURN_FAILURE;
    }

    if(iden->type != ARRAY)
    {
        log_error("Can't access to a non array variable (%s).", id)
        setErrorFailure();
        return RETURN_FAILURE;
    }

    if(offset == NULL) {
        setErrorFailure();
        return RETURN_FAILURE;
    }

    // slot -> address of table
    MemorySlot slot = iden->memory;
    CHECK_ERROR_RETURN(RETURN_FAILURE)
    // offset -> address of offset
    asm_readFromStack("$t0", CALCULATE_OFFSET(offset));

    if(offset->label == NULL)
    {
        asm_readFromStack("$t0", CALCULATE_OFFSET(offset));
        freeMemory(offset);
    } else {
        asm_loadLabelIntoRegister(offset->label, "$t0");
    }

    // check if array out of bounds
    asm_code_printf("\tli $t1, %d\n", iden->size)
    // error management
    asm_code_printf("\tbge $t0, $t1, %s\n", ASM_OUT_OF_BOUNDS_ERROR_FUNCTION_NAME)
    asm_code_printf("\tblt $t0, $zero, %s\n", ASM_OUT_OF_BOUNDS_ERROR_FUNCTION_NAME)

    // load address of table
    if(slot->label == NULL)
    {
        asm_readFromStack("$t1", CALCULATE_OFFSET(slot));
        freeMemory(slot);
    } else {
        asm_loadLabelIntoRegister(slot->label, "$t1");
    }

    asm_code_printf("\tmul $t2, $t0, %d\n", ASM_INTEGER_SIZE)
    // access to $t1[$t0] address
    asm_code_printf("\tadd $t1, $t1, $t2\n")

    // START READ

    // read start address from concatenation from stack into $t5
    asm_code_printf("\tla $a0, %s\n", ASM_VAR_GLOBAL_READ_BUFFER_NAME)
    asm_code_printf("\tla $a1, %d\n", ASM_VAR_GLOBAL_READ_BUFFER_SIZE)
    asm_syscall(READ_STRING);

    // string is in ASM_VAR_GLOBAL_READ_BUFFER_NAME
    // init a size counter
    asm_loadLabelAddressIntoRegister(ASM_VAR_GLOBAL_READ_BUFFER_NAME, "$t3");
    asm_useBufferLenFunction("$t3", "$t2");
    asm_code_printf("\taddi $t2, $t2, 1 # Add NUL char\n")

    asm_code_printf("\tmove $a0, $t2\n")
    asm_syscall(SBRK);
    asm_code_printf("\tmove $t2, $v0\n") // move start address to t2

    // write heap address to stack
    asm_code_printf("\tsw $t2, 0($t1)\n")
    asm_useBufferWriteFunction("$t3", "$t2", "$t2");

    // At the end write $zero
    asm_code_printf("\tsb $zero, 0($t2)\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

MemorySlot doGetVariableAddress(char *id, bool negative, bool isOperandInt)
{
    log_trace("doGetVariableAddress")

    VariablePosition pos = searchIdentifierPosition(listRangeVariable, id);
    CHECK_ERROR_RETURN(NULL)

    if(pos->indexIdentifier == NOTFOUND) {
        log_error("The variable was not found.")
        setErrorFailure();
        return NULL;
    }

    MemorySlot slot = getOffsetOfIdentifier(pos->rangePosition->listIdentifier, pos->indexIdentifier);
    CHECK_ERROR_RETURN(NULL)

    // convert to int
    if(isOperandInt)
    {
        if(slot->label == NULL)
        {
            asm_readFromStack("$t0", CALCULATE_OFFSET(slot));
        } else {
            asm_loadLabelIntoRegister(slot->label, "$t0");
            // set value to a new stack address
            slot = reserveBlockMemorySlot(listRangeVariable);
            CHECK_ERROR_RETURN(NULL)
        }

        // convert string to int (variables contains numbers as chars)
        asm_useAtoiFunction("$t0", "$t0");

        if(slot->label == NULL)
        {
            asm_getStackAddress("$t1", CALCULATE_OFFSET(slot));
        } else {
            asm_loadLabelAddressIntoRegister(slot->label, "$t1");
        }

        if(negative) {
            asm_code_printf("\tli $t2, -1\n")
            asm_code_printf("\tmul $t0, $t0, $t2\n")
        }

        asm_code_printf("\tsw $t0, 0($t1)\n")
        CHECK_ERROR_RETURN(NULL)
        return slot;
    }

    // No modification if positive
    if(!negative) return slot;
    if(slot->label != NULL)
    {
        slot = reserveBlockMemorySlot(listRangeVariable);
        CHECK_ERROR_RETURN(NULL)
    }

    asm_code_printf("\tli $t1, -1\n")
    asm_code_printf("\tmul $t0, $t0, $t1\n")

    if(slot->label == NULL)
    {
        asm_getStackAddress("$t2", CALCULATE_OFFSET(slot));
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t2");
    }
    asm_code_printf("\tsw $t0, 0($t2)\n")

    CHECK_ERROR_RETURN(NULL)
    return slot;
}

MemorySlot doGetArrayAddress(char *id, MemorySlot offset, bool negative,
                             bool isOperandInt)
{
    log_trace("doGetArrayAddress(%s)", id)

    VariablePosition pos = searchIdentifierPosition(listRangeVariable, id);
    CHECK_ERROR_RETURN(NULL)

    if(pos->indexIdentifier == NOTFOUND) {
        log_error("The variable was not found.")
        setErrorFailure();
        return NULL;
    }

    Identifier iden = pos->rangePosition->listIdentifier->Identifiers[pos->indexIdentifier];

    if(iden->type != ARRAY)
    {
        log_error("Can't access to a non array variable.")
        setErrorFailure();
        return NULL;
    }

    asm_code_printf("\t# Start of getting value of array %s\n", id)

    // slot -> address of table
    MemorySlot slot = getOffsetOfIdentifier(pos->rangePosition->listIdentifier, pos->indexIdentifier);
    CHECK_ERROR_RETURN(NULL)

    // offset -> address of offset
    if(offset->label == NULL)
    {
        asm_readFromStack("$t0", CALCULATE_OFFSET(offset));
    } else {
        asm_loadLabelIntoRegister(offset->label, "$t0");
        // set value to a new stack address
        offset = reserveBlockMemorySlot(listRangeVariable);
        CHECK_ERROR_RETURN(NULL)
    }

    // check if not array out of bounds
    asm_code_printf("\tli $t1, %d\n", iden->size)
    // error management
    asm_code_printf("\tbge $t0, $t1, %s\n", ASM_OUT_OF_BOUNDS_ERROR_FUNCTION_NAME)
    asm_code_printf("\tblt $t0, $zero, %s\n", ASM_OUT_OF_BOUNDS_ERROR_FUNCTION_NAME)

    // load address of table
    if(slot->label == NULL)
    {
        asm_readFromStack("$t1", CALCULATE_OFFSET(slot));
    } else {
        asm_loadLabelIntoRegister(slot->label, "$t1");
    }

    asm_code_printf("\tmul $t2, $t0, %d\n", ASM_INTEGER_SIZE)
    // access to $t1[$t0] address
    asm_code_printf("\tadd $t1, $t1, $t2\n")

    // load value of $t1[$t0] -> address pointer to heap
    asm_code_printf("\tlw $t3, 0($t1)\n")

    // check if array element is not allocated (-1)
    asm_code_printf("\tli $t4, -1\n")
    asm_code_printf("\tbeq $t3, $t4, %s\n", ASM_ARRAY_ELEMENT_NOT_ALLOCATED_ERROR_FUNCTION_NAME)
    CHECK_ERROR_RETURN(NULL)

    if(!isOperandInt)
    {
        if(offset->label == NULL)
        {
            asm_getStackAddress("$t3", CALCULATE_OFFSET(offset));
        } else {
            asm_loadLabelAddressIntoRegister(offset->label, "$t3");
        }

        // get value -> $t1[$t0]
        asm_code_printf("\tlw $t1, 0($t1)\n")
        asm_code_printf("\tsw $t1, 0($t3)\n")

        asm_code_printf("\t# End of getting value of array: %s\n", id)
        CHECK_ERROR_RETURN(NULL)
        return offset;
    }

    // $t1 = atoi($t1[$t0]])
    asm_useAtoiFunction("$t3", "$t0");

    if(offset->label == NULL)
    {
        asm_getStackAddress("$t1", CALCULATE_OFFSET(offset));
    } else {
        asm_loadLabelAddressIntoRegister(offset->label, "$t1");
    }

    if(negative) {
        asm_code_printf("\tli $t2, -1\n")
        asm_code_printf("\tmul $t0, $t0, $t2\n")
    }

    asm_code_printf("\tsw $t0, 0($t1)\n")
    asm_code_printf("\t# End of getting value of array: %s\n", id)

    CHECK_ERROR_RETURN(NULL)
    return offset;
}

// Utils
int parseInt32(const char *word, int *err)
{
    char *endptr;
    long int parsed = strtol(word, &endptr, 10);

    if (endptr == word || *endptr != '\0')
    {
        log_error("Invalid number for parseInt32(%s)", word);
        *err = 1;
        setErrorFailure();
        return RETURN_FAILURE;
    }

    if (parsed < INT_MIN || parsed > INT_MAX)
    {
        // The number is not within the range of a 32-bit integer
        log_error("Number is not 32-bit for parseInt32(%s)", word);
        *err = 1;
        setErrorFailure();
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
        setErrorFailure();
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

MemorySlot doWriteInt(char *val)
{
    log_trace("doWriteInt(%s)", val)
    CHECK_TYPE(checkWordIsInt(val))

    MemorySlot mem = reserveBlockMemorySlot(listRangeVariable);
    CHECK_ERROR_RETURN(NULL)
    asm_getStackAddress("$t0", CALCULATE_OFFSET(mem));
    CHECK_ERROR_RETURN(NULL)

    int parsed;
    int err = 0;
    if ((parsed = parseInt32(val, &err)) == RETURN_FAILURE && err)
    {
        // if(val != NULL) free(val);
        return NULL;
    } // in this case : setErrorFailure() has already been called
    mem->value = val;
    asm_code_printf("\tli $t1, %d\n", parsed)
    asm_code_printf("\tsw $t1, 0($t0)\n")

    CHECK_ERROR_RETURN(NULL)
    return mem;
}

int doParseTableInt(const char *val)
{
    log_trace("doParseTableInt(%s)", val)

    if (!checkWordIsInt(val))
    {
        log_error("%s should be int type.", val)
        setErrorFailure();
        return RETURN_FAILURE;
    }

    int parsedSize;
    if ((parsedSize = parseInt32(val, NULL)) == RETURN_FAILURE)
    {
        // in this case : setErrorFailure() has already been called
        log_error("Cant parse int32(%s)", val)
        return parsedSize;
    }

    if(parsedSize <= 0)
    {
        log_error("Table size should be > 0 (actually: %d)", parsedSize)
        setErrorFailure();
        return RETURN_FAILURE;
    }

    return parsedSize;
}

int doStringRead(const char *id)
{
    log_trace("doStringRead(%s)", id)

    Identifier iden = getIdentifier((char *) id, true, false);
    CHECK_ERROR_RETURN(RETURN_FAILURE)
    MemorySlot slot = iden->memory;
    if(slot == NULL)
    {
        setErrorFailure();
        return RETURN_FAILURE;
    }

    asm_code_printf("\tla $a0, %s\n", ASM_VAR_GLOBAL_READ_BUFFER_NAME)
    asm_code_printf("\tla $a1, %d\n", ASM_VAR_GLOBAL_READ_BUFFER_SIZE)
    asm_syscall(READ_STRING);

    // string is in ASM_VAR_GLOBAL_READ_BUFFER_NAME
    // init a size counter
    asm_loadLabelAddressIntoRegister(ASM_VAR_GLOBAL_READ_BUFFER_NAME, "$t0");
    asm_useBufferLenFunction("$t0", "$t1");
    asm_code_printf("\taddi $t1, $t1, 1 # Add NUL char\n")

    asm_code_printf("\tmove $a0, $t1\n")
    asm_syscall(SBRK);
    asm_code_printf("\tmove $t1, $v0\n") // move start address to v0
    // write heap address to stack
    if(slot->label == NULL)
    {
        asm_getStackAddress("$t2", CALCULATE_OFFSET(slot));
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t2");
    }
    asm_code_printf("\tsw $t1, 0($t2)\n")
    asm_useBufferWriteFunction("$t0", "$t1", "$t1");

    // At the end write $zero
    asm_code_printf("\tsb $zero, 0($t1)\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

MemorySlot convertIntToString(MemorySlot slot)
{
    log_trace("convertIntToString(%s)", slot)

    // /!\ slot not reallocated because it should always be temp

    if(slot->label == NULL)
    {
        asm_getStackAddress("$t0", CALCULATE_OFFSET(slot));
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t0");
    }
    asm_code_printf("\tla $t1, 0($t0)\n")

    asm_useIntToStringFunction("$t1", "$t1");

    // rewrite stack to new address
    asm_code_printf("\tsw $t1, 0($t0)\n")

    CHECK_ERROR_RETURN(NULL)
    return slot;
}

MemorySlot doUnaryCheck(MemorySlot slot, bool negative)
{
    log_trace("doUnaryCheck(%d, %s)", slot, negative ? "-" : "+")
    if(!negative) return slot;

    // /!\ slot not reallocated because it should always be temp
    if(slot->label == NULL)
    {
        asm_getStackAddress("$t0", CALCULATE_OFFSET(slot));
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t0");
    }

    asm_code_printf("\tlw $t1, 0($t0)\n")
    asm_code_printf("\tli $t2, -1\n")
    asm_code_printf("\tmul $t1, $t1, $t2\n")
    asm_code_printf("\tsw $t1, 0($t0)\n")

    CHECK_ERROR_RETURN(NULL)
    return slot;
}

int doDeclareFunction(Marker mark)
{
    // TODO: handle returns
    // from actual position to start position (mark)
    deleteRangeVariable(listRangeVariable); // delete one block
    asm_subtractInternalOffset(ASM_VAR_REGISTERS_CACHE_COUNT); // +1 is $ra
    asm_code_printf("\tjr $ra\n")
    asm_code_printf("\tend_%s:\n", mark->lbl)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

Marker doFunctionStartMarker(char* id)
{
    Identifier identifier = getIdentifier(id, false, false);
    Marker mark = newMarker();
    CHECK_ERROR_RETURN(NULL)

    if(identifier != NULL)
    {
        // mark->lbl => NULL identifier already exists
        mark->code = listInstruction->cursorCode;
        mark->index = listInstruction->cursorCode->numberCode;
        return mark;
    }

    mark->lbl = id;

    // Create identifier and start code
    identifier = getIdentifier(id, true, false);
    CHECK_ERROR_RETURN(NULL)
    identifier->type = FUNCTION;

    asm_code_printf("\tj end_%s\n", id)

    mark->code = listInstruction->cursorCode;
    mark->index = listInstruction->cursorCode->numberCode - 1;

    asm_code_printf("\tstart_%s:\n", id) // function name

    // creation du nouveau block
    addRangeVariable(listRangeVariable, BLOCK_FUNCTION);
    listRangeVariable->cursor->currentFunction = identifier;

    CHECK_ERROR_RETURN(NULL)
    return mark;
}

int doFunctionCall(char* id, MemorySlotList list)
{
    Identifier identifier = getIdentifier(id, false, false);
    CHECK_ERROR_RETURN(RETURN_FAILURE)
    if(identifier == NULL)
    {
        log_error("Function you trying to call is not existing.")
        free(id);
        return RETURN_FAILURE;
    }

    if(identifier->type != FUNCTION)
    {
        log_error("The variable you trying to call is not a function.")
        free(id);
        return RETURN_FAILURE;
    }

    int count = 0;

    do
    {
        count++;
        list = list->next;
    } while (list != NULL);

    if(count != identifier->size) {
        log_error("Function call %s requires exactly %d arguments", id, identifier->size);
        return RETURN_FAILURE;
    }
    // TODO:
    asm_code_printf("\t\n")

    do
    {
        list = list->next;
    } while (list != NULL);

    asm_code_printf("\tjal start_%s\n", id)
    free(id);

    destroyMemoryList(list);

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

MemorySlot doGetArgument(MemorySlot slot)
{
    if(slot == NULL || slot->value == NULL)
        return NULL;

    int err;
    int val = parseInt32(slot->value, &err);
    if (err == RETURN_FAILURE) return NULL;

    if(val < 1) {
        log_error("Arguments are starting at index 1")
        return NULL;
    }
    RangeVariable currCursor = listRangeVariable->cursor;

    if(currCursor->blockType == BLOCK_FUNCTION)
    {
        // Set argument count
        if(currCursor->currentFunction != NULL)
        {
            if(currCursor->currentFunction->size < val)
                currCursor->currentFunction->size = val;
        }

        asm_code_printf("\tadd $t2, $sp, $s7\n")
        if((val - 1) > 0)
        {
            asm_code_printf("\taddi $t2, $t2, %d\n", (val - 1) * ASM_INTEGER_SIZE)
        }
    } else {
        asm_code_printf("\tli $t0, %d\n", val)
        asm_code_printf("\tlw $t1, %s\n", ASM_VAR_ARGC)
        asm_code_printf("\tbgt $t0, $t1, %s\n", ASM_NON_EXISTENT_ARGUMENT_ERROR_FUNCTION_NAME)
        asm_code_printf("\tlw $t2, %s\n", ASM_VAR_ARGV_START)
        asm_code_printf("\taddi $t2, $t2, %d\n", (val - 1) * ASM_INTEGER_SIZE)
    }

    asm_code_printf("\tlw $t1, 0($t2)\n")
    asm_getStackAddress("$t2", CALCULATE_OFFSET(slot));
    asm_code_printf("\tsw $t1, 0($t2)\n")

    return slot;
}

int doReturn(MemorySlot slot)
{
    return RETURN_SUCCESS;
}