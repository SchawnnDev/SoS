#include <regex.h>
#include "compilation.h"
#include "parser.h"
#include "lexer.h"

int errorType (const char *msg, ...)
{

    return (EXIT_FAILURE);
}

void initStruct()
{
    log_trace("Started initStruct")
    listRangeVariable = initListRangeVariable();
    listTmp = initListTmp();
}

int compile(FILE *inputFile, FILE *outputFile)
{
    log_trace("Started compile (%d, %d)", inputFile, outputFile)
    initStruct();
    outFile = outputFile == NULL ? stdout : outputFile;
    yyin = inputFile;
    return yyparse();
}

void AddIdentifier(char *name)
{
    addIdentifier(listRangeVariable, name);
}

void AddIntoListTmp(char *value)
{
    addIntoListTmp(listTmp, value);
}

void SetValuesFromListTmp(char *name)
{
    addIdentifier(listRangeVariable, name);
    setValuesFromListTmp(listRangeVariable, name, listTmp);
    cleanListTmp(listTmp);
    listTmp = initListTmp();
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
        log_error("String matches regular expression");
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

int checkWordIsId(const char* word)
{
    return checkRegex("^[a-zA-Z_][a-zA-Z0-9_]*", word);
}

int checkWordIsInt(const char* word)
{
    return checkRegex("[+-]?\\d+", word);
}