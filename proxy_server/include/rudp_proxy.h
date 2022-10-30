//
// Created by Shik Hur on 2022-10-29.
//

#ifndef UDP_PROXY_RUDP_PROXY_H
#define UDP_PROXY_RUDP_PROXY_H


#include "rudp_types.h"
#include "option_handler.h"


#define MAX_DATA_LENGTH (1024)
#define MY_SUCCESS_CODE (0)
#define MY_FAILURE_CODE (1)
#define THREAD_FAIL (2)


typedef struct my_thread_arg
{
    struct sockaddr_in *server_addr;
    struct sockaddr_in *client_addr;
    struct options *opts;
} my_thread_arg_t;


int init_sockaddr(struct sockaddr_in *addr, const struct options *opts);
int init_server_sockaddr(struct sockaddr_in *server_addr, const struct options *opts);
int do_proxy(struct options *opts, struct sockaddr_in *server_addr, struct sockaddr_in *client_addr);

_Noreturn void *deliver_packet_from_client_to_server(void *argument);

_Noreturn void *deliver_res_from_server_to_client(void *argument);


#endif //UDP_PROXY_RUDP_PROXY_H

