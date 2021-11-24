#include "icmp.h"


ICMP::ICMP(){
    type = ICMP_ECHO;
    code = 0;
    id = getpid()&0xffff;
    seq = 0;
    checksum();
}

ICMP::ICMP(std::vector<uint8_t> &_data){
    type = ICMP_ECHO;
    code = 0;
    id = getpid()&0xffff;
    seq = 0;
    data = _data;
    checksum();
}


ICMP::ICMP(uint8_t _type, uint8_t _code, uint16_t _id, uint16_t _seq){
    type = _type;
    code = _code;
    id = _id;
    seq = _seq;
    checksum();
}


ICMP::ICMP(uint8_t _type, uint8_t _code, uint16_t _id, uint16_t _seq, std::vector<uint8_t> &_data){
    type = _type;
    code = _code;
    id = _id;
    seq = _seq;
    data = _data;
    checksum();
}

void ICMP::checksum(){
    cksum = 0;
    auto byte_array = encode();
    uint32_t sum = 0;
    int len = byte_array.size();
    for (auto it = byte_array.begin(); len > 1; it++, len -= 2)
    {
        sum += *it << 8;
        it++;
        sum += *it;
    }
    if(len > 0)
        sum += byte_array.back();
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    cksum = ~sum;
}

std::vector<uint8_t> ICMP::encode(){
    std::vector<uint8_t> byte_array;
    byte_array.push_back(type);
    byte_array.push_back(code);
    byte_array.insert(byte_array.end(),(uint8_t*)&cksum, ((uint8_t*)&cksum)+2);
    byte_array.insert(byte_array.end(),(uint8_t*)&id, ((uint8_t*)&id)+2);
    byte_array.insert(byte_array.end(),(uint8_t*)&seq, ((uint8_t*)&seq)+2);
    byte_array.insert(byte_array.end(), data.begin(), data.end());
    return byte_array;
}

int ICMP::decode(std::vector<uint8_t> &byte_array){
    if(byte_array.size() < 8)
        return -1; //insufficient data for complete header
    if(byte_array.size() > MAXDATASIZE)
        return -2; //exceeded maximum size for a icmp packet 
    auto it = byte_array.begin();
    type = *it++;
    code = *it++;
    cksum = *it++;
    cksum += *it++ << 8;
    id = *it++;
    id += *it++ << 8;
    seq = *it++;
    seq += *it++ << 8;
    data.insert(data.begin(),it,byte_array.end());
    return 0;
}

void ICMP::increment_seq(){
    seq++;
    checksum();
}

bool ICMP::check_id(pid_t pid){
    return id == pid;
}

void ICMP::to_string(){ 
    std::cout << "seq=" << seq << ", id=" << id;
}

void ICMP::set_payload(int64_t time){
    int len = sizeof(time);
    std::vector<uint8_t> byte_data((uint8_t*)&time,(uint8_t*)(&time)+len);
    data = byte_data;
    checksum();  
}