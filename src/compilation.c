#include <regex.h>
#include <limits.h>
#include "compilation.h"
#include "parser.h"
#include "lexer.h"
#include "asm.h"
#include "labels.h"

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
    int result = yyparse();
    if (result != RETURN_SUCCESS) return result;
    return writeToFile(listInstruction,
                       outputFile == NULL ? stdout : outputFile);
}

/*!
 * \fn void assign()
 * \brief Fonction qui ajoute l'identifiant à la liste et transmet les données qui le compose
*/
void assign()
{
    log_trace("assign (void)")
    addIdentifier(listRangeVariable, listIdentifierOrder->cursor->name);
    setValuesFromListTmp(listRangeVariable, listIdentifierOrder->cursor->name,
                         listTmp);
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
    CHECKPOINTER(listTmp);
    CHECKPOINTER(listTmp->cursor);
    int values = listTmp->cursor->numberValues;

    for (int i = 0; i < values; ++i)
    {
        char *val = listTmp->cursor->values[i];
        // CHECKPOINTER(strcat(finalStr, val));
        if (listTmp->cursor->types[i] == TYPE_STACK)
        {
            // print_string
            continue;
        }


        asm_code_printf("la $a0, %s", val)
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

    int reg1 = atoi(listTmp->cursor->values[listTmp->cursor->numberValues - 2]);
    int reg2 = atoi(listTmp->cursor->values[listTmp->cursor->numberValues - 1]);
    deleteListTmp(listTmp);
    addListTmp(listTmp, initTmpValues(listTmp->cursor));

    asm_code_printf("li $t1, %d", reg1)
    asm_code_printf("li $t2, %d", reg2)

    switch (currentBooleanExpression)
    {
        case BOOL_EQ:
            break;
        case BOOL_NEQ:
            break;
        case BOOL_GT:
            asm_code_printf("li gteeeetetetete, %d", reg2)
            break;
        case BOOL_GE:
            break;
        case BOOL_LT:
            break;
        case BOOL_LE:
            break;
        case L_AND:
            break;
        case L_OR:
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
        perror("doDeclareStaticArray : you must have one indentifier.");
        return RETURN_FAILURE;
    }

    if (listTmp->cursor->numberValues == 0)
    {
        log_error("numberValues : %d", listTmp->cursor->numberValues)
        perror("doDeclareStaticArray : you must have one value.");
        return RETURN_FAILURE;
    }

    const int size = atoi(listTmp->cursor->values[0]);

    if(size <= 0)
    {
        log_error("doDeclareStaticArray: size should be greater than 0 (actual: %d)", size)
        return RETURN_FAILURE;
    }

    char *id = idToLabel(listIdentifierOrder->cursor->name);

    if (asm_writeStaticArray(id, size) == RETURN_FAILURE)
        return RETURN_FAILURE;

    free(id);
    deleteListTmp(listTmp);
    addListTmp(listTmp, initTmpValues(listTmp->cursor));

    return RETURN_SUCCESS;
}

int writeQuotedString(const char *str)
{
    const char *label = createNewLabel();
    int result = asm_writeAsciiz(label, str);

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
    int result = asm_writeAsciiz(label, copy);

    // Don't forget to add label to tmp list
    if (result == RETURN_SUCCESS)
        addValueIntoListTmp(label);

    free(copy);
    return result;
}