

#include "error.h"
#include "option_handler.h"
#include "udp_sender.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    int result;
    struct options opts;
    options_init(&opts);
    result = parse_arguments(argc, argv, &opts);
    if (result != 0)
    {
        fprintf(stderr, "Usage: %s -o <proxy server address> [-p port_number]\n", argv[0]);         // NOLINT(cert-err33-c)
        exit(EXIT_FAILURE);                                                                         // NOLINT(concurrency-mt-unsafe)
    }

    struct sockaddr_in addr;
    struct sockaddr_in to_addr;
    struct sockaddr_in from_addr;
    struct timeval tv;
    tv.tv_sec = TIMEOUT_IN_SECOND;
    tv.tv_usec = 0;

    // open a socket
    opts.sock_fd = socket(AF_INET, SOCK_DGRAM, 0);       // NOLINT(android-cloexec-socket)
    if (opts.sock_fd == -1)
    {
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] open a socket", EXIT_FAILURE);
    }
    // set timeout
    setsockopt(opts.sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

    // init addr
    result = init_sockaddr(&addr);
    if (result != 0)
    {
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] initiate sockaddr_in", EXIT_FAILURE);
    }

    // bind
    result = bind(opts.sock_fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
    if (result != 0)
    {
        close(opts.sock_fd);
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] bind", EXIT_FAILURE);
    }

    // init proxy_server addr
    result = init_proxy_sockaddr(&to_addr, &opts);
    if (result != 0)
    {
        close(opts.sock_fd);
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] initiate proxy server's sockaddr_in", EXIT_FAILURE);
    }

    result = do_client(&opts, &to_addr, &from_addr);
    if (result != -0)
    {
        close(opts.sock_fd);
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] sendto", EXIT_FAILURE);
    }

    close(opts.sock_fd);
    return EXIT_SUCCESS;
}
