//
// Created by Shik Hur on 2022-10-28.
//


#include "udp_receiver.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
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

int do_server(struct options *opts, struct sockaddr_in *proxy_addr, const struct sockaddr_in *addr)
{
    rudp_packet_t packet;
    socklen_t from_addr_len = sizeof(struct sockaddr_in);
    int current_seq_no = -1;
    int result;

    ssize_t nread;
    char buffer[MAX_DATA_LENGTH];
    do
    {
        nread = recvfrom(opts->fd_in, &packet, sizeof(rudp_packet_t), 0, (struct sockaddr *)&proxy_addr, &from_addr_len);
        if (nread == -1)
        {
            return MY_FAILURE_CODE;
        }

        deserialize_packet(&packet);
        rudp_header_t response_packet_header;
        rudp_packet_t *response_packet;
        if (packet.header.packet_type == RUDP_SYN)
        {
            if (packet.header.seq_no == 0)
            {
                fprintf(stdout, "[Start receiving a message from client]\n"); // NOLINT(cert-err33-c, concurrency-mt-unsafe)
                // if the packet is the first packet of a client, open a socket for sending ACK packet.
                result = open_socket_for_response(opts, proxy_addr);
                if (result == MY_FAILURE_CODE)
                {
                    return OPEN_SOCKET_FAILURE_CODE;
                }
            }

            // check if the data is corrupted or not.
            uint16_t check_sum_of_data_in_pack;
            check_sum_of_data_in_pack = generate_crc16(packet.data, packet.data_length);
            if (check_sum_of_data_in_pack != packet.check_sum)
            {
                init_rudp_header(RUDP_NAK, packet.header.seq_no, &response_packet_header);
                response_packet = create_rudp_packet_malloc(&response_packet_header, 0, NULL);
                sendto(opts->fd_out, response_packet, sizeof(rudp_packet_t), 0, (const struct sockaddr *) proxy_addr, sizeof(struct sockaddr_in));
                free(response_packet);
                continue;
            }

            // if the sender sent the same packet with previous one, send ACK to client again for the same packet.
            // this situation only happens when the client can not receive the ACK of the packet from the server
            if (packet.header.seq_no != current_seq_no)
            {
                init_rudp_header(RUDP_ACK, packet.header.seq_no, &response_packet_header);
                response_packet = create_rudp_packet_malloc(&response_packet_header, 0, NULL);
                sendto(opts->fd_out, response_packet, sizeof(rudp_packet_t), 0, (const struct sockaddr *) proxy_addr, sizeof(struct sockaddr_in));
                free(response_packet);
                continue;
            }

            // copy data from the client, print it on stdout
            strncpy(buffer, packet.data, packet.data_length);
            buffer[packet.data_length] = '\0';
            fprintf(stdout, "\t%s\n", buffer);                                              // NOLINT(cert-err33-c)

            // send ACK
            init_rudp_header(RUDP_ACK, current_seq_no, &response_packet_header);
            response_packet = create_rudp_packet_malloc(&response_packet_header, 0, NULL);
            sendto(opts->fd_out, response_packet, sizeof(rudp_packet_t), 0, (const struct sockaddr *) proxy_addr, sizeof(struct sockaddr_in));

            // free response packet and increase current_seq_no
            current_seq_no++;
        }
        else if (packet.header.packet_type == RUDP_FIN)
        {
            fprintf(stdout, "[Finish the message transmission]\n");    // NOLINT(cert-err33-c)
            // send FIN
            init_rudp_header(RUDP_FIN, current_seq_no, &response_packet_header);
            response_packet = create_rudp_packet_malloc(&response_packet_header, 0, NULL);
            sendto(opts->fd_out, response_packet, sizeof(rudp_packet_t), 0, (const struct sockaddr *) proxy_addr, sizeof(struct sockaddr_in));
            current_seq_no = -1;
            close(opts->fd_out);
        }
        else continue;
        free(response_packet);
    } while (nread > 0);

    return MY_SUCCESS_CODE;
}

void deserialize_packet(rudp_packet_t *packet)
{
    packet->header.packet_type = ntohs(packet->header.packet_type);
    packet->header.seq_no = ntohs(packet->header.seq_no);
    packet->data_length = ntohs(packet->data_length);
    packet->check_sum = ntohs(packet->check_sum);
}

int open_socket_for_response(struct options *opts, struct sockaddr_in *to_addr)
{
    int result;
    opts->fd_out = socket(AF_INET, SOCK_DGRAM, 0);                // NOLINT(android-cloexec-socket)
    if(opts->fd_out == -1)
    {
        return MY_FAILURE_CODE;
    }

    result = bind(opts->fd_out, (struct sockaddr*)to_addr, sizeof(struct sockaddr_in));
    if (result != 0)
    {
        return MY_FAILURE_CODE;
    }

    return MY_SUCCESS_CODE;
}
