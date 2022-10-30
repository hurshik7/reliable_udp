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


/**
 * Generates crc16 checksum value.
 * @param data a string represent a message
 * @param data_length a positive integer represents the length of the data
 * @return uint16_t integer represents the checksum value of the data
 */
uint16_t generate_crc16(const char *data, size_t data_length);

/**
 * Returns a pointer to rudp_packet.
 *  It uses malloc to allocate the memory for the rudp_packet so that the function which calls this should free it.
 * @param header a rudp_header pointer
 * @param data_length a positive integer represents the length of the data
 * @param data a string represents the data
 * @return a rudp_packet pointer
 */
rudp_packet_t *create_rudp_packet_malloc(const rudp_header_t *header, size_t data_length, const char *data);

/**
 * Initiate rudp_header.
 *  It also serializes contents of the packet in network order.
 * @param type an uint16_t integer represents a type of rudp_packet
 * @param seq_no an uint32_t integer represents a sequence number of a packet
 * @param header_out a rudp_header pointer, which will have the above values
 */
void init_rudp_header(uint16_t type, uint32_t seq_no, rudp_header_t *header_out);

/**
 * Deserialize a rudp_packet into the host order.
 * @param packet an rudp_packet pointer
 */
void deserialize_packet(rudp_packet_t *packet);


#endif //UDP_CLIENT_RUDP_TYPES_H

