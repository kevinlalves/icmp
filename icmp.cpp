#include "icmp.h"


ICMP::ICMP(){
    type = ICMP_ECHO;
    code = 0;
    id = getpid()&0xffff;
    seq = 0;
    cksum = 0;
    cksum = checksum(header_length);
}

ICMP::ICMP(std::vector<uint8_t> &_data){
    type = ICMP_ECHO;
    code = 0;
    id = getpid()&0xffff;
    seq = 0;
    data = _data;
    cksum = 0;
    cksum = checksum(header_length+data.size());
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
    std::vector<uint8_t> byte_array;
    byte_array.push_back(type);
    byte_array.push_back(code);
    byte_array.push_back(cksum&0xff);
    byte_array.push_back(cksum&(~0xff));
    byte_array.push_back(id&0xff);
    byte_array.push_back(id&(~0xff));
    byte_array.push_back(seq&0xff);
    byte_array.push_back(seq&(~0xff));
    byte_array.insert(byte_array.begin(), data.begin(), data.end());
    return byte_array;
}

int ICMP::decode(std::vector<uint8_t> &byte_array){
    if(byte_array.size() < 20)
        return -1; //insufficient data for complete header
    if(byte_array.size() > MAXDATASIZE)
        return -2; //exceeded maximum size for a icmp packet 
    auto it = byte_array.begin();
    code = *it++;
    type = *it++;
    cksum = *it++;
    cksum += *it++ << 8;
    id = *it++;
    id += *it++ << 8;
    seq = *it++;
    seq = *it++ << 8;
    data.insert(byte_array.begin(),it, byte_array.end());
    return 0;
}

void ICMP::increment_seq(){
    seq++;
    cksum = 0;
    cksum = checksum(header_length+data.size());
}

bool ICMP::check_id(pid_t pid){
    return id == pid;
}

void ICMP::info(){ 
    std::cout << ": seq=" << seq << ", id=" << id;
}

void ICMP::payload(long long time){
    int len = sizeof(time);
    std::vector<uint8_t> byte_data((uint8_t*)&time,(uint8_t*)(&time+len));
    data = byte_data;
    cksum = 0;
    cksum = checksum(header_length+data.size());  
}