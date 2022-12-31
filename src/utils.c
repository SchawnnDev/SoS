#include "utils.h"

void raler(int syserr, const char *msg, ...) {
    va_list ap;

    va_start (ap, msg);
    vfprintf(stderr, msg, ap);
    fprintf(stderr, "\n");
    va_end (ap);

    if (syserr == 1)
        perror("");

    exit(EXIT_FAILURE);
}

int countWithoutBackslash(char *str) {
    int len = (int) strlen(str);
    if (len == 0) return 0;
    int size = len;
    for (int j = 0; j < size; ++j) {
        if (str[j] == '\\' && j + 1 < size) {
            len -= 1;
        }
    }
    return len <= 0 ? 0 : len;
}
