//
// Created by Shik Hur on 2022-10-29.
//


#include "rudp_proxy.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


int init_sockaddr(struct sockaddr_in *addr, const struct options *opts)
{
    addr->sin_family = AF_INET;
    addr->sin_port = htons(opts->port_in);
    addr->sin_addr.s_addr = inet_addr(opts->ip_in);

    if(addr->sin_addr.s_addr == (in_addr_t) -1) // NOLINT(clang-analyzer-core.UndefinedBinaryOperatorResult)
    {
        return MY_FAILURE_CODE;
    }
    return MY_SUCCESS_CODE;
}

int init_server_sockaddr(struct sockaddr_in *server_addr, const struct options *opts)
{
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(opts->port_out);
    server_addr->sin_addr.s_addr = inet_addr(opts->ip_out);

    if (server_addr->sin_addr.s_addr == (in_addr_t) -1)
    {
        return MY_FAILURE_CODE;
    }
    return MY_SUCCESS_CODE;
}

int do_proxy(struct options *opts, struct sockaddr_in *server_addr, struct sockaddr_in *client_addr)
{
    pthread_t get_drop_packet_thread;
    pthread_t get_drop_res_thread;
    int clnt_to_server_result;
    int server_to_clnt_result;
    my_thread_arg_t thread_arg;
    thread_arg.server_addr = server_addr;
    thread_arg.client_addr = client_addr;
    thread_arg.opts = opts;

    clnt_to_server_result = pthread_create(&get_drop_packet_thread, NULL, deliver_packet_from_client_to_server, (void *) &thread_arg);
    server_to_clnt_result = pthread_create(&get_drop_res_thread, NULL, deliver_res_from_server_to_client, (void *) &thread_arg);
    if (clnt_to_server_result != 0 || server_to_clnt_result != 0)
    {
        return THREAD_FAIL;
    }

    pthread_join(get_drop_packet_thread, NULL);
    pthread_join(get_drop_res_thread, NULL);

    return MY_SUCCESS_CODE;
}

_Noreturn void *deliver_packet_from_client_to_server(void *argument)
{
    struct sockaddr_in *server_addr = ((my_thread_arg_t *) argument)->server_addr;
    struct sockaddr_in *client_addr = ((my_thread_arg_t *) argument)->client_addr;
    int out_sock_fd = ((my_thread_arg_t *) argument)->opts->out_sock_fd;
    int in_sock_fd = ((my_thread_arg_t *) argument)->opts->in_sock_fd;
    int chance = ((my_thread_arg_t *) argument)->opts->chance_drop_packet;

    rudp_packet_t packet;
    socklen_t from_addr_len = sizeof(socklen_t);
    ssize_t nread;
    ssize_t nwrite;

    srand(time(NULL));                          // NOLINT(cert-msc32-c,cert-msc51-cpp)
    while (1)
    {
        nread = recvfrom(in_sock_fd, &packet, sizeof(rudp_packet_t), 0, (struct sockaddr *) client_addr, &from_addr_len);
        if (nread > 0)
        {
            int random_num = (rand() % 100) + 1;        // NOLINT(cert-msc30-c,cert-msc50-cpp,clang-analyzer-security.insecureAPI.rand,concurrency-mt-unsafe,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
            if (random_num <= chance)
            {
                nwrite = sendto(out_sock_fd, &packet, sizeof(rudp_packet_t), 0, (struct sockaddr *) server_addr, sizeof(struct sockaddr_in));
                if (nwrite != -1)
                {
                    fprintf(stdout, "[Client ----------RUDP_PACKET------>>>> Server]\n");        // NOLINT(cert-err33-c)
                }
            }
        }
    }
}

_Noreturn void *deliver_res_from_server_to_client(void *argument)
{
    struct sockaddr_in *client_addr = ((my_thread_arg_t *) argument)->client_addr;
    int out_sock_fd = ((my_thread_arg_t *) argument)->opts->out_sock_fd;
    int in_sock_fd = ((my_thread_arg_t *) argument)->opts->in_sock_fd;
    int chance = ((my_thread_arg_t *) argument)->opts->chance_drop_res;
    struct sockaddr_in from_addr;

    rudp_packet_t packet;
    socklen_t from_addr_len;
    ssize_t nread;
    ssize_t nwrite;
    srand(time(NULL));                          // NOLINT(cert-msc32-c,cert-msc51-cpp)
    while (1)
    {
        nread = recvfrom(out_sock_fd, &packet, sizeof(rudp_packet_t), 0, (struct sockaddr *) &from_addr, &from_addr_len);
        if (nread > 0)
        {
            int random_num = (rand() % 100) + 1;        // NOLINT(cert-msc30-c,cert-msc50-cpp,clang-analyzer-security.insecureAPI.rand,concurrency-mt-unsafe,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
            if (random_num <= chance)
            {
                nwrite = sendto(in_sock_fd, &packet, sizeof(rudp_packet_t), 0, (struct sockaddr *) client_addr, sizeof(struct sockaddr_in));
                if (nwrite != 1)
                {
                    fprintf(stdout, "[Client <<<<-----------ACK------------- Server] \n");          // NOLINT(cert-err33-c)
                }
            }
        }
    }
}

