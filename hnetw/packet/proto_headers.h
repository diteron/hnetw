#pragma once

#include <stdint.h>

struct ipv4_hdr {
    uint8_t   header_length : 4;
    uint8_t   version : 4;
    uint8_t   ecn : 2;                 // Explicit Congestion Notification
    uint8_t   dscp : 6;                // Differentiated Services Code Point
    uint16_t  total_length;
    uint16_t  id;
    uint8_t   fragment_offset_5 : 5;   // Fragment offset (first 5 bits)
    uint8_t   more_fragment_f : 1;
    uint8_t   dont_fragment_f : 1;
    uint8_t   reserved_zero_f : 1;
    uint8_t   fragment_offset_8;       // Fragment offset (last 8 bits, total = 13)
    uint8_t   time_to_live;
    uint8_t   protocol;
    uint16_t  checksum;
    uint32_t  src_address;
    uint32_t  dest_address;
};

struct tcp_hdr {
    uint8_t   src_port;
    uint8_t   dest_port;
    uint32_t  seq_num;
    uint32_t  ack_num;
    uint8_t   reserved_part : 4;
    uint8_t   data_offset : 4;
    uint8_t   finish_f : 1;
    uint8_t   sync_f : 1;
    uint8_t   reset_f : 1;
    uint8_t   push_f : 1;
    uint8_t   ack_f : 1;
    uint8_t   urgent_f : 1;
    uint8_t   ecn_echo_f : 1;
    uint8_t   cwr_f : 1;
    uint8_t   window;
    uint8_t   checksum;
    uint8_t   urgent_pointer;
};

struct udp_hdr {
    uint16_t src_port;
    uint16_t dest_port;
    uint16_t udp_packet_length;
    uint16_t checksum;
};
