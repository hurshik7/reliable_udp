//
// Created by Shik Hur on 2022-10-28.
//

#include "udp_receiver.h"

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

int do_server(const struct options *opts, struct sockaddr_in *proxy_addr, const struct sockaddr_in *addr)
{
    rudp_packet_t packet;
    socklen_t from_addr_len = sizeof(struct sockaddr_in);
    int current_seq_no = -1;

    ssize_t nread;
    char buffer[MAX_DATA_LENGTH];
    do
    {
        nread = recvfrom(opts->fd_in, &packet, sizeof(rudp_packet_t), 0, (struct sockaddr *)&proxy_addr, &from_addr_len);
        if (nread == -1)
        {
            return MY_FAILURE_CODE;
        }

        // packet 처리
            // if it's not valid (checksum, correct sequence no) -> send NAK and discard the packet or just discard the packet
            // if it's valid,
                // get ip address of proxy, show the message (if the seq_no is 0 -> show message) -> send ACK packet

            // if it's FIN -> show message, end line

    } while (nread > 0);

    return MY_SUCCESS_CODE;
}
