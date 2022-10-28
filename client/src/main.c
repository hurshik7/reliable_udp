

#include "error.h"
#include "option_handler.h"
#include "udp_sender.h"
#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    // open a socket
    opts.fd_out = socket(AF_INET, SOCK_DGRAM, 0); // NOLINT(android-cloexec-socket)
    if(opts.fd_out == -1)
    {
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] open a socket", EXIT_FAILURE);
    }

    // init addr
    result = init_sockaddr(&addr);
    if (result != 0)
    {
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] initiate sockaddr_in", EXIT_FAILURE);
    }

    // bind
    result = bind(opts.fd_out, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
    if(result != 0)
    {
        close(opts.fd_out);
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] bind", EXIT_FAILURE);
    }

    // init proxy_server addr
    result = init_proxy_sockaddr(&to_addr, &opts);
    if (result != 0)
    {
        close(opts.fd_out);
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] initiate proxy server's sockaddr_in", EXIT_FAILURE);
    }

    result = do_client(&opts, &to_addr, &addr);
    if (result != -0)
    {
        close(opts.fd_out);
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] sendto", EXIT_FAILURE);
    }

//
//    printf("wrote %ld\n", nwrote);
//    strncpy(data, argv[3], strlen(argv[3]));
//    write(STDOUT_FILENO, data, nwrote);
//    close(fd);

    return EXIT_SUCCESS;
}
