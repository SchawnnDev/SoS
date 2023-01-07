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
    asm_code_printf("\t# Args handling\n")
    asm_writeArgsToStack();
    //asm_code_printf("\n")
    //asm_code_printf("j _main\n")
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
    asm_writePrintErrorAndExit();
    asm_writeOutOfBoundsErrorFunction();
    asm_writeNotANumberErrorFunction();
    asm_writeArrayElementNotAllocatedErrorFunction();
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

        if(temp->slot->label == NULL)
        {
            asm_readFromStack("$t1", getMipsOffset(temp->slot));
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
        asm_getStackAddress("$t1", getMipsOffset(memorySlot));
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
            asm_readFromStack("$t1", getMipsOffset(temp->slot));
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
    MemorySlot slot = getIdentifier(name, true, false)->memory;
    if (slot == NULL) return slot;

    if(memorySlot->label == NULL)
    {
        asm_readFromStack("$t0", getMipsOffset(memorySlot));
        memorySlot->used = false;
    } else {
        asm_loadLabelIntoRegister(memorySlot->label, "$t0");
    }

    if(slot->label == NULL)
    {
        asm_getStackAddress("$t1", getMipsOffset(slot));
        slot->used = false;
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t1");
    }

    asm_code_printf("\tsw $t0, 0($t1)\n")

    return slot;
}

int assignArrayValue(char *name, MemorySlot offset, MemorySlot concat)
{
    log_trace("assignArrayValue(%s)", name)
    Identifier iden = getIdentifier(name, false, false);

    if (iden == NULL)
    {
        log_error("Array %s was not set, cant assign.", name)
        return RETURN_FAILURE;
    }

    if(iden->type != ARRAY)
    {
        log_error("Can't access to a non array variable (%s).", name)
        return RETURN_FAILURE;
    }

    if(offset == NULL || concat == NULL) {
        return RETURN_FAILURE;
    }

    // slot -> address of table
    MemorySlot slot = iden->memory;
    // offset -> address of offset
    if(offset->label == NULL)
    {
        asm_readFromStack("$t0", getMipsOffset(offset));
        freeMemory(offset);
    } else {
        asm_loadLabelIntoRegister(offset->label, "$t0");
    }

    // check if array out of bounds
    asm_code_printf("\tli $t1, %d\n", iden->arraySize)
    // error management
    asm_code_printf("\tbge $t0, $t1, %s\n", ASM_OUT_OF_BOUNDS_ERROR_FUNCTION_NAME)
    asm_code_printf("\tblt $t0, $zero, %s\n", ASM_OUT_OF_BOUNDS_ERROR_FUNCTION_NAME)

    // load address of table
    if(slot->label == NULL)
    {
        asm_readFromStack("$t1", getMipsOffset(slot));
    } else {
        asm_loadLabelIntoRegister(slot->label, "$t1");
    }
    asm_code_printf("\tmul $t2, $t0, %d\n", ASM_INTEGER_SIZE)
    // access to $t1[$t0] address
    asm_code_printf("\tadd $t1, $t1, $t2\n")

    // read start address from concatenation from stack into $t5
    if(concat->label == NULL)
    {
        asm_readFromStack("$t5", getMipsOffset(concat));
        freeMemory(concat);
    } else {
        asm_loadLabelIntoRegister(concat->label, "$t5");
    }
    asm_code_printf("\tsw $t5, 0($t1)\n")

    return RETURN_SUCCESS;
}

