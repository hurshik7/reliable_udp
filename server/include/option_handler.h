//
// Created by Shik Hur on 2022-10-28.
//

#ifndef UDP_SERVER_OPTION_HANDLER_H
#define UDP_SERVER_OPTION_HANDLER_H


#include <arpa/inet.h>


#define DEFAULT_PORT (5050)          // NOLINT(modernize-macro-to-enum)
#define MAX_ARG_COUNT (5)            // NOLINT(modernize-macro-to-enum)
#define FAIL (1)                     // NOLINT(modernize-macro-to-enum)
#define FAIL_WITH_MSG (2)            // NOLINT(modernize-macro-to-enum)


/**
 * Struct options contains ip address and port number of this server, and a file descriptor for a socket.
 */
struct options
{
    char *ip_in;
    in_port_t port_in;
    int sock_fd;
};

/**
 * Initiate options.
 * @param opts struct options
 */
void options_init(struct options *opts);

/**
 * Parse command line arguments and store the values in options struct.
 * @param argc an integer represents the number of command line arguments
 * @param argv an array of string contains command line arguments as string
 * @param opts struct options
 * @return an integer represents the status code
 */
int parse_arguments(int argc, char *argv[], struct options *opts);


#endif //UDP_SERVER_OPTION_HANDLER_H

