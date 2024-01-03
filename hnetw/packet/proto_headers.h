#pragma once

#include <stdint.h>

/*------------------ IP header ------------------*/

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

/*------------------ IGMP headers ------------------*/

struct igmp_v1_hdr {
    uint8_t     type : 4;
    uint8_t     version : 4;
    uint8_t     unused;
    uint16_t    checksum;
    uint32_t    group_addr;
};

struct igmp_v2_hdr {
    uint8_t     type;
    uint8_t     resp_time;
    uint16_t    checksum;
    uint32_t    group_addr;
};

struct igmp_v3_membs_qry {
    struct igmp_v2_hdr common_v2_part;
    
    uint8_t     qrv : 3;
    uint8_t     s_flg : 1;
    uint8_t     resv : 4;
    uint8_t     qqic;
    uint16_t    src_num;
};

struct igmp_v3_membs_rep {
    uint8_t     type;
    uint8_t     reserved8;
    uint16_t    checksum;
    uint16_t    reserved16;
    uint16_t    group_rec_num;
};

struct igmp_v3_group_rec {
    uint8_t     rec_type;
    uint8_t     aux_data_len;
    uint16_t    src_num;
    uint32_t    mcast_addr;
};

/*------------------ Transport layer headers ------------------*/

struct tcp_hdr {
    uint16_t   src_port;
    uint16_t   dest_port;
    uint32_t   seq_num;
    uint32_t   ack_num;
    uint8_t    reserved_part : 4;
    uint8_t    data_offset : 4;
    uint8_t    finish_f : 1;
    uint8_t    sync_f : 1;
    uint8_t    reset_f : 1;
    uint8_t    push_f : 1;
    uint8_t    ack_f : 1;
    uint8_t    urgent_f : 1;
    uint8_t    ecn_echo_f : 1;
    uint8_t    cwr_f : 1;
    uint16_t   window;
    uint16_t   checksum;
    uint16_t   urgent_pointer;
};

struct udp_hdr {
    uint16_t src_port;
    uint16_t dest_port;
    uint16_t udp_packet_length;
    uint16_t checksum;
};
