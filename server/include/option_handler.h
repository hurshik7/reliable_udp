//
// Created by Shik Hur on 2022-10-28.
//

#ifndef UDP_CLIENT_OPTION_HANDLER_H
#define UDP_CLIENT_OPTION_HANDLER_H


#include <arpa/inet.h>


#define DEFAULT_PORT (5000)          // NOLINT(modernize-macro-to-enum)
#define MAX_ARG_COUNT (5)            // NOLINT(modernize-macro-to-enum)
#define FAIL (1)                     // NOLINT(modernize-macro-to-enum)
#define FAIL_WITH_MSG (2)            // NOLINT(modernize-macro-to-enum)


struct options
{
    char *ip_in;
    char *ip_out;
    in_port_t port_in;
    in_port_t port_out;
    int fd_in;
    int fd_out;
};

void options_init(struct options *opts);
int parse_arguments(int argc, char *argv[], struct options *opts);


#endif //UDP_CLIENT_OPTION_HANDLER_H
