#include "conversion.h"
#include "error.h"
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>


in_port_t parse_port(const char *buff, int radix)
{
    char *end;
    long sl;
    in_port_t port;
    const char *msg;

    errno = 0;
    sl = strtol(buff, &end, radix);

    if(end == buff)
    {
        msg = "not a decimal number";
    }
    else if(*end != '\0')
    {
        msg = "%s: extra characters at end of input";
    }
    else if((sl == LONG_MIN || sl == LONG_MAX) && ERANGE == errno)
    {
        msg = "out of range of type long";
    }
    else if(sl > UINT16_MAX)
    {
        msg = "greater than UINT16_MAX";
    }
    else if(sl < 0)
    {
        msg = "less than 0";
    }
    else
    {
        msg = NULL;
    }

    if(msg)
    {
        fatal_message(__FILE__, __func__ , __LINE__, msg, 2);
    }

    port = (in_port_t)sl;

    return port;
}


size_t parse_size_t(const char *buff, int radix)
{
    char *end;
    uintmax_t max;
    size_t ret_val;
    const char *msg;

    errno = 0;
    max = strtoumax(buff, &end, radix);

    if(end == buff)
    {
        msg = "not a decimal number";
    }
    else if(*end != '\0')
    {
        msg = "%s: extra characters at end of input";
    }
    else if((max == UINTMAX_MAX || max == 0) && ERANGE == errno)
    {
        msg = "out of range of type uintmax_t";
    }
    else if(max > SIZE_MAX)
    {
        msg = "greater than SIZE_MAX";
    }
    else
    {
        msg = NULL;
    }

    if(msg)
    {
        fatal_message(__FILE__, __func__ , __LINE__, msg, 2);
    }

    ret_val = (size_t)max;

    return ret_val;
}
