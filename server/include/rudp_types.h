//
// Created by Shik Hur on 2022-10-28.
//

#ifndef UDP_CLIENT_RUDP_TYPES_H
#define UDP_CLIENT_RUDP_TYPES_H


#include <arpa/inet.h>


#define MAX_DATA_LENGTH (1024)
#define RUDP_SYN (0)
#define RUDP_ACK (1)
#define RUDP_FIN (2)
#define RUDP_NAK (3)


typedef struct rudp_header {
    uint16_t packet_type;
    uint16_t seq_no;
} rudp_header_t;

typedef struct rudp_packet {
    struct rudp_header header;     // 4 byte
    uint16_t data_length;          // 2 byte
    uint16_t check_sum;            // 2 byte
    char data[MAX_DATA_LENGTH];    // 1024 byte
} rudp_packet_t;


uint16_t generate_crc16(const char *data, size_t data_length);
rudp_packet_t *create_rudp_packet_malloc(const rudp_header_t *header, size_t data_length, const char *data);
void init_rudp_header(uint16_t type, uint16_t seq_no, rudp_header_t *header_out);
void deserialize_packet(rudp_packet_t *packet);


#endif //UDP_CLIENT_RUDP_TYPES_H

