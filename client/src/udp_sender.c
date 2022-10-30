//
// Created by Shik Hur on 2022-10-28.
//

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

    if (proxy_addr->sin_addr.s_addr == (in_addr_t) -1)
    {
        return MY_FAILURE_CODE;
    }
    return MY_SUCCESS_CODE;
}

int do_client(const struct options *opts, struct sockaddr_in *proxy_addr, struct sockaddr_in *from_addr)
{
    uint32_t current_seq = 0;
    char buffer[MAX_DATA_LENGTH];
    ssize_t nwrote;
    ssize_t nread;
    rudp_packet_t response_packet;
    socklen_t from_addr_len = sizeof(struct sockaddr_in);

    printf("[message to send]: ");
    while (fgets(buffer, MAX_DATA_LENGTH, stdin) != NULL)
    {
        // create rudp fin packet header
        rudp_header_t header;
        header.packet_type = RUDP_SYN;
        header.seq_no = current_seq;

        // create rudp fin packet
        rudp_packet_t *packet = create_rudp_packet_malloc(&header, strlen(buffer), buffer);

        // sendto proxy server
        nwrote = sendto(opts->sock_fd, packet, sizeof(rudp_packet_t), 0, (const struct sockaddr *) proxy_addr, sizeof(struct sockaddr_in));
        if (nwrote == -1)
        {
            free(packet);
            return MY_FAILURE_CODE;
        }

wait_response_packet:
        nread = recvfrom(opts->sock_fd, &response_packet, sizeof(rudp_packet_t), 0, (struct sockaddr *)from_addr, &from_addr_len);
        if (nread == -1)
        {
            // timeout reached (3000 ms)
            fprintf(stdout, "\t|______________________________[TIMEOUT]: The packet has been sent again\n");        // NOLINT(cert-err33-c)
            nwrote = sendto(opts->sock_fd, packet, sizeof(rudp_packet_t), 0, (const struct sockaddr *) proxy_addr, sizeof(struct sockaddr_in));
            if (nwrote == -1)
            {
                free(packet);
                return MY_FAILURE_CODE;
            }
            goto wait_response_packet;
        }

        deserialize_packet(&response_packet);
        // if it receives NAK, or it receives ACK but the seq_no is not equal to the fin packet it sent, resend the fin packet.
        if (response_packet.header.packet_type == RUDP_NAK || (response_packet.header.seq_no != current_seq || response_packet.header.packet_type != RUDP_ACK))
        {
            nwrote = sendto(opts->sock_fd, packet, sizeof(rudp_packet_t), 0, (const struct sockaddr *) proxy_addr, sizeof(struct sockaddr_in));
            if (nwrote == -1)
            {
                free(packet);
                return MY_FAILURE_CODE;
            }
            goto wait_response_packet;
        }

        fprintf(stdout, "\t|______________________________[received ACK]\n");           // NOLINT(cert-err33-c)
        // increase sequence number and continue
        free(packet);
        current_seq++;
        printf("[message to send]: ");
    }

    // send FIN
    do
    {
        send_fin(current_seq, opts->sock_fd, proxy_addr);
        nread = recvfrom(opts->sock_fd, &response_packet, sizeof(rudp_packet_t), 0, (struct sockaddr *)from_addr, &from_addr_len);
        if (nread == -1)
        {
            fprintf(stdout, "\t|__FIN_________________________[TIMEOUT]: The FIN packet has been sent again\n");        // NOLINT(cert-err33-c)
            continue;
        }
        deserialize_packet(&response_packet);
    } while (nread == -1 || (response_packet.header.seq_no != current_seq || response_packet.header.packet_type != RUDP_ACK));

    fprintf(stdout, "\n[Finish sending messages (Received ACK for the FIN)]\n");                // NOLINT(cert-err33-c)
    return MY_SUCCESS_CODE;
}

int send_fin(uint32_t current_seq, int sock_fd, struct sockaddr_in *proxy_addr)
{
    ssize_t nwrote;
    // send FIN, get ACK
    rudp_header_t fin_header;
    init_rudp_header(RUDP_FIN, current_seq, &fin_header);
    rudp_packet_t *fin_packet = create_rudp_packet_malloc(&fin_header, 0, NULL);
    nwrote = sendto(sock_fd, fin_packet, sizeof(rudp_packet_t), 0, (const struct sockaddr *) proxy_addr, sizeof(struct sockaddr_in));
    if (nwrote == -1)
    {
        free(fin_packet);
        return MY_FAILURE_CODE;
    }
    free(fin_packet);
    return MY_SUCCESS_CODE;
}

