#include <regex.h>
#include <limits.h>
#include "compilation.h"
#include "parser.h"
#include "lexer.h"

int errorType(const char *msg, ...) {

    return (EXIT_FAILURE);
}

/*!
 * \fn void initStruct()
 * \brief Fonction qui initialise les structures
*/
void initStruct() {
    log_trace("Started initStruct")
    listRangeVariable = initListRangeVariable();
    listIdentifierOrder = initListIdentifierOrder();
    listTmp = initListTmp();
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
int compile(FILE *inputFile, FILE *outputFile) {
    log_trace("Started compile (%d, %d)", inputFile, outputFile)
    initStruct();
    outFile = outputFile == NULL ? stdout : outputFile;
    yyin = inputFile;
    return yyparse();
}

/*!
 * \fn void assign()
 * \brief Fonction qui ajoute l'identifiant à la liste et transmet les données qui le compose
*/
void assign() {
    log_trace("assign (void)")
    addIdentifier(listRangeVariable, listIdentifierOrder->cursor->name);
    setValuesFromListTmp(listRangeVariable,listIdentifierOrder->cursor->name,listTmp);
    deleteListTmp(listTmp);
}

void assignArray() {

}

/*!
 * \fn void addIdOrder(char * name)
 * \brief Fonction qui ajoute l'identifiant à la liste des ordes d'appartion des identificateurs
 *
 * \param name : char*, le nom de l'identificateur
*/
void addIdOrder(char * name)
{
    addIdentifierOrder(listIdentifierOrder,name);
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
    setTypeIdentifierOrder(listIdentifierOrder,type);
}

/*!
 * \fn void addTmpValuesListTmp
 * \brief Fonction qui ajoute une structure de valeur temporaire
*/
void addTmpValuesListTmp() {
    addListTmp( listTmp, initTmpValues(listTmp->cursor) );
}

/*!
 * \fn void setTypeOrder(int type)
 * \brief Fonction qui remplie la liste temporaire
 *
 * \param value : char*, le contenue
*/
void addValueIntoListTmp(char *value) {
    addIntoListTmp(listTmp, value);
}

int checkRegex(const char *pattern, const char *string) {
    log_trace("checkRegex(%s, %s)", pattern, string)
    //char *pattern = "^[a-zA-Z_][a-zA-Z0-9_]*";
    //char *string = "hello";
    regex_t regex;
    int ret;

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        log_error("Error compiling regular expression")
        return -1;
    }

    ret = regexec(&regex, string, 0, NULL, 0);
    if (ret == 0) {
        log_trace("String matches regular expression");
        regfree(&regex);
        return 1;
    }

    if (ret == REG_NOMATCH) {
        log_error("String does not match regular expression");
        regfree(&regex);
        return 0;
    }

    log_error("Error executing regular expression");
    return -1;
}

int checkWordIsId(const char *word) {
    return checkRegex("^[a-zA-Z_][a-zA-Z0-9_]*", word);
}

int checkWordIsInt(const char *word) {
    return checkRegex("^[+-]?[0-9]+", word);
}

int parseInt32(const char *word) {
    char *endptr;
    long int parsed = strtol(word, &endptr, 10);

    if (endptr == word || *endptr != '\0') {
        log_error("Invalid number for parseInt32(%s)", word);
        return -1;
    }

    if (parsed < INT_MIN || parsed > INT_MAX) {
        // The number is not within the range of a 32-bit integer
        log_error("Number is not 32-bit for parseInt32(%s)", word);
        return -1;
    }

    return (int) parsed;
}

/**
*  MIPS CODE GENERATION
*/

int asm_syscall(syscall_t type) {
    log_trace("asm_syscall of type %s", stringFromSyscall(type))
    asm_fprintf("li $v0, %d\n", type)
    asm_fprintf("syscall\n")
    return RETURN_SUCCESS;
}
