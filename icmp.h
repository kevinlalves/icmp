// icmp.h -- interface for creation and manipulation of ICMP packets

#ifndef __icmp_h_
#define __icmp_h_

#include <unistd.h>
#include <vector>
#include <iostream>
#define ICMP_ECHO 8
#define ICMP_REPLY 0

const int max_datasize = 65535;
const int header_length = 8;

class ICMP{
    public:
        std::vector<uint8_t> encode();
        int decode(std::vector<uint8_t> &byte_array);
        ICMP();
        ICMP(std::vector<uint8_t> &_data);
        ICMP(uint8_t _type, uint8_t _code, uint16_t _id, uint16_t _seq);
        ICMP(uint8_t _type, uint8_t _code, uint16_t _id, uint16_t _seq, std::vector<uint8_t> &_data);
        void increment_seq();
        bool check_id(pid_t pid);
        void to_string();
        void set_payload(int64_t time);
    private:
        uint8_t type;
        uint8_t code;
        uint16_t cksum;
        uint16_t id;
        uint16_t seq;
        std::vector<uint8_t> data;
        void checksum();
};


#endif
