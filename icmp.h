// icmp.h -- interface for creation and manipulation of ICMP packets

#ifndef __icmp_h_
#define __icmp_h_

#include <netinet/in.h>
#include <unistd.h>

#include <iostream>
#include <vector>

const uint8_t kIcmpEcho = 8;
const uint8_t kIcmpReply = 0;
const int kMaxDatasize = 65535;
const int kHeaderLength = 8;

class Icmp {
  public:
    std::vector<uint8_t> Encode();
    int Decode(std::vector<uint8_t> &byte_array);
    Icmp();
    Icmp(uint8_t type, uint8_t code, uint16_t id, uint16_t seq);
    void IncrementSeq();
    void ToString();
    // Set bytes of data for addional general info (anything goes)
    void SetPayload(std::vector<uint8_t> data);
    // Decodes only the payload, interpreting the bytes as 64-bits integer
    int64_t DecodeData();
  private:
    uint8_t type_;
    uint8_t code_;
    uint16_t id_;
    uint16_t seq_;
    std::vector<uint8_t> data_;
    uint16_t Checksum();
};


#endif