//
// Created by Shik Hur on 2022-10-28.
//

#include "rudp_types.h"

uint16_t generate_crc16(const char *data_p, size_t length)
{
    unsigned char x;
    uint16_t crc = 0xFFFF;

    while (length--)
    {
        x = crc >> 8 ^ *data_p++;
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t) (x << 12)) ^ ((uint16_t) (x <<5)) ^ ((uint16_t) x);
    }
    return crc;
}
