//
// Created by Shik Hur on 2022-10-28.
//


#include "udp_receiver.h"
#include <stdio.h>
#include <string.h>


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
    char sender_ip_addr[MAX_IP_ADDRESS_LENGTH];
    do
    {
        nread = recvfrom(opts->fd_in, &packet, sizeof(rudp_packet_t), 0, (struct sockaddr *)&proxy_addr, &from_addr_len);
        if (nread == -1)
        {
            return MY_FAILURE_CODE;
        }

        uint16_t check_sum_of_data_in_pack;
        deserialize_packet(&packet);
        check_sum_of_data_in_pack = generate_crc16(packet.data, packet.data_length);
        if (check_sum_of_data_in_pack != packet.check_sum)
        {
            // send NAK and continue;
        }

        if (packet.header.seq_no != current_seq_no + 1)
        {
            // discard this packet.
        }

        if (packet.header.packet_type == RUDP_SYN)
        {
            if (packet.header.seq_no == 0)
            {
                //get_ip_str((const struct sockaddr *)proxy_addr, sender_ip_addr);
                fprintf(stdout, "[Start receiving a message from client]\n"); // NOLINT(cert-err33-c, concurrency-mt-unsafe)
                //fprintf(stdout, "[TEST %s]\n", sender_ip_addr);                             // NOLINT(cert-err33-c)
            }

            // send ACK here

            strncpy(buffer, packet.data, packet.data_length);
            buffer[packet.data_length] = '\0';
            fprintf(stdout, "\t%s\n", buffer);                                              // NOLINT(cert-err33-c)
        }
        else if (packet.header.packet_type == RUDP_FIN)
        {
            // send ACK to FIN here
            fprintf(stdout, "[Finish the message transmission]\n");    // NOLINT(cert-err33-c)
            current_seq_no = -1;
        }
        else
        {
            continue;
        }
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

void get_ip_str(const struct sockaddr *sa, char *out_str)
{
    switch(sa->sa_family)
    {
        case AF_INET:
            inet_ntop(AF_INET, &(((const struct sockaddr_in*)sa)->sin_addr), out_str, MAX_IP_ADDRESS_LENGTH);  // NOLINT(clang-diagnostic-cast-align)
            break;
        case AF_INET6:
            inet_ntop(AF_INET6, &(((const struct sockaddr_in6 *)sa)->sin6_addr), out_str, MAX_IP_ADDRESS_LENGTH); // NOLINT(clang-diagnostic-cast-align)
            break;
        default:
            strncpy(out_str, "Unknown AF", MAX_IP_ADDRESS_LENGTH);
            out_str[MAX_IP_ADDRESS_LENGTH - 1] = '\0';
    }
}
