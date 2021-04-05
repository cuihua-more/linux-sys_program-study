#include "get_num.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>

static void 
gnFail(const char *fname, const char *msg, const char *arg, const char *name)
{
    fprintf(stderr, "%s error", fname);
    
    if (!name) {
        fprintf(stderr, " (in %s)", name);
    }

    fprintf(stderr, ": %s \n", msg);

    if (arg != NULL && *arg != '\0') {
        fprintf(stderr, "   offending text: %s\n", arg);
    }

    exit(EXIT_FAILURE);
}

static long 
getnum(const char *fname, const char *arg, int flags, const char *name)
{
    int base;
    long res;
    char *endptr;

    if (arg == NULL || *arg == '\0')
    {
        gnFail(fname, "null or emtry string", arg, name);
    }

    base = (flags & GN_ANY_BASE) ? 0 : (flags & GN_BASE_8) ? 8
                                   : (flags & GN_BASE_16)  ? 16
                                                           : 10;

    errno = 0;
    res = strtol(arg, &endptr, base);

    if (errno != 0) {
        gnFail(fname, "strtol() failed", arg, name);
    }

    if (*endptr != '\0') {
        gnFail(fname, "nonmumeric characters", arg, name);
    }

    if ((flags & GN_NONNEG) && res < 0) {
        gnFail(fname, "negative value not allowed", arg, name);
    }

    if ((flags & GN_GT_0) && res <= 0) {
        gnFail(fname, "value must be > 0", arg, name);
    }

    return res;
}

long getLong(const char *arg, int flags, const char *name)
{
    return getnum("getLong", arg, flags, name);
}

int getInt(const char *arg, int flags, const char *name)
{
    int res;
    
    res = getnum("getInt", arg, flags, name);
    if (res > INT_MAX || res < INT_MIN) {
        gnFail("getInt", "integer out of range", arg, name);
    }

    return (int)res;
}