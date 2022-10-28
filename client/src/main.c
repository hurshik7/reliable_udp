

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
    char data[MAX_DATA_LENGTH];
    ssize_t nwrote;

    // open a socket
    opts.fd_out = socket(AF_INET, SOCK_DGRAM, 0); // NOLINT(android-cloexec-socket)
    if(opts.fd_out == -1)
    {
        perror("[FAIL] open a socket");
        exit(EXIT_FAILURE);                       // NOLINT(concurrency-mt-unsafe)
    }

    // init addr
    result = init_sockaddr(&addr);
    if (result != 0)
    {
        perror("[FAIL] initiate sockaddr_in");
        exit(EXIT_FAILURE);
    }

    // bind
    result = bind(opts.fd_out, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
    if(result != 0)
    {
        close(opts.fd_out);
        perror("[FAIL] bind");
        exit(EXIT_FAILURE);  // NOLINT(concurrency-mt-unsafe)
    }

    // init proxy_server addr
    result = init_proxy_sockaddr(&to_addr, &opts);
    if (result != 0)
    {
        perror("[FAIL] initiate proxy server's sockaddr_in");
        exit(EXIT_FAILURE);
    }

    result = do_client(&opts, &to_addr, &addr);
    if (result != -0)
    {
        close(opts.fd_out);
        perror("[FAIL] sendto");
        exit(EXIT_FAILURE);  // NOLINT(concurrency-mt-unsafe)
    }

//
//    printf("wrote %ld\n", nwrote);
//    strncpy(data, argv[3], strlen(argv[3]));
//    write(STDOUT_FILENO, data, nwrote);
//    close(fd);

    return EXIT_SUCCESS;
}
