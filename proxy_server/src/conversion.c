#include "conversion.h"
#include "error.h"
#include <errno.h>
#include <inttypes.h>
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

uint8_t parse_uint8(const char *buff, int radix, int *is_error_out)
{
    char *end;
    uintmax_t max;
    uint8_t ret_val;
    const char *msg;

    errno = 0;
    max = strtoumax(buff, &end, radix);

    if (end == buff)
    {
        msg = "not a decimal number";
    }
    else if (*end != '\0')
    {
        msg = "extra characters at end of input";
    }
    else if ((max == UINTMAX_MAX || max == 0) && ERANGE == errno)
    {
        msg = "out of range of type uintmax_t";
    }
    else if (max >= 101)                    // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    {
        msg = "out of range [0 - 100]";
    }
    else
    {
        msg = NULL;
    }

    if (msg)
    {
        fprintf(stderr, "%s\n", msg);       // NOLINT(cert-err33-c)
        *is_error_out = EXIT_FAILURE;
        return 101;                         // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    }

    ret_val = (uint8_t)max;

    return ret_val;
}
