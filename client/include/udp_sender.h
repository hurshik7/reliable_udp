//
// Created by Shik Hur on 2022-10-28.
//

#ifndef UDP_CLIENT_UDP_SENDER_H
#define UDP_CLIENT_UDP_SENDER_H


#include "rudp_types.h"


#define MAX_DATA_LENGTH (1024)
#define MY_SUCCESS_CODE (0)
#define MY_FAILURE_CODE (1)
#define TIMEOUT_IN_SECOND (2)


struct options;


int init_sockaddr(struct sockaddr_in *addr);
int init_proxy_sockaddr(struct sockaddr_in *proxy_addr, const struct options *opts);
int do_client(const struct options *opts, struct sockaddr_in *proxy_addr, struct sockaddr_in *from_addr);
int send_fin(int current_seq, int sock_fd, struct sockaddr_in *proxy_addr);


#endif //UDP_CLIENT_UDP_SENDER_H

