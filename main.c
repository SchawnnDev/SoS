#include <stdio.h>
#include <argtable3.h>
#include <log.h>
#include "parser.h"

#include "listRangeVar.h"

/* global arg_xxx structs */
struct arg_lit *help, *version, *tos;
struct arg_int *verb, *level;
struct arg_file *o;
struct arg_end *end;

/* global values */
FILE* outputFile;
ListRangeVariable listRangeVariable;
ListTmp listTmp;

int initStruct()
{
    listRangeVariable = initListRangeVariable();
    listTmp = initListTmp();
}

int handle_args(int argc, char **argv)
{
    /* the global arg_xxx structs are initialised within the argtable */
    void *argtable[] = {
            help    = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
            version = arg_litn(NULL, "version", 0, 1, "display team members"),
            tos     = arg_litn("tos", "tos", 0, 1, "display table of symbols"),
            level   = arg_intn(NULL, "level", "<n>", 0, 1, "foo value"),
            verb    = arg_intn("v", "verbose", "<n>",0, 1, "verbose output"),
            o       = arg_filen("o", NULL, "<file>", 0, 1, "output file"),
            end     = arg_end(20),
    };

    int exitcode = 0;
    const char* progname = "sos";

    int nerrors;
    nerrors = arg_parse(argc, argv, argtable);

    switch (verb->count)
    {
        case 0:
            log_set_level(LOG_TRACE);
            break;
        case 1:
            log_set_level(LOG_DEBUG);
            break;
        case 2:
            log_set_level(LOG_INFO);
            break;
        case 3:
            log_set_level(LOG_WARN);
            break;
        case 4:
            log_set_level(LOG_ERROR);
            break;
        case 5:
            log_set_level(LOG_FATAL);
            break;
        default:
            // If no verbose given, set log to quiet by default
            log_set_quiet(true);
            break;
    }
    log_debug("Verbose was set to level %d.", verb->count)

    if(version->count > 0)
    {
        log_info("Version: version->count > 0")
        initStruct();
        yyparse();
        goto exit;
    }

    /* special case: '--help' takes precedence over error reporting */
    if (help->count > 0)
    {
        printf("Usage: %s", progname);
        arg_print_syntax(stdout, argtable, "\n");
        printf("Demonstrate command-line parsing in argtable3.\n\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\n");
        exitcode = 0;
        goto exit;
    }

    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0)
    {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout, end, progname);
        printf("Try '%s --help' for more information.\n", progname);
        exitcode = 1;
        goto exit;
    }

    exit:
    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return exitcode;
}


int main(int argc, char **argv) {
    printf("Hello, World!\n");
    return handle_args(argc, argv);
}


