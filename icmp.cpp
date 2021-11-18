#include "icmp.h"


ICMP::ICMP(){
    type = ICMP_ECHO;
    code = 0;
    id = getpid()&0xffff;
    seq = 1;
    cksum = 0;
    cksum = checksum(header_length);
}

ICMP::ICMP(uint8_t _type, uint8_t _code, uint16_t _id, uint16_t _seq){
    type = _type;
    code = _code;
    id = _id;
    seq = _seq;
    cksum = 0;
    cksum = checksum(header_length);
}


ICMP::ICMP(uint8_t _type, uint8_t _code, uint16_t _id, uint16_t _seq, std::vector<uint8_t> &_data){
    type = _type;
    code = _code;
    id = _id;
    seq = _seq;
    data = _data;
    cksum = 0;
    cksum = checksum(header_length+data.size());
}

uint16_t ICMP::checksum(int len){
    uint32_t sum = 0;
    uint16_t *parcel = (uint16_t*)this;
    uint16_t answer = 0;
    while(len > 1){
        sum += *parcel++;
        len -= 2;
    }
    if(len == 1){
        *(uint8_t*)(&answer) = *(uint8_t*)parcel;
        sum += answer;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return answer;
}

std::vector<uint8_t> ICMP::encode(){
    
}