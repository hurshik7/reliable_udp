

#include "error.h"
#include "udp_receiver.h"
#include <arpa/inet.h>
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
        fprintf(stderr, "Usage: %s -i <ip address> [-p port_number]\n", argv[0]);         // NOLINT(cert-err33-c)
        exit(EXIT_FAILURE);                                                               // NOLINT(concurrency-mt-unsafe)
    }


    struct sockaddr_in addr;
    struct sockaddr_in from_addr;
    int option;

    // create a socket
    opts.fd_in = socket(AF_INET, SOCK_DGRAM, 0);
    if(opts.fd_in == -1)
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
    setsockopt(opts.fd_in, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    // bind
    result = bind(opts.fd_in, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    if(result == -1)
    {
        close(opts.fd_in);
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] bind", EXIT_FAILURE);
    }

    result = do_server(&opts, &from_addr, &addr);
    if (result == MY_FAILURE_CODE)
    {
        close(opts.fd_in);
        fatal_message(__FILE__, __FUNCTION__, __LINE__, "[FAIL] recevfrom", EXIT_FAILURE);
    }


//
//    printf("recvfrom: %s\n", inet_ntoa(from_addr.sin_addr));
//    //printf("read %ld\n", nread);
//    //write(STDOUT_FILENO, data, nread);

    close(opts.fd_in);
    return EXIT_SUCCESS;
}
