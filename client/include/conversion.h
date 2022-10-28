#ifndef V4_CONVERSION_H
#define V4_CONVERSION_H


#include <netinet/in.h>


/**
 * Parse string to in_port_t.
 * @param buff a string represents port number in string
 * @param radix an integer represents base
 * @param is_error_out a pointer represents if there is an error in this function or not
 * @return port number in in_port_t type
 */
in_port_t parse_port(const char *buff, int radix, int *is_error_out);


#endif //V4_CONVERSION_H