int doExit(MemorySlot slot)
{
    if(slot == NULL)
    {
        asm_syscall(SYS_EXIT);
        return RETURN_SUCCESS;
    }

    if(slot->label == NULL)
    {
        asm_readFromStack("$a0", getMipsOffset(slot));
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
        asm_readFromStack("$t0", getMipsOffset(left));
    } else {
        asm_loadLabelIntoRegister(left->label, "$t0");
        // identifier so find a new memory slot
        left = reserveMemorySlot();
    }

    if(right->label == NULL)
    {
        asm_readFromStack("$t1", getMipsOffset(right));
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
    asm_getStackAddress("$t1", getMipsOffset(left));
    asm_code_printf("\tsw $t0, 0($t1)\n")

    asm_code_printf("\n\t#End of operation code\n\n")

    return left;
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

        if(list->slot->label == NULL)
        {
            asm_readFromStack("$a0", getMipsOffset(list->slot));
            freeMemory(list->slot);
        } else {
            asm_loadLabelIntoRegister(list->slot->label, "$a0");
        }

        asm_jal(ASM_DISPLAY_STRING_FUNCTION_NAME);

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

int doMarkerTest()
{
    char* then = (char*)createNewLabel();
    asm_code_printf("\t%s:\n",then)
    completeOneUnDefineGoto(listInstruction, "");
    completeTrueList(listInstruction,then);

    return RETURN_SUCCESS;
}

int doMarkerElse()
{
    char* then = (char*)createNewLabel();
    asm_code_printf("\t%s:\n",then)
    completeFalseList(listInstruction,then);

    return RETURN_SUCCESS;
}

int doMarkerFi()
{
    char* then = (char*)createNewLabel();
    asm_code_printf("\t%s:\n",then)
    completeFalseList(listInstruction,then);
    completeUnDefineGoto(listInstruction,then);

    return RETURN_SUCCESS;
}

int doMarkerEndInstruction()
{
    addIntoUnDefineGoto(listInstruction,"\tj");
    asm_code_printf("\n")
    return RETURN_SUCCESS;
}

int doMarkerLoop()
{
    asm_code_printf("\n\t# Start of Test block of LOOP\n")
    addIntoUnDefineGoto(listInstruction,"\t");
    asm_code_printf("\n")
    return RETURN_SUCCESS;
}

int doMarkerEndLoop()
{
    char* then = (char*)createNewLabel();
    asm_code_printf("\t%s:\n",then)
    completeFalseList(listInstruction,then);
    completeUnDefineGoto(listInstruction,then);

    asm_code_printf("\n\t# End of Test block of LOOP\n\n")
    return RETURN_SUCCESS;
}

int doMarkerDone()
{
    char* then = (char*)createNewLabel();
    completeUnDefineGoto(listInstruction,then);
    asm_code_printf("\n\tj %s\n",then)
    return RETURN_SUCCESS;
}

int doMarkerForList()
{

}

int addBlock()
{
    // TODO: block type ????
    int returnValue;
    returnValue = addRangeVariable(listRangeVariable, BLOCK_MAIN);
    returnValue += addStructListGoTo(listInstruction);

    return returnValue;
}

int deleteBlock()
{
    int returnValue;
    returnValue = deleteRangeVariable(listRangeVariable);
    returnValue += deleteStructListGoTo(listInstruction);

    return returnValue;
}

MemorySlot doBoolExpression(MemorySlot left, boolExpr_t boolExpr, MemorySlot right)
{
    log_trace("doBoolExpression (int %d)", boolExpr)

    asm_code_printf("\n\t# Start of Test block of ope %d\n\n", boolExpr)

    if((boolExpr != L_AND) && (boolExpr != L_OR)){

        if (left == NULL || right == NULL) {
            log_error("Cant do bool expr on null")
            return NULL;
        }

        if(left->label == NULL)
        {
            asm_readFromStack("$t0", getMipsOffset(left));
            freeMemory(left);
        } else {
            asm_loadLabelIntoRegister(left->label, "$t0");
        }

        if(right->label == NULL)
        {
            asm_readFromStack("$t1", getMipsOffset(right));
            freeMemory(right);
        } else {
            asm_loadLabelIntoRegister(right->label, "$t1");
        }

    }

    if (boolExpr == BOOL_EQ || boolExpr == BOOL_NEQ || boolExpr == BOOL_GT ||
        boolExpr == BOOL_GE || boolExpr == BOOL_LT || boolExpr == BOOL_LE)
    {
        asm_useAtoiFunction("$t0","$t0");
        asm_useAtoiFunction("$t1","$t1");
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
            completeOneUnDefineGoto(listInstruction,block);
            completeTrueList(listInstruction,block);

            block = (char*)createNewLabel();
            completeOneUnDefineGoto(listInstruction,block);
            completeTrueList(listInstruction,block);

            block = (char*)createNewLabel();
            asm_code_printf("\t%s:\n",block)
            addIntoTrueList(listInstruction,"\tj");
            asm_code_printf("\n")

            block = (char*)createNewLabel();
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
            asm_code_printf("\t%s:\n",block)
            completeOneUnDefineGoto(listInstruction,block1);
            completeTrueList(listInstruction,block);
            completeTrueList(listInstruction,block);

            addIntoTrueList(listInstruction,"\tj");
            asm_code_printf("\n")

            block = (char*)createNewLabel();
            block1 = (char*)createNewLabel();
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
    return NULL;
}

MemorySlot doEmptyBoolExpression( boolExpr_t boolExpr, MemorySlot right)
{
    log_trace("doEmptyBoolExpression (int %d)", boolExpr)

    asm_code_printf("\n\t# Start of Test block of ope %d\n", boolExpr)

    if ( right == NULL) {
        log_error("Cant do bool expr on null")
        return NULL;
    }

    if(right->label == NULL)
    {
        asm_readFromStack("$t1", getMipsOffset(right));
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
    }

    VariablePosition pos = searchIdentifierPosition(listRangeVariable, id);

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
        return NULL;
    }

    return iden;
}

int doDeclareStaticArray(char *id, int size)
{
    log_trace("doDeclareStaticArray(%s, %d)", id, size)

    Identifier iden = getIdentifier(id, false, false);

    if(iden != NULL)
    {
        log_error("Array %s was already declared.", id)
        return RETURN_FAILURE;
    }
    asm_code_printf("\t# Start of declaration of table %s : %d\n", id, size)

    // Now create identifier
    iden = getIdentifier(id, true, false);

    // add array size & type of identifier
    iden->arraySize = size;
    iden->type = ARRAY;

    MemorySlot slot = iden->memory;
    if(slot == NULL) return RETURN_FAILURE;

    const char *label = createNewLabel();
    asm_writeStaticArray(label, size);
    if(slot->label == NULL)
    {
        asm_getStackAddress("$t0", getMipsOffset(slot));
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t0");
    }
    asm_loadLabelAddressIntoRegister(label, "$t1");
    asm_code_printf("\tsw $t1, 0($t0)\n")
    free((char*)label);
    asm_code_printf("\t# end of declaration of table %s\n", id)
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
    asm_loadLabelAddressIntoRegister(label, "$t0");
    //asm_allocateOnHeap("$t1", (int)len - 1);
    //asm_useBufferWriteFunction("$t0", "$t1", "$t1");
    if(slot->label == NULL)
    {
        asm_getStackAddress("$t1", getMipsOffset(slot));
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t1");
    }
    asm_code_printf("\tsw $t0, 0($t1)\n")
    free(copy);
    return slot;
}

MemorySlot addWordToMemory(const char *str) {
    MemorySlot slot = reserveMemorySlot();
    const char* label = createNewLabel();
    asm_data_printf("\t%s: .asciiz \"%s\"\n", label, str)
    asm_loadLabelAddressIntoRegister(label, "$t0");
    //asm_allocateOnHeap("$t1", (int)len - 1);
    //asm_useBufferWriteFunction("$t0", "$t1", "$t1");
    if(slot->label == NULL)
    {
        asm_getStackAddress("$t1", getMipsOffset(slot));
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t1");
    }
    asm_code_printf("\tsw $t0, 0($t1)\n")
    return slot;
}

int doArrayRead(char *id, MemorySlot offset)
{
    log_trace("doStringRead(%s)", id)
    Identifier iden = getIdentifier(id, false, false);

    if (iden == NULL)
    {
        log_error("Array %s was not set, cant assign.", id)
        return RETURN_FAILURE;
    }

    if(iden->type != ARRAY)
    {
        log_error("Can't access to a non array variable (%s).", id)
        return RETURN_FAILURE;
    }

    if(offset == NULL) {
        return RETURN_FAILURE;
    }

    // slot -> address of table
    MemorySlot slot = iden->memory;
    // offset -> address of offset
    asm_readFromStack("$t0", getMipsOffset(offset));

    if(offset->label == NULL)
    {
        asm_readFromStack("$t0", getMipsOffset(offset));
        freeMemory(offset);
    } else {
        asm_loadLabelIntoRegister(offset->label, "$t0");
    }

    // check if array out of bounds
    asm_code_printf("\tli $t1, %d\n", iden->arraySize)
    // error management
    asm_code_printf("\tbge $t0, $t1, %s\n", ASM_OUT_OF_BOUNDS_ERROR_FUNCTION_NAME)
    asm_code_printf("\tblt $t0, $zero, %s\n", ASM_OUT_OF_BOUNDS_ERROR_FUNCTION_NAME)

    // load address of table
    if(slot->label == NULL)
    {
        asm_readFromStack("$t1", getMipsOffset(slot));
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

    return RETURN_SUCCESS;
}

MemorySlot doGetVariableAddress(char *id, bool negative, bool isOperandInt)
{
    log_trace("doGetVariableAddress")

    VariablePosition pos = searchIdentifierPosition(listRangeVariable, id);

    if(pos->indexIdentifier == NOTFOUND) {
        log_error("The variable was not found.")
        return NULL;
    }

    MemorySlot slot = getOffsetOfIdentifier(pos->rangePosition->listIdentifier, pos->indexIdentifier);

    // convert to int
    if(isOperandInt)
    {
        if(slot->label == NULL)
        {
            asm_readFromStack("$t0", getMipsOffset(slot));
        } else {
            asm_loadLabelIntoRegister(slot->label, "$t0");
            // set value to a new stack address
            slot = reserveMemorySlot();
        }

        // convert string to int (variables contains numbers as chars)
        asm_useAtoiFunction("$t0", "$t0");

        if(slot->label == NULL)
        {
            asm_getStackAddress("$t1", getMipsOffset(slot));
        } else {
            asm_loadLabelAddressIntoRegister(slot->label, "$t1");
        }

        if(negative) {
            asm_code_printf("\tli $t2, -1\n")
            asm_code_printf("\tmul $t0, $t0, $t2\n")
        }

        asm_code_printf("\tsw $t0, 0($t1)\n")
        return slot;
    }

    // No modification if positive
    if(!negative) return slot;
    if(slot->label != NULL) slot = reserveMemorySlot();

    asm_code_printf("\tli $t1, -1\n")
    asm_code_printf("\tmul $t0, $t0, $t1\n")

    if(slot->label == NULL)
    {
        asm_getStackAddress("$t2", getMipsOffset(slot));
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t2");
    }
    asm_code_printf("\tsw $t0, 0($t2)\n")

    return slot;
}

MemorySlot doGetArrayAddress(char *id, MemorySlot offset, bool negative,
                             bool isOperandInt)
{
    log_trace("doGetArrayAddress(%s)", id)

    VariablePosition pos = searchIdentifierPosition(listRangeVariable, id);

    if(pos->indexIdentifier == NOTFOUND) {
        log_error("The variable was not found.")
        return NULL;
    }

    Identifier iden = pos->rangePosition->listIdentifier->Identifiers[pos->indexIdentifier];

    if(iden->type != ARRAY)
    {
        log_error("Can't access to a non array variable.")
        return NULL;
    }

    asm_code_printf("\t# Start of getting value of array %s\n", id)

    // slot -> address of table
    MemorySlot slot = getOffsetOfIdentifier(pos->rangePosition->listIdentifier, pos->indexIdentifier);
    // offset -> address of offset
    if(offset->label == NULL)
    {
        asm_readFromStack("$t0", getMipsOffset(offset));
    } else {
        asm_loadLabelIntoRegister(offset->label, "$t0");
        // set value to a new stack address
        offset = reserveMemorySlot();
    }

    // check if not array out of bounds
    asm_code_printf("\tli $t1, %d\n", iden->arraySize)
    // error management
    asm_code_printf("\tbge $t0, $t1, %s\n", ASM_OUT_OF_BOUNDS_ERROR_FUNCTION_NAME)
    asm_code_printf("\tblt $t0, $zero, %s\n", ASM_OUT_OF_BOUNDS_ERROR_FUNCTION_NAME)

    // load address of table
    if(slot->label == NULL)
    {
        asm_readFromStack("$t1", getMipsOffset(slot));
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

    if(!isOperandInt)
    {
        if(offset->label == NULL)
        {
            asm_getStackAddress("$t3", getMipsOffset(offset));
        } else {
            asm_loadLabelAddressIntoRegister(offset->label, "$t3");
        }

        // get value -> $t1[$t0]
        asm_code_printf("\tlw $t1, 0($t1)\n")
        asm_code_printf("\tsw $t1, 0($t3)\n")

        asm_code_printf("\t# End of getting value of array: %s\n", id)
        return offset;
    }

    // $t1 = atoi($t1[$t0]])
    asm_useAtoiFunction("$t3", "$t0");

    if(offset->label == NULL)
    {
        asm_getStackAddress("$t1", getMipsOffset(offset));
    } else {
        asm_loadLabelAddressIntoRegister(offset->label, "$t1");
    }

    if(negative) {
        asm_code_printf("\tli $t2, -1\n")
        asm_code_printf("\tmul $t0, $t0, $t2\n")
    }

    asm_code_printf("\tsw $t0, 0($t1)\n")
    asm_code_printf("\t# End of getting value of array: %s\n", id)
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
    MemorySlot slot = getIdentifier((char *) id, true, false)->memory;
    if(slot == NULL) return RETURN_FAILURE;

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
        asm_getStackAddress("$t2", getMipsOffset(slot));
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t2");
    }
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

    if(slot->label == NULL)
    {
        asm_getStackAddress("$t0", getMipsOffset(slot));
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t0");
    }
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
    if(slot->label == NULL)
    {
        asm_getStackAddress("$t0", getMipsOffset(slot));
    } else {
        asm_loadLabelAddressIntoRegister(slot->label, "$t0");
    }
    asm_code_printf("\tlw $t1, 0($t0)\n")
    asm_code_printf("\tli $t2, -1\n")
    asm_code_printf("\tmul $t1, $t1, $t2\n")
    asm_code_printf("\tsw $t1, 0($t0)\n")
    return slot;
}

int doDeclareFunction(Marker mark)
{
    // TODO: handle returns
    // from actual position to start position (mark)
    deleteRangeVariable(listRangeVariable); // delete one block
    asm_loadRegistersFromStack();
    setMemoryCurrentStackOffset(getMemoryCurrentStackOffset() - ASM_VAR_REGISTERS_CACHE_SIZE);
    asm_code_printf("\tjr $ra\n")
    asm_code_printf("\tend_%s:\n", mark->lbl)
    return RETURN_SUCCESS;
}

Marker doFunctionStartMarker(char* id)
{
    Identifier identifier = getIdentifier(id, false, false);
    Marker mark = newMarker();

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
    identifier->type = FUNCTION;

    // creation du nouveau block
    addRangeVariable(listRangeVariable, BLOCK_FUNCTION);

    asm_code_printf("\tj end_%s\n", id)

    mark->code = listInstruction->cursorCode;
    mark->index = listInstruction->cursorCode->numberCode - 1;

    asm_code_printf("\tstart_%s:\n", id) // function name
    asm_writeRegistersToStack(); // 3

    expandMemorySlots( getMemoryCurrentStackOffset() +
            ASM_VAR_REGISTERS_CACHE_COUNT,false
    );

    setMemoryCurrentStackOffset(getMemoryCurrentStackOffset() + ASM_VAR_REGISTERS_CACHE_SIZE);


    return mark;
}

int doFunctionCall(char* id, MemorySlotList list)
{
    Identifier identifier = getIdentifier(id, false, false);

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

    asm_code_printf("\tjal start_%s\n", id)
    free(id);

    return RETURN_SUCCESS;
}