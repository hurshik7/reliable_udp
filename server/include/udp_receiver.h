//
// Created by Shik Hur on 2022-10-28.
//

#ifndef UDP_SERVER_UDP_RECEIVER_H
#define UDP_SERVER_UDP_RECEIVER_H


#define MY_FAILURE_CODE (1)
#define OPEN_SOCKET_FAILURE_CODE (2)
#define MY_SUCCESS_CODE (0)
#define MAX_IP_ADDRESS_LENGTH (16)


#include "option_handler.h"
#include "rudp_types.h"


int init_sockaddr(struct sockaddr_in *addr, const struct options *opts);
int do_server(struct options *opts, struct sockaddr_in *proxy_addr, const struct sockaddr_in *addr);
void deserialize_packet(rudp_packet_t *packet);
int open_socket_for_response(struct options *opts, struct sockaddr_in *to_addr);


#endif //UDP_SERVER_UDP_RECEIVER_H
