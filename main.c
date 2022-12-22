#include <stdio.h>
#include <argtable3.h>
#include <log.h>
#include <errno.h>
#include "parser.h"

#include "listRangeVar.h"
#include "lexer.h"

/* global arg_xxx structs */
struct arg_file *file;
struct arg_lit *help, *version, *tos;
struct arg_int *verb;
struct arg_file *o;
struct arg_end *end;

/* global values */

int compile(FILE *inputFile, FILE *outputFile)
{
    log_trace("Started compile (%d, %d)", inputFile, outputFile)
    if (initStruct() != 0) return -1;
    yyin = inputFile;
    return yyparse();
}

int handle_args(int argc, char **argv)
{
    /* the global arg_xxx structs are initialised within the argtable */
    void *argtable[] = {
            file = arg_filen(NULL, NULL, "<file>", 1, 1, "input files"),
            help = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
            version = arg_litn(NULL, "version", 0, 1, "display team members"),
            tos = arg_litn(NULL, "tos", 0, 1, "display table of symbols"),
            verb = arg_intn("v", "verbose", "<n>", 0, 1, "verbose output"),
            o = arg_filen("o", NULL, "<file>", 0, 1, "output file"),
            end = arg_end(20),
    };

    int exitcode = 0;
    const char *progname = "sos";

    int nerrors;
    nerrors = arg_parse(argc, argv, argtable);

    if (verb->count > 0)
    {
        switch (*verb->ival)
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

        log_debug("Verbose was set to level %d.", *verb->ival)

    } else {
        log_set_quiet(true);
    }

    /* special case: '--help' takes precedence over error reporting */
    if (help->count > 0)
    {
        printf("Usage: %s", progname);
        arg_print_syntax(stdout, argtable, "\n");
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

    if (version->count > 0)
    {
        printf("Version: Quentin, Axel, Matthieu, Paul\n");
        goto exit;
    }

    if (tos->count > 0)
    {
        printf("Table of symbols...\n");
        goto exit;
    }

    FILE *inputFile = NULL;
    FILE *outputFile = NULL;

    if (file->count > 0)
    {
        const char *fileName = file->filename[0];
        log_debug("File was specified (%s)", fileName)
        inputFile = fopen(fileName, "r");
        if (inputFile == NULL)
        {
            log_error("Cannot open file %s (%s)\n", fileName, strerror(errno))
            goto exit;
        } else
        {
            log_debug("File was loaded (%s)", fileName)
        }

    }

    if (o->count > 0)
    {
        const char *fileName = o->filename[0];
        log_debug("Output file was specified (%s)", fileName)
        inputFile = fopen(fileName, "w");
        if (inputFile == NULL)
        {
            log_error(
                    "Cannot open file %s (%s). Defaulting to output to stdout.",
                    fileName, strerror(errno))
        } else
        {
            log_debug("File was loaded (%s)", fileName)
        }
    } else
    {
        log_debug("No output file : defaulting output to stdout")
    }

    if ((exitcode = compile(inputFile, outputFile)) != 0)
    {
        printf("An error occured.\n");
    }

    if (inputFile != NULL)
        fclose(inputFile);

    if (outputFile != NULL)
        fclose(outputFile);

    exit:
    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return exitcode;
}


int main(int argc, char **argv)
{
    printf("Hello, World!\n");
    return handle_args(argc, argv);
}


