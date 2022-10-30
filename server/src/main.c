

#include "error.h"
#include "udp_receiver.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>


/**
 * Runs rudp server.
 * @param argc the number of command line arguments
 * @param argv an array of string represents command line arguments
 * @return an integer represents exit code
 */
int main(int argc, char *argv[])
{
    int result;
    struct options opts;
    options_init(&opts);
    result = parse_arguments(argc, argv, &opts);
    if (result != 0)
    {
        fprintf(stderr, "Usage: %s -i <ip address> [-p port_number]\n", argv[0]);         // NOLINT(cert-err33-c)
        exit(EXIT_FAILURE);                                                               // NOLINT(concurrency-mt-unsafe)
    }

    struct sockaddr_in addr;
    struct sockaddr_in from_addr;
    int option;

    // create a socket
    opts.sock_fd = socket(AF_INET, SOCK_DGRAM, 0);                                           // NOLINT(android-cloexec-socket)
    if(opts.sock_fd == -1)
    {
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] socket", EXIT_FAILURE);
    }

    // init addr
    result = init_sockaddr(&addr, &opts);
    if (result != 0)
    {
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] initiate sockaddr_in", EXIT_FAILURE);
    }

    // set up the socket
    option = 1;
    setsockopt(opts.sock_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    // bind
    result = bind(opts.sock_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    if(result == -1)
    {
        close(opts.sock_fd);
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] bind", EXIT_FAILURE);
    }

    result = do_server(&opts, &from_addr);
    if (result == MY_FAILURE_CODE)
    {
        close(opts.sock_fd);
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] recevfrom", EXIT_FAILURE);
    }
    else if (result == OPEN_SOCKET_FAILURE_CODE)
    {
        close(opts.sock_fd);
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] open socket to response a packet", EXIT_FAILURE);
    }

    close(opts.sock_fd);
    return EXIT_SUCCESS;
}

