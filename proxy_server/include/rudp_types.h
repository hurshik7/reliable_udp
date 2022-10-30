//
// Created by Shik Hur on 2022-10-28.
//

#ifndef UDP_PROXY_RUDP_TYPES_H
#define UDP_PROXY_RUDP_TYPES_H


#include <arpa/inet.h>


#define MAX_DATA_LENGTH (1024)


/**
 * Struct rudp_header contains the type of a packet, and an positive integer represents sequence number.
 */
typedef struct rudp_header {
    uint16_t packet_type;
    uint32_t seq_no;
} rudp_header_t;

/**
 * Struct rudp_packet contains rudp_header, the length of a data, checksum value, and data.
 */
typedef struct rudp_packet {
    struct rudp_header header;     // 4 byte
    uint16_t data_length;          // 2 byte
    uint16_t check_sum;            // 2 byte
    char data[MAX_DATA_LENGTH];    // 1024 byte
} rudp_packet_t;


#endif //UDP_PROXY_RUDP_TYPES_H

