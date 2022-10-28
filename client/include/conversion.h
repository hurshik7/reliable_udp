#ifndef V4_CONVERSION_H
#define V4_CONVERSION_H


#include <netinet/in.h>


/**
 * Parse string to in_port_t.
 * @param buff a string represents port number in string
 * @param radix an integer represents base
 * @return port number in in_port_t type
 */
in_port_t parse_port(const char *buff, int radix);

/**
 * Parse string to size_t
 * @param buff a number in string
 * @param radix an integer represents base
 * @return size_t type integer (unsigned integer)
 */
size_t parse_size_t(const char *buff, int radix);


#endif //V4_CONVERSION_H
