#include "conversion.h"
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


in_port_t parse_port(const char *buff, int radix, int *is_error_out)
{
    char *end;
    long sl;
    in_port_t port;
    const char *msg;

    errno = 0;
    sl = strtol(buff, &end, radix);

    if (end == buff)
    {
        msg = "[ERROR: parsing port number] not a decimal number";
    }
    else if (*end != '\0')
    {
        msg = "[ERROR: parsing port number] extra characters at end of input";
    }
    else if ((sl == LONG_MIN || sl == LONG_MAX) && ERANGE == errno)
    {
        msg = "[ERROR: parsing port number] out of range of type long";
    }
    else if (sl > UINT16_MAX)
    {
        msg = "[ERROR: parsing port number] greater than UINT16_MAX";
    }
    else if (sl < 0)
    {
        msg = "[ERROR: parsing port number] less than 0";
    }
    else
    {
        msg = NULL;
    }

    if (msg)
    {
        fprintf(stderr, "%s\n", msg);       // NOLINT(cert-err33-c)
        *is_error_out = EXIT_FAILURE;
        return 0;
    }

    port = (in_port_t)sl;

    return port;
}

