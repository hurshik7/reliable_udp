

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
    if (result != 0) {
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
//
//    addr.sin_family = AF_INET;
//    addr.sin_port = 0;
//    addr.sin_addr.s_addr = inet_addr(argv[1]);
//
//    if(to_addr.sin_addr.s_addr == (in_addr_t) -1) // NOLINT(clang-analyzer-core.UndefinedBinaryOperatorResult)
//    {
//        perror("inet_addr");
//        exit(EXIT_FAILURE); // NOLINT(concurrency-mt-unsafe)
//    }
//
//    result = bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
//
//    if(result == -1)
//    {
//        close(fd);
//        perror("bind");
//        exit(EXIT_FAILURE);  // NOLINT(concurrency-mt-unsafe)
//    }
//
//    to_addr.sin_family = AF_INET;
//    to_addr.sin_port = htons(DEFAULT_PORT);
//    to_addr.sin_addr.s_addr = inet_addr(argv[2]);
//
//    if(to_addr.sin_addr.s_addr ==  (in_addr_t)-1)
//    {
//        perror("inet_addr");
//        exit(EXIT_FAILURE); // NOLINT(concurrency-mt-unsafe)
//    }
//
//    nwrote = sendto(fd, argv[3], strlen(argv[3]), 0, (struct sockaddr *)&to_addr, sizeof(to_addr));
//
//    if(nwrote == -1)
//    {
//        close(fd);
//        perror("recvfrom");
//        exit(EXIT_FAILURE);  // NOLINT(concurrency-mt-unsafe)
//    }
//
//    printf("wrote %ld\n", nwrote);
//    strncpy(data, argv[3], strlen(argv[3]));
//    write(STDOUT_FILENO, data, nwrote);
//    close(fd);
//
    return EXIT_SUCCESS;
}
