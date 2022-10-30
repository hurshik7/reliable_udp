

#include "error.h"
#include "option_handler.h"
#include "rudp_proxy.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>


/**
 * Runs proxy server.
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
    if (result != 0 || opts.ip_in == NULL || opts.ip_out == NULL)
    {
        fprintf(stderr, "Usage: %s -i <ip address> [-p port_number] -o <server ip address> [-P server port number] [-r chance of dropping a packet to server] [-R chance of dropping an ACK to client]\n", argv[0]);         // NOLINT(cert-err33-c)
        exit(EXIT_FAILURE);                                                                                                            // NOLINT(concurrency-mt-unsafe)
    }

    struct sockaddr_in addr;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    // open a socket
    opts.in_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);       // NOLINT(android-cloexec-socket)
    if (opts.in_sock_fd == -1)
    {
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] open a socket", EXIT_FAILURE);
    }
    opts.out_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);      // NOLINT(android-cloexec-socket)
    if (opts.out_sock_fd == -1)
    {
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] open a socket", EXIT_FAILURE);
    }

    // init addr
    result = init_sockaddr(&addr, &opts);
    if (result != 0)
    {
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] initiate sockaddr_in", EXIT_FAILURE);
    }

    // bind
    result = bind(opts.in_sock_fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
    if (result != 0)
    {
        close(opts.in_sock_fd);
        close(opts.out_sock_fd);
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] bind", EXIT_FAILURE);
    }
    addr.sin_port = 0; // get an available port to communicate with server
    result = bind(opts.out_sock_fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
    if (result != 0)
    {
        close(opts.in_sock_fd);
        close(opts.out_sock_fd);
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] bind", EXIT_FAILURE);
    }

    //init server addr
    result = init_server_sockaddr(&server_addr, &opts);
    if (result != 0)
    {
        close(opts.in_sock_fd);
        close(opts.out_sock_fd);
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] initiate server's sockaddr_in", EXIT_FAILURE);
    }

    result = do_proxy(&opts, &server_addr, &client_addr);
    if (result == THREAD_FAIL)
    {
        close(opts.in_sock_fd);
        close(opts.out_sock_fd);
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] create thread", EXIT_FAILURE);
    }
    else if (result != 0)
    {
        close(opts.in_sock_fd);
        close(opts.out_sock_fd);
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] do_proxy", EXIT_FAILURE);
    }

    close(opts.in_sock_fd);
    close(opts.out_sock_fd);
    return EXIT_SUCCESS;
}

