// icmp.h -- interface for creation and manipulation of ICMP packets

#ifndef __icmp_h_
#define __icmp_h_

#include <unistd.h>

#include <iostream>
#include <vector>

const int kIcmpEcho = 8;
const int kIcmpReply = 0;
const int kMaxDatasize = 65535;
const int kHeaderLength = 8;

class Icmp{
    public:
        std::vector<uint8_t> Encode();
        int Decode(std::vector<uint8_t> &byte_array);
        Icmp();
        Icmp(std::vector<uint8_t> &_data);
        Icmp(uint8_t _type, uint8_t _code, uint16_t _id, uint16_t _seq);
        Icmp(uint8_t _type, uint8_t _code, uint16_t _id, uint16_t _seq, std::vector<uint8_t> &_data);
        void IncrementSeq();
        bool CheckId(pid_t pid);
        void ToString();
        void SetPayload(int64_t time);
    private:
        uint8_t type;
        uint8_t code;
        uint16_t cksum;
        uint16_t id;
        uint16_t seq;
        std::vector<uint8_t> data;
        void Checksum();
};


#endif