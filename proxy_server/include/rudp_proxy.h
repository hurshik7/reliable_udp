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


int init_sockaddr(struct sockaddr_in *addr);
int init_server_sockaddr(struct sockaddr_in *server_addr, const struct options *opts);
int do_proxy();


#endif //UDP_PROXY_RUDP_PROXY_H

