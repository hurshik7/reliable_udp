//
// Created by Shik Hur on 2022-10-29.
//

#include "rudp_proxy.h"


int init_sockaddr(struct sockaddr_in *addr)
{
    addr->sin_family = AF_INET;
    addr->sin_port = 0;
    addr->sin_addr.s_addr = htonl(INADDR_ANY);

    if(addr->sin_addr.s_addr == (in_addr_t) -1) // NOLINT(clang-analyzer-core.UndefinedBinaryOperatorResult)
    {
        return MY_FAILURE_CODE;
    }
    return MY_SUCCESS_CODE;
}

int init_server_sockaddr(struct sockaddr_in *server_addr, const struct options *opts)
{
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(opts->port_in);
    server_addr->sin_addr.s_addr = inet_addr(opts->ip_in);

    if (server_addr->sin_addr.s_addr == (in_addr_t) -1)
    {
        return MY_FAILURE_CODE;
    }
    return MY_SUCCESS_CODE;
}

int do_proxy()
{
    // create two threads
    // one is for getting a rudp packet from client and send it to server randomly

    // the other one is for getting a rudp response packet from server and send it to client randomly

    return MY_SUCCESS_CODE;
}

