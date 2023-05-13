#include "common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void die(const char *msg)
{
    int err = errno;                                            // header: errno (set in case any error occurs)
    fprintf(stderr, "[%d] %s (%s)\n", err, msg, strerror(err)); // header: stdio
    abort();
}

void msg(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
}
