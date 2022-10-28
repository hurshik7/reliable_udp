//
// Created by Shik Hur on 2022-10-28.
//

#include "option_handler.h"
#include "udp_sender.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

int init_proxy_sockaddr(struct sockaddr_in *proxy_addr, const struct options *opts)
{
    proxy_addr->sin_family = AF_INET;
    proxy_addr->sin_port = htons(opts->port_out);
    proxy_addr->sin_addr.s_addr = inet_addr(opts->ip_out);

    if(proxy_addr->sin_addr.s_addr ==  (in_addr_t)-1)
    {
        return MY_FAILURE_CODE;
    }
    return MY_SUCCESS_CODE;
}

int do_client(const struct options *opts, struct sockaddr_in *proxy_addr, const struct sockaddr_in *addr)
{
    int current_seq = 0;
    char buffer[MAX_DATA_LENGTH];
    ssize_t nwrote;

    printf("[message to send]: ");
    while (fgets(buffer, MAX_DATA_LENGTH, stdin) != NULL)
    {
        // create rudp packet header
        rudp_header_t header;
        header.packet_type = RUDP_SYN;
        header.seq_no = current_seq;

        // create rudp packet
        rudp_packet_t *packet = create_rudp_packet_malloc(&header, strlen(buffer), buffer);

        // set up timer

        // sendto proxy server
        nwrote = sendto(opts->fd_out, packet, sizeof(rudp_packet_t), 0, (const struct sockaddr *) proxy_addr, sizeof(struct sockaddr_in));
        if (nwrote == -1)
        {
            free(packet);
            return MY_FAILURE_CODE;
        }

        // wait for the ACK
            // if it gets ACK -> check the seq_number, if it's valid -> free the packet and continue;
            // it it gets NAK -> send the packet again
            // time out -> send the packet again.

        // increase sequence number and continue
        free(packet);
        current_seq++;
        printf("[message to send]: ");
    }

    // send FIN, get ACK

    // return
    return MY_SUCCESS_CODE;
}

rudp_packet_t *create_rudp_packet_malloc(const rudp_header_t *header, const size_t data_length, const char *data)
{
    rudp_packet_t *packet = (rudp_packet_t *) malloc(sizeof(rudp_packet_t));
    memcpy(&packet->header, header, sizeof(rudp_header_t));
    packet->data_length = data_length;
    strncpy(packet->data, data, data_length);
    uint16_t check_sum = generate_crc16(data, data_length);
    packet->check_sum = check_sum;

    // change values into network order
    packet->header.packet_type = htons(packet->header.packet_type);
    packet->header.seq_no = htons(packet->header.seq_no);
    packet->data_length = htons(packet->data_length);
    packet->check_sum = htons(packet->check_sum);

    return packet;
}

