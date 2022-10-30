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


/**
 * Struct my_thread_arg to pass data to thread.
 */
typedef struct my_thread_arg
{
    struct sockaddr_in *server_addr;
    struct sockaddr_in *client_addr;
    struct options *opts;
} my_thread_arg_t;

/**
 * Set an ip address and port number to be used to a socket.
 * @param addr a sockaddr_in pointer which will contains the address of the host
 * @param opts a struct options pointer
 * @return an integer represents the status code
 */
int init_sockaddr(struct sockaddr_in *addr, const struct options *opts);

/**
 * Set the ip address and port number of the server.
 * @param server_addr a sockaddr_in pointer which will contains the address of the server
 * @param opts struct options
 * @return an integer represents the status code
 */
int init_server_sockaddr(struct sockaddr_in *server_addr, const struct options *opts);

/**
 * Receive a packet from client and pass to the server and vice versa.
 *  It uses pthread to implement multi-threading environment. One thread does receive a packet from a client
 *  and pass it to the server and the other does vice versa.
 * @param opts a struct options pointer
 * @param server_addr a sockaddr_in pointer points to the address of the server
 * @param client_addr a sockaddr_in pointer points to the sockaddr_in variable will save the address of the client
 * @return an integer represents the status code
 */
int do_proxy(struct options *opts, struct sockaddr_in *server_addr, struct sockaddr_in *client_addr);

/**
 * Receive a rudp packet from client and pass it to the sever.
 * @param argument a pointer to struct my_thread_arg
 * @return _Noreturn, this function continues to run until the user exits the program
 */
_Noreturn void *deliver_packet_from_client_to_server(void *argument);

/**
 * Receive a rudp response packet (ACK or NAK) and pass it to the client.
 * @param argument a pointer to struct my_thread_arg
 * @return _Noreturn, this function continues to run until the user exits the program
 */
_Noreturn void *deliver_res_from_server_to_client(void *argument);


#endif //UDP_PROXY_RUDP_PROXY_H

