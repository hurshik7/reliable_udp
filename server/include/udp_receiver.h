//
// Created by Shik Hur on 2022-10-28.
//

#ifndef UDP_SERVER_UDP_RECEIVER_H
#define UDP_SERVER_UDP_RECEIVER_H


#define MY_FAILURE_CODE (1)
#define MY_SUCCESS_CODE (0)


#include "option_handler.h"
#include "rudp_types.h"


int init_sockaddr(struct sockaddr_in *addr, const struct options *opts);
int do_server(const struct options *opts, struct sockaddr_in *proxy_addr, const struct sockaddr_in *addr);

#endif //UDP_SERVER_UDP_RECEIVER_H
