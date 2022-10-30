//
// Created by Shik Hur on 2022-10-28.
//

#ifndef UDP_CLIENT_UDP_SENDER_H
#define UDP_CLIENT_UDP_SENDER_H


#include "rudp_types.h"
#include "option_handler.h"


#define MAX_DATA_LENGTH (1024)
#define MY_SUCCESS_CODE (0)
#define MY_FAILURE_CODE (1)
#define TIMEOUT_IN_SECOND (2)


/**
 * Set an ip address and port number to be used to a socket.
 * @param addr a sockaddr_in pointer which will contains the address of the host
 * @return an integer represents the status code
 */
int init_sockaddr(struct sockaddr_in *addr);

/**
 * Set the ip address and port number of the proxy server.
 * @param proxy_addr a sockaddr_in pointer which will contains the address of the proxy server
 * @param opts struct options
 * @return an integer represents the status code
 */
int init_proxy_sockaddr(struct sockaddr_in *proxy_addr, const struct options *opts);

/**
 * Read messages from stdin and send it to the proxy server.
 * @param opts struct options
 * @param proxy_addr a sockaddr_in pointer points to the address of the proxy server
 * @param from_addr a sockaddr_in pointer points to the sockaddr_in variable will save the address of the source host
 * @return an integer represents the status code
 */
int do_client(const struct options *opts, struct sockaddr_in *proxy_addr, struct sockaddr_in *from_addr);

/**
 * Sends a FIN packet to the proxy server.
 * @param current_seq an uint32_t integer represents a sequence number
 * @param sock_fd an integer represents the file descriptor of a socket
 * @param proxy_addr a sockaddr_in pointer points the address of the proxy server
 * @return an integer represents the status code
 */
int send_fin(uint32_t current_seq, int sock_fd, struct sockaddr_in *proxy_addr);


#endif //UDP_CLIENT_UDP_SENDER_H

