//
// Created by Shik Hur on 2022-10-28.
//

#include "rudp_types.h"
#include <stdlib.h>
#include <string.h>


uint16_t generate_crc16(const char *data_p, size_t length)
{
    unsigned char x;
    uint16_t crc = 0xFFFF;          // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

    while (length--)
    {
        x = crc >> 8 ^ *data_p++;   // NOLINT(hicpp-signed-bitwise, readability-magic-numbers, cppcoreguidelines-avoid-magic-numbers)
        x ^= x >> 4;                // NOLINT(hicpp-signed-bitwise, readability-magic-numbers, cppcoreguidelines-avoid-magic-numbers)
        crc = (crc << 8) ^ ((uint16_t) (x << 12)) ^ ((uint16_t) (x <<5)) ^ ((uint16_t) x);   // NOLINT(hicpp-signed-bitwise, readability-magic-numbers, cppcoreguidelines-avoid-magic-numbers)
    }
    return crc;
}

rudp_packet_t *create_rudp_packet_malloc(const rudp_header_t *header, const size_t data_length, const char *data)
{
    rudp_packet_t *packet = (rudp_packet_t *) malloc(sizeof(rudp_packet_t));
    memcpy(&packet->header, header, sizeof(rudp_header_t));
    if (header->packet_type == RUDP_SYN)
    {
        packet->data_length = data_length - 1; // ignore the '\n' at the end of the data
        strncpy(packet->data, data, data_length);
        uint16_t check_sum = generate_crc16(data, packet->data_length);
        packet->check_sum = check_sum;
    }
    else
    {
        // RUDP_ACK or RDUP_FIN
        packet->data_length = 0;
        packet->check_sum = 0;
        memset(packet->data, 0, MAX_DATA_LENGTH);
    }
    // change values into network order
    packet->header.packet_type = htons(packet->header.packet_type);
    packet->header.seq_no = htonl(packet->header.seq_no);
    packet->data_length = htons(packet->data_length);
    packet->check_sum = htons(packet->check_sum);

    return packet;
}

void init_rudp_header(uint16_t type, uint32_t seq_no, rudp_header_t *header_out)
{
    header_out->packet_type = type;
    header_out->seq_no = seq_no;
}

void deserialize_packet(rudp_packet_t *packet)
{
    packet->header.packet_type = ntohs(packet->header.packet_type);
    packet->header.seq_no = ntohl(packet->header.seq_no);
    packet->data_length = ntohs(packet->data_length);
    packet->check_sum = ntohs(packet->check_sum);
}

